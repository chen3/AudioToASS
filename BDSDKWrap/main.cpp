//
// Created by mx404 on 5/6/19.
//
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include <grpcpp/grpcpp.h>

#include "ProductId.h"
#include "Sdk.h"
#include "SdkConfig.h"
#include "Message/Message.h"
#include "Project.grpc.pb.h"

using cn::mx404::audiotoass::AudioStream;
using cn::mx404::audiotoass::Empty;
using cn::mx404::audiotoass::ErrorContent;
using cn::mx404::audiotoass::ErrorString;
using cn::mx404::audiotoass::Frame;
using cn::mx404::audiotoass::JsonString;
using cn::mx404::audiotoass::Config;
using mx404::BDSpeedSDKWrapper::ProductID;
using mx404::BDSpeedSDKWrapper::SDK;
using mx404::BDSpeedSDKWrapper::SDKConfig;
using mx404::BDSpeedSDKWrapper::SDKMessage::Message;
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::Status;
using grpc::StatusCode;
using std::cerr;
using std::endl;
using std::exception;
using std::invalid_argument;
using std::make_shared;
using std::runtime_error;
using std::shared_ptr;
using std::string;

namespace {

    class GRpcStatusException final : public runtime_error {
    public:
        GRpcStatusException(const Status& status)
            : runtime_error("GRpcStatusException, ErrorCode:" + std::to_string(static_cast<int>(status.error_code()))
                            + ", ErrorMessage:" + status.error_message()
                            + ", ErrorDetails:" + status.error_details())
            , m_errorCode(status.error_code())
            , m_errorMessage(status.error_message())
            , m_errorDetails(status.error_details())
        {
        }

        inline StatusCode errorCode() const {
            return m_errorCode;
        }
        inline std::string errorMessage() const  {
            return m_errorMessage;
        }
        inline std::string errorDetails() const {
            return m_errorDetails;
        }
    private:
        StatusCode m_errorCode;
        std::string m_errorMessage;
        std::string m_errorDetails;
    };

    class ArgumentException final : public invalid_argument {
    public:
        explicit ArgumentException(const std::string& whatArg, int exitCode)
            : invalid_argument(whatArg)
            , m_exitCode(exitCode)
        {
        }

        inline int exitCode() const {
            return m_exitCode;
        }
    private:
        int m_exitCode;
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
            : m_port(port)
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
            throw GRpcStatusException(status);
        }
        inline int port() const {
            return m_port;
        }

        void receiveFrameLoop(std::function<void(const Frame&)> callback) {
            ClientContext context;
            std::unique_ptr<ClientReader<Frame>> reader(stub->GetStream(&context, empty));
            Frame frame;
            while (reader->Read(&frame)) {
                callback(frame);
            }
            Status status = reader->Finish();
            if (status.ok()) {
                return;
            }
            throw GRpcStatusException(status);
        }

        void sentRecognitionStartMessage() {
            ClientContext context;
            Status status = stub->RecognitionStart(&context, empty, nullptr);
            if (!status.ok()) {
                throw GRpcStatusException(status);
            }
        }
        void sentRecognitionEndMessage() {
            ClientContext context;
            Status status = stub->RecognitionEnd(&context, empty, nullptr);
            if (!status.ok()) {
                throw GRpcStatusException(status);
            }
        }
        void sentSentenceStartMessage() {
            ClientContext context;
            Status status = stub->SentenceStart(&context, empty, nullptr);
            if (!status.ok()) {
                throw GRpcStatusException(status);
            }
        }
        void sentSentenceEndMessage() {
            ClientContext context;
            Status status = stub->SentenceEnd(&context, empty, nullptr);
            if (!status.ok()) {
                throw GRpcStatusException(status);
            }
        }
        void sentSentenceFinishData(const std::string& jsonString) {
            ClientContext context;
            JsonString json;
            json.set_data(jsonString);
            Status status = stub->SentenceFinishData(&context, json, nullptr);
            if (!status.ok()) {
                throw GRpcStatusException(status);
            }
        }
        void sentSentenceFlushData(const std::string& jsonString) {
            ClientContext context;
            JsonString json;
            json.set_data(jsonString);
            Status status = stub->SentenceFlushData(&context, json, nullptr);
            if (!status.ok()) {
                throw GRpcStatusException(status);
            }
        }
        void sentUnknowError(Message& message) {
            int size = config.unknowerrorkeys_size();
            ErrorContent errorContent;
            ::google::protobuf::Map<string, string> map = errorContent.data();
            for (int i = 0; i < size; ++i) {
                const string key = config.unknowerrorkeys(i);
                string value;
                if (message.get(key, value)) {
                    map[key] = value;
                }
            }
            ClientContext context;
            Status status = stub->UnknowError(&context, errorContent, nullptr);
            if (!status.ok()) {
                throw GRpcStatusException(status);
            }
        }
        void sentBDSDKStartFail(const string& errorString) {
            int size = config.unknowerrorkeys_size();
            ClientContext context;
            ErrorString str;
            str.set_errorstring(errorString);
            Status status = stub->BDSDKStartFail(&context, str, nullptr);
            if (!status.ok()) {
                throw GRpcStatusException(status);
            }
        }
    private:
        int m_port;
        std::shared_ptr<AudioStream::Stub> stub;
        Config config;
    };
}

int main(int argc, char* argv[]) {
    try {
        LocalServer local(parseArguments(argc, argv));
        local.initConfig();

        // test config
        shared_ptr<SDKConfig> sdkConfig = make_shared<SDKConfig>("10555002", "YourOwnName",
                                        "jhRA15uv8Lvd4r9qbtmOODMv", make_shared<ProductID>(15362));
        string errorString;
        shared_ptr<SDK> sdk = SDK::getInstance(sdkConfig, errorString);

        local.receiveFrameLoop([](Frame frame) {

        });
    } catch (ArgumentException& ex) {
        cerr << ex.what() << endl;
        return ex.exitCode();
    } catch (exception& ex) {
        cerr << ex.what() << endl;
        return -1;
    }
    return 0;
}

void parseArguments(int argc, char* argv[]) {

}
