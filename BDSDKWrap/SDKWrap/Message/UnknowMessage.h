//
// Created by mx404 on 4/8/19.
//
#pragma once

#include <memory>

#include "ReceiveMessage.hpp"

namespace mx404::BDSpeedSDKWrapper {
    class SDK;
}

namespace mx404::BDSpeedSDKWrapper::SDKMessage {

    class Message;

    class UnknowMessage : public ReceiveMessage {
    public:
        int getStatusCode() const;
        std::shared_ptr<mx404::BDSpeedSDKWrapper::SDKMessage::Message> getMessage() const;
    private:
        explicit UnknowMessage(std::shared_ptr<Message>  message, int statusCode);
        friend class mx404::BDSpeedSDKWrapper::SDK;

        int statusCode;
        std::shared_ptr<Message> message;
    };
}