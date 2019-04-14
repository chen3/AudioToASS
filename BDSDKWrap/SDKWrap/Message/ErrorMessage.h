//
// Created by mx404 on 4/8/19.
//
#pragma once

#include <string>

#include "ReceiveMessage.hpp"

namespace mx404::BDSpeedSDKWrapper {
    class SDK;
}

namespace mx404::BDSpeedSDKWrapper::SDKMessage {
    class ErrorMessage : public ReceiveMessage {
    public:
        int getErrorCode() const;
        int getErrorDomain() const;
        std::string getErrorDescription() const;
        std::string getSerialNumber() const;
    private:
        explicit ErrorMessage(int errorCode, int errorDomain,
                const std::string& errorDescription, const std::string& serialNumber);
        friend class mx404::BDSpeedSDKWrapper::SDK;
        int errorCode;
        int errorDomain;
        std::string errorDescription;
        std::string serialNumber;
    };
}
