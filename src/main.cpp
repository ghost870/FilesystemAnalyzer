#include <iostream>

#include "Fat32.hpp"

int main(int argc, char const *argv[])
{
    Fat32 *filesystem = new Fat32("/home/ghost/Pobrane/fs.dd");

    for (auto e: filesystem->getClusterNumbers(6))
    {
        std::cout << e << std::endl;
    }

    return 0;
}