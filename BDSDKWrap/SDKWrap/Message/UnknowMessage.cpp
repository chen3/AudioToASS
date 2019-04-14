//
// Created by mx404 on 4/8/19.
//
#include "UnknowMessage.h"

using mx404::BDSpeedSDKWrapper::SDKMessage::Message;
using mx404::BDSpeedSDKWrapper::SDKMessage::UnknowMessage;

UnknowMessage::UnknowMessage(std::shared_ptr<Message>  message, int statusCode)
    : message(std::move(message))
    , statusCode(statusCode)
{
}

int UnknowMessage::getStatusCode() const {
    return statusCode;
}

std::shared_ptr<Message> UnknowMessage::getMessage() const {
    return message;
}
