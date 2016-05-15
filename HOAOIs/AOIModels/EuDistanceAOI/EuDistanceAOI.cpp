//
//  EuDistanceAOI.cpp
//  AOIs
//
//  Created by zklgame on 4/5/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "EuDistanceAOI.hpp"

bool EuDistanceAOI::addPublisher(GameObject *obj) {
    this -> publishers[obj -> id] = obj;
    
    this -> findSubscribersInTheirRange(obj, ADD_MESSAGE);
    
    return true;
}

bool EuDistanceAOI::addSubscriber(GameObject *obj) {
    this -> subscribers[obj -> id] = obj;
    
    this -> findPublishersInRange(obj, ADD_MESSAGE);
    
    return true;
}

bool EuDistanceAOI::removePublisher(GameObject *obj) {
    if (this -> publishers . find(obj -> id) == this -> publishers . end()) {
        return false;
    }
    
    this -> findSubscribersInTheirRange(obj, REMOVE_MESSAGE);
    
    this -> publishers . erase(obj -> id);
    
    return true;
}

bool EuDistanceAOI::removeSubscriber(GameObject *obj) {
    if (this -> subscribers . find(obj -> id) == this -> subscribers . end()) {
        return false;
    }
    
    this -> findPublishersInRange(obj, REMOVE_MESSAGE);
    
    this -> subscribers . erase(obj -> id);
    
    return true;
}

bool EuDistanceAOI::addHugePublisher(HOGameObject *obj) {
    this -> hugePublishers[obj -> id] = obj;
    
    this -> findSubscribersInTheirRangeForHO(obj, ADD_MESSAGE);
    
    return true;
}

bool EuDistanceAOI::addHugeSubscriber(HOGameObject *obj) {
    this -> hugeSubscribers[obj -> id] = obj;
    
    this -> findPublishersInRangeForHO(obj, ADD_MESSAGE);
    
    return true;
}

bool EuDistanceAOI::removeHugePublisher(HOGameObject *obj) {
    if (this -> hugePublishers . find(obj -> id) == this -> hugePublishers . end()) {
        return false;
    }
    
    this -> findSubscribersInTheirRangeForHO(obj, REMOVE_MESSAGE);
    
    this -> hugePublishers . erase(obj -> id);
    
    return true;
}

bool EuDistanceAOI::removeHugeSubscriber(HOGameObject *obj) {
    if (this -> hugeSubscribers . find(obj -> id) == this -> hugeSubscribers . end()) {
        return false;
    }
    
    this -> findPublishersInRangeForHO(obj, REMOVE_MESSAGE);
    
    this -> hugeSubscribers . erase(obj -> id);
    
    return true;
}

bool EuDistanceAOI::onPublisherMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
    // to find all the subscribers who can subscribe the publisher in old position
    map<entity_t, GameObject *> oldSubscribers = BaseAOI::findSubscribersInTheirRange(obj -> id, oldPosX, oldPosY);
    // to find all the subscribers who can subscribe the publisher in new position
    map<entity_t, GameObject *> newSubscribers = BaseAOI::findSubscribersInTheirRange(obj -> id, obj -> posX, obj -> posY);
    
    list<GameObject *> addSet, moveSet, removeSet;
    map<entity_t, GameObject *> :: iterator iter;
    for (iter = oldSubscribers . begin(); iter != oldSubscribers . end(); iter ++) {
        if (newSubscribers.find(iter -> first) != newSubscribers.end()) {
            moveSet.push_back(iter -> second);
            newSubscribers.erase(iter -> first);
        } else {
            removeSet.push_back(iter -> second);
        }
    }
    if (newSubscribers.size() > 0) {
        for (iter = newSubscribers . begin(); iter != newSubscribers . end(); iter ++) {
            addSet.push_back(iter -> second);
        }
    }
    
    this -> aoiEventManager -> onAddPublisher(obj, addSet);
    this -> aoiEventManager -> onRemovePublisher(obj, removeSet);
    this -> aoiEventManager -> onMovePublisher(obj, moveSet);
    
    return true;
}

bool EuDistanceAOI::onSubscriberMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
    // to find all the publishers the obj can subscribe in old position
    map<entity_t, GameObject *> oldPublishers = findPublishersInRange(obj -> id, oldPosX, oldPosY, obj -> range);
    // to find all the publishers the obj can subscribe in new position
    map<entity_t, GameObject *> newPublishers = findPublishersInRange(obj -> id, obj -> posX, obj -> posY, obj -> range);
    
    list<GameObject *> addSet, moveSet, removeSet;
    map<entity_t, GameObject *> :: iterator iter;
    for (iter = oldPublishers . begin(); iter != oldPublishers . end(); iter ++) {
        if (newPublishers.find(iter -> first) != newPublishers.end()) {
            moveSet.push_back(iter -> second);
            newPublishers.erase(iter -> first);
        } else {
            removeSet.push_back(iter -> second);
        }
    }
    if (newPublishers.size() > 0) {
        for (iter = newPublishers . begin(); iter != newPublishers . end(); iter ++) {
            addSet.push_back(iter -> second);
        }
    }
    
    // do no change to moveSet 
    this -> aoiEventManager -> onAddSubscriber(obj, addSet);
    this -> aoiEventManager -> onRemoveSubscriber(obj, removeSet);
    
    return true;
}

bool EuDistanceAOI::onHugePublisherMove(HOGameObject *obj, position_t oldMinPosX, position_t oldMinPosY, position_t oldMaxPosX, position_t oldMaxPosY) {
    // to find all the subscribers who can subscribe the publisher in old position
    map<entity_t, GameObject *> oldSubscribers = BaseAOI::findSubscribersInTheirRangeForHO(obj -> id, oldMinPosX, oldMinPosY, oldMaxPosX, oldMaxPosY);
    // to find all the subscribers who can subscribe the publisher in new position
    map<entity_t, GameObject *> newSubscribers = BaseAOI::findSubscribersInTheirRangeForHO(obj -> id, obj -> minPosX, obj -> minPosY, obj -> maxPosX, obj -> maxPosY);
    
    list<GameObject *> addSet, moveSet, removeSet;
    map<entity_t, GameObject *> :: iterator iter;
    for (iter = oldSubscribers . begin(); iter != oldSubscribers . end(); iter ++) {
        if (newSubscribers.find(iter -> first) != newSubscribers.end()) {
            moveSet.push_back(iter -> second);
            newSubscribers.erase(iter -> first);
        } else {
            removeSet.push_back(iter -> second);
        }
    }
    if (newSubscribers.size() > 0) {
        for (iter = newSubscribers . begin(); iter != newSubscribers . end(); iter ++) {
            addSet.push_back(iter -> second);
        }
    }
    
    this -> aoiEventManager -> onAddPublisher(obj, addSet);
    this -> aoiEventManager -> onRemovePublisher(obj, removeSet);
    this -> aoiEventManager -> onMovePublisher(obj, moveSet);
    
    return true;
}

bool EuDistanceAOI::onHugeSubscriberMove(HOGameObject *obj, position_t oldMinPosX, position_t oldMinPosY, position_t oldMaxPosX, position_t oldMaxPosY) {
    // to find all the publishers the obj can subscribe in old position
    map<entity_t, GameObject *> oldPublishers = findPublishersInRangeForHO(obj -> id, oldMinPosX, oldMinPosY, oldMaxPosX, oldMaxPosY, obj -> range);
    // to find all the publishers the obj can subscribe in new position
    map<entity_t, GameObject *> newPublishers = findPublishersInRangeForHO(obj -> id, obj -> minPosX, obj -> minPosY, obj -> maxPosX, obj -> maxPosY, obj -> range);
    
    list<GameObject *> addSet, moveSet, removeSet;
    map<entity_t, GameObject *> :: iterator iter;
    for (iter = oldPublishers . begin(); iter != oldPublishers . end(); iter ++) {
        if (newPublishers.find(iter -> first) != newPublishers.end()) {
            moveSet.push_back(iter -> second);
            newPublishers.erase(iter -> first);
        } else {
            removeSet.push_back(iter -> second);
        }
    }
    if (newPublishers.size() > 0) {
        for (iter = newPublishers . begin(); iter != newPublishers . end(); iter ++) {
            addSet.push_back(iter -> second);
        }
    }
    
    // do no change to moveSet
    this -> aoiEventManager -> onAddSubscriber(obj, addSet);
    this -> aoiEventManager -> onRemoveSubscriber(obj, removeSet);
    
    return true;
}

void EuDistanceAOI::findSubscribersInTheirRange(GameObject *obj, state_t state) {
    map<entity_t, GameObject *> subMaps = this -> BaseAOI::findSubscribersInTheirRange(obj -> id, obj -> posX, obj -> posY);
    list<GameObject *> subs = transMapsToLists(subMaps);
   
    // send messages to above subscribers
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddPublisher(obj, subs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemovePublisher(obj, subs);
    }
    
}

void EuDistanceAOI::findPublishersInRange(GameObject *obj, state_t state) {
    map<entity_t, GameObject *> pubMaps = this -> findPublishersInRange(obj -> id, obj -> posX, obj -> posY, obj -> range);
    list<GameObject *> pubs = transMapsToLists(pubMaps);
    
    // receive messages from above publishers
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddSubscriber(obj, pubs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemoveSubscriber(obj, pubs);
    }
    
}

map<entity_t, GameObject *> EuDistanceAOI::findPublishersInRange(entity_t objId, position_t posX, position_t posY, position_t range) {
    map<entity_t, GameObject *> pubs;
    map<entity_t, GameObject *>::iterator iter;
    map<entity_t, HOGameObject *>::iterator hugeIter;
    for (iter = this -> publishers . begin(); iter != this -> publishers . end(); iter ++) {
        if (isInRange2(posX, posY, iter -> second -> posX, iter -> second -> posY, range)) {
            pubs[iter -> first] = iter -> second;
        }
    }
    
    position_t minPosX, minPosY, maxPosX, maxPosY;
    for (hugeIter = this -> hugePublishers . begin(); hugeIter != this -> hugePublishers . end(); hugeIter ++) {
        minPosX = hugeIter -> second -> minPosX;
        minPosY = hugeIter -> second -> minPosY;
        maxPosX = hugeIter -> second -> maxPosX;
        maxPosY = hugeIter -> second -> maxPosY;
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
            pubs[hugeIter -> first] = hugeIter -> second;
        }
    }
    
    pubs.erase(objId);
    
    return pubs;
}

map<entity_t, GameObject *> EuDistanceAOI::findPublishersInRangeForHO(entity_t objId, position_t minPosX, position_t minPosY, position_t maxPosX, position_t maxPosY, position_t range) {
    map<entity_t, GameObject *> pubs;
    map<entity_t, GameObject *>::iterator iter;
    map<entity_t, HOGameObject *>::iterator hugeIter;
    
//    minPosX = minPosX < range ? 0 : minPosX - range;
//    minPosY = minPosY < range ? 0 : minPosY - range;
//    maxPosX = maxPosX + range >= this -> worldWidth ? this -> worldWidth - 1 : maxPosX + range;
//    maxPosY = maxPosY + range >= this -> worldHeight ? this -> worldHeight - 1 : maxPosY + range;
    
    position_t posX, posY;
    for (iter = this -> publishers . begin(); iter != this -> publishers . end(); iter ++) {
        posX = iter -> second -> posX;
        posY = iter -> second -> posY;
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
    }

    // huge object does not influence each other
    
//    position_t otherMinPosX, otherMinPosY, otherMaxPosX, otherMaxPosY;
//    for (hugeIter = this -> hugePublishers . begin(); hugeIter != this -> hugePublishers . end(); hugeIter ++) {
//        otherMinPosX = hugeIter -> second -> minPosX;
//        otherMinPosY = hugeIter -> second -> minPosY;
//        otherMaxPosX = hugeIter -> second -> maxPosX;
//        otherMaxPosY = hugeIter -> second -> maxPosY;
//        
//        if (minPosX > otherMaxPosX || maxPosX < otherMinPosX || minPosY > otherMaxPosY || maxPosY < otherMinPosY) {
//            // blank
//        } else {
//            pubs[hugeIter -> first] = hugeIter -> second;
//        }
//    }
    
//        if (isPointInRect(minPosX, minPosY, otherMinPosX, otherMinPosY, otherMaxPosX, otherMaxPosY) ||
//            isPointInRect(minPosX, maxPosY, otherMinPosX, otherMinPosY, otherMaxPosX, otherMaxPosY) ||
//            isPointInRect(maxPosX, minPosY, otherMinPosX, otherMinPosY, otherMaxPosX, otherMaxPosY) ||
//            isPointInRect(maxPosX, maxPosY, otherMinPosX, otherMinPosY, otherMaxPosX, otherMaxPosY) ||
//            isPointInRect(otherMinPosX, otherMinPosY, minPosX, minPosY, maxPosX, maxPosY) ||
//            isPointInRect(otherMinPosX, otherMaxPosY, minPosX, minPosY, maxPosX, maxPosY) ||
//            isPointInRect(otherMaxPosX, otherMinPosY, minPosX, minPosY, maxPosX, maxPosY) ||
//            isPointInRect(otherMaxPosX, otherMaxPosY, minPosX, minPosY, maxPosX, maxPosY)
//            ) {
//            pubs[hugeIter -> first] = hugeIter -> second;
//        }
//    }
    
    pubs.erase(objId);
    
    return pubs;
}

void EuDistanceAOI::findSubscribersInTheirRangeForHO(HOGameObject *obj, state_t state) {
    map<entity_t, GameObject *> subMaps = this -> BaseAOI::findSubscribersInTheirRangeForHO(obj -> id, obj -> minPosX, obj -> minPosY, obj -> maxPosX, obj -> maxPosY);
    list<GameObject *> subs = transMapsToLists(subMaps);
    
    // send messages to above subscribers
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddPublisher(obj, subs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemovePublisher(obj, subs);
    }

}

void EuDistanceAOI::findPublishersInRangeForHO(HOGameObject *obj, state_t state) {
    map<entity_t, GameObject *> pubMaps = this -> findPublishersInRangeForHO(obj -> id, obj -> minPosX, obj -> minPosY, obj -> maxPosX, obj -> maxPosY, obj -> range);
    list<GameObject *> pubs = transMapsToLists(pubMaps);
    
    // receive messages from above publishers
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddSubscriber(obj, pubs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemoveSubscriber(obj, pubs);
    }
}


