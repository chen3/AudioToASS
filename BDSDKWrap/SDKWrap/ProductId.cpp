//
// Created by mx404 on 4/8/19.
//
#include "ProductId.h"

using mx404::BDSpeedSDKWrapper::ProductID;

ProductID::ProductID(int id)
    : id(id)
{
}

int ProductID::getID() const {
    return id;
}