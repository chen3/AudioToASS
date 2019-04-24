//
// Created by mx404 on 4/8/19.
//
#pragma once

#include <string>

#include "../DLLPublic.h"
#include "ReceiveMessage.hpp"

namespace DLLPublic mx404::BDSpeedSDKWrapper {
    class DLLPublic SDK;
}

namespace DLLPublic mx404::BDSpeedSDKWrapper::SDKMessage {
    class DLLPublic SentenceFlushData : public ReceiveMessage {
    public:
        DLLPublic std::string getJsonString() const;
    private:
        explicit SentenceFlushData(std::string  jsonString);
        friend class mx404::BDSpeedSDKWrapper::SDK;

        std::string jsonString;
    };
}
