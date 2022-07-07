#include "Filesystem.hpp"

Filesystem::~Filesystem()
{
    delete [] data;
}

bool Filesystem::loadFile(const std::string &filename)
{

}