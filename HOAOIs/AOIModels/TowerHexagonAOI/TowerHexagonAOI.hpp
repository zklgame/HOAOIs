//
//  TowerHexagonAOI.hpp
//  AOIs
//
//  Created by zklgame on 4/15/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef TowerHexagonAOI_hpp
#define TowerHexagonAOI_hpp

#include "../BaseAOI/BaseAOI.hpp"
#include "../TowerAOI/Tower.hpp"

class TowerHexagonAOI: public BaseAOI {
public:
    TowerHexagonAOI(position_t worldWidth, position_t worldHeight, position_t towerLength);
    virtual ~TowerHexagonAOI();
    
private:
    position_t worldWidth, worldHeight;
    position_t towerLength;
    position_t towerX, towerY; // record the number of towers in X and Y
    map<position_t, map<position_t, Tower*> > towers;
    
    bool addPublisher(GameObject *obj);
    bool addSubscriber(GameObject *obj);
    bool removePublisher(GameObject *obj);
    bool removeSubscriber(GameObject *obj);

    bool onPublisherMove(GameObject *obj, position_t oldPosX, position_t oldPosY);
    bool onSubscriberMove(GameObject *obj, position_t oldPosX, position_t oldPosY);
    
    // ADD or REMOVE: find by obj
    // if state = 1, add; if state = 2, remove
    void findSubscribersInTheirRange(GameObject *obj, state_t state);
    void findPublishersInRange(GameObject *obj, state_t state);
    
    // MOVE: find by position
    map<entity_t, GameObject *> findPublishersInRange(entity_t objId, position_t posX, position_t posY, position_t range);

    bool isFullCoveredTower(position_t posX, position_t posY, position_t range, double centerX, double centerY);
    bool isPartialCoveredTower(position_t posX, position_t posY, position_t range, double centerX, double centerY);
    bool isCoveredBySide(position_t posX, position_t posY, position_t range, double pos1X, double pos1Y, double pos2X, double pos2Y);
    
    bool classifyTower(position_t posX, position_t posY, position_t range, int i, int j, list<Tower*> &fullCoveredTowers, list<Tower*> &partialCoveredTowers);
    
    void getTowerPosByXY(position_t posX, position_t posY, position_t &i, position_t &j);
    double getDistance(double posX, double posY, double anotherPosX, double anotherPosY);
    void getCenterByTowerPos(position_t i, position_t j, double &centerX, double &centerY);
};

#endif /* TowerHexagonAOI_hpp */
