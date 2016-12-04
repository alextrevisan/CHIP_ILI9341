#ifndef SPI_H
#define SPI_H

#include <stdint.h>

class SPI
{
    public:
        SPI(const char* device);
        virtual ~SPI();
        int begin();
        int transfer(uint8_t byte);
        int transfer(uint8_t* byte, unsigned int len);
        int getmode();
        void setmode(int m);
    private:
    int fd;
    uint32_t mode = 0;
    uint8_t bits = 0;
    uint32_t speed = 48000000;
    uint16_t delay = 0;
};

#endif // SPI_H
