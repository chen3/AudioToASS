#include <utility>

//
// Created by mx404 on 4/8/19.
//
#include "SentenceFlushData.h"

using mx404::BDSpeedSDKWrapper::SDKMessage::SentenceFlushData;

SentenceFlushData::SentenceFlushData(std::string  jsonString)
    : jsonString(std::move(jsonString))
{
}

std::string SentenceFlushData::getJsonString() const {
    return jsonString;
}