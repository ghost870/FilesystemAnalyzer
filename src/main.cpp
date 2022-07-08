#include <iostream>

#include "Fat32.hpp"

int main(int argc, char const *argv[])
{
    Fat32 *filesystem = new Fat32("/home/ghost/Pobrane/fs.dd");

    return 0;
}