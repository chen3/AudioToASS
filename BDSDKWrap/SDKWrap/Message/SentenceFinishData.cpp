//
// Created by mx404 on 4/8/19.
//
#include "SentenceFinishData.h"

using mx404::BDSpeedSDKWrapper::SDKMessage::SentenceFinishData;

SentenceFinishData::SentenceFinishData(std::string jsonString)
    : jsonString(std::move(jsonString))
{
}

std::string SentenceFinishData::getJsonString() const {
    return jsonString;
}