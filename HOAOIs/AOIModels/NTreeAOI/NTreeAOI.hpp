//
//  NTreeAOI.hpp
//  AOIs
//
//  Created by zklgame on 4/18/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef NTreeAOI_hpp
#define NTreeAOI_hpp

#include "../BaseAOI/BaseAOI.hpp"
#include "NTree.hpp"

class NTreeAOI: public BaseAOI {
public:
    NTreeAOI(position_t worldWidth, position_t worldHeight, entity_t towerMaxPublisherNum, position_t maxLevel);
    virtual ~NTreeAOI();

private:
    position_t worldWidth, worldHeight;
    entity_t towerMaxPublisherNum; // the max entity num in each tower

    NTree * tree;

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

    
};

#endif /* NTreeAOI_hpp */
