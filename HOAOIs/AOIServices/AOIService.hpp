//
//  AOIService.hpp
//  AOIs
//
//  Created by zklgame on 4/5/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef AOIService_hpp
#define AOIService_hpp

#include "../AOIModels/BaseAOI/BaseAOI.hpp"

#include <iostream>
using namespace::std;

// the BASE class of all the AoiServiceClasses
class AOIService {
public:
    AOIService() {};
    virtual ~AOIService() {};
    
    virtual bool addObject(GameObject *obj) {
        return this -> aoi -> addObject(obj);
    }
    virtual bool removeObject(GameObject *obj) {
        return this -> aoi -> removeObject(obj);
    }
    virtual bool onObjectMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
        return this -> aoi -> onObjectMove(obj, oldPosX, oldPosY);
    }
    
    virtual bool addHugeObject(HOGameObject *obj) {
        return this -> aoi -> addHugeObject(obj);
    }
    virtual bool removeHugeObject(HOGameObject *obj) {
        return this -> aoi -> removeHugeObject(obj);
    }
    virtual bool onHugeObjectMove(HOGameObject *obj, position_t oldMinPosX, position_t oldMinPosY, position_t oldMaxPosX, position_t oldMaxPosY) {
        return this -> aoi -> onHugeObjectMove(obj, oldMinPosX, oldMinPosY, oldMaxPosX, oldMaxPosY);
    }
    
protected:
    BaseAOI *aoi;
};

#endif /* AOIService_hpp */
