//
// Created by mx404 on 4/8/19.
//
#pragma once

namespace mx404::BDSpeedSDKWrapper {
    class ProductID {
    public:
        explicit ProductID(int id);
        int getID() const;
    private:
        int id;
    };
}
