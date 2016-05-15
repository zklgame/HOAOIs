//
//  HOGameObject.cpp
//  HOAOIs
//
//  Created by zklgame on 5/12/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "HOGameObject.hpp"
#include "GameWorld.hpp"

HOGameObject::HOGameObject(entity_t id, type_t type, position_t minPosX, position_t minPosY, position_t maxPosX, position_t maxPosY, position_t range): GameObject(id, type, 0, 0, range), minPosX(minPosX), minPosY(minPosY), maxPosX(maxPosX), maxPosY(maxPosY) {
    this -> isHugeObject = true;
}

HOGameObject::~HOGameObject() {
}

void HOGameObject::move(position_t newMinPosX, position_t newMinPosY, position_t newMaxPosX, position_t newMaxPosY) {
    this -> world -> onHugeObjectMove(this, newMinPosX, newMinPosY, newMaxPosX, newMaxPosY);
}
