#ifndef FILESYSTEM
#define FILESYSTEM

#include <cstdint>
#include <string>

class Filesystem {
    public:
        virtual ~Filesystem();
        bool loadFile(const std::string &filename);

    protected:
        uint8_t* data = nullptr;
};

#endif