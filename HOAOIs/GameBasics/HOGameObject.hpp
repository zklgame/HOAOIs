//
//  HOGameObject.hpp
//  HOAOIs
//
//  Created by zklgame on 5/12/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef HOGameObject_hpp
#define HOGameObject_hpp

#include "GameObject.hpp"

class GameWorld;

class NTree;

class HOGameObject: public GameObject {
public:
    HOGameObject(entity_t id, type_t type, position_t minPosX, position_t minPosY, position_t maxPosX, position_t maxPosY, position_t range);
    virtual ~HOGameObject();
    
    int32_t minPosX, minPosY, maxPosX, maxPosY;
    
    void move(position_t newMinPosX, position_t newMinPosY, position_t newMaxPosX, position_t newMaxPosY);

    
};


#endif /* HOGameObject_hpp */
