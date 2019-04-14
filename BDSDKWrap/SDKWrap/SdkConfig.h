//
// Created by mx404 on 4/8/19.
//
#pragma once

#include <memory>
#include <string>

namespace mx404::BDSpeedSDKWrapper {

    class ProductID;

    class SDKConfig {
    public:
        SDKConfig(const std::string& appID, const std::string& appName,
                  const std::string& chunkKey, std::shared_ptr<ProductID> productID);

        std::string getAppID() const;

        void setAppID(const std::string& appID);

        std::string getAppName() const;

        void setAppName(const std::string& appName);

        std::string getChunkKey() const;

        void setChunkKey(const std::string& chunkKey);

        std::shared_ptr<ProductID> getProductID() const;

        void setProductID(std::shared_ptr<ProductID> productID);

        std::string getDNNDatFilePath() const;

        void setDNNDatFilePath(const std::string& path);

        std::string getCMVNDatFilePath() const;

        void setCMVNDatFilePath(const std::string& path);

        static std::string getDefaultDNNDatFilePath();

        static void setDefaultDNNDatFilePath(const std::string& path);

        static std::string getDefaultCMVNDatFilePath();

        static void setDefaultCMVNDatFilePath(const std::string& path);

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
