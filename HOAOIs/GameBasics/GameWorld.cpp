//
//  GameWorld.cpp
//  AOIs
//
//  Created by zklgame on 4/5/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "GameWorld.hpp"
#include <iostream>

using namespace std;

GameWorld::GameWorld() {
    new(this)GameWorld(10000, 10000);
}

GameWorld::GameWorld(position_t width, position_t length): width(width), length(length), entityNum(0), hugeEntityNum(0), addMessageNum(0), moveMessageNum(0), leaveMessageNum(0) {
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
    cout << "Create  !!! GameWorld width: " << width << ", length: " << length << endl;
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n" << endl;
}

GameWorld::~GameWorld() {
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
    cout << "Destroy !!! GameWorld" << endl;
    cout << "GameObjects Num: " << this -> entityNum << endl;
    cout << "HugeGameObjects Num: " << this -> hugeEntityNum << endl;
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n" << endl;
}

bool GameWorld::addObject(GameObject *obj) {
    this -> gameObjects[obj -> id] = obj;
    entityNum ++;
    
    obj -> world = this;
    
    return this -> aoi -> addObject(obj);    
}

bool GameWorld::removeObject(GameObject *obj) {
    bool isRemoved= false;
    
    if (this -> gameObjects. find(obj -> id) != this -> gameObjects . end()) {
        this -> gameObjects . erase(obj -> id);
        entityNum --;
        
        this -> aoi -> removeObject(obj);
        
        isRemoved = true;
        
    }
    
    return isRemoved;
}

bool GameWorld::addHugeObject(HOGameObject *obj) {
    this -> hugeGameObjects[obj -> id] = obj;
    hugeEntityNum ++;
    
    obj -> world = this;
    
    return this -> aoi -> addHugeObject(obj);
}

bool GameWorld::removeHugeObject(HOGameObject *obj) {
    bool isRemoved= false;
    
    if (this -> hugeGameObjects. find(obj -> id) != this -> hugeGameObjects . end()) {
        this -> hugeGameObjects . erase(obj -> id);
        hugeEntityNum --;
        
        this -> aoi -> removeHugeObject(obj);
        
        isRemoved = true;
        
    }
    
    return isRemoved;
}

bool GameWorld::onObjectMove(GameObject *obj, position_t newPosX, position_t newPosY) {
    position_t oldPosX = obj -> posX;
    position_t oldPosY = obj -> posY;
    
    obj -> posX = newPosX;
    obj -> posY = newPosY;
    
    return this -> aoi -> onObjectMove(obj, oldPosX, oldPosY);
}

bool GameWorld::onHugeObjectMove(HOGameObject *obj, position_t newMinPosX, position_t newMinPosY, position_t newMaxPosX, position_t newMaxPosY) {
    position_t oldMinPosX = obj -> minPosX;
    position_t oldMinPosY = obj -> minPosY;
    position_t oldMaxPosX = obj -> maxPosX;
    position_t oldMaxPosY = obj -> maxPosY;
    
    obj -> minPosX = newMinPosX;
    obj -> minPosY = newMinPosY;
    obj -> maxPosX = newMaxPosX;
    obj -> maxPosY = newMaxPosY;
    
    return this -> aoi -> onHugeObjectMove(obj, oldMinPosX, oldMinPosY, oldMaxPosX, oldMaxPosY);
}



