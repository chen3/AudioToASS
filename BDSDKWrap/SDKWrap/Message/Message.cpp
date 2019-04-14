//
// Created by mx404 on 4/7/19.
//
#include "Message.h"

#include <cassert>

#include <BDSSDKMessage.hpp>

using mx404::BDSpeedSDKWrapper::SDKMessage::Message;
using std::string;

Message::Message(const std::string &name) {
    assert(!name.empty());
    message = std::make_shared<bds::BDSSDKMessage>(name);
}

Message::Message(bds::BDSSDKMessage *message) {
    assert(message != nullptr);
    this->message = std::shared_ptr<bds::BDSSDKMessage>(message, [](const bds::BDSSDKMessage *) {});
}

string Message::getName() const {
    return message->name;
}

void Message::set(const std::string &key, int value) {
    assert(!key.empty());
    message->set_parameter(key, value);
}

void Message::set(const std::string &key, float value) {
    assert(!key.empty());
    message->set_parameter(key, value);
}

void Message::set(const std::string &key, const std::string &value) {
    assert(!key.empty());
    message->set_parameter(key, value);
}

void Message::set(const std::string &key, const std::vector<char> &value) {
    assert(!key.empty());

    int length = value.size();
    char* datas = new char[length];
    std::shared_ptr<char> chars
            = std::shared_ptr<char>(datas, [](const char *data) { delete[] data; });
    map[key] = chars;
    for (int i = 0; i < length; ++i) {
        datas[i] = value[i];
    }

    message->set_parameter(key, datas, length);
}

void Message::set(const std::string &key, const std::vector <std::string> &value) {
    assert(!key.empty());
    assert(!value.empty());
    message->set_parameter(key, value);
}

bool Message::get(const std::string &key, int &value) {
    assert(!key.empty());
    return message->get_parameter(key, value);
}

bool Message::get(const std::string &key, float &value) {
    assert(!key.empty());
    return message->get_parameter(key, value);
}

bool Message::get(const std::string &key, std::string &value) {
    assert(!key.empty());
    return message->get_parameter(key, value);
}

bool Message::get(const std::string &key, std::vector<char> &value) {
    assert(!key.empty());
    assert(value.empty());

    const char *datas;
    int length;
    bool result = message->get_parameter(key, datas, length);
    assert(length >= 0);
    if (!result) {
        return false;
    }
    value.clear();
    for (int i = 0; i < length; ++i) {
        value.push_back(static_cast<unsigned char>(datas[i]));
    }
    return true;
}

bool Message::get(const std::string &key, std::vector <std::string> &value) {
    assert(!key.empty());
    assert(value.empty());
    return message->get_parameter(key, value);
}