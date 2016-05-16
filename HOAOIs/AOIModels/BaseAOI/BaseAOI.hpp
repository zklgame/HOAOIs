//
//  BaseAOI.hpp
//  AOIs
//
//  Created by zklgame on 4/7/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef BaseAOI_hpp
#define BaseAOI_hpp

#include "../../GameBasics/GameObject.hpp"
#include "../../GameBasics/HOGameObject.hpp"
#include "../../AOIEventManager.hpp"

#include <map>
#include <inttypes.h>

class BaseAOI {
public:
    inline BaseAOI() { this -> aoiEventManager = new AOIEventManager(); };
    inline virtual ~BaseAOI() { delete this -> aoiEventManager; };
    
    virtual bool addObject(GameObject *obj);
    virtual bool removeObject(GameObject *obj);
    virtual bool onObjectMove(GameObject *obj, position_t oldPosX, position_t oldPosY);
    
    virtual bool addHugeObject(HOGameObject *obj);
    virtual bool removeHugeObject(HOGameObject *obj);
    virtual bool onHugeObjectMove(HOGameObject *obj, position_t oldMinPosX, position_t oldMinPosY, position_t oldMaxPosX, position_t oldMaxPosY);
    
protected:
    map<entity_t, GameObject *> subscribers;
    map<entity_t, HOGameObject *> hugeSubscribers;

    AOIEventManager *aoiEventManager;
    
    virtual bool addPublisher(GameObject *obj) = 0;
    virtual bool addSubscriber(GameObject *obj) = 0;
    virtual bool removePublisher(GameObject *obj) = 0;
    virtual bool removeSubscriber(GameObject *obj) = 0;
    
    virtual bool addHugePublisher(HOGameObject *obj) = 0;
    virtual bool addHugeSubscriber(HOGameObject *obj) = 0;
    virtual bool removeHugePublisher(HOGameObject *obj) = 0;
    virtual bool removeHugeSubscriber(HOGameObject *obj) = 0;
    
    virtual bool onPublisherMove(GameObject *obj, position_t oldPosX, position_t oldPosY) = 0;
    virtual bool onSubscriberMove(GameObject *obj, position_t oldPosX, position_t oldPosY) = 0;
    
    virtual bool onHugePublisherMove(HOGameObject *obj, position_t oldMinPosX, position_t oldMinPosY, position_t oldMaxPosX, position_t oldMaxPosY) = 0;
    virtual bool onHugeSubscriberMove(HOGameObject *obj, position_t oldMinPosX, position_t oldMinPosY, position_t oldMaxPosX, position_t oldMaxPosY) = 0;
    
    // ADD or REMOVE: find by obj
    // if state = 1, add; if state = 2, remove
    virtual void findSubscribersInTheirRange(GameObject *obj, state_t state) = 0;
    virtual void findPublishersInRange(GameObject *obj, state_t state) = 0;
    
    virtual void findSubscribersInTheirRangeForHO(HOGameObject *obj, state_t state) = 0;
    virtual void findPublishersInRangeForHO(HOGameObject *obj, state_t state) = 0;
    
    // MOVE: find by position
    map<entity_t, GameObject *> findSubscribersInTheirRange(entity_t objId, position_t posX, position_t posY) {
        map<entity_t, GameObject *> subs;
        map<entity_t, GameObject *>::iterator iter;
        map<entity_t, HOGameObject *>::iterator hugeIter;
        
        for (iter = this -> subscribers . begin(); iter != this -> subscribers . end(); iter ++) {
            if (isInRange2(posX, posY, iter -> second -> posX, iter -> second -> posY, iter -> second -> range)) {
                subs[iter -> first] = iter -> second;
            }
        }
        
        position_t range, minPosX, minPosY, maxPosX, maxPosY;
        for (hugeIter = this -> hugeSubscribers . begin(); hugeIter != this -> hugeSubscribers . end(); hugeIter ++) {
            range = hugeIter -> second -> range;
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
                subs[hugeIter -> first] = hugeIter -> second;
            }
        }
        
        subs.erase(objId);
        
        return subs;
    };
    
    map<entity_t, GameObject *> findSubscribersInTheirRangeForHO(entity_t objId, position_t minPosX, position_t minPosY, position_t maxPosX, position_t maxPosY) {
        map<entity_t, GameObject *> subs;
        map<entity_t, GameObject *>::iterator iter;
        map<entity_t, HOGameObject *>::iterator hugeIter;
        
        position_t range, posX, posY;
        for (iter = this -> subscribers . begin(); iter != this -> subscribers . end(); iter ++) {
            range = iter -> second -> range;
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
                subs[iter -> first] = iter -> second;
            }
        }
        
        // hugeObject does not influence each other
        
//        position_t otherMinPosX, otherMinPosY, otherMaxPosX, otherMaxPosY;
//        for (hugeIter = this -> hugeSubscribers . begin(); hugeIter != this -> hugeSubscribers . end(); hugeIter ++) {
//            range = hugeIter -> second -> range;
//            otherMinPosX = hugeIter -> second -> minPosX;
//            otherMinPosY = hugeIter -> second -> minPosY;
//            otherMaxPosX = hugeIter -> second -> maxPosX;
//            otherMaxPosY = hugeIter -> second -> maxPosY;
//            
//            if (minPosX > otherMaxPosX + range || otherMinPosX - range > maxPosX || minPosY > otherMaxPosY + range || otherMinPosY - range > maxPosY) {
//                // blank
//            } else {
//                subs[hugeIter -> first] = hugeIter -> second;
//            }
//        }
        
//            if (isPointInRect(minPosX, minPosY, otherMinPosX - range, otherMinPosY - range, otherMaxPosX + range, otherMaxPosY + range) ||
//                isPointInRect(minPosX, maxPosY, otherMinPosX - range, otherMinPosY - range, otherMaxPosX + range, otherMaxPosY + range) ||
//                isPointInRect(maxPosX, minPosY, otherMinPosX - range, otherMinPosY - range, otherMaxPosX + range, otherMaxPosY + range) ||
//                isPointInRect(maxPosX, maxPosY, otherMinPosX - range, otherMinPosY - range, otherMaxPosX + range, otherMaxPosY + range) ||
//                
//                isPointInRect(otherMinPosX - range, otherMinPosY - range, minPosX, minPosY, maxPosX, maxPosY) ||
//                isPointInRect(otherMinPosX - range, otherMaxPosY + range, minPosX, minPosY, maxPosX, maxPosY) ||
//                isPointInRect(otherMaxPosX + range, otherMinPosY - range, minPosX, minPosY, maxPosX, maxPosY) ||
//                isPointInRect(otherMaxPosX + range, otherMaxPosY + range, minPosX, minPosY, maxPosX, maxPosY)
//                ) {
//                subs[hugeIter -> first] = hugeIter -> second;
//            }
//        }
        
        subs.erase(objId);
        
        return subs;

    }
    
    virtual map<entity_t, GameObject *> findPublishersInRange(entity_t objId, position_t posX, position_t posY, position_t range) = 0;
    virtual map<entity_t, GameObject *> findPublishersInRangeForHO(entity_t objId, position_t minPosX, position_t minPosY, position_t maxPosX, position_t maxPosY, position_t range) = 0;
    
    inline virtual bool isInRange2(int32_t posX, int32_t posY, int32_t otherPosX, int32_t otherPosY, int32_t range) {
        position_t distance2 = (posX - otherPosX) * (posX - otherPosX) + (posY - otherPosY) * (posY - otherPosY);
        return distance2 <= range * range;
    }
    
    inline virtual bool isPointInRect(int32_t posX, int32_t posY, int32_t minPosX, int32_t minPosY, int32_t maxPosX, int32_t maxPosY) {
        if (posX >= minPosX && posX <= maxPosX && posY >= minPosY && posY <= maxPosY) {
            return true;
        }
        return false;
    }
    
    inline virtual bool isInRange2WithDouble(double posX, double posY, double otherPosX, double otherPosY, double range) {
        double distance2 = (posX - otherPosX) * (posX - otherPosX) + (posY - otherPosY) * (posY - otherPosY);
        return distance2 <= range * range;
    }
    
    inline virtual bool isInRange(position_t pos, position_t otherPos, position_t range) {
        position_t del = pos >= otherPos ? pos - otherPos : otherPos - pos;
        return del <= range;
    }
    
    inline virtual bool isInRangeWithDouble(double pos, double otherPos, double range) {
        double del = pos >= otherPos ? pos - otherPos : otherPos - pos;
        return del <= range;
    }
    
    inline virtual list<GameObject *> transMapsToLists(map<entity_t, GameObject *> maps) {
        list<GameObject *> objects;
        map<entity_t, GameObject *>::iterator iter;
        for (iter = maps.begin(); iter != maps.end(); iter ++) {
            objects.push_back(iter -> second);
        }
        return objects;
    }
    
    inline virtual map<entity_t, GameObject *> addTwoMaps(map<entity_t, GameObject *> map1, map<entity_t, GameObject *> map2) {
        map<entity_t, GameObject *>::iterator iter;
        for (iter = map2.begin(); iter != map2.end(); iter ++) {
            if (!iter -> second -> isHugeObject) {
                map1[iter -> first] = iter -> second;
            }
        }
        return map1;
    }

};

#endif /* BaseAOI_hpp */
