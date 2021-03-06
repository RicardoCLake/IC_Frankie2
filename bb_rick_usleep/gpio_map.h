#ifndef _BEAGLEBONE_GPIO_H_
#define _BEAGLEBONE_GPIO_H_

#define GPIO1_START_ADDR 0x4804C000
#define GPIO1_END_ADDR   0x4804DFFF
#define GPIO2_START_ADDR 0x481AC000
#define GPIO2_END_ADDR   0x481ADFFF
#define GPIO3_START_ADDR 0x481AE000
#define GPIO3_END_ADDR   0x481AFFFF

#define GPIO_START_ADDR  GPIO1_START_ADDR
#define GPIO_END_ADDR    GPIO3_END_ADDR
#define GPIO1_OFFSET     (GPIO1_START_ADDR - GPIO_START_ADDR)
#define GPIO2_OFFSET     (GPIO2_START_ADDR - GPIO_START_ADDR)
#define GPIO3_OFFSET     (GPIO3_START_ADDR - GPIO_START_ADDR)

#define GPIO_SIZE (GPIO_END_ADDR - GPIO_START_ADDR)

#define GPIO_OE 0x134
#define GPIO_DATAIN 0x138   //in an input mode, reads if is low (0) or high (1)
#define GPIO_DATAOUT 0x13C    //in an output mode, it sets gpio to high (1) or low (0)
#define GPIO_SETDATAOUT 0x194   //in an output mode, sets gpio high (1)
#define GPIO_CLEARDATAOUT 0x190   //in an output mode, sets gpio low (putting also 1)

#endif //_BEAGLEBONE_GPIO_H_
