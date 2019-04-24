//
// Created by mx404 on 4/7/19.
//
#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

#include "../DLLPublic.h"

namespace bds {
    class BDSSDKMessage;
}

namespace DLLPublic mx404::BDSpeedSDKWrapper {
    class DLLPublic SDK;
}

namespace DLLPublic mx404::BDSpeedSDKWrapper::SDKMessage {
    class DLLPublic Message {
    public:
        DLLPublic explicit Message(const std::string &name);

        DLLPublic virtual ~Message() = default;

        DLLPublic std::string getName() const;

        DLLPublic void set(const std::string &key, int value);
        DLLPublic void set(const std::string &key, float value);
        DLLPublic void set(const std::string &key, const std::string &value);
        DLLPublic void set(const std::string &key, const std::vector<char> &value);
        DLLPublic void set(const std::string &key, const std::vector <std::string> &value);

        DLLPublic bool get(const std::string &key, int &value);
        DLLPublic bool get(const std::string &key, float &value);
        DLLPublic bool get(const std::string &key, std::string &value);
        DLLPublic bool get(const std::string &key, std::vector<char> &value);
        DLLPublic bool get(const std::string &key, std::vector <std::string> &value) ;

    protected:
        std::shared_ptr <bds::BDSSDKMessage> message;
        friend class mx404::BDSpeedSDKWrapper::SDK;
        explicit Message(bds::BDSSDKMessage *message);

    private:
        std::unordered_map <std::string, std::shared_ptr<char>> map;
    };

}