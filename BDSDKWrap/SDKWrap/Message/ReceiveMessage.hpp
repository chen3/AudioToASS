//
// Created by mx404 on 4/8/19.
//
#pragma once

#include "../DLLPublic.h"

namespace mx404::BDSpeedSDKWrapper::SDKMessage {
    class DLLPublic ReceiveMessage {
    public:
        DLLPublic virtual ~ReceiveMessage() = default;
    protected:
        DLLPublic ReceiveMessage() = default;
    };
}
