#ifndef SPI_CONTROL_H
#define SPI_CONTROL_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <linux/spi/spidev.h>

static const char   *spiDev0  = "/dev/spidev0.0" ;
static const char   *spiDev1  = "/dev/spidev0.1" ;
static  u_int8_t     spiBPW   = 8 ;
static  u_int16_t    spiDelay = 0 ;
static u_int32_t     spiSpeeds [2] ;
static int          spiFds [2] ;

class SPI_Control
{
public:
    SPI_Control();
    ~SPI_Control();
    int SPIDataRW (int channel, unsigned char *tx_data, unsigned char *rx_data, int len);
    int SPISetupMode (int channel, int speed, int mode);
    int Open(int channel, int speed,int mode);//open spi dev
    int Close (int fd);//close spi dev by fd.fd is return by open()
};

#endif // SPI_CONTROL_H
