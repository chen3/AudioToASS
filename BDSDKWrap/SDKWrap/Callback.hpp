//
// Created by mx404 on 4/8/19.
//
#pragma once

#include <memory>

namespace mx404::BDSpeedSDKWrapper::SDKMessage {
    class ReceiveMessage;
}

namespace mx404::BDSpeedSDKWrapper {
    class Callback {
    public:
        virtual ~Callback() = default;
        virtual void callback(std::shared_ptr<SDKMessage::ReceiveMessage> message) = 0;
        virtual void error(const std::string& errorDescription) = 0;
    };
}
