//
// Created by mx404 on 5/6/19.
//
#include <exception>
#include <iostream>
#include <string>

#include <grpcpp/grpcpp.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Accept only one parameter." << std::endl;
        return -1;
    }
    int port = 0;
    try {
        std::string::size_type sz;
        std::string sPort(argv[1]);
        port = std::stoi(sPort, &sz);
        if (sz != sPort.length()) {
            std::cerr << "Parameter not a number." << std::endl;
            return -2;
        }
        if (port <= 0) {
            std::cerr << "Parameter must greater than 0." << std::endl;
            return -3;
        }
    }
    catch (std::invalid_argument) {
        std::cerr << "Parameter not a number." << std::endl;
        return -2;
    }
    catch (std::out_of_range) {
        std::cerr << "Parameter can't greater than int max value(" << std::numeric_limits<decltype(port)>::max() << ") or less than 0" << std::endl;
        return -4;
    }

    grpc::CreateChannel("localhost:" + std::to_string(port), grpc::InsecureChannelCredentials());

    return 0;
}
