//
// Created by mx404 on 4/7/19.
//
#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

namespace bds {
    class BDSSDKMessage;
}

namespace mx404::BDSpeedSDKWrapper {
    class SDK;
}

namespace mx404::BDSpeedSDKWrapper::SDKMessage {
    class Message {
    public:
        explicit Message(const std::string &name);

        virtual ~Message() = default;

        std::string getName() const;

        void set(const std::string &key, int value);
        void set(const std::string &key, float value);
        void set(const std::string &key, const std::string &value);
        void set(const std::string &key, const std::vector<char> &value);
        void set(const std::string &key, const std::vector <std::string> &value);

        bool get(const std::string &key, int &value);
        bool get(const std::string &key, float &value);
        bool get(const std::string &key, std::string &value);
        bool get(const std::string &key, std::vector<char> &value);
        bool get(const std::string &key, std::vector <std::string> &value) ;

    protected:
        std::shared_ptr <bds::BDSSDKMessage> message;
        friend class mx404::BDSpeedSDKWrapper::SDK;
        explicit Message(bds::BDSSDKMessage *message);

    private:
        std::unordered_map <std::string, std::shared_ptr<char>> map;
    };

}