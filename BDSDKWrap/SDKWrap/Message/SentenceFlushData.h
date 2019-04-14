//
// Created by mx404 on 4/8/19.
//
#pragma once

#include <string>

#include "ReceiveMessage.hpp"

namespace mx404::BDSpeedSDKWrapper {
    class SDK;
}

namespace mx404::BDSpeedSDKWrapper::SDKMessage {
    class SentenceFlushData : public ReceiveMessage {
    public:
        std::string getJsonString() const;
    private:
        explicit SentenceFlushData(std::string  jsonString);
        friend class mx404::BDSpeedSDKWrapper::SDK;

        std::string jsonString;
    };
}
