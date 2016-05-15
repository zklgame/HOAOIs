//
//  NTreeAOI.cpp
//  AOIs
//
//  Created by zklgame on 4/18/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "NTreeAOI.hpp"

NTreeAOI::NTreeAOI(position_t worldWidth, position_t worldHeight, entity_t towerMaxPublisherNum, position_t maxLevel): worldWidth(worldWidth), worldHeight(worldHeight), towerMaxPublisherNum(towerMaxPublisherNum) {
    this -> tree = new NTree(0, worldWidth, 0, worldHeight, towerMaxPublisherNum, maxLevel, 0);
}

NTreeAOI::~NTreeAOI() {
    delete this -> tree;
}

bool NTreeAOI::addPublisher(GameObject *obj) {
    this -> tree -> addPublisher(obj);
    
    this -> findSubscribersInTheirRange(obj, ADD_MESSAGE);
    
    return true;
}

bool NTreeAOI::addSubscriber(GameObject *obj) {
    this -> subscribers[obj -> id] = obj;
    
    this -> findPublishersInRange(obj, ADD_MESSAGE);
    
    return true;
}

bool NTreeAOI::removePublisher(GameObject *obj) {
    
    this -> findSubscribersInTheirRange(obj, REMOVE_MESSAGE);
    
    this -> tree -> removePublisher(obj);
    
    return true;
}

bool NTreeAOI::removeSubscriber(GameObject *obj) {
    
    this -> findPublishersInRange(obj, REMOVE_MESSAGE);
    
    this -> subscribers . erase(obj -> id);
    
    return true;
}

bool NTreeAOI::addHugePublisher(HOGameObject *obj) {
    this -> tree -> addHugePublisher(obj);
    
    this -> findSubscribersInTheirRangeForHO(obj, ADD_MESSAGE);
    
    return true;
}

bool NTreeAOI::addHugeSubscriber(HOGameObject *obj) {
    this -> hugeSubscribers[obj -> id] = obj;
    
    this -> findPublishersInRangeForHO(obj, ADD_MESSAGE);
    
    return true;
}

bool NTreeAOI::removeHugePublisher(HOGameObject *obj) {
    this -> findSubscribersInTheirRangeForHO(obj, REMOVE_MESSAGE);
    
    this -> tree -> removeHugePublisher(obj);
    
    return true;
}

bool NTreeAOI::removeHugeSubscriber(HOGameObject *obj) {
    this -> findPublishersInRangeForHO(obj, REMOVE_MESSAGE);
    
    this -> hugeSubscribers . erase(obj -> id);
    
    return true;
}

bool NTreeAOI::onPublisherMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
    map<entity_t, GameObject *> oldSubMaps, newSubMaps;
    
    oldSubMaps = this -> BaseAOI::findSubscribersInTheirRange(obj -> id, oldPosX, oldPosY);
    
    if (oldPosX != obj -> posX || oldPosY != obj -> posY) {
        this -> tree -> removePublisherByPos(obj -> id, oldPosX, oldPosY);
        this -> tree -> addPublisher(obj);
    }
    
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

bool NTreeAOI::onSubscriberMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
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

bool NTreeAOI::onHugePublisherMove(HOGameObject *obj, position_t oldMinPosX, position_t oldMinPosY, position_t oldMaxPosX, position_t oldMaxPosY) {
    map<entity_t, GameObject *> oldSubMaps, newSubMaps;
    
    oldSubMaps = this -> BaseAOI::findSubscribersInTheirRangeForHO(obj -> id, oldMinPosX, oldMinPosY, oldMaxPosX, oldMaxPosY);
    
    if (oldMinPosX != obj -> minPosX || oldMinPosY != obj -> minPosY || oldMaxPosX != obj -> maxPosX || oldMaxPosY != obj -> maxPosY) {
        this -> tree -> removeHugePublisherByPos(obj -> id, oldMinPosX, oldMinPosY, oldMaxPosX, oldMaxPosY);
        this -> tree -> addHugePublisher(obj);
    }
    
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

bool NTreeAOI::onHugeSubscriberMove(HOGameObject *obj, position_t oldMinPosX, position_t oldMinPosY, position_t oldMaxPosX, position_t oldMaxPosY) {
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

void NTreeAOI::findSubscribersInTheirRange(GameObject *obj, state_t state) {
    map<entity_t, GameObject *> subMaps = this -> BaseAOI::findSubscribersInTheirRange(obj -> id, obj -> posX, obj -> posY);
    list<GameObject *> subs = transMapsToLists(subMaps);
    
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddPublisher(obj, subs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemovePublisher(obj, subs);
    }
}

void NTreeAOI::findPublishersInRange(GameObject *obj, state_t state) {
    map<entity_t, GameObject *> pubMaps = this -> findPublishersInRange(obj -> id, obj -> posX, obj -> posY, obj -> range);
    list<GameObject *> pubs = transMapsToLists(pubMaps);
    
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddSubscriber(obj, pubs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemoveSubscriber(obj, pubs);
    }
}

void NTreeAOI::findSubscribersInTheirRangeForHO(HOGameObject *obj, state_t state) {
    map<entity_t, GameObject *> subMaps = this -> BaseAOI::findSubscribersInTheirRangeForHO(obj -> id, obj -> minPosX, obj -> minPosY, obj -> maxPosX, obj -> maxPosY);
    list<GameObject *> subs = transMapsToLists(subMaps);
    
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddPublisher(obj, subs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemovePublisher(obj, subs);
    }
}

void NTreeAOI::findPublishersInRangeForHO(HOGameObject *obj, state_t state) {
    map<entity_t, GameObject *> pubMaps = this -> findPublishersInRangeForHO(obj -> id, obj -> minPosX, obj -> minPosY, obj -> maxPosX, obj -> maxPosY, obj -> range);
    list<GameObject *> pubs = transMapsToLists(pubMaps);
    
    // receive messages from above publishers
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddSubscriber(obj, pubs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemoveSubscriber(obj, pubs);
    }

}

map<entity_t, GameObject *> NTreeAOI::findPublishersInRange(entity_t objId, position_t posX, position_t posY, position_t range) {
    map<entity_t, GameObject *> pubs = this -> tree -> findPublishersInRange(posX, posY, range);
    pubs.erase(objId);
    
    return pubs;
}

map<entity_t, GameObject *> NTreeAOI::findPublishersInRangeForHO(entity_t objId, position_t minPosX, position_t minPosY, position_t maxPosX, position_t maxPosY, position_t range) {
    map<entity_t, GameObject *> pubs = this -> tree -> findPublishersInRangeForHO(minPosX, minPosY, maxPosX, maxPosY, range);
    pubs.erase(objId);
    
    return pubs;
}


