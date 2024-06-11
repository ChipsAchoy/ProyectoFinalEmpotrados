#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>


class ClientSocket {
public:
    ClientSocket(const std::string& server_ip, int port);
    ~ClientSocket();
    bool connectToServer();
    bool sendMessage(const std::string& message);
    std::string receiveMessage();

private:
    std::string server_ip;
    int port;
    int sock;
    struct sockaddr_in server_address;
};

#endif // CLIENTSOCKET_H
