//
// Created by mx404 on 4/8/19.
//
#include "Sdk.h"

#include <cassert>

#include <ASR/bds_asr_key_definitions.hpp>
#include <ASR/bds_ASRDefines.hpp>
#include <BDSpeechSDK.hpp>
#include <BDSSDKMessage.hpp>
#include <utility>

#include "Callback.hpp"
#include "DefaultCallback.h"
#include "./Message/CancelMessage.hpp"
#include "./Message/ErrorMessage.h"
#include "./Message/ReceiveMessage.hpp"
#include "./Message/RecognitionEndMessage.hpp"
#include "./Message/RecognitionStartMessage.hpp"
#include "./Message/Message.h"
#include "./Message/PushMessage.h"
#include "./Message/SentenceEndMessage.hpp"
#include "./Message/SentenceFinishData.h"
#include "./Message/SentenceFlushData.h"
#include "./Message/SentenceStartMessage.hpp"
#include "./Message/UnknowMessage.h"
#include "ProductId.h"
#include "SdkConfig.h"

using mx404::BDSpeedSDKWrapper::Callback;
using mx404::BDSpeedSDKWrapper::DefaultCallback;
using mx404::BDSpeedSDKWrapper::ProductID;
using mx404::BDSpeedSDKWrapper::SDK;
using mx404::BDSpeedSDKWrapper::SDKMessage::CancelMessage;
using mx404::BDSpeedSDKWrapper::SDKMessage::ErrorMessage;
using mx404::BDSpeedSDKWrapper::SDKMessage::Message;
using mx404::BDSpeedSDKWrapper::SDKMessage::ReceiveMessage;
using mx404::BDSpeedSDKWrapper::SDKMessage::RecognitionStartMessage;
using mx404::BDSpeedSDKWrapper::SDKMessage::RecognitionEndMessage;
using mx404::BDSpeedSDKWrapper::SDKMessage::SentenceStartMessage;
using mx404::BDSpeedSDKWrapper::SDKMessage::SentenceFlushData;
using mx404::BDSpeedSDKWrapper::SDKMessage::SentenceFinishData;
using mx404::BDSpeedSDKWrapper::SDKMessage::SentenceEndMessage;
using mx404::BDSpeedSDKWrapper::SDKMessage::PushMessage;
using mx404::BDSpeedSDKWrapper::SDKMessage::UnknowMessage;

namespace mx404::BDSpeedSDKWrapper {
    class InternalCaller {
    public:
        static void callback(bds::BDSSDKMessage& message, void* param) {
            assert(param != nullptr);
            static_cast<SDK*>(param)->callback1(&message);
        }
    };
}

SDK::~SDK() {
    bds::BDSpeechSDK::release_instance(sdk);
}

std::shared_ptr<SDK> SDK::getInstance(const std::shared_ptr <SDKConfig> &sdkConfig, std::string &errorDescription) {
    bds::BDSpeechSDK* sdk =  bds::BDSpeechSDK::get_instance(bds::SDK_TYPE_ASR, errorDescription);
    return sdk == nullptr ? std::shared_ptr<SDK>() : std::shared_ptr<SDK>(new SDK(sdk, sdkConfig));
}

bool SDK::post(const std::shared_ptr <PushMessage> &message, std::string &errorDescription) {
    return sdk->post(*(message->message.message.get()), errorDescription);
}

bool SDK::post(const std::shared_ptr<Message> &message, std::string &errorDescription) {
    return sdk->post(*(message->message.get()), errorDescription);
}

void SDK::cleanUp() {
    bds::BDSpeechSDK::do_cleanup();
}

std::string SDK::getSDKVersion() {
    return bds::BDSpeechSDK::get_sdk_version();
}

void SDK::setCallback(std::shared_ptr <Callback> callback) {
    this->callback = std::move(callback);
}

bool SDK::init(std::string &errorDescription) {
    std::shared_ptr<Message> configMessage = createConfigMessage(config);
    return post(configMessage, errorDescription);
}

bool SDK::start(std::string &errorDescription) {
    std::shared_ptr<Message> startMessage = createStartMessage(config);
    return post(startMessage, errorDescription);
}

bool SDK::stop(std::string &errorDescription) {
    std::shared_ptr<Message> emptyMessage = std::make_shared<Message>(bds::ASR_CMD_PUSH_AUDIO);
    emptyMessage->set(bds::DATA_CHUNK, std::vector<char>());
    std::string error1;
    bool result1 = post(emptyMessage, error1);
    std::string error2;
    bool result2 = post(std::make_shared<Message>(bds::ASR_CMD_STOP), error2);
    bool result = result1 && result2;
    if (result1) {
        if (!result2) {
            errorDescription = error2;
        }
    } else if (result2) {
        errorDescription = error1;
    } else {
        errorDescription = error1 + "\n" + error2;
    }
    return result;
}

SDK::SDK(bds::BDSpeechSDK* sdk, std::shared_ptr<SDKConfig> sdkConfig)
    : config(sdkConfig)
    , sdk(sdk)
{
    assert(sdk != nullptr);
    assert(sdkConfig != nullptr);
    sdk->set_event_listener(mx404::BDSpeedSDKWrapper::InternalCaller::callback, this);
    setCallback(std::shared_ptr<Callback>(DefaultCallback::getInstance()));
}

void SDK::callback1(bds::BDSSDKMessage *message) {
    if (callback == nullptr) {
        return;
    }
    if(message->name != bds::asr_callback_name) {
        callback->error(std::string("message name(") + message->name + ") not is bds::asr_callback_name(" + bds::asr_callback_name + ")");
        return;
    }
    int statusCode;
    if (!message->get_parameter(bds::CALLBACK_ASR_STATUS, statusCode)) {
        callback->error("Failure to acquire status code");
        return;
    }
    std::shared_ptr<ReceiveMessage> msg;
    switch (statusCode) {
        case bds::EVoiceRecognitionClientWorkStatusStartWorkIng: {
            msg = std::dynamic_pointer_cast<ReceiveMessage>(
                    std::shared_ptr<RecognitionStartMessage>(new RecognitionStartMessage()));
        } break;
        case bds::EVoiceRecognitionClientWorkStatusLongSpeechEnd: {
            msg = std::dynamic_pointer_cast<ReceiveMessage>(
                    std::shared_ptr<RecognitionEndMessage>(new RecognitionEndMessage()));
        } break;
        case bds::EVoiceRecognitionClientWorkStatusStart: {
            msg = std::dynamic_pointer_cast<ReceiveMessage>(
                    std::shared_ptr<SentenceStartMessage>(new SentenceStartMessage()));
        } break;
        case bds::EVoiceRecognitionClientWorkStatusEnd: {
            msg = std::dynamic_pointer_cast<ReceiveMessage>(
                    std::shared_ptr<SentenceEndMessage>(new SentenceEndMessage()));
        } break;
        case bds::EVoiceRecognitionClientWorkStatusFlushData: {
            std::string jsonString;
            if (!message->get_parameter(bds::CALLBACK_ASR_RESULT, jsonString)) {
                callback->error("get flush data json fail");
                return;
            }
            msg = std::dynamic_pointer_cast<ReceiveMessage>(
                    std::shared_ptr<SentenceFlushData>(new SentenceFlushData(jsonString)));
        } break;
        case bds::EVoiceRecognitionClientWorkStatusFinish: {
            std::string jsonString;
            if (!message->get_parameter(bds::CALLBACK_ASR_RESULT, jsonString)) {
                callback->error("get finish data json fail");
                return;
            }
            msg = std::dynamic_pointer_cast<ReceiveMessage>(
                    std::shared_ptr<SentenceFinishData>(new SentenceFinishData(jsonString)));
        } break;
        case bds::EVoiceRecognitionClientWorkStatusCancel: {
            msg = std::dynamic_pointer_cast<ReceiveMessage>(
                    std::shared_ptr<CancelMessage>(new CancelMessage()));
        } break;
        case bds::EVoiceRecognitionClientWorkStatusError: {
            int errorCode;
            if (!message->get_parameter(bds::CALLBACK_ERROR_CODE, errorCode)) {
                callback->error("get error code fail");
                return;
            }
            int errorDomain;
            if (!message->get_parameter(bds::CALLBACK_ERROR_DOMAIN, errorDomain)) {
                callback->error("get error domain fail, code:" + std::to_string(errorCode));
                return;
            }
            std::string errorDescription;
            if (!message->get_parameter(bds::CALLBACK_ERROR_DESC, errorDescription)) {
                callback->error("get error description fail, code:" + std::to_string(errorCode)
                                + ", domain:" + std::to_string(errorDomain));
                return;
            }
            std::string serialNumber;
            if (!message->get_parameter(bds::CALLBACK_ERROR_SERIAL_NUM, serialNumber)) {
                callback->error("get error serial number fail, code:" + std::to_string(errorCode)
                                + ", domain:" + std::to_string(errorDomain) + ", description:" + errorDescription);
                return;
            }
            msg = std::dynamic_pointer_cast<ReceiveMessage>(
                    std::shared_ptr<ErrorMessage>(
                            new ErrorMessage(errorCode, errorDomain, errorDescription, serialNumber)));
        } break;
        default: {
            std::shared_ptr<Message> m(new Message(message));
            msg = std::dynamic_pointer_cast<ReceiveMessage>(
                    std::shared_ptr<UnknowMessage>(new UnknowMessage(m, statusCode)));
        }
    }
    callback->callback(msg);
}

std::shared_ptr<Message> SDK::createConfigMessage(const std::shared_ptr <SDKConfig> &sdkConfig) {
    std::shared_ptr<Message> configMessage = std::make_shared<Message>(bds::ASR_CMD_CONFIG);
    configMessage->set(bds::ASR_PARAM_KEY_APP_ID, sdkConfig->getAppID());
    configMessage->set(bds::ASR_PARAM_KEY_CHUNK_KEY, sdkConfig->getChunkKey());

    configMessage->set(bds::ASR_PARAM_KEY_PRODUCT_ID, std::to_string(sdkConfig->getProductID()->getID()));

    configMessage->set(bds::ASR_PARAM_KEY_ENABLE_LONG_SPEECH, 1);
    configMessage->set(bds::ASR_PARAM_KEY_CHUNK_ENABLE, 1);

    configMessage->set(bds::ASR_PARAM_KEY_MFE_DNN_DAT_FILE, sdkConfig->getDNNDatFilePath());
    configMessage->set(bds::ASR_PARAM_KEY_MFE_CMVN_DAT_FILE, sdkConfig->getCMVNDatFilePath());

    configMessage->set(bds::ASR_PARAM_KEY_COMPRESSION_TYPE,
                       bds::TBDVoiceRecognitionAudioCompressionType::EVR_AUDIO_COMPRESSION_PCM);
    return configMessage;
}

std::shared_ptr<Message> SDK::createStartMessage(const std::shared_ptr <SDKConfig> &sdkConfig) {
    std::shared_ptr<Message> startMessage = std::make_shared<Message>(bds::ASR_CMD_START);
    startMessage->set(bds::ASR_PARAM_KEY_APP, sdkConfig->getAppName());
    startMessage->set(bds::ASR_PARAM_KEY_PLATFORM, "linux");
    startMessage->set(bds::ASR_PARAM_KEY_SDK_VERSION, "LINUX TEST");
    return startMessage;
}