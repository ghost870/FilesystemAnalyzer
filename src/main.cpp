#include <iostream>

#include "Fat32.hpp"

int main(int argc, char const *argv[])
{
    Fat32 *filesystem = new Fat32("/home/ghost/Pobrane/fs.dd");

    filesystem->listDirectoryEntries(1576 * 2 + 32);

    return 0;
}