//
// Created by mx404 on 4/8/19.
//
#include "ErrorMessage.h"

#include <cassert>

using mx404::BDSpeedSDKWrapper::SDKMessage::ErrorMessage;

ErrorMessage::ErrorMessage(int errorCode, int errorDomain,
        const std::string& errorDescription, const std::string& serialNumber)
    : errorCode(errorCode)
    , errorDomain(errorDomain)
    , errorDescription(errorDescription)
    , serialNumber(serialNumber)
{
    assert(!errorDescription.empty());
    assert(!serialNumber.empty());
}

int ErrorMessage::getErrorCode() const {
    return errorCode;
}

int ErrorMessage::getErrorDomain() const {
    return errorDomain;
}

std::string ErrorMessage::getErrorDescription() const {
    return errorDescription;
}

std::string ErrorMessage::getSerialNumber() const {
    return serialNumber;
}
