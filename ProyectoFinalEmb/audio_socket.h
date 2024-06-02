#ifndef AUDIO_SOCKET_H
#define AUDIO_SOCKET_H

#include <boost/asio.hpp>
#include <vector>
#include <string>

class AudioSocket {
public:
    AudioSocket(const std::string& host, unsigned short port);
    void send(const std::vector<unsigned char>& data);
    std::vector<unsigned char> receive();

private:
    std::string host_;
    unsigned short port_;
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
};

#endif // AUDIO_SOCKET_H

