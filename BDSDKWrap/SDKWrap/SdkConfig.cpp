//
// Created by mx404 on 4/8/19.
//
#include "SdkConfig.h"

#include <cassert>
#include <utility>

#include "ProductId.h"

using mx404::BDSpeedSDKWrapper::ProductID;
using mx404::BDSpeedSDKWrapper::SDKConfig;

SDKConfig::SDKConfig(const std::string &appID, const std::string &appName, const std::string &chunkKey,
                     std::shared_ptr<ProductID> productID) {
    assert(!appID.empty());
    assert(!appName.empty());
    assert(!chunkKey.empty());

    this->appID = appID;
    this->appName = appName;
    this->chunkKey = chunkKey;
    this->productID = std::move(productID);
    DNNDatFilePath = getDefaultDNNDatFilePath();
    CMVNDatFilePath = getDefaultCMVNDatFilePath();
}

std::string SDKConfig::getAppID() const {
    return appID;
}

void SDKConfig::setAppID(const std::string &appID) {
    assert(!appID.empty());
    this->appID = appID;
}

std::string SDKConfig::getAppName() const {
    return appName;
}

void SDKConfig::setAppName(const std::string &appName) {
    assert(!appName.empty());
    this->appName = appName;
}

std::string SDKConfig::getChunkKey() const {
    return chunkKey;
}

void SDKConfig::setChunkKey(const std::string &chunkKey) {
    assert(!chunkKey.empty());
    this->chunkKey = chunkKey;
}

std::shared_ptr<ProductID> SDKConfig::getProductID() const {
    return productID;
}

void SDKConfig::setProductID(std::shared_ptr<ProductID> productID) {
    this->productID = std::move(productID);
}

std::string SDKConfig::getDNNDatFilePath() const {
    return DNNDatFilePath;
}

void SDKConfig::setDNNDatFilePath(const std::string &path) {
    assert(!path.empty());
    DNNDatFilePath = path;
}

std::string SDKConfig::getCMVNDatFilePath() const {
    return CMVNDatFilePath;
}

void SDKConfig::setCMVNDatFilePath(const std::string &path) {
    assert(!path.empty());
    CMVNDatFilePath = path;
}

std::string SDKConfig::getDefaultDNNDatFilePath() {
    return defaultDNNDatFilePath;
}

void SDKConfig::setDefaultDNNDatFilePath(const std::string &path) {
    assert(!path.empty());
    defaultDNNDatFilePath = path;
}

std::string SDKConfig::getDefaultCMVNDatFilePath() {
    return defaultCMVNDatFilePath;
}

void SDKConfig::setDefaultCMVNDatFilePath(const std::string &path) {
    assert(!path.empty());
    defaultCMVNDatFilePath = path;
}

std::string SDKConfig::defaultDNNDatFilePath = "./Resources/ASRResource/BdsEasrMfeDnn.dat";
std::string SDKConfig::defaultCMVNDatFilePath = "./Resources/ASRResource/BdsEasrMfeCmvn.dat";