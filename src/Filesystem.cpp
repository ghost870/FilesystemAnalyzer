#include <stdexcept>
#include <fstream>

#include "Filesystem.hpp"

Filesystem::Filesystem(const std::string &filename)
{
    if (!loadFile(filename))
    {
        throw std::runtime_error("Could not open file " + filename);
    }
}

Filesystem::~Filesystem()
{
    delete [] data;
}

bool Filesystem::loadFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
	
    if (!file)
    {
        return false;
    }

    dataSize = file.tellg();

    file.seekg(0, std::ios::beg);

    data = new unsigned char[dataSize];

    file.read((char*)data, dataSize);

    return true;
}