//
//  GameWorld.hpp
//  AOIs
//
//  Created by zklgame on 4/5/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef GameWorld_hpp
#define GameWorld_hpp

#include "GameObject.hpp"
#include "../AOIServices/AOIService.hpp"

#include <map>

using namespace std;

class GameWorld {
    
public:
    GameWorld();
    GameWorld(position_t width, position_t length);
    virtual ~GameWorld();
    
    position_t width;
    position_t length;
    
    entity_t entityNum;
    entity_t hugeEntityNum;
    
    AOIService * aoi;
    
    // Suppose GameWorld gets GameObject* from the LoginApp
    // And the GameWorld do nothing with GameObject's creation or destruction
    bool addObject(GameObject *obj);
    bool removeObject(GameObject *obj);
    
    bool addHugeObject(HOGameObject *obj);
    bool removeHugeObject(HOGameObject *obj);
    
    bool onObjectMove(GameObject *obj, position_t newPosX, position_t newPosY);
    bool onHugeObjectMove(HOGameObject *obj, position_t newMinPosX, position_t newMinPosY, position_t newMaxPosX, position_t newMaxPosY);
    
    // for test
    entity_t addMessageNum;
    entity_t leaveMessageNum;
    entity_t moveMessageNum;
    
private:
    map<entity_t, GameObject*> gameObjects;
    map<entity_t, HOGameObject *> hugeGameObjects;
    
};

#endif /* GameWorld_hpp */
