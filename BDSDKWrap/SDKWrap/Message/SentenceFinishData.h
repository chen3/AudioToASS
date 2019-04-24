//
// Created by mx404 on 4/8/19.
//
#pragma once

#include <string>

#include "../DLLPublic.h"
#include "ReceiveMessage.hpp"

namespace mx404::BDSpeedSDKWrapper {
    class SDK;
}

namespace mx404::BDSpeedSDKWrapper::SDKMessage {
    class DLLPublic SentenceFinishData : public ReceiveMessage {
    public:
        DLLPublic std::string getJsonString() const;
    private:
        explicit SentenceFinishData(std::string  jsonString);
        friend class mx404::BDSpeedSDKWrapper::SDK;

        std::string jsonString;
    };
}