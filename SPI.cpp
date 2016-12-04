#include "SPI.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <cstdio>
#include <unistd.h>
#include <cstring>

SPI::SPI(const char* device)
{
    fd = open(device, O_RDWR);
	if (fd < 0)
		printf("can't open device");
}

SPI::~SPI()
{
    close(fd);
    //dtor
}

int SPI::begin()
{
    /*
	 * spi mode
	 */
	int ret;
	ret = ioctl(fd, SPI_IOC_RD_MODE32, &mode);
	if (ret == -1)
		printf("can't get spi mode");

    mode |= SPI_TX_QUAD;
    ret = ioctl(fd, SPI_IOC_WR_MODE32, &mode);
	if (ret == -1)
		printf("can't set spi mode");

	/*
	 * bits per word
	 */
    //bits=1;
	//ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		printf("can't get bits per word");

	/*
	 * max speed hz
	 */
    speed = 50000000;
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		printf("can't get max speed hz");


	printf("spi mode: 0x%x\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
	return ret;
}

int SPI::transfer(uint8_t byte)
{
    int ret;
    uint8_t tx[2] = {byte,'\0'};

    ret = transfer(tx,1);

	return ret;
}
int SPI::transfer(uint8_t* byte, unsigned int len)
{
    /*int ret = write(fd, byte, len);

	return ret;*/
	struct spi_ioc_transfer tr[10];

    memset((void *)tr, 0, sizeof(struct spi_ioc_transfer)*10);

    for(int i=0; i<len;i+=64)
    {
        tr[i].tx_buf = (__u64)(byte+i);
        tr[i].len = len;
        tr[i].speed_hz = speed;
        tr[i].bits_per_word = bits;
    }


    int ret = ioctl(fd, SPI_IOC_MESSAGE(1), tr);
    if (ret < 1)
        printf("Erro ao enviar mensagem spi\n");
    return ret;
}

int SPI::getmode()
{
    return mode;
}

void SPI::setmode(int m)
{
    mode = m;
    /*
	 * spi mode
	 */
	int ret = ioctl(fd, SPI_IOC_WR_MODE32, &mode);
	if (ret == -1)
		printf("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE32, &mode);
	if (ret == -1)
		printf("can't get spi mode");
}
