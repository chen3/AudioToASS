//
// Created by mx404 on 4/8/19.
//
#pragma once

#include <vector>

#include "Message.h"

namespace mx404::BDSpeedSDKWrapper {
    class SDK;
}

namespace mx404::BDSpeedSDKWrapper::SDKMessage {
    class PushMessage {
    public:
        PushMessage();

        void setData(const std::vector<char>& value);

    private:
        Message message;
        friend class mx404::BDSpeedSDKWrapper::SDK;
    };
}
