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
    class SentenceFinishData : public ReceiveMessage {
    public:
        std::string getJsonString() const;
    private:
        explicit SentenceFinishData(std::string  jsonString);
        friend class mx404::BDSpeedSDKWrapper::SDK;

        std::string jsonString;
    };
}