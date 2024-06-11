#ifndef GPIO_H
#define GPIO_H

// Function prototypes
void gpioInitPtrs();
void gpioSetMode(unsigned int pin, unsigned int mode);
void gpioWrite(unsigned int pin, unsigned char bit);
unsigned char gpioRead(unsigned int pin);

#endif // GPIO_H
