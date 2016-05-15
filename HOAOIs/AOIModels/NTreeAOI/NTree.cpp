//
//  NTree.cpp
//  AOIs
//
//  Created by zklgame on 4/19/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "NTree.hpp"

#include <iostream>

NTree::NTree(position_t minX, position_t maxX, position_t minY, position_t maxY, entity_t maxPublisherNum, position_t maxLevel, position_t level):minX(minX), maxX(maxX), minY(minY), maxY(maxY), maxPublisherNum(maxPublisherNum), isSplit(false), parent(NULL), childrenNum(4), maxLevel(maxLevel), level(level) {
    for (int i = 0; i < childrenNum; i ++) {
        this -> children[i] = NULL;
    }
    cout << endl <<  "new NTree level : " << level << endl << endl;
}

NTree::~NTree() {
    if (isSplit) {
        for (int i = 0; i < childrenNum; i ++) {
            delete this -> children[i];
        }
    }
}

// 0 1
// 3 2

bool NTree::addPublisher(GameObject *obj) {
        
    if (this -> isSplit) {
        if (obj -> posX < (minX + maxX) / 2) {
            if (obj -> posY < (minY + maxY) / 2) {
                this -> children[0] -> addPublisher(obj);
            } else {
                this -> children[3] -> addPublisher(obj);
            }
        } else {
            if (obj -> posY < (minY + maxY) / 2) {
                this -> children[1] -> addPublisher(obj);
            } else {
                this -> children[2] -> addPublisher(obj);
            }
        }
    } else {
        this -> publishers[obj -> id] = obj;
        
        obj -> tree = this;
    
        if (this -> getPublisherNum() > maxPublisherNum && this -> level < this -> maxLevel) {
            // split the Node
            NTree *subTree0 = new NTree(minX, (minX + maxX) / 2, minY, (minY + maxY) / 2, maxPublisherNum, this -> maxLevel, this -> level + 1);
            NTree *subTree1 = new NTree((minX + maxX) / 2, maxX, minY, (minY + maxY) / 2, maxPublisherNum, this -> maxLevel, this -> level + 1);
            NTree *subTree2 = new NTree((minX + maxX) / 2, maxX, (minY + maxY) / 2, maxY, maxPublisherNum, this -> maxLevel, this -> level + 1);
            NTree *subTree3 = new NTree(minX, (minX + maxX) / 2, (minY + maxY) / 2, maxY, maxPublisherNum, this -> maxLevel, this -> level + 1);
            
            this -> isSplit = true;
            
            this -> children[0] = subTree0;
            this -> children[1] = subTree1;
            this -> children[2] = subTree2;
            this -> children[3] = subTree3;
            
            subTree0 -> parent = subTree1 -> parent = subTree2 -> parent = subTree3 -> parent = this;
            
            // put all the publishers into children
            map<entity_t, GameObject *>::iterator iter;
            position_t posX, posY;
            for (iter = this -> publishers . begin(); iter != this -> publishers . end(); iter ++) {
                if (iter -> second -> isHugeObject) {
                    HOGameObject *obj = (HOGameObject *)(iter -> second);
                    this -> operateHugePublisherToChildren(obj, 1);
                    continue;
                }
                
                posX = iter -> second -> posX;
                posY = iter -> second -> posY;
                if (posX < (minX + maxX) / 2) {
                    if (posY < (minY + maxY) / 2) {
                        subTree0 -> addPublisher(iter -> second);
                    } else {
                        subTree3 -> addPublisher(iter -> second);
                    }
                } else {
                    if (posY < (minY + maxY) / 2) {
                        subTree1 -> addPublisher(iter -> second);
                    } else {
                        subTree2 -> addPublisher(iter -> second);
                    }
                }
            }
            
            this -> publishers . clear();
        }
    }
    
    return true;
}

bool NTree::removePublisher(GameObject *obj) {
        
    if (this -> isSplit) {
        if (obj -> posX < (minX + maxX) / 2) {
            if (obj -> posY < (minY + maxY) / 2) {
                this -> children[0] -> removePublisher(obj);
            } else {
                this -> children[3] -> removePublisher(obj);
            }
        } else {
            if (obj -> posY < (minY + maxY) / 2) {
                this -> children[1] -> removePublisher(obj);
            } else {
                this -> children[2] -> removePublisher(obj);
            }
        }
    } else {
        this -> publishers . erase(obj -> id);
        
        if (this -> parent) {
            entity_t entityNumInAllChildren = this -> parent -> getPublisherNum();
            // if all Num < 1 /2 * maxPublisherNum, merge
            if (entityNumInAllChildren < 0.5 * maxPublisherNum) {
                // merge child Nodes
                this -> parent -> mergeChildTrees();
            }
        }
    }

    return true;
}

bool NTree::addHugePublisher(HOGameObject *obj) {
   
    if (this -> isSplit) {
        this -> operateHugePublisherToChildren(obj, 1);
    } else {
        this -> addPublisher(obj);
    }
    
    return true;
}

bool NTree::removeHugePublisher(HOGameObject *obj) {
    if (this -> isSplit) {
        this -> operateHugePublisherToChildren(obj, 2);
    } else {
        this -> removePublisher(obj);
    }
    
    return true;
}

bool NTree::removePublisherByPos(entity_t objId, position_t posX, position_t posY) {
    if (this -> isSplit) {
        if (posX < (minX + maxX) / 2) {
            if (posY < (minY + maxY) / 2) {
                this -> children[0] -> removePublisherByPos(objId, posX, posY);
            } else {
                this -> children[3] -> removePublisherByPos(objId, posX, posY);
            }
        } else {
            if (posY < (minY + maxY) / 2) {
                this -> children[1] -> removePublisherByPos(objId, posX, posY);
            } else {
                this -> children[2] -> removePublisherByPos(objId, posX, posY);
            }
        }
        return true;
    } else {
        if (this -> publishers . find(objId) != this -> publishers . end()) {
            this -> publishers . erase(objId);
        
            if (this -> parent) {
                entity_t entityNumInAllChildren = this -> parent -> getPublisherNum();
                // if all Num < 1 /2 * maxPublisherNum, merge
                if (entityNumInAllChildren < 0.5 * maxPublisherNum) {
                    // merge child Nodes
                    this -> parent -> mergeChildTrees();
                }
            }
            
            return true;
            
        }
        
        cout << "ERROR ERROR ERROR ERROR" << endl;
        cout << objId << " " << posX << " " << posY << endl;
        cout << minX << " " << maxX << " " << minY << " " << maxX << endl;
        return  false;
    }
}

bool NTree::removeHugePublisherByPos(entity_t objId, position_t minPosX, position_t minPosY, position_t maxPosX, position_t maxPosY) {
    if (this -> isSplit) {
        
        if (minPosX < (minX + maxX) / 2) {
            if (maxPosX < (minX + maxX) / 2) {
                if (minPosY < (minY + maxY) / 2) {
                    if (maxPosY < (minY + maxY) / 2) {
                        this -> children[0] -> removeHugePublisherByPos(objId, minPosX, minPosY, maxPosX, maxPosY);
                    } else {
                        this -> children[0] -> removeHugePublisherByPos(objId, minPosX, minPosY, maxPosX, maxPosY);
                        this -> children[3] -> removeHugePublisherByPos(objId, minPosX, minPosY, maxPosX, maxPosY);
                    }
                } else {
                    this -> children[3] -> removeHugePublisherByPos(objId, minPosX, minPosY, maxPosX, maxPosY);
                }
            } else {
                if (minPosY < (minY + maxY) / 2) {
                    if (maxPosY < (minY + maxY) / 2) {
                        this -> children[0] -> removeHugePublisherByPos(objId, minPosX, minPosY, maxPosX, maxPosY);
                        this -> children[1] -> removeHugePublisherByPos(objId, minPosX, minPosY, maxPosX, maxPosY);
                    } else {
                        this -> children[0] -> removeHugePublisherByPos(objId, minPosX, minPosY, maxPosX, maxPosY);
                        this -> children[1] -> removeHugePublisherByPos(objId, minPosX, minPosY, maxPosX, maxPosY);
                        this -> children[2] -> removeHugePublisherByPos(objId, minPosX, minPosY, maxPosX, maxPosY);
                        this -> children[3] -> removeHugePublisherByPos(objId, minPosX, minPosY, maxPosX, maxPosY);
                    }
                } else {
                    this -> children[2] -> removeHugePublisherByPos(objId, minPosX, minPosY, maxPosX, maxPosY);
                    this -> children[3] -> removeHugePublisherByPos(objId, minPosX, minPosY, maxPosX, maxPosY);
                }
            }
        } else {
            if (minPosY < (minY + maxY) / 2) {
                if (maxPosY < (minY + maxY) / 2) {
                    this -> children[1] -> removeHugePublisherByPos(objId, minPosX, minPosY, maxPosX, maxPosY);
                } else {
                    this -> children[1] -> removeHugePublisherByPos(objId, minPosX, minPosY, maxPosX, maxPosY);
                    this -> children[2] -> removeHugePublisherByPos(objId, minPosX, minPosY, maxPosX, maxPosY);
                }
            } else {
                this -> children[2] -> removeHugePublisherByPos(objId, minPosX, minPosY, maxPosX, maxPosY);
            }
        }
        
        return true;
    } else {
        if (this -> publishers . find(objId) != this -> publishers . end()) {
            this -> publishers . erase(objId);
            
            if (this -> parent) {
                entity_t entityNumInAllChildren = this -> parent -> getPublisherNum();
                // if all Num < 1 /2 * maxPublisherNum, merge
                if (entityNumInAllChildren < 0.5 * maxPublisherNum) {
                    // merge child Nodes
                    this -> parent -> mergeChildTrees();
                }
            }
            
            return true;
            
        }
        
        cout << "HUGE HUGE ERROR ERROR ERROR" << endl;
        return  false;
    }
}

entity_t NTree::getPublisherNum() {
    if (!isSplit) {
        return (entity_t)(this -> publishers . size());
    } else {
        entity_t sum = 0;
        for (int i = 0; i < childrenNum; i ++) {
            sum += this -> children[i] -> getPublisherNum();
        }
        return sum;
    }
}

map<entity_t, GameObject *> NTree::getAllPublishers() {
    if (!isSplit) {
        return this -> publishers;
    } else {
        map<entity_t, GameObject *> pubs, tmpPubs;
        map<entity_t, GameObject *>::iterator iter;
        for (int i = 0; i < childrenNum; i ++) {
            tmpPubs = this -> children[i] -> getAllPublishers();
            for (iter = tmpPubs.begin(); iter != tmpPubs.end(); iter ++) {
                pubs[iter -> first] = iter -> second;
            }
        }
        return pubs;
    }
}

void NTree::mergeChildTrees() {
    map<entity_t, GameObject *> pubs;
    map<entity_t, GameObject *>::iterator iter;
    
    pubs = this -> getAllPublishers();
    for (iter = pubs.begin(); iter !=  pubs.end(); iter ++) {
        this -> publishers[iter -> first] = iter -> second;
        iter -> second -> tree = this;
    }
    
    // important!!! put isSplit behind getAllPublishers!!!
    isSplit = false;
    for (int i = 0; i < childrenNum; i ++) {
        delete this -> children[i];
        this -> children[i] = NULL;
    }
    
}

void NTree::classifyTree(position_t posX, position_t posY, position_t range, list<NTree *> &fullCoveredTrees, list<NTree *> &partialCoveredTrees) {
    if (!isSplit) {
        if (isFullCoveredTree(posX, posY, range)) {
            fullCoveredTrees.push_back(this);
        } else if (isPartialCoveredTree(posX, posY, range)) {
            partialCoveredTrees.push_back(this);
        }
    } else {
        for (int i = 0; i < childrenNum; i ++) {
            if (children[i] -> isFullCoveredTree(posX, posY, range) || children[i] -> isPartialCoveredTree(posX, posY, range)) {
                children[i] -> classifyTree(posX, posY, range, fullCoveredTrees, partialCoveredTrees);
            }
        }
    }
}

void NTree::classifyTree(position_t minPosX, position_t minPosY, position_t maxPosX, position_t maxPosY, position_t range, list<NTree *> &fullCoveredTrees, list<NTree *> &partialCoveredTrees) {
    
    position_t newMinPosX, newMinPosY, newMaxPosX, newMaxPosY;
    newMinPosX = minPosX < range ? 0 : minPosX - range;
    newMinPosY = minPosY < range ? 0 : minPosY - range;
    newMaxPosX = maxPosX + range >= this -> maxX ? this -> maxX - 1 : maxPosX + range;
    newMaxPosY = maxPosY + range >= this -> maxY ? this -> maxY - 1 : maxPosY + range;
    
    if (!isSplit) {
        if (newMinPosX >= maxX || newMaxPosX <= minX || newMinPosY >= maxY || newMaxPosY <= minY) {
        } else {
            if ((minX >= newMinPosX && maxX <= newMaxPosX && minY >= minPosY && maxY <= maxPosY) || (minX >= minPosX && maxX <= maxPosX && minY >= newMinPosY && maxY <= newMaxPosY) ) {
                fullCoveredTrees.push_back(this);
            } else {
                partialCoveredTrees.push_back(this);
            }
        }
    } else {
        for (int i = 0; i < childrenNum; i ++) {
            if (newMinPosX >= children[i] -> maxX || newMaxPosX <= children[i] -> minX || newMinPosY >= children[i] -> maxY || newMaxPosY <= children[i] -> minY) {
                
            } else {
                children[i] -> classifyTree(minPosX, minPosY, maxPosX, maxPosY, range, fullCoveredTrees, partialCoveredTrees);
            }
        }
    }
}

map<entity_t, GameObject *> NTree::findPublishersInRange(position_t posX, position_t posY, position_t range) {
    map<entity_t, GameObject *> pubs;
    list<NTree *> fullCoveredArea, partialCoveredArea;
    map<entity_t, GameObject *>::iterator iter;
    list<NTree *>::iterator treeIter;
    
    classifyTree(posX, posY, range, fullCoveredArea, partialCoveredArea);
    
    for (treeIter = fullCoveredArea.begin(); treeIter != fullCoveredArea.end(); treeIter ++) {
        for (iter = (*treeIter) -> publishers . begin(); iter != (*treeIter) -> publishers . end(); iter ++) {
            pubs[iter -> first] = iter -> second;
        }
    }
    HOGameObject *obj;
    int32_t minPosX, minPosY, maxPosX, maxPosY;
    for (treeIter = partialCoveredArea.begin(); treeIter != partialCoveredArea.end(); treeIter ++) {
        for (iter = (*treeIter) -> publishers . begin(); iter != (*treeIter) -> publishers . end(); iter ++) {
            if (iter -> second -> isHugeObject) {
                obj = (HOGameObject *)(iter -> second);
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
                    pubs[iter -> first] = iter -> second;
                }
                
                continue;
            }
            
            if (isInRange2(posX, posY, iter -> second -> posX, iter -> second -> posY, range)) {
                pubs[iter -> first] = iter -> second;
            }
        }
    }
    
    return pubs;
}

map<entity_t, GameObject *> NTree::findPublishersInRangeForHO(position_t minPosX, position_t minPosY, position_t maxPosX, position_t maxPosY, position_t range) {
    map<entity_t, GameObject *> pubs;
    list<NTree *> fullCoveredArea, partialCoveredArea;
    map<entity_t, GameObject *>::iterator iter;
    list<NTree *>::iterator treeIter;
    
    classifyTree(minPosX, minPosY, maxPosX, maxPosY, range, fullCoveredArea, partialCoveredArea);
    
    for (treeIter = fullCoveredArea.begin(); treeIter != fullCoveredArea.end(); treeIter ++) {
        for (iter = (*treeIter) -> publishers . begin(); iter != (*treeIter) -> publishers . end(); iter ++) {
            pubs[iter -> first] = iter -> second;
        }
    }
    
    position_t posX, posY;
    position_t newMinPosX, newMinPosY, newMaxPosX, newMaxPosY;
    newMinPosX = minPosX < range ? 0 : minPosX - range;
    newMinPosY = minPosY < range ? 0 : minPosY - range;
    newMaxPosX = maxPosX + range >= this -> maxX ? this -> maxX - 1 : maxPosX + range;
    newMaxPosY = maxPosY + range >= this -> maxY ? this -> maxY - 1 : maxPosY + range;
    
    for (treeIter = partialCoveredArea.begin(); treeIter != partialCoveredArea.end(); treeIter ++) {
        for (iter = (*treeIter) -> publishers . begin(); iter != (*treeIter) -> publishers . end(); iter ++) {
            if (iter -> second -> isHugeObject) {
                continue;
            }
            
            posX = iter -> second -> posX;
            posY = iter -> second -> posY;
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
                
                pubs[iter -> first] = iter -> second;
            }
        }
    }
    
    return pubs;
}


