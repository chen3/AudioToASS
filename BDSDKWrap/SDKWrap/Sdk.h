//
// Created by mx404 on 4/8/19.
//
#pragma once

#include <memory>
#include <string>
#include "./DLLPublic.h"
#include "./Message/ErrorMessage.h"

namespace bds {
    class BDSpeechSDK;
    class BDSSDKMessage;
}

namespace DLLPublic mx404::BDSpeedSDKWrapper {
    class DLLPublic SDKConfig;
    class DLLPublic Callback;
    class InternalCaller;
}

namespace DLLPublic mx404::BDSpeedSDKWrapper::SDKMessage {
    class DLLPublic Message;
    class DLLPublic PushMessage;
}

namespace DLLPublic mx404::BDSpeedSDKWrapper {
    class DLLPublic SDK {
    public:
        ~SDK();
        DLLPublic static std::shared_ptr<SDK> getInstance(
                const std::shared_ptr<mx404::BDSpeedSDKWrapper::SDKConfig>& sdkConfig, std::string& errorDescription);

        DLLPublic bool post(const std::shared_ptr<mx404::BDSpeedSDKWrapper::SDKMessage::PushMessage>& message,
                std::string& errorDescription);

        DLLPublic bool post(const std::shared_ptr <SDKMessage::Message> &message, std::string &errorDescription);

        DLLPublic static void cleanUp();

        DLLPublic static std::string getSDKVersion();

        DLLPublic void setCallback(std::shared_ptr<mx404::BDSpeedSDKWrapper::Callback> callback);

        DLLPublic bool init(std::string& errorDescription);

        DLLPublic bool start(std::string& errorDescription);

        DLLPublic bool stop(std::string& errorDescription);

    private:
        bds::BDSpeechSDK* sdk;
        SDK(bds::BDSpeechSDK* sdk, std::shared_ptr<SDKConfig> sdkConfig);

        std::shared_ptr<Callback> callback;
        std::shared_ptr<SDKConfig> config;

        friend class InternalCaller;

        void callback1(bds::BDSSDKMessage* message);

        static std::shared_ptr<SDKMessage::Message> createConfigMessage(const std::shared_ptr<SDKConfig>& sdkConfig);

        static std::shared_ptr<SDKMessage::Message> createStartMessage(const std::shared_ptr<SDKConfig>& sdkConfig);

    };
}
