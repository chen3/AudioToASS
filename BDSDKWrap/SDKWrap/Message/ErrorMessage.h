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
    class DLLPublic ErrorMessage : public ReceiveMessage {
    public:
        DLLPublic int getErrorCode() const;
        DLLPublic int getErrorDomain() const;
        DLLPublic std::string getErrorDescription() const;
        DLLPublic std::string getSerialNumber() const;
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
