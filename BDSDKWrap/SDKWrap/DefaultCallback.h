//
// Created by mx404 on 4/8/19.
//
#pragma once

#include "Callback.hpp"

namespace mx404::BDSpeedSDKWrapper {
    class DefaultCallback : public Callback {
    public:
        void callback(std::shared_ptr<SDKMessage::ReceiveMessage> message);
        void error(const std::string& errorDescription);

        static std::shared_ptr<DefaultCallback> getInstance();

    private:
        DefaultCallback() = default;
        static std::shared_ptr<DefaultCallback> instance;
    };
}
