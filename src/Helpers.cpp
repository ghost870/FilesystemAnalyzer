#include "Helpers.h"

uint8_t U8(const unsigned char* data, uint64_t index)
{
    return data[index];
}

uint16_t U16(const unsigned char* data, uint64_t index)
{
    return data[index] | (data[index + 1] << 8);
}

uint32_t U32(const unsigned char* data, uint64_t index)
{
    return data[index] | (data[index + 1] << 8) | (data[index + 2] << 16) | (data[index + 3] << 24);
}