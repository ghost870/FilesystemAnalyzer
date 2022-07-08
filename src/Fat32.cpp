#include <stdexcept>

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
    if (dataSize < 0x2F)
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