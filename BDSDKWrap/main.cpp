//
// Created by mx404 on 5/6/19.
//
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include <grpcpp/grpcpp.h>

#include "Project.grpc.pb.h"

using cn::mx404::audiotoass::AudioStream;
using cn::mx404::audiotoass::Empty;
using cn::mx404::audiotoass::Frame;
using cn::mx404::audiotoass::JsonString;
using cn::mx404::audiotoass::Config;
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::Status;
using std::cerr;
using std::endl;
using std::exception;
using std::invalid_argument;

namespace {

    class ArgumentException : public invalid_argument {
    public:
        explicit ArgumentException(const std::string& whatArg, int exitCode)
            : invalid_argument(whatArg)
            , exitCode(exitCode)
        {
        }

        int getExitCode() const {
            return exitCode;
        }

    private:
        int exitCode;
    };

    int parseArguments(int argc, char* argv[]) {
        // check argument
        if (argc != 2) {
            throw ArgumentException(std::string("Accept only one parameter. Current parameter count(") + std::to_string(argc - 1) + ").", -2);
        }
        int port = 0;
        std::string sPort(argv[1]);
        try {
            std::string::size_type sz;
            port = std::stoi(sPort, &sz);
            if (sz != sPort.length()) {
                throw ArgumentException(std::string("Parameter(") + sPort + ") not a number.", -3);
            }
            if (port <= 0) {
                throw ArgumentException(std::string("Parameter(") + sPort + ") must greater than 0.", -4);
            }
        }
        catch (std::invalid_argument) {
            throw ArgumentException(std::string("Parameter(") + sPort + ") not a number.", -3);
        }
        catch (std::out_of_range) {
            throw ArgumentException(std::string("Parameter(") + sPort + ") can't greater than int max value("
                        + std::to_string(std::numeric_limits<decltype(port)>::max()) + ") or less than 0", -5);
        }
        return port;
    }

    const Empty empty;

    class LocalServer
    {
    public:
        LocalServer(int port)
            : port(port)
        {
            assert(port > 0);
            std::shared_ptr<Channel> channel = grpc::CreateChannel("localhost:" + std::to_string(port), grpc::InsecureChannelCredentials());
            stub = AudioStream::NewStub(channel);
            initConfig();
        }
        void initConfig() {
            ClientContext context;
            Status status = stub->GetConfig(&context, empty, &config);
            if (status.ok()) {
                return;
            }
            // TODO throw exception
        }
        int getPort() const {
            return port;
        }
    private:
        int port;
        std::shared_ptr<AudioStream::Stub> stub;
        Config config;
    };
}
void work(int port);

int main(int argc, char* argv[]) {
    try {
        int port = parseArguments(argc, argv);
        LocalServer local(port);
        local.initConfig();

    } catch (ArgumentException& ex) {
        cerr << ex.what() << endl;
        return ex.getExitCode();
    } catch(exception& ex) {
        cerr << ex.what() << endl;
        return -1;
    }
    return 0;
}

void work(int port) {
    std::shared_ptr<Channel> channel = grpc::CreateChannel("localhost:" + std::to_string(port), grpc::InsecureChannelCredentials());
    std::shared_ptr<AudioStream::Stub> stub = AudioStream::NewStub(channel);
    ClientContext context;

    Status status = stub->GetConfig(&context, empty, &config);
    if (!status.ok()) {
        // TODO output or throw
        return;
    }
    std::unique_ptr<ClientReader<Frame>> frameStream = stub->GetStream(&context, empty);
    Frame frame;
    while (frameStream->Read(&frame)) {
        const char* c = frame.data().c_str();
    }
}

void parseArguments(int argc, char* argv[]) {

}
