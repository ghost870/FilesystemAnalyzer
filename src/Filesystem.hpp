#ifndef FILESYSTEM
#define FILESYSTEM

#include <cstdint>
#include <string>
#include <fstream>

class Filesystem {
    public:
        Filesystem(const std::string &filename);
        virtual ~Filesystem();

    protected:
        char* data = nullptr;
        std::streamsize dataSize = 0;
    
    private:
        bool loadFile(const std::string &filename);
};

#endif