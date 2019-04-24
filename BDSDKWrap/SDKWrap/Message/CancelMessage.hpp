//
// Created by mx404 on 4/8/19.
//
#pragma once

#include "../DLLPublic.h"
#include "ReceiveMessage.hpp"

namespace DLLPublic mx404::BDSpeedSDKWrapper {
    class DLLPublic SDK;
}

namespace DLLPublic mx404::BDSpeedSDKWrapper::SDKMessage {
    class DLLPublic CancelMessage : public ReceiveMessage {
    private:
        DLLPublic CancelMessage() = default;
        friend class mx404::BDSpeedSDKWrapper::SDK;
    };
}
