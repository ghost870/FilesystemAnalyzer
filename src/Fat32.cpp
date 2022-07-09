#include <cstdint>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "Fat32.hpp"
#include "Helpers.h"

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
    if (dataSize <= BS_ROOT_CLUSTER_OFFSET + 3)
    {
        return false;
    }

    bytesPerSector = U16(data, BS_BYTES_PER_SECTOR_OFFSET);
    sectorsPerCluster = U8(data, BS_SECTORS_PER_CLUSTER_OFFSET);
    reservedSectors = U16(data, BS_RESERVED_SECTORS_OFFSET);
    fatSectorNumber = U32(data, BS_FAT_SECTOR_NUMBER_OFFSET);
    rootCluster = U32(data, BS_ROOT_CLUSTER_OFFSET);

    return true;
}

void Fat32::calculateParameters()
{
    bytesPerCluster = sectorsPerCluster * bytesPerSector;
    fatOffset = reservedSectors * bytesPerSector;
    fatSize = (uint64_t)fatSectorNumber * bytesPerSector;
    fatCount = fatSize / FAT_ELEMENT_SIZE;
    rootOffset = fatOffset + fatSize * FAT_NUM_OF_TABLES;
    rootSize = bytesPerCluster;
}

std::vector<uint32_t> Fat32::getClusterNumbers(uint32_t index) const
{
    std::vector<uint32_t> clusterIndexes;

    uint32_t e = index;
    uint32_t iterationCounter = 0;
    while (e != FAT_EOC && iterationCounter < UINT32_MAX)
    {
        clusterIndexes.push_back(e + rootOffset / bytesPerCluster - 1);
        uint64_t fatIndex = (uint64_t)fatOffset + e * FAT_ELEMENT_SIZE;

        if (dataSize <= fatIndex + 3)
        {
            return {};
        }

        e = U32(data, fatIndex);

        iterationCounter++;
    }

    return clusterIndexes;
}

void Fat32::listDirectoryEntries(uint32_t absoluteClusterNumber) const
{
    uint64_t entryOffset = (uint64_t)absoluteClusterNumber * bytesPerCluster + DIRECTORY_FIRST_ENTRY_OFFSET;

    uint32_t iterationCounter = 0;
    while (dataSize > (entryOffset + DIRECTORY_ENTRY_FILE_SIZE_OFFSET + 3) && data[entryOffset] != 0 && iterationCounter < UINT32_MAX)
    {
        uint64_t firstClusterOffset = entryOffset + DIRECTORY_ENTRY_FIRST_CLUSTER_OFFSET;
        uint64_t fileSizeOffset = entryOffset + DIRECTORY_ENTRY_FILE_SIZE_OFFSET;

        for (int i = 0; i <= DIRECTORY_ENTRY_NAME_LENGTH; i++)
        {
            std::cout << data[entryOffset + i];
        }

        std::cout << U16(data, firstClusterOffset);
        std::cout << ":";
        std::cout << U16(data, fileSizeOffset);
        std::cout << std::endl;

        entryOffset += DIRECTORY_ENTRY_SIZE;

        iterationCounter++;
    }
}