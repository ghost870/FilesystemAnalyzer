#ifndef FAT32
#define FAT32

#include <cstdint>

#include "Filesystem.hpp"

class Fat32 : public Filesystem {
    public:
        Fat32(const std::string &filename);

        uint16_t getBytesPerSector() const { return bytesPerSector; };
        uint8_t getSectorsPerCluster() const { return sectorsPerCluster; };
        uint16_t getReservedSectors() const { return reservedSectors; };
        uint32_t getFatSectorNumber() const { return fatSectorNumber; };
        uint32_t getRootCluster() const { return rootCluster; };

    private:
        uint16_t bytesPerSector;
        uint8_t sectorsPerCluster;
        uint16_t reservedSectors;
        uint32_t fatSectorNumber;
        uint32_t rootCluster;

        bool parseBootSector();
};

#endif