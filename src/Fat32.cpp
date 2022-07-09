#include <cstdint>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "Fat32.hpp"

Fat32::Fat32(const std::string &filename) : Filesystem(filename)
{
    if (!parseBootSector())
    {
        throw std::runtime_error("Input file too small");
    }

    calculateParameters();
}

bool Fat32::parseBootSector()
{
    if (dataSize <= 0x2F)
    {
        return false;
    }

    bytesPerSector = data[0xB] | (data[0xB + 1] << 8);
    sectorsPerCluster = data[0xD];
    reservedSectors = data[0xE] | (data[0xE + 1] << 8);
    fatSectorNumber = data[0x24] | (data[0x24 + 1] << 8) | (data[0x24 + 2] << 16) | (data[0x24 + 3] << 24);
    rootCluster = data[0x2C] | (data[0x2C + 1] << 8) | (data[0x2C + 2] << 16) | (data[0x2C + 3] << 24);

    return true;
}

void Fat32::calculateParameters()
{
    bytesPerCluster = sectorsPerCluster * bytesPerSector;
    fatOffset = reservedSectors * bytesPerSector;
    fatSize = (uint64_t)fatSectorNumber * bytesPerSector;
    fatCount = fatSize / 4;
    rootOffset = fatOffset + fatSize * 2;
    // This is not true. It can be more than one cluster.
    rootSize = bytesPerCluster;
}

std::vector<uint32_t> Fat32::getClusterNumbers(uint32_t index) const
{
    std::vector<uint32_t> clusterIndexes;

    uint32_t e = index;
    uint32_t iterationCounter = 0;
    while (e != 0x0FFFFFFF && iterationCounter < UINT32_MAX)
    {
        clusterIndexes.push_back(e + rootOffset / bytesPerCluster - 1);
        uint64_t fatIndex = (uint64_t)fatOffset + e * 4;

        if (dataSize <= fatIndex + 3)
        {
            return {};
        }

        e = data[fatIndex] | (data[fatIndex + 1] << 8) | (data[fatIndex + 2] << 16) | (data[fatIndex + 3] << 24);

        iterationCounter++;
    }

    return clusterIndexes;
}

int Fat32::listDirectoryEntries(uint32_t index) const
{
    uint64_t entryOffset = (uint64_t)index * bytesPerCluster + 0x20;

    uint32_t iterationCounter = 0;
    while (dataSize > (entryOffset + 0x1F) && data[entryOffset] != 0 && iterationCounter < UINT32_MAX)
    {
        uint64_t firstClusterOffset = entryOffset + 0x1A;
        uint64_t fileSizeOffset = entryOffset + 0x1C;

        for (int i = 0; i <= 10; i++)
        {
            std::cout << data[entryOffset + i];
        }

        std::cout << (data[firstClusterOffset] | (data[firstClusterOffset + 1] << 8));
        std::cout << ":";
        std::cout << (data[fileSizeOffset] | (data[fileSizeOffset + 1] << 8) | (data[fileSizeOffset + 2] << 16) | (data[fileSizeOffset + 3] << 24));
        std::cout << std::endl;

        entryOffset += 0x40;

        iterationCounter++;
    }

    return 0;
}