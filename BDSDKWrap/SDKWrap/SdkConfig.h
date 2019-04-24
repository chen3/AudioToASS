//
// Created by mx404 on 4/8/19.
//
#pragma once

#include <memory>
#include <string>

#include "./DLLPublic.h"

namespace mx404::BDSpeedSDKWrapper {

    class ProductID;

    class DLLPublic SDKConfig {
    public:
        DLLPublic SDKConfig(const std::string& appID, const std::string& appName,
                  const std::string& chunkKey, std::shared_ptr<ProductID> productID);

        DLLPublic std::string getAppID() const;

        DLLPublic void setAppID(const std::string& appID);

        DLLPublic std::string getAppName() const;

        DLLPublic void setAppName(const std::string& appName);

        DLLPublic std::string getChunkKey() const;

        DLLPublic void setChunkKey(const std::string& chunkKey);

        DLLPublic std::shared_ptr<ProductID> getProductID() const;

        DLLPublic void setProductID(std::shared_ptr<ProductID> productID);

        DLLPublic std::string getDNNDatFilePath() const;

        DLLPublic void setDNNDatFilePath(const std::string& path);

        DLLPublic std::string getCMVNDatFilePath() const;

        DLLPublic void setCMVNDatFilePath(const std::string& path);

        DLLPublic static std::string getDefaultDNNDatFilePath();

        DLLPublic static void setDefaultDNNDatFilePath(const std::string& path);

        DLLPublic static std::string getDefaultCMVNDatFilePath();

        DLLPublic static void setDefaultCMVNDatFilePath(const std::string& path);

    private:
        static std::string defaultDNNDatFilePath;
        static std::string defaultCMVNDatFilePath;

        std::string appID;
        std::string appName;
        std::string chunkKey;
        std::shared_ptr<ProductID> productID;
        std::string DNNDatFilePath;
        std::string CMVNDatFilePath;
    };
}
