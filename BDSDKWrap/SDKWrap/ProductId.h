//
// Created by mx404 on 4/8/19.
//
#pragma once

#include "./DLLPublic.h"

namespace mx404::BDSpeedSDKWrapper {
    class DLLPublic ProductID {
    public:
        DLLPublic explicit ProductID(int id);

        DLLPublic int getID() const;

    private:
        int id;
    };
}
