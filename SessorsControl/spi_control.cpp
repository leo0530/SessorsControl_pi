#include "spi_control.h"

SPI_Control::SPI_Control()
{

}
SPI_Control::~SPI_Control()
{

}
/*
* SPIDataRW:
*    Write and Read a block of data over the SPI bus.
*    Note the data ia being read into the transmit buffer, so will
*    overwrite it!
*    This is also a full-duplex operation.
*********************************************************************************
*********************************************************************************/
int SPI_Control::SPIDataRW (int channel, unsigned char *tx_data, unsigned char *rx_data, int len)
{
  //int i = 0;
  struct spi_ioc_transfer spi ;

  channel &= 1 ;

  memset (&spi, 0, sizeof (spi)) ;

  spi.tx_buf        = (unsigned long)tx_data ;
  spi.rx_buf        = (unsigned long)rx_data ;
  spi.len           = len ;
  spi.delay_usecs   = spiDelay ;
  spi.speed_hz      = spiSpeeds [channel] ;
  spi.bits_per_word = spiBPW ;

return ioctl (spiFds [channel], SPI_IOC_MESSAGE(1), &spi) ; //SPI_IOC_MESSAGE(1)的1表示spi_ioc_transfer的数量
}


/*
* SPISetupMode:
*    Open the SPI device, and set it up, with the mode, etc.
*********************************************************************************
*********************************************************************************/

int SPI_Control::SPISetupMode (int channel, int speed, int mode)
{
  int fd ;

  if ((fd = open (channel == 0 ? spiDev0 : spiDev1, O_RDWR)) < 0)
  {
    printf("Unable to open SPI device: %s\n", strerror (errno)) ;
    return -1;
  }

  spiSpeeds [channel] = speed ;
  spiFds    [channel] = fd ;

/*
* 设置spi的读写模式：
*  Mode 0： CPOL=0, CPHA=0
*  Mode 1： CPOL=0, CPHA=1
*  Mode 2： CPOL=1, CPHA=0
*  Mode 3： CPOL=1, CPHA=1
*  这里我们默认设置为模式0
*********************************************************************************
*/
  if (ioctl (fd, SPI_IOC_WR_MODE, &mode) < 0)
  {
    printf("Can't set spi mode: %s\n", strerror (errno)) ;
    return -1;
  }

  if (ioctl (fd, SPI_IOC_RD_MODE, &mode) < 0)
  {
    printf("Can't get spi mode: %s\n", strerror (errno)) ;
    return -1;
  }

/*
* spi的读写bit/word设置可写
*    这里设置为8个位为一个字节
*********************************************************************************
*/
  if (ioctl (fd, SPI_IOC_WR_BITS_PER_WORD, &spiBPW) < 0)
  {
    printf("Can't set bits per word: %s\n", strerror (errno))  ;
    return -1;
  }

  if (ioctl (fd, SPI_IOC_RD_BITS_PER_WORD, &spiBPW) < 0)
  {
    printf("Can't get bits per word: %s\n", strerror (errno))  ;
    return -1;
  }

/*
* 设置spi读写速率
*********************************************************************************
*/
  if (ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
  {
    printf("Can't set max speed hz: %s\n", strerror (errno));
    return -1;
  }

  if (ioctl (fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0)
  {
    printf("Can't get max speed hz: %s\n", strerror (errno));
    return -1;
  }

  return fd ;
}


/*
* SPISetup:
*    Open the SPI device, and set it up, etc. in the default MODE 0
*********************************************************************************
*********************************************************************************/

int SPI_Control::Open (int channel, int speed,int mode)
{
  return SPISetupMode (channel, speed, mode) ;
}

int SPI_Control::Close (int fd)
{
    return close(fd);
}
