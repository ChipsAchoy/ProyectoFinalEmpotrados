#include <sys/mman.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "../include/gpio.h"

// GPIO base address for Raspberry Pi
static unsigned GPIO_BASE = 0xfe200000;

// Registers pointers
volatile unsigned int *gpfsel;
volatile unsigned int *gpset;
volatile unsigned int *gpclr;
volatile unsigned int *gplev;

void gpioInitPtrs(){
    int fd = -1;
    if ((fd = open("/dev/mem", O_RDWR, 0)) == -1)
        err(1, "Error opening /dev/mem");

    gpfsel = (unsigned int*)mmap(0, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
    if (gpfsel == MAP_FAILED)
        errx(1, "Error during mapping GPIO");

    gpset = gpfsel + 0x7;
    gpclr = gpfsel + 0xA;
    gplev = gpfsel + 0xD;
}

void gpioSetMode(unsigned int pin, unsigned int mode){
    unsigned int reg = pin / 10;
    unsigned int shift = (pin % 10) * 3;
    unsigned int mask = 0x7 << shift;

    gpfsel[reg] = (gpfsel[reg] & ~mask) | ((mode & 0x7) << shift);
}

void gpioWrite(unsigned int pin, unsigned char bit){
    if (bit)
        gpset[pin / 32] = (1 << (pin % 32));
    else
        gpclr[pin / 32] = (1 << (pin % 32));
}

unsigned char gpioRead(unsigned int pin){
    return (gplev[pin / 32] & (1 << (pin % 32))) != 0;
}
