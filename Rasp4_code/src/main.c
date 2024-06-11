#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../include/gpio.h"


#define PORT 8666
#define BUF_SIZE 1024
#define CHUNK_SIZE 1024

int main(int argc, char const *argv[]){

    gpioInitPtrs();

    // Configure four GPIO pins as output
    for (unsigned int pin = 2; pin <= 5; ++pin)
        gpioSetMode(pin, 1); // Mode 1 is output

   // Crear socket
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUF_SIZE] = {0};

    // Crear socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Error al crear el socket");
        return 1;
    }

    int send_buff_size = 131072; // Set your desired buffer size here
    if (setsockopt(server_fd, SOL_SOCKET, SO_SNDBUF, &send_buff_size, sizeof(send_buff_size)) == -1) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

   // Vincular el socket al puerto
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Error al vincular el socket al puerto");
        return 1;
    }

    // Escuchar en el puerto
    if (listen(server_fd, 3) < 0) {
        perror("Error al escuchar en el puerto");
        return 1;
    }



    while (1) {
        // Aceptar la conexión entrante
        printf("Esperando conexión...\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Error al aceptar la conexión");
            return 1;
        }

        // Leer el mensaje del socket
        valread = read(new_socket, buffer, BUF_SIZE);
        if (valread < 0) {
            perror("Error al leer del socket");
            close(new_socket);
            continue;
        }

        
        // Enviar el estado de las entradas GPIO 2 a 5 si se solicita
        if (strcmp(buffer, "get_inputs") == 0) {
            char input_states[4] = {0};
            
            for (unsigned int pin = 2; pin <= 5; ++pin) {
                input_states[pin - 2] = gpioRead(pin);
            }

            send(new_socket, input_states, strlen(input_states), 0);
        }
        
        
        // Cerrar el socket
        close(new_socket);
    }

    return 0;
}
