//
// Created by mx404 on 4/8/19.
//
#pragma once

#include "ReceiveMessage.hpp"

namespace mx404::BDSpeedSDKWrapper {
    class SDK;
}

namespace mx404::BDSpeedSDKWrapper::SDKMessage {
    class SentenceEndMessage : public ReceiveMessage {
    private:
        SentenceEndMessage() = default;
        friend class mx404::BDSpeedSDKWrapper::SDK;
    };
}