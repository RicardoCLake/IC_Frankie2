#include "Gpio.h"  

volatile void* Gpio::gpioAddr = NULL;

Gpio::Gpio(volatile unsigned int offset)
{
    gpioOeAddr = (volatile unsigned int*)gpioAddr + offset / sizeof(unsigned int) + GPIO_OE / sizeof(unsigned int);
    gpioSetdataoutAddr = (volatile unsigned int*)gpioAddr + offset / sizeof(unsigned int) + GPIO_SETDATAOUT / sizeof(unsigned int);
    gpioCleardataoutAddr = (volatile unsigned int*)gpioAddr + offset / sizeof(unsigned int) + GPIO_CLEARDATAOUT / sizeof(unsigned int);
}

Gpio::~Gpio()
{
}

void Gpio::setOE (unsigned int ports)
{
    unsigned int reg;
    reg = *gpioOeAddr;
    reg = reg & (0xFFFFFFFF - ports);
    *gpioOeAddr = reg;
}

void Gpio::setDataOut (unsigned int ports)
{
    *gpioSetdataoutAddr = ports;
}

void Gpio::clearDataOut (unsigned int ports)
{
    *gpioCleardataoutAddr = ports;
}

bool Gpio::gpioMap ()
{
    int fd = open("/dev/mem", O_RDWR);
    std::cout << "Mapping " << GPIO_START_ADDR << " - " << GPIO_END_ADDR << " (size: " << GPIO_SIZE << ")" << std::endl;
    Gpio::gpioAddr = mmap(0, GPIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_START_ADDR);
    if (Gpio::gpioAddr == MAP_FAILED) 
    {
        std::cout << "Unable to map GPIO" << std::endl;
        return 0;
    }
    return 1;
}


volatile void* Gpio::getGpioAddr()
{
    return gpioAddr;
}
