#ifndef GPIO_H
#define GPIO_H


#ifdef __cplusplus
extern "C" {
#endif

// Function prototypes
void gpioInitPtrs();
void gpioSetMode(unsigned int pin, unsigned int mode);
void gpioWrite(unsigned int pin, unsigned char bit);
unsigned char gpioRead(unsigned int pin);

#ifdef __cplusplus
}
#endif


#endif // GPIO_H
