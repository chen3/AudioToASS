//
// Created by mx404 on 4/8/19.
//

#include "PushMessage.h"

#include <cassert>

#include <BDSSDKMessage.hpp>
#include <ASR/bds_asr_key_definitions.hpp>

using mx404::BDSpeedSDKWrapper::SDKMessage::PushMessage;

PushMessage::PushMessage()
    : message(bds::ASR_CMD_PUSH_AUDIO)
{
}

void PushMessage::setData(const std::vector<char> &value) {
    assert(!value.empty());
    message.set(bds::DATA_CHUNK, value);
}