//
//  TowerAOI.hpp
//  AOIs
//
//  Created by zklgame on 4/13/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef TowerAOI_hpp
#define TowerAOI_hpp

#include "../BaseAOI/BaseAOI.hpp"
#include "Tower.hpp"

class TowerAOI: public BaseAOI {
public:
    TowerAOI(position_t worldWidth, position_t worldHeight, position_t towerWidth, position_t towerHeight);
    virtual ~TowerAOI();
    
private:
    position_t worldWidth, worldHeight;
    position_t towerWidth, towerHeight;
    position_t towerX, towerY; // record the number of towers in X and Y
    map<position_t, map<position_t, Tower*> > towers;
    
    bool addPublisher(GameObject *obj);
    bool addSubscriber(GameObject *obj);
    bool removePublisher(GameObject *obj);
    bool removeSubscriber(GameObject *obj);
    
    bool addHugePublisher(HOGameObject *obj);
    bool addHugeSubscriber(HOGameObject *obj);
    bool removeHugePublisher(HOGameObject *obj);
    bool removeHugeSubscriber(HOGameObject *obj);

    bool onPublisherMove(GameObject *obj, position_t oldPosX, position_t oldPosY);
    bool onSubscriberMove(GameObject *obj, position_t oldPosX, position_t oldPosY);
    
    bool onHugePublisherMove(HOGameObject *obj, position_t oldMinPosX, position_t oldMinPosY, position_t oldMaxPosX, position_t oldMaxPosY);
    bool onHugeSubscriberMove(HOGameObject *obj, position_t oldMinPosX, position_t oldMinPosY, position_t oldMaxPosX, position_t oldMaxPosY);

    
    // ADD or REMOVE: find by obj
    // if state = 1, add; if state = 2, remove
    void findSubscribersInTheirRange(GameObject *obj, state_t state);
    void findPublishersInRange(GameObject *obj, state_t state);
    
    void findSubscribersInTheirRangeForHO(HOGameObject *obj, state_t state);
    void findPublishersInRangeForHO(HOGameObject *obj, state_t state);
    
    // MOVE: find by position
    map<entity_t, GameObject *> findPublishersInRange(entity_t objId, position_t posX, position_t posY, position_t range);
    
    map<entity_t, GameObject *> findPublishersInRangeForHO(entity_t objId, position_t minPosX, position_t minPosY, position_t maxPosX, position_t maxPosY, position_t range);

    
    bool isFullCoveredTower(position_t posX, position_t posY, position_t range, position_t pos1X, position_t pos1Y, position_t pos2X, position_t pos2Y);
    bool isPartialCoveredTower(position_t posX, position_t posY, position_t range, position_t pos1X, position_t pos1Y, position_t pos2X, position_t pos2Y);
    bool isCoveredBySide(position_t posX, position_t posY, position_t range, position_t pos1X, position_t pos1Y, position_t pos2X, position_t pos2Y);
    
    bool classifyTower(position_t posX, position_t posY, position_t range, int i, int j, list<Tower*> &fullCoveredTowers, list<Tower*> &partialCoveredTowers);
    
};

#endif /* TowerAOI_hpp */
