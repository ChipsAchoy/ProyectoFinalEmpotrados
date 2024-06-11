#include <stdio.h>
#include <unistd.h>
#include "../include/gpio.h"

int main(int argc, char const *argv[]){
    gpioInitPtrs();

    // Configure four GPIO pins as output
    for (unsigned int pin = 2; pin <= 5; ++pin)
        gpioSetMode(pin, 1); // Mode 1 is output

    // Toggle GPIO pins in a loop
    while (1) {
        //read pins 2 to 5
        printf("Pin values: ");
        for (unsigned int pin = 2; pin <= 5; ++pin) {
            printf("%d ", gpioRead(pin));
        }
        printf("\n");
            
    }

    return 0;
}
