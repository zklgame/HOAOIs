//
//  TowerAOI.cpp
//  AOIs
//
//  Created by zklgame on 4/13/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "TowerAOI.hpp"

#include <cmath>

TowerAOI::TowerAOI(position_t worldWidth, position_t worldHeight, position_t towerWidth, position_t towerHeight): worldWidth(worldWidth), worldHeight(worldHeight), towerWidth(towerWidth), towerHeight(towerHeight) {
    // tower range: [ ... )
    this -> towerX = worldWidth / towerWidth + 1;
    this -> towerY = worldHeight / towerHeight + 1;
    
    for (position_t i = 0; i < this -> towerX; i ++) {
        for (position_t j = 0; j < this -> towerY; j ++) {
            this -> towers[i][j] = new Tower();
        }
    }
}

TowerAOI::~TowerAOI() {
    for (position_t i = 0; i < this -> towerX; i ++) {
        for (position_t j = 0; j < this -> towerY; j ++) {
            delete this -> towers[i][j];
        }
    }
}

bool TowerAOI::addPublisher(GameObject *obj) {
    position_t towerPosX = obj -> posX / this -> towerWidth;
    position_t towerPosY = obj -> posY / this -> towerHeight;
    this -> towers[towerPosX][towerPosY] -> addPublisher(obj);
    
    this -> findSubscribersInTheirRange(obj, ADD_MESSAGE);
    
    return true;
}

bool TowerAOI::addSubscriber(GameObject *obj) {

    this -> subscribers[obj -> id] = obj;
    
    this -> findPublishersInRange(obj, ADD_MESSAGE);
    
    return true;
}

bool TowerAOI::removePublisher(GameObject *obj) {
    this -> findSubscribersInTheirRange(obj, REMOVE_MESSAGE);
    
    position_t towerPosX = obj -> posX / this -> towerWidth;
    position_t towerPosY = obj -> posY / this -> towerHeight;
    this -> towers[towerPosX][towerPosY] -> removePublisher(obj);
    
    return true;
}

bool TowerAOI::removeSubscriber(GameObject *obj) {
    this -> findPublishersInRange(obj, REMOVE_MESSAGE);
    
    this -> subscribers . erase(obj -> id);
    
    return true;
}

bool TowerAOI::addHugePublisher(HOGameObject *obj) {
    position_t towerMinPosX = obj -> minPosX / this -> towerWidth;
    position_t towerMinPosY = obj -> minPosY / this -> towerHeight;
    position_t towerMaxPosX = obj -> maxPosX / this -> towerWidth;
    position_t towerMaxPosY = obj -> maxPosY / this -> towerHeight;
    
    for (position_t i = towerMinPosX; i <= towerMaxPosX; i ++) {
        for (position_t j = towerMinPosY; j <= towerMaxPosY; j ++) {
            this -> towers[i][j] -> addPublisher(obj);
        }
    }
        
    this -> findSubscribersInTheirRangeForHO(obj, ADD_MESSAGE);
    
    return true;
}

bool TowerAOI::addHugeSubscriber(HOGameObject *obj) {
    this -> hugeSubscribers[obj -> id] = obj;
    
    this -> findPublishersInRangeForHO(obj, ADD_MESSAGE);
    
    return true;
}

bool TowerAOI::removeHugePublisher(HOGameObject *obj) {
    this -> findSubscribersInTheirRangeForHO(obj, REMOVE_MESSAGE);
    
    position_t towerMinPosX = obj -> minPosX / this -> towerWidth;
    position_t towerMinPosY = obj -> minPosY / this -> towerHeight;
    position_t towerMaxPosX = obj -> maxPosX / this -> towerWidth;
    position_t towerMaxPosY = obj -> maxPosY / this -> towerHeight;

    for (position_t i = towerMinPosX; i <= towerMaxPosX; i ++) {
        for (position_t j = towerMinPosY; j <= towerMaxPosY; j ++) {
            this -> towers[i][j] -> removePublisher(obj);
        }
    }
    
    return true;
}

bool TowerAOI::removeHugeSubscriber(HOGameObject *obj) {
    this -> findPublishersInRangeForHO(obj, REMOVE_MESSAGE);
    
    this -> hugeSubscribers . erase(obj -> id);
    
    return true;
}

bool TowerAOI::onPublisherMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
    if (oldPosX != obj -> posX || oldPosY != obj -> posY) {
        this -> towers[oldPosX / towerWidth][oldPosY / towerHeight] -> removePublisher(obj);
        this -> towers[obj -> posX / towerWidth][obj -> posY / towerHeight] -> addPublisher(obj);
    }
    
    map<entity_t, GameObject *> oldSubMaps, newSubMaps;
    oldSubMaps = this -> BaseAOI::findSubscribersInTheirRange(obj -> id, oldPosX, oldPosY);
    newSubMaps = this -> BaseAOI::findSubscribersInTheirRange(obj -> id, obj -> posX, obj -> posY);
    
    list<GameObject *> addSet, moveSet, removeSet;
    map<entity_t, GameObject *> :: iterator iter;
    for (iter = oldSubMaps . begin(); iter != oldSubMaps . end(); iter ++) {
        if (newSubMaps.find(iter -> first) != newSubMaps.end()) {
            moveSet.push_back(iter -> second);
            newSubMaps.erase(iter -> first);
        } else {
            removeSet.push_back(iter -> second);
        }
    }
    if (newSubMaps.size() > 0) {
        for (iter = newSubMaps . begin(); iter != newSubMaps . end(); iter ++) {
            addSet.push_back(iter -> second);
        }
    }
    
    this -> aoiEventManager -> onAddPublisher(obj, addSet);
    this -> aoiEventManager -> onRemovePublisher(obj, removeSet);
    this -> aoiEventManager -> onMovePublisher(obj, moveSet);
    
    return true;
}

bool TowerAOI::onSubscriberMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
    map<entity_t, GameObject *> oldPubMaps, newPubMaps;
    
    oldPubMaps = this -> findPublishersInRange(obj -> id, oldPosX, oldPosY, obj -> range);
    
    newPubMaps = this -> findPublishersInRange(obj -> id, obj -> posX, obj -> posY, obj -> range);
        
    list<GameObject *> addSet, moveSet, removeSet;
    map<entity_t, GameObject *> :: iterator iter;
    for (iter = oldPubMaps . begin(); iter != oldPubMaps . end(); iter ++) {
        if (newPubMaps.find(iter -> first) != newPubMaps.end()) {
            moveSet.push_back(iter -> second);
            newPubMaps.erase(iter -> first);
        } else {
            removeSet.push_back(iter -> second);
        }
    }
    if (newPubMaps.size() > 0) {
        for (iter = newPubMaps . begin(); iter != newPubMaps . end(); iter ++) {
            addSet.push_back(iter -> second);
        }
    }
    
    // do no change to moveSet
    this -> aoiEventManager -> onAddSubscriber(obj, addSet);
    this -> aoiEventManager -> onRemoveSubscriber(obj, removeSet);
    
    return true;
}

bool TowerAOI::onHugePublisherMove(HOGameObject *obj, position_t oldMinPosX, position_t oldMinPosY, position_t oldMaxPosX, position_t oldMaxPosY) {
    position_t oldTowerMinPosX = oldMinPosX / this -> towerWidth;
    position_t oldTowerMinPosY = oldMinPosY / this -> towerHeight;
    position_t oldTowerMaxPosX = oldMaxPosX / this -> towerWidth;
    position_t oldTowerMaxPosY = oldMaxPosY / this -> towerHeight;
    
    position_t towerMinPosX = obj -> minPosX / this -> towerWidth;
    position_t towerMinPosY = obj -> minPosY / this -> towerHeight;
    position_t towerMaxPosX = obj -> maxPosX  / this -> towerWidth;
    position_t towerMaxPosY = obj -> maxPosY  / this -> towerHeight;
    
    if (oldTowerMinPosX != towerMinPosX || oldTowerMinPosY != towerMinPosY || oldTowerMaxPosX != towerMaxPosX || oldTowerMaxPosY != towerMaxPosY) {
        for (position_t i = oldTowerMinPosX; i <= oldTowerMaxPosX; i ++) {
            for (position_t j = oldTowerMinPosY; j <= oldTowerMaxPosY; j ++) {
                this -> towers[i][j] -> removePublisher(obj);
            }
        }
        for (position_t i = towerMinPosX; i <= towerMaxPosX; i ++) {
            for (position_t j = towerMinPosY; j <= towerMaxPosY; j ++) {
                this -> towers[i][j] -> addPublisher(obj);
            }
        }
    }
    
    map<entity_t, GameObject *> oldSubMaps, newSubMaps;
    oldSubMaps = this -> BaseAOI::findSubscribersInTheirRangeForHO(obj -> id, oldMinPosX, oldMinPosY, oldMaxPosX, oldMaxPosY);
    newSubMaps = this -> BaseAOI::findSubscribersInTheirRangeForHO(obj -> id, obj -> minPosX, obj -> minPosY, obj -> maxPosX, obj -> maxPosY);
    
    list<GameObject *> addSet, moveSet, removeSet;
    map<entity_t, GameObject *> :: iterator iter;
    for (iter = oldSubMaps . begin(); iter != oldSubMaps . end(); iter ++) {
        if (newSubMaps.find(iter -> first) != newSubMaps.end()) {
            moveSet.push_back(iter -> second);
            newSubMaps.erase(iter -> first);
        } else {
            removeSet.push_back(iter -> second);
        }
    }
    if (newSubMaps.size() > 0) {
        for (iter = newSubMaps . begin(); iter != newSubMaps . end(); iter ++) {
            addSet.push_back(iter -> second);
        }
    }
    
    this -> aoiEventManager -> onAddPublisher(obj, addSet);
    this -> aoiEventManager -> onRemovePublisher(obj, removeSet);
    this -> aoiEventManager -> onMovePublisher(obj, moveSet);
    
    return true;
}

bool TowerAOI::onHugeSubscriberMove(HOGameObject *obj, position_t oldMinPosX, position_t oldMinPosY, position_t oldMaxPosX, position_t oldMaxPosY) {
    map<entity_t, GameObject *> oldPubMaps, newPubMaps;
    
    oldPubMaps = this -> findPublishersInRangeForHO(obj -> id, oldMinPosX, oldMinPosY, oldMaxPosX, oldMaxPosY, obj -> range);
    newPubMaps = this -> findPublishersInRangeForHO(obj -> id, obj -> minPosX, obj -> minPosY, obj -> maxPosX, obj -> maxPosY, obj -> range);
    
    list<GameObject *> addSet, moveSet, removeSet;
    map<entity_t, GameObject *> :: iterator iter;
    for (iter = oldPubMaps . begin(); iter != oldPubMaps . end(); iter ++) {
        if (newPubMaps.find(iter -> first) != newPubMaps.end()) {
            moveSet.push_back(iter -> second);
            newPubMaps.erase(iter -> first);
        } else {
            removeSet.push_back(iter -> second);
        }
    }
    if (newPubMaps.size() > 0) {
        for (iter = newPubMaps . begin(); iter != newPubMaps . end(); iter ++) {
            addSet.push_back(iter -> second);
        }
    }
    
    // do no change to moveSet
    this -> aoiEventManager -> onAddSubscriber(obj, addSet);
    this -> aoiEventManager -> onRemoveSubscriber(obj, removeSet);
    
    return true;
}

void TowerAOI::findSubscribersInTheirRange(GameObject *obj, state_t state) {
    map<entity_t, GameObject *> subMaps = this -> BaseAOI::findSubscribersInTheirRange(obj -> id, obj -> posX, obj -> posY);
    list<GameObject *> subs = transMapsToLists(subMaps);
    
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddPublisher(obj, subs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemovePublisher(obj, subs);
    }
}

void TowerAOI::findPublishersInRange(GameObject *obj, state_t state) {
    map<entity_t, GameObject *> pubMaps = this -> findPublishersInRange(obj -> id, obj -> posX, obj -> posY, obj -> range);
    list<GameObject *> pubs = transMapsToLists(pubMaps);
    
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddSubscriber(obj, pubs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemoveSubscriber(obj, pubs);
    }
}

void TowerAOI::findSubscribersInTheirRangeForHO(HOGameObject *obj, state_t state) {
    map<entity_t, GameObject *> subMaps = this -> BaseAOI::findSubscribersInTheirRangeForHO(obj -> id, obj -> minPosX, obj -> minPosY, obj -> maxPosX, obj -> maxPosY);
    list<GameObject *> subs = transMapsToLists(subMaps);
    
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddPublisher(obj, subs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemovePublisher(obj, subs);
    }
}

void TowerAOI::findPublishersInRangeForHO(HOGameObject *obj, state_t state) {
    map<entity_t, GameObject *> pubMaps = this -> findPublishersInRangeForHO(obj -> id, obj -> minPosX, obj -> minPosY, obj -> maxPosX, obj -> maxPosY, obj -> range);
    list<GameObject *> pubs = transMapsToLists(pubMaps);
    
    // receive messages from above publishers
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddSubscriber(obj, pubs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemoveSubscriber(obj, pubs);
    }
}

map<entity_t, GameObject *> TowerAOI::findPublishersInRange(entity_t objId, position_t posX, position_t posY, position_t range) {
    map<entity_t, GameObject *> pubs, partialPubs;
    list<Tower*> fullCoveredTowers, partialCoveredTowers;
    list<Tower*>::iterator iter;
    map<entity_t, GameObject *>::iterator pubMapIter;
    
    position_t towerPosX = posX / this -> towerWidth;
    position_t towerPosY = posY / this -> towerHeight;
    
    position_t i, j;
    for (i = towerPosX; i < this -> towerX; i ++) {
        for (j = towerPosY; j < this -> towerY; j ++) {
            if (!classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers)) {
                break;
            }
        }
        if (towerPosY != 0) {
            for (j = towerPosY - 1; j != 0; j --) {
                if (!classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers)) {
                    break;
                }
            }
            if (0 == j) {
                classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers);
            }
        }
    }
    if (towerPosX != 0) {
        for (i = towerPosX - 1; i != 0; i --) {
            for (j = towerPosY; j < this -> towerY; j ++) {
                if (!classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers)) {
                    break;
                }
            }
            if (towerPosY != 0) {
                for (j = towerPosY - 1; j != 0; j --) {
                    if (!classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers)) {
                        break;
                    }
                }
                if (0 == j) {
                    classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers);
                }
            }
        }
        if (0 == i) {
            for (j = towerPosY; j < this -> towerY; j ++) {
                if (!classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers)) {
                    break;
                }
            }
            if (towerPosY != 0) {
                for (j = towerPosY - 1; j != 0; j --) {
                    if (!classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers)) {
                        break;
                    }
                }
                if (0 == j) {
                    classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers);
                }
            }
        }
    }
    
    for (iter = fullCoveredTowers . begin(); iter != fullCoveredTowers . end(); iter ++) {
        pubs = addTwoMaps(pubs, (*iter) -> getPublishers());
    }

    int32_t minPosX, minPosY, maxPosX, maxPosY;
    // traversal all the partialCoveredTowers to find all the object in range
    for (iter = partialCoveredTowers . begin(); iter != partialCoveredTowers . end(); iter ++) {
        partialPubs = (*iter) -> getPublishers();
        for (pubMapIter = partialPubs.begin(); pubMapIter != partialPubs.end(); pubMapIter ++) {
            if (!pubMapIter -> second -> isHugeObject) {
                if (isInRange2(posX, posY, pubMapIter -> second -> posX, pubMapIter -> second -> posY, range)) {
                    pubs[pubMapIter -> first] = pubMapIter -> second;
                }
            } else {
                HOGameObject *obj = (HOGameObject *)(pubMapIter -> second);
                minPosX = obj -> minPosX;
                minPosY = obj -> minPosY;
                maxPosX = obj -> maxPosX;
                maxPosY = obj -> maxPosY;
                
                if (isPointInRect(posX, posY, minPosX - range, minPosY - range, maxPosX + range, maxPosY + range)) {
                    if (posX <= minPosX && posY <= minPosY) {
                        if (!isInRange2(posX, posY, minPosX, minPosY, range)) {
                            continue;
                        }
                    } else if (posX <= minPosX && posY >= maxPosY) {
                        if (!isInRange2(posX, posY, minPosX, maxPosY, range)) {
                            continue;
                        }
                    } else if(posX >= maxPosX && posY <= minPosY) {
                        if (!isInRange2(posX, posY, maxPosX, minPosY, range)) {
                            continue;
                        }
                    } else if(posX >= maxPosX && posY >= maxPosY) {
                        if (!isInRange2(posX, posY, maxPosX, maxPosY, range)) {
                            continue;
                        }
                    }
                    pubs[pubMapIter -> first] = pubMapIter -> second;
                }
            }
        }
    }
    
    pubs.erase(objId);
    
    return pubs;
}

map<entity_t, GameObject *> TowerAOI::findPublishersInRangeForHO(entity_t objId, position_t minPosX, position_t minPosY, position_t maxPosX, position_t maxPosY, position_t range) {
    map<entity_t, GameObject *> pubs, partialPubs;
    list<Tower*> fullCoveredTowers, partialCoveredTowers;
    list<Tower*>::iterator iter;
    map<entity_t, GameObject *>::iterator pubMapIter;

    position_t newMinPosX, newMinPosY, newMaxPosX, newMaxPosY;
    newMinPosX = minPosX < range ? 0 : minPosX - range;
    newMinPosY = minPosY < range ? 0 : minPosY - range;
    newMaxPosX = maxPosX + range >= this -> worldWidth ? this -> worldWidth - 1 : maxPosX + range;
    newMaxPosY = maxPosY + range >= this -> worldHeight ? this -> worldHeight - 1 : maxPosY + range;
    
    position_t newTowerMinPosX = newMinPosX / this -> towerWidth;
    position_t newTowerMinPosY = newMinPosY / this -> towerHeight;
    position_t newTowerMaxPosX = newMaxPosX / this -> towerWidth;
    position_t newTowerMaxPosY = newMaxPosY / this -> towerHeight;
    position_t towerMinPosX = minPosX / this -> towerWidth;
    position_t towerMinPosY = minPosY / this -> towerHeight;
    position_t towerMaxPosX = maxPosX / this -> towerWidth;
    position_t towerMaxPosY = maxPosY / this -> towerHeight;
    
    for (position_t i = newTowerMinPosX; i <= towerMinPosX; i ++) {
        for (position_t j = newTowerMinPosY; j <= towerMinPosY; j ++) {
            partialCoveredTowers.push_back(this -> towers[i][j]);
        }
        for (position_t j = towerMaxPosY; j <= newTowerMaxPosY; j ++) {
            partialCoveredTowers.push_back(this -> towers[i][j]);
        }
    }
    for (position_t i = towerMaxPosX; i <= newTowerMaxPosX; i ++) {
        for (position_t j = newTowerMinPosY; j <= towerMinPosY; j ++) {
            partialCoveredTowers.push_back(this -> towers[i][j]);
        }
        for (position_t j = towerMaxPosY; j <= newTowerMaxPosY; j ++) {
            partialCoveredTowers.push_back(this -> towers[i][j]);
        }
    }
    
    for (position_t j = towerMinPosY + 1; j < towerMaxPosY; j ++) {
        partialCoveredTowers.push_back(this -> towers[newTowerMinPosX][j]);
        partialCoveredTowers.push_back(this -> towers[newTowerMaxPosX][j]);
    }
    for (position_t i = towerMinPosX + 1; i < towerMaxPosX; i ++) {
        partialCoveredTowers.push_back(this -> towers[i][newTowerMinPosY]);
        partialCoveredTowers.push_back(this -> towers[i][newTowerMaxPosY]);
    }

    for (position_t i = newTowerMinPosX + 1; i < newTowerMaxPosX; i ++) {
        for (position_t j = towerMinPosY + 1; j < towerMaxPosY; j ++) {
            fullCoveredTowers.push_back(this -> towers[i][j]);
        }
    }
    for (position_t i = towerMinPosX + 1; i < towerMaxPosX; i ++) {
        for (position_t j = newTowerMinPosY + 1; j <= towerMinPosY; j ++) {
            fullCoveredTowers.push_back(this -> towers[i][j]);
        }
        for (position_t j = towerMaxPosY; j < newTowerMaxPosY; j ++) {
            fullCoveredTowers.push_back(this -> towers[i][j]);
        }
    }
    
//    for (position_t i = newTowerMinPosX; i <= newTowerMaxPosX; i ++) {
//        for (position_t j = newTowerMinPosY; j <= towerMinPosY; j ++) {
//            partialCoveredTowers.push_back(this -> towers[i][j]);
//        }
//        for (position_t j = towerMaxPosY; j <= newTowerMaxPosY; j ++) {
//            partialCoveredTowers.push_back(this -> towers[i][j]);
//        }
//    }
//    
//    for (position_t j = towerMinPosY + 1; j < towerMaxPosY; j ++) {
//        partialCoveredTowers.push_back(this -> towers[newTowerMinPosX][j]);
//        partialCoveredTowers.push_back(this -> towers[newTowerMaxPosX][j]);
//    }
//    
//    for (position_t i = newTowerMinPosX + 1; i < newTowerMaxPosX; i ++) {
//        for (position_t j = towerMinPosY + 1; j < towerMaxPosY; j ++) {
//            fullCoveredTowers.push_back(this -> towers[i][j]);
//        }
//    }
    
    for (iter = fullCoveredTowers . begin(); iter != fullCoveredTowers . end(); iter ++) {
        pubs = addTwoMaps(pubs, (*iter) -> getPublishers());
    }
    
    position_t posX, posY;
    // traversal all the partialCoveredTowers to find all the object in range
    for (iter = partialCoveredTowers . begin(); iter != partialCoveredTowers . end(); iter ++) {
        partialPubs = (*iter) -> getPublishers();
        for (pubMapIter = partialPubs.begin(); pubMapIter != partialPubs.end(); pubMapIter ++) {
            if (!pubMapIter -> second -> isHugeObject) {
                posX = pubMapIter -> second -> posX;
                posY = pubMapIter -> second -> posY;
                if (isPointInRect(posX, posY, newMinPosX, newMinPosY, newMaxPosX, newMaxPosY)) {
                    
                    if (posX <= minPosX && posY <= minPosY) {
                        if (!isInRange2(posX, posY, minPosX, minPosY, range)) {
                            continue;
                        }
                    } else if (posX <= minPosX && posY >= maxPosY) {
                        if (!isInRange2(posX, posY, minPosX, maxPosY, range)) {
                            continue;
                        }
                    } else if(posX >= maxPosX && posY <= minPosY) {
                        if (!isInRange2(posX, posY, maxPosX, minPosY, range)) {
                            continue;
                        }
                    } else if(posX >= maxPosX && posY >= maxPosY) {
                        if (!isInRange2(posX, posY, maxPosX, maxPosY, range)) {
                            continue;
                        }
                    }
                    
                    pubs[pubMapIter -> first] = pubMapIter -> second;
                }
            }
            
//            else {
//                HOGameObject *obj = (HOGameObject *)(pubMapIter -> second);
//                
//                if (obj -> minPosX > maxPosX || obj -> maxPosX < minPosX || obj -> minPosY > maxPosY || obj -> maxPosY < minPosY) {
//                    // blank
//                } else {
//                    pubs[pubMapIter -> first] = pubMapIter -> second;
//                }
//            }
        }
    }
    
    pubs.erase(objId);
    
    return pubs;

}

bool TowerAOI::isFullCoveredTower(position_t posX, position_t posY, position_t range, position_t pos1X, position_t pos1Y, position_t pos2X, position_t pos2Y) {
    if ((isInRange2(posX, posY, pos1X, pos1Y, range)) &&
        (isInRange2(posX, posY, pos2X, pos2Y, range)) &&
        (isInRange2(posX, posY, pos1X, pos2Y, range)) &&
        (isInRange2(posX, posY, pos2X, pos1Y, range))) {
        return true;
    }
    
    return false;
}

bool TowerAOI::isPartialCoveredTower(position_t posX, position_t posY, position_t range, position_t pos1X, position_t pos1Y, position_t pos2X, position_t pos2Y) {
    // angel in circle
    if ((isInRange2(posX, posY, pos1X, pos1Y, range)) ||
        (isInRange2(posX, posY, pos2X, pos2Y, range)) ||
        (isInRange2(posX, posY, pos1X, pos2Y, range)) ||
        (isInRange2(posX, posY, pos2X, pos1Y, range))) {
        return true;
    }
    
    // side in circle
    if ((isCoveredBySide(posX, posY, range, pos1X, pos1Y, pos2X, pos2Y)) ||
        (isCoveredBySide(posX, posY, range, pos2X, pos1Y, pos1X, pos2Y)) ||
        (isCoveredBySide(posY, posX, range, pos1Y, pos1X, pos2Y, pos2X)) ||
        (isCoveredBySide(posY, posX, range, pos2Y, pos1X, pos1Y, pos2X))) {
        return true;
    }
    
    return false;
}

bool TowerAOI::isCoveredBySide(position_t posX, position_t posY, position_t range, position_t pos1X, position_t pos1Y, position_t pos2X, position_t pos2Y) {
    if (isInRange(posX, pos1X, range)) {
        position_t dis2 = range * range - (posX - pos1X) * (posX - pos1X);
        position_t testposY = sqrt(dis2) + posY;
        return (testposY * testposY) >= pos1Y * pos1Y && (testposY * testposY) <= pos2Y * pos2Y;
    }
    
    return false;
}

bool TowerAOI::classifyTower(position_t posX, position_t posY, position_t range, int i, int j, list<Tower*> &fullCoveredTowers, list<Tower*> &partialCoveredTowers) {
    if (isFullCoveredTower(posX, posY, range, i * towerWidth, j * towerHeight, (i + 1) * towerWidth, (j + 1) * towerHeight)) {
        fullCoveredTowers.push_back(towers[i][j]);
    } else if (isPartialCoveredTower(posX, posY, range, i * towerWidth, j * towerHeight, (i + 1) * towerWidth, (j + 1) * towerHeight)) {
        partialCoveredTowers.push_back(towers[i][j]);
    } else if (i == posX / towerWidth && j == posY / towerHeight) {
        partialCoveredTowers.push_back(towers[i][j]);
    } else {
        return false;
    }
    
    return true;
}

