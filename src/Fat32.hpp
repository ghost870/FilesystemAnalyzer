#ifndef FAT32
#define FAT32

#include <cstdint>
#include <vector>

#include "Filesystem.hpp"

class Fat32 : public Filesystem {
    public:
        Fat32(const std::string &filename);

        uint16_t getBytesPerSector() const { return bytesPerSector; };
        uint8_t getSectorsPerCluster() const { return sectorsPerCluster; };
        uint16_t getReservedSectors() const { return reservedSectors; };
        uint32_t getFatSectorNumber() const { return fatSectorNumber; };
        uint32_t getRootCluster() const { return rootCluster; };

        uint32_t getBytesPerCluster() const { return bytesPerCluster; };
        uint32_t getFatOffset() const { return fatOffset; };
        uint64_t getFatSize() const { return fatSize; };
        uint64_t getFatCount() const { return fatCount; };
        uint64_t getRootOffset() const { return rootOffset; };
        uint32_t getRootSize() const { return rootSize; };

        std::vector<uint32_t> getClusterNumbers(uint32_t index) const;

    private:
        uint16_t bytesPerSector;
        uint8_t sectorsPerCluster;
        uint16_t reservedSectors;
        uint32_t fatSectorNumber;
        uint32_t rootCluster;

        uint32_t bytesPerCluster;
        uint32_t fatOffset;
        uint64_t fatSize;
        uint64_t fatCount;
        uint64_t rootOffset;
        uint32_t rootSize;

        bool parseBootSector();
        void calculateParameters();
};

#endif