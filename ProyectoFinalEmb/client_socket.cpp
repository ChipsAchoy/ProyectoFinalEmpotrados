#include "client_socket.h"
#include <cstring> // Incluir cstring para memset


ClientSocket::ClientSocket(const std::string& server_ip, int port)
    : server_ip(server_ip), port(port), sock(-1) {
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
}

ClientSocket::~ClientSocket() {
    if (sock != -1) {
        close(sock);
    }
}

bool ClientSocket::connectToServer() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error al crear el socket");
        return false;
    }

    if (inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr) <= 0) {
        perror("Dirección inválida/ No soportada");
        return false;
    }

    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Error al conectar con el servidor");
        return false;
    }

    return true;
}

bool ClientSocket::sendMessage(const std::string& message) {
    if (send(sock, message.c_str(), message.length(), 0) < 0) {
        perror("Error al enviar el mensaje");
        return false;
    }
    return true;
}

std::string ClientSocket::receiveMessage() {
    char buffer[1024] = {0};
    int valread = read(sock, buffer, sizeof(buffer) - 1);
    if (valread < 0) {
        perror("Error al recibir el mensaje");
        return "";
    }
    return std::string(buffer, valread);
}
