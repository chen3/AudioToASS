//
// Created by mx404 on 4/8/19.
//
#include "DefaultCallback.h"

#include <iostream>

#include "../SDKWrapper/Message/CancelMessage.hpp"
#include "../SDKWrapper/Message/ErrorMessage.h"
#include "../SDKWrapper/Message/ReceiveMessage.hpp"
#include "../SDKWrapper/Message/RecognitionEndMessage.hpp"
#include "../SDKWrapper/Message/RecognitionStartMessage.hpp"
#include "../SDKWrapper/Message/SentenceEndMessage.hpp"
#include "../SDKWrapper/Message/SentenceFinishData.h"
#include "../SDKWrapper/Message/SentenceFlushData.h"
#include "../SDKWrapper/Message/SentenceStartMessage.hpp"
#include "../SDKWrapper/Message/UnknowMessage.h"


using mx404::BDSpeedSDKWrapper::DefaultCallback;
using mx404::BDSpeedSDKWrapper::SDKMessage::CancelMessage;
using mx404::BDSpeedSDKWrapper::SDKMessage::ErrorMessage;
using mx404::BDSpeedSDKWrapper::SDKMessage::ReceiveMessage;
using mx404::BDSpeedSDKWrapper::SDKMessage::RecognitionEndMessage;
using mx404::BDSpeedSDKWrapper::SDKMessage::RecognitionStartMessage;
using mx404::BDSpeedSDKWrapper::SDKMessage::SentenceEndMessage;
using mx404::BDSpeedSDKWrapper::SDKMessage::SentenceFinishData;
using mx404::BDSpeedSDKWrapper::SDKMessage::SentenceFlushData;
using mx404::BDSpeedSDKWrapper::SDKMessage::SentenceStartMessage;
using mx404::BDSpeedSDKWrapper::SDKMessage::UnknowMessage;

void DefaultCallback::error(const std::string &errorDescription) {
    std::cerr << "error:" << errorDescription << std::endl;
}

void DefaultCallback::callback(std::shared_ptr<ReceiveMessage> message) {
    std::shared_ptr<ErrorMessage> msg1 = std::dynamic_pointer_cast<ErrorMessage>(message);
    if (msg1 != nullptr) {
        std::cerr << "ErrorMessage, code:" << msg1->getErrorCode()
                  << ", error domain:" << msg1->getErrorDomain()
                  << ", error description:" << msg1->getErrorDescription()
                  << ", serial number:" << msg1->getSerialNumber()
                  << std::endl;
        return;
    }
    std::shared_ptr<UnknowMessage> msg2 = std::dynamic_pointer_cast<UnknowMessage>(message);
    if (msg2 != nullptr) {
        std::cout << "UnknowMessage, status code:" << msg2->getStatusCode() << std::endl;
        return;
    }
    if (std::dynamic_pointer_cast<CancelMessage>(message) != nullptr) {
        std::cout << "CancelMessage" << std::endl;
        return;
    }
    if (std::dynamic_pointer_cast<RecognitionEndMessage>(message) != nullptr) {
        std::cout << "RecognitionEndMessage" << std::endl;
        return;
    }
    if (std::dynamic_pointer_cast<RecognitionStartMessage>(message) != nullptr) {
        std::cout << "RecognitionStartMessage" << std::endl;
        return;
    }
    if (std::dynamic_pointer_cast<SentenceEndMessage>(message) != nullptr) {
        std::cout << "SentenceEndMessage" << std::endl;
        return;
    }
    std::shared_ptr<SentenceFinishData> msg3 = std::dynamic_pointer_cast<SentenceFinishData>(message);
    if (msg3 != nullptr) {
        std::cout << "SentenceFinishData, json:" << msg3->getJsonString() << std::endl;
        return;
    }
    std::shared_ptr<SentenceFlushData> msg4 = std::dynamic_pointer_cast<SentenceFlushData>(message);
    if (msg3 != nullptr) {
        std::cout << "SentenceFlushData, json:" << msg4->getJsonString() << std::endl;
        return;
    }
    if (std::dynamic_pointer_cast<SentenceStartMessage>(message) != nullptr) {
        std::cout << "SentenceStartMessage" << std::endl;
        return;
    }
}

std::shared_ptr<DefaultCallback> DefaultCallback::getInstance() {
    return instance;
}

std::shared_ptr<DefaultCallback> DefaultCallback::instance = std::shared_ptr<DefaultCallback>(new DefaultCallback());