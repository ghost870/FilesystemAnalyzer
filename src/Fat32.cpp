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
    if (dataSize <= BS_ROOT_CLUSTER_OFFSET + 3)
    {
        return false;
    }

    bytesPerSector = data[BS_BYTES_PER_SECTOR_OFFSET] | (data[BS_BYTES_PER_SECTOR_OFFSET + 1] << 8);
    sectorsPerCluster = data[BS_SECTORS_PER_CLUSTER_OFFSET];
    reservedSectors = data[BS_RESERVED_SECTORS_OFFSET] | (data[BS_RESERVED_SECTORS_OFFSET + 1] << 8);
    fatSectorNumber = data[BS_FAT_SECTOR_NUMBER_OFFSET] | (data[BS_FAT_SECTOR_NUMBER_OFFSET + 1] << 8) | (data[BS_FAT_SECTOR_NUMBER_OFFSET + 2] << 16) | (data[BS_FAT_SECTOR_NUMBER_OFFSET + 3] << 24);
    rootCluster = data[BS_ROOT_CLUSTER_OFFSET] | (data[BS_ROOT_CLUSTER_OFFSET + 1] << 8) | (data[BS_ROOT_CLUSTER_OFFSET + 2] << 16) | (data[BS_ROOT_CLUSTER_OFFSET + 3] << 24);

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

        e = data[fatIndex] | (data[fatIndex + 1] << 8) | (data[fatIndex + 2] << 16) | (data[fatIndex + 3] << 24);

        iterationCounter++;
    }

    return clusterIndexes;
}

int Fat32::listDirectoryEntries(uint32_t index) const
{
    uint64_t entryOffset = (uint64_t)index * bytesPerCluster + DIRECTORY_FIRST_ENTRY_OFFSET;

    uint32_t iterationCounter = 0;
    while (dataSize > (entryOffset + DIRECTORY_ENTRY_FILE_SIZE_OFFSET + 3) && data[entryOffset] != 0 && iterationCounter < UINT32_MAX)
    {
        uint64_t firstClusterOffset = entryOffset + DIRECTORY_ENTRY_FIRST_CLUSTER_OFFSET;
        uint64_t fileSizeOffset = entryOffset + DIRECTORY_ENTRY_FILE_SIZE_OFFSET;

        for (int i = 0; i <= DIRECTORY_ENTRY_NAME_LENGTH; i++)
        {
            std::cout << data[entryOffset + i];
        }

        std::cout << (data[firstClusterOffset] | (data[firstClusterOffset + 1] << 8));
        std::cout << ":";
        std::cout << (data[fileSizeOffset] | (data[fileSizeOffset + 1] << 8) | (data[fileSizeOffset + 2] << 16) | (data[fileSizeOffset + 3] << 24));
        std::cout << std::endl;

        entryOffset += DIRECTORY_ENTRY_SIZE;

        iterationCounter++;
    }

    return 0;
}