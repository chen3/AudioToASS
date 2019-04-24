//
// Created by mx404 on 4/8/19.
//
#pragma once

#include "../DLLPublic.h"
#include "ReceiveMessage.hpp"

namespace mx404::BDSpeedSDKWrapper {
    class SDK;
}

namespace mx404::BDSpeedSDKWrapper::SDKMessage {
    class DLLPublic CancelMessage : public ReceiveMessage {
    private:
        DLLPublic CancelMessage() = default;
        friend class mx404::BDSpeedSDKWrapper::SDK;
    };
}
