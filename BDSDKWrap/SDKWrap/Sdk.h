//
// Created by mx404 on 4/8/19.
//
#pragma once

#include <memory>
#include <string>
#include "../SDKWrapper/Message/ErrorMessage.h"

namespace bds {
    class BDSpeechSDK;
    class BDSSDKMessage;
}

namespace mx404::BDSpeedSDKWrapper {
    class SDKConfig;
    class Callback;
    class InternalCaller;
}

namespace mx404::BDSpeedSDKWrapper::SDKMessage {
    class Message;
    class PushMessage;
}

namespace mx404::BDSpeedSDKWrapper {
    class SDK {
    public:
        ~SDK();
        static std::shared_ptr<SDK> getInstance(
                const std::shared_ptr<mx404::BDSpeedSDKWrapper::SDKConfig>& sdkConfig, std::string& errorDescription);

        bool post(const std::shared_ptr<mx404::BDSpeedSDKWrapper::SDKMessage::PushMessage>& message,
                std::string& errorDescription);

        bool post(const std::shared_ptr <SDKMessage::Message> &message, std::string &errorDescription);

        static void cleanUp();

        static std::string getSDKVersion();

        void setCallback(std::shared_ptr<mx404::BDSpeedSDKWrapper::Callback> callback);

        bool init(std::string& errorDescription);

        bool start(std::string& errorDescription);

        bool stop(std::string& errorDescription);

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
