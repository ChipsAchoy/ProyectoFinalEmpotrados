#include "audio_socket.h"
#include <iostream>

AudioSocket::AudioSocket(const std::string& host, unsigned short port)
    : host_(host), port_(port), acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), socket_(io_context_) {
    acceptor_.accept(socket_);
}

void AudioSocket::send(const std::vector<unsigned char>& data) {
    boost::asio::write(socket_, boost::asio::buffer(data));
}

std::vector<unsigned char> AudioSocket::receive() {
    std::vector<unsigned char> data(1024); // Adjust the size as needed
    size_t len = socket_.read_some(boost::asio::buffer(data));
    data.resize(len);
    return data;
}

void AudioSocket::sendString(const std::string& data) {
    boost::asio::write(socket_, boost::asio::buffer(data));
}
