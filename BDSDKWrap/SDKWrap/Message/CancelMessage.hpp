//
// Created by mx404 on 4/8/19.
//
#pragma once

#include "ReceiveMessage.hpp"

namespace mx404::BDSpeedSDKWrapper {
    class SDK;
}

namespace mx404::BDSpeedSDKWrapper::SDKMessage {
    class CancelMessage : public ReceiveMessage {
    private:
        CancelMessage() = default;
        friend class mx404::BDSpeedSDKWrapper::SDK;
    };
}
