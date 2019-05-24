//
// Created by mx404 on 5/6/19.
//
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>

#include "Callback.hpp"
#include "ProductId.h"
#include "Sdk.h"
#include "SdkConfig.h"
#include "Message/Message.h"
#include "Message/PushMessage.h"
#include "Message/ReceiveMessage.hpp"
#include "Project.grpc.pb.h"

using cn::mx404::audiotoass::AudioStream;
using cn::mx404::audiotoass::Config;
using cn::mx404::audiotoass::Empty;
using cn::mx404::audiotoass::ErrorContent;
using cn::mx404::audiotoass::ErrorString;
using cn::mx404::audiotoass::Frame;
using cn::mx404::audiotoass::JsonString;
using cn::mx404::audiotoass::SDKVersion;
using mx404::BDSpeedSDKWrapper::Callback;
using mx404::BDSpeedSDKWrapper::ProductID;
using mx404::BDSpeedSDKWrapper::SDK;
using mx404::BDSpeedSDKWrapper::SDKConfig;
using mx404::BDSpeedSDKWrapper::SDKMessage::Message;
using mx404::BDSpeedSDKWrapper::SDKMessage::PushMessage;
using mx404::BDSpeedSDKWrapper::SDKMessage::ReceiveMessage;
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::Status;
using grpc::StatusCode;
using std::cerr;
using std::endl;
using std::exception;
using std::function;
using std::invalid_argument;
using std::make_shared;
using std::runtime_error;
using std::shared_ptr;
using std::string;
using std::vector;

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
            SDKVersion version;
            version.set_version(SDK::getSDKVersion());
            Status status = stub->GetConfig(&context, version, &m_config);
            if (status.ok()) {
                return;
            }
            throw GRpcStatusException(status);
        }
        inline int port() const {
            return m_port;
        }
        inline const Config& config() const {
            return m_config;
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

        void sendRecognitionStartMessage() {
            ClientContext context;
            Status status = stub->RecognitionStart(&context, empty, nullptr);
            if (!status.ok()) {
                throw GRpcStatusException(status);
            }
        }
        void sendRecognitionEndMessage() {
            ClientContext context;
            Status status = stub->RecognitionEnd(&context, empty, nullptr);
            if (!status.ok()) {
                throw GRpcStatusException(status);
            }
        }
        void sendSentenceStartMessage() {
            ClientContext context;
            Status status = stub->SentenceStart(&context, empty, nullptr);
            if (!status.ok()) {
                throw GRpcStatusException(status);
            }
        }
        void sendSentenceEndMessage() {
            ClientContext context;
            Status status = stub->SentenceEnd(&context, empty, nullptr);
            if (!status.ok()) {
                throw GRpcStatusException(status);
            }
        }
        void sendSentenceFinishData(const std::string& jsonString) {
            ClientContext context;
            JsonString json;
            json.set_data(jsonString);
            Status status = stub->SentenceFinishData(&context, json, nullptr);
            if (!status.ok()) {
                throw GRpcStatusException(status);
            }
        }
        void sendSentenceFlushData(const std::string& jsonString) {
            ClientContext context;
            JsonString json;
            json.set_data(jsonString);
            Status status = stub->SentenceFlushData(&context, json, nullptr);
            if (!status.ok()) {
                throw GRpcStatusException(status);
            }
        }
        void sendUnknowError(Message& message) {
            const Config c = config();
            int size = c.unknowerrorkeys_size();
            ErrorContent errorContent;
            ::google::protobuf::Map<string, string> map = errorContent.data();
            for (int i = 0; i < size; ++i) {
                const string key = c.unknowerrorkeys(i);
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
        void sendBDSDKStartFail(const string& errorString) {
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
        Config m_config;
    };

    class LambdaCallback : public Callback {
    public:
        LambdaCallback(function<void(shared_ptr<ReceiveMessage>)> callback,
                       function<void(const string&)> errorCallback)
            : m_callback(callback)
            , errorCallback(errorCallback)
        {
        }
        virtual ~LambdaCallback() override = default;
        virtual void callback(shared_ptr<ReceiveMessage> message) override {
            m_callback(message);
        }
        virtual void error(const string& errorDescription) override {
            errorCallback(errorDescription);
        }
    private:
        function<void(shared_ptr<ReceiveMessage>)> m_callback;
        function<void(const string&)> errorCallback;
    };
}

int main(int argc, char* argv[]) {
    try {
        LocalServer local(parseArguments(argc, argv));
        local.initConfig();

        shared_ptr<SDKConfig> sdkConfig;
#ifndef NDEBUG
        // test config
        sdkConfig = make_shared<SDKConfig>("10555002", "YourOwnName",
                                           "jhRA15uv8Lvd4r9qbtmOODMv", make_shared<ProductID>(15362));
        sdkConfig->setDNNDatFilePath("../Library/BDSpeedSDK/resources/asr_resource/bds_easr_mfe_dnn.dat");
        sdkConfig->setCMVNDatFilePath("../Library/BDSpeedSDKresources/asr_resource/bds_easr_mfe_cmvn.dat");
#else
        const Config config = local.config();
        sdkConfig = make_shared<SDKConfig>(config.appid(), config.appname(),
                                           config.chunkkey(), make_shared<ProductID>(config.productid()));
        sdkConfig->setDNNDatFilePath(config.dnndatfilepath());
        sdkConfig->setCMVNDatFilePath(config.cmvndatfilepath());
#endif
        string errorString;
        shared_ptr<SDK> sdk = SDK::getInstance(sdkConfig, errorString);

        if (sdk == nullptr) {
            std::cerr << errorString << std::endl;
            local.sendBDSDKStartFail(errorString);
            return -10;
        }
        if (!sdk->init(errorString)) {
            std::cerr << errorString << std::endl;
            local.sendBDSDKStartFail(errorString);
            return -11;
        }
        if (!sdk->start(errorString)) {
            std::cerr << errorString << std::endl;
            local.sendBDSDKStartFail(errorString);
            return -12;
        }

        sdk->setCallback(make_shared<LambdaCallback>([](shared_ptr<ReceiveMessage> message) {

        }, [](const string& errorDescription) {

        }));
        
        shared_ptr<PushMessage> pushMessage = make_shared<PushMessage>();
        vector<char> datas;
        local.receiveFrameLoop([pushMessage, &datas, sdk, &local](Frame frame) {
            datas.clear();
            for (const char& c : frame.data()) {
                datas.push_back(c);
            }
            pushMessage->setData(datas);
            string errorString;
            if (!sdk->post(pushMessage, errorString)) {
                std::cerr << errorString << std::endl;
                //TODO send error string to server
            }
        });
        SDK::cleanUp();
        return 0;
    } catch (ArgumentException& ex) {
        cerr << ex.what() << endl;
        return ex.exitCode();
    } catch (exception& ex) {
        cerr << ex.what() << endl;
        return -1;
    }
}
