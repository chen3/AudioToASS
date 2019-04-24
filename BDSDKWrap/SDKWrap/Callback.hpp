//
// Created by mx404 on 4/8/19.
//
#pragma once

#include <memory>

#include "./DLLPublic.h"

namespace mx404::BDSpeedSDKWrapper::SDKMessage {
    class ReceiveMessage;
}

namespace mx404::BDSpeedSDKWrapper {
    class DLLPublic Callback {
    public:
        DLLPublic virtual ~Callback() = default;
        DLLPublic virtual void callback(std::shared_ptr<SDKMessage::ReceiveMessage> message) = 0;
        DLLPublic virtual void error(const std::string& errorDescription) = 0;
    };
}
