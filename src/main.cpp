#include <iostream>

#include "Fat32.hpp"

int main(int argc, char const *argv[])
{
    Fat32 *filesystem = new Fat32("/home/ghost/Pobrane/fs.dd");

    std::cout << filesystem->getBytesPerSector() << std::endl;
    std::cout << unsigned(filesystem->getSectorsPerCluster()) << std::endl;
    std::cout << filesystem->getReservedSectors() << std::endl;
    std::cout << filesystem->getFatSectorNumber() << std::endl;
    std::cout << filesystem->getRootCluster() << std::endl;

    return 0;
}