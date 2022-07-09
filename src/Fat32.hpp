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
        void listDirectoryEntries(uint32_t index) const;

    private:
        static inline int const BS_BYTES_PER_SECTOR_OFFSET = 0xB;
        static inline int const BS_SECTORS_PER_CLUSTER_OFFSET = 0xD;
        static inline int const BS_RESERVED_SECTORS_OFFSET = 0xE;
        static inline int const BS_FAT_SECTOR_NUMBER_OFFSET = 0x24;
        static inline int const BS_ROOT_CLUSTER_OFFSET = 0x2C;

        static inline int const FAT_NUM_OF_TABLES = 2;
        static inline int const FAT_ELEMENT_SIZE = 4;
        static inline int const FAT_EOC = 0x0FFFFFFF;

        static inline int const DIRECTORY_FIRST_ENTRY_OFFSET = 0x20;
        static inline int const DIRECTORY_ENTRY_SIZE = 0x40;
        static inline int const DIRECTORY_ENTRY_FIRST_CLUSTER_OFFSET = 0x1A;
        static inline int const DIRECTORY_ENTRY_FILE_SIZE_OFFSET = 0x1C;
        static inline int const DIRECTORY_ENTRY_NAME_LENGTH = 10;

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