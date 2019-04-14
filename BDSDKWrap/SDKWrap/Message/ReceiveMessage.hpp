//
// Created by mx404 on 4/8/19.
//
#pragma once

namespace mx404::BDSpeedSDKWrapper::SDKMessage {
    class ReceiveMessage {
    public:
        virtual ~ReceiveMessage() = default;
    protected:
        ReceiveMessage() = default;
    };
}
