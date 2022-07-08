#ifndef FILESYSTEM
#define FILESYSTEM

#include <cstdint>
#include <string>

class Filesystem {
    public:
        Filesystem(const std::string &filename);
        virtual ~Filesystem();

    protected:
        char* data = nullptr;
    
    private:
        bool loadFile(const std::string &filename);
};

#endif