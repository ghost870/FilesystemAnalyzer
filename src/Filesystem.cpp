#include <fstream>

#include "Filesystem.hpp"

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

    std::streamsize filesize = file.tellg();

    file.seekg(0, std::ios::beg);

    delete [] data;

    data = new char[filesize];

    file.read(data, filesize);

    return true;
}