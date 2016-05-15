//
//  EuDistance_TowerAOI.cpp
//  AOIs
//
//  Created by zklgame on 4/14/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "../GameBasics/GameWorld.hpp"
#include "../AOIServices/EuDistanceAOIService.hpp"
#include "../AOIServices/NTreeAOIService.hpp"

#include <iostream>
#include <vector>

using namespace std;

int main() {
    // create object data
    entity_t objectNum = 10000;
    entity_t movedNum = 5000;
    entity_t leaveNum = 2000;
    
    vector<GameObject *> gameObjects, gameObjectsCopy2;
    vector<GameObject *> MovedgameObjects1, MovedgameObjects2;
    vector<GameObject *> leavedObjects1, leavedObjects2;
    vector<int> movedPosX;
    vector<int> movedPosY;
    
    // for huge object
    entity_t hugeObjectNum = 1000;
    entity_t hugeMovedNum = 500;
    entity_t hugeLeaveNum = 200;
    vector<HOGameObject *> hugeGameObjects, hugeGameObjectsCopy2;
    vector<HOGameObject *> hugeMovedgameObjects1, hugeMovedgameObjects2;
    vector<HOGameObject *> hugeLeavedObjects1, hugeLeavedObjects2;
    vector<int> movedMinPosX, movedMinPosY, movedMaxPosX, movedMaxPosY;
    
    entity_t id;
    type_t type;
    position_t posX, posY, range;
    position_t maxRange = 200;
    position_t minPosX, minPosY, maxPosX, maxPosY;
    
    srand((int)time(0));
    for (int i = 0; i < objectNum; i ++) {
        type = rand() % 3 + 1;
        posX = rand() % 10000;
        posY = rand() % 10000;
        range = rand() % maxRange;
        GameObject *obj = new GameObject(i, type, posX, posY, range);
        GameObject *obj2 = new GameObject(i, type, posX, posY, range);
        gameObjects.push_back(obj);
        gameObjectsCopy2.push_back(obj2);
    }
    
    srand((int)time(0));
    for (int i = 0; i < movedNum; i ++) {
        id = rand() % objectNum;
        MovedgameObjects1.push_back(gameObjects[id]);
        MovedgameObjects2.push_back(gameObjectsCopy2[id]);
        movedPosX.push_back(rand() % 500);
        movedPosY.push_back(rand() % 500);
    }
    
    srand((int)time(0));
    for (int i = 0; i < leaveNum; i ++) {
        id = rand() % objectNum;
        leavedObjects1.push_back(gameObjects[id]);
        leavedObjects2.push_back(gameObjectsCopy2[id]);
    }
    
    // make hugeSize to be 200 - 500
    position_t hugeWidth, hugeHeight;
    position_t hugeLengthBase = 300;
    srand((int)time(0));
    for (int i = objectNum; i < objectNum + hugeObjectNum; i ++) {
        type = rand() % 3 + 1;
//        type = 1;
        hugeWidth = rand() % hugeLengthBase + 200;
        hugeHeight = rand() % hugeLengthBase + 200;
        minPosX = rand() % (10000 - 500);
        minPosY = rand() % (10000 - 500);
        maxPosX = minPosX + hugeWidth;
        maxPosY = minPosY + hugeHeight;
        range = rand() % maxRange;
        
        HOGameObject *obj = new HOGameObject(i, type, minPosX, minPosY, maxPosX, maxPosY, range);
        HOGameObject *obj2 = new HOGameObject(i, type, minPosX, minPosY, maxPosX, maxPosY, range);
        hugeGameObjects.push_back(obj);
        hugeGameObjectsCopy2.push_back(obj2);
    }
    
    srand((int)time(0));
    position_t tmp1, tmp2, tmpMinPosX, tmpMinPosY;
    for (int i = 0; i < hugeMovedNum; i ++) {
        id = rand() % hugeObjectNum;
        hugeMovedgameObjects1.push_back(hugeGameObjects[id]);
        hugeMovedgameObjects2.push_back(hugeGameObjectsCopy2[id]);
        tmp1 = rand() % 500;
        tmp2 = rand() % 500;
        tmpMinPosX = hugeGameObjects[id] -> minPosX < tmp1 ? 0 : hugeGameObjects[id] -> minPosX - tmp1;
        tmpMinPosY = hugeGameObjects[id] -> minPosY < tmp2 ? 0 : hugeGameObjects[id] -> minPosY - tmp2;
        movedMinPosX.push_back(tmpMinPosX);
        movedMinPosY.push_back(tmpMinPosY);
        movedMaxPosX.push_back(hugeGameObjects[id] -> maxPosX - hugeGameObjects[id] -> minPosX + tmpMinPosX);
        movedMaxPosY.push_back(hugeGameObjects[id] -> maxPosY - hugeGameObjects[id] -> minPosY + tmpMinPosY);
    }
    
    srand((int)time(0));
    for (int i = 0; i < hugeLeaveNum; i ++) {
        id = rand() % hugeObjectNum;
        hugeLeavedObjects1.push_back(hugeGameObjects[id]);
        hugeLeavedObjects2.push_back(hugeGameObjectsCopy2[id]);
    }
    
    GameWorld *world1 = new GameWorld();
    world1 -> aoi = new EuDistanceAOIService(10000, 10000);
    GameWorld * world2 = new GameWorld();
    world2 -> aoi = new NTreeAOIService(10000, 10000, 100, 5);
    
    //////////////////////
    // test addObject
    //////////////////////
    vector<GameObject *>::iterator iter;
    for (iter = gameObjects . begin(); iter != gameObjects . end(); iter ++) {
        world1 -> addObject(*iter);
    }
    for (iter = gameObjectsCopy2 . begin(); iter != gameObjectsCopy2 . end(); iter ++) {
        world2 -> addObject(*iter);
    }
    
    vector<HOGameObject *>::iterator hugeIter;
    for (hugeIter = hugeGameObjects . begin(); hugeIter != hugeGameObjects . end(); hugeIter ++) {
        world1 -> addHugeObject(*hugeIter);
    }
    for (hugeIter = hugeGameObjectsCopy2 . begin(); hugeIter != hugeGameObjectsCopy2 . end(); hugeIter ++) {
        world2 -> addHugeObject(*hugeIter);
    }
    
    bool isAddRight = true;
    for (int i = 0; i < objectNum; i ++) {
        if (gameObjects[i] -> messageNum == gameObjectsCopy2[i] -> messageNum &&
            gameObjects[i] -> addMessageNum == gameObjectsCopy2[i] -> addMessageNum &&
            gameObjects[i] -> leaveMessageNum == gameObjectsCopy2[i] -> leaveMessageNum &&
            gameObjects[i] -> moveMessageNum == gameObjectsCopy2[i] -> moveMessageNum) {
            
        } else {
            cout << "isAddRight : FALSE!!! at " << i << endl << endl;
            
            isAddRight = false;
            return 1;
        }
    }
    
    for (int i = 0; i < hugeObjectNum; i ++) {
        if (hugeGameObjects[i] -> messageNum == hugeGameObjectsCopy2[i] -> messageNum &&
            hugeGameObjects[i] -> addMessageNum == hugeGameObjectsCopy2[i] -> addMessageNum &&
            hugeGameObjects[i] -> leaveMessageNum == hugeGameObjectsCopy2[i] -> leaveMessageNum &&
            hugeGameObjects[i] -> moveMessageNum == hugeGameObjectsCopy2[i] -> moveMessageNum) {
            
        } else {
            cout << "isAddRight : FALSE!!! at hugeObject " << i << endl << endl;
            
            isAddRight = false;
            return 1;
        }
    }
    
    
    cout << "??????????????????????????????????" << endl;
    cout << "ADD OBJECT PASS" << endl;
    cout << "??????????????????????????????????" << endl << endl;
    
    
    //////////////////////
    // test moveObject
    //////////////////////
    for (int i = 0; i < MovedgameObjects1.size(); i ++) {
        MovedgameObjects1[i] -> move(movedPosX[i], movedPosY[i]);
    }
    for (int i = 0; i < MovedgameObjects2.size(); i ++) {
        MovedgameObjects2[i] -> move(movedPosX[i], movedPosY[i]);
    }
    for (int i = 0; i < hugeMovedgameObjects1.size(); i ++) {
        hugeMovedgameObjects1[i] -> move(movedMinPosX[i], movedMinPosY[i], movedMaxPosX[i], movedMaxPosY[i]);
    }
    for (int i = 0; i < hugeMovedgameObjects2.size(); i ++) {
        hugeMovedgameObjects2[i] -> move(movedMinPosX[i], movedMinPosY[i], movedMaxPosX[i], movedMaxPosY[i]);
    }
    
    bool isMoveRight = true;
    for (int i = 0; i < objectNum; i ++) {
        if (gameObjects[i] -> messageNum == gameObjectsCopy2[i] -> messageNum &&
            gameObjects[i] -> addMessageNum == gameObjectsCopy2[i] -> addMessageNum &&
            gameObjects[i] -> leaveMessageNum == gameObjectsCopy2[i] -> leaveMessageNum &&
            gameObjects[i] -> moveMessageNum == gameObjectsCopy2[i] -> moveMessageNum) {
            
        } else {
            cout << "isMoveRight : FALSE!!! at " << i << endl << endl;
            isMoveRight = false;
            return 1;
        }
    }
    
    for (int i = 0; i < hugeObjectNum; i ++) {
        if (hugeGameObjects[i] -> messageNum == hugeGameObjectsCopy2[i] -> messageNum &&
            hugeGameObjects[i] -> addMessageNum == hugeGameObjectsCopy2[i] -> addMessageNum &&
            hugeGameObjects[i] -> leaveMessageNum == hugeGameObjectsCopy2[i] -> leaveMessageNum &&
            hugeGameObjects[i] -> moveMessageNum == hugeGameObjectsCopy2[i] -> moveMessageNum) {
            
        } else {
            cout << "isMoveHugeRight : FALSE!!! at hugeObject " << i << endl << endl;
            
            isMoveRight = false;
            return 1;
        }
    }
    
    //////////////////////
    // test removeObject
    //////////////////////
    for (int i = 0; i < leavedObjects1.size(); i ++) {
        world1 -> removeObject(leavedObjects1[i]);
    }
    for (int i = 0; i < leavedObjects2.size(); i ++) {
        world2 -> removeObject(leavedObjects2[i]);
    }
    
    for (int i = 0; i < hugeLeavedObjects1.size(); i ++) {
        world1 -> removeHugeObject(hugeLeavedObjects1[i]);
    }
    for (int i = 0; i < hugeLeavedObjects2.size(); i ++) {
        world2 -> removeHugeObject(hugeLeavedObjects2[i]);
    }
    
    bool isRemoveRight = true;
    for (int i = 0; i < objectNum; i ++) {
        if (gameObjects[i] -> messageNum == gameObjectsCopy2[i] -> messageNum &&
            gameObjects[i] -> addMessageNum == gameObjectsCopy2[i] -> addMessageNum &&
            gameObjects[i] -> leaveMessageNum == gameObjectsCopy2[i] -> leaveMessageNum &&
            gameObjects[i] -> moveMessageNum == gameObjectsCopy2[i] -> moveMessageNum) {
            
        } else {
            cout << "isRemoveRight : FALSE!!! at " << i << endl << endl;
            isRemoveRight = false;
            
            //test code
            cout << gameObjects[i] -> id << " | " << uint16_t(gameObjects[i] -> type) << " | " << gameObjects[i] -> posX << " | " << gameObjects[i] -> posY << " | " << gameObjects[i] -> range << " | " << gameObjects[i] -> messageNum << " | " << gameObjects[i] -> addMessageNum << " | " << gameObjects[i] -> moveMessageNum << " | " << gameObjects[i] -> leaveMessageNum << endl;
            
            cout << i << " | " << uint16_t(gameObjectsCopy2[i] -> type) << " | " << gameObjectsCopy2[i] -> posX << " | " << gameObjectsCopy2[i] -> posY << " | " << gameObjectsCopy2[i] -> range << " | " << gameObjectsCopy2[i] -> messageNum << " | " << gameObjectsCopy2[i] -> addMessageNum << " | " << gameObjectsCopy2[i] -> moveMessageNum << " | " << gameObjectsCopy2[i] -> leaveMessageNum << endl;
            
            map<entity_t, entity_t>::iterator listIter;
            cout << "more: " << endl;
            for (listIter = gameObjectsCopy2[i] -> removeMessageDetail . begin(); listIter != gameObjectsCopy2[i] -> removeMessageDetail . end(); listIter ++) {
            
                if (gameObjects[i] -> removeMessageDetail . find(listIter -> first) == gameObjects[i] -> removeMessageDetail . end()) {
                    if (listIter -> first < objectNum) {
                        cout << listIter -> first << " | " << uint16_t(gameObjects[listIter -> first] -> type) << " | " << gameObjects[listIter -> first] -> posX << " | " << gameObjects[listIter -> first] -> posY << endl;
                    } else {
                        cout << listIter -> first << " | " << uint16_t(hugeGameObjects[listIter -> first - objectNum] -> type) << " | " << hugeGameObjects[listIter -> first - objectNum] -> minPosX << " | " << hugeGameObjects[listIter -> first - objectNum] -> maxPosX << " | " << hugeGameObjects[listIter -> first - objectNum] -> minPosY << " | " << hugeGameObjects[listIter -> first - objectNum] -> maxPosY << " | " << hugeGameObjects[listIter -> first - objectNum] -> range << endl;
                    }
                }
            }
            
            cout << "less: " << endl;
            for (listIter = gameObjects[i] -> removeMessageDetail . begin(); listIter != gameObjects[i] -> removeMessageDetail . end(); listIter ++) {
                if (gameObjectsCopy2[i] -> removeMessageDetail . find(listIter -> first) == gameObjectsCopy2[i] -> removeMessageDetail . end()) {
                    if (listIter -> first < objectNum) {
                        cout << listIter -> first << " | " << uint16_t(gameObjects[listIter -> first] -> type) << " | " << gameObjects[listIter -> first] -> posX << " | " << gameObjects[listIter -> first] -> posY << endl;
                    } else {
                        cout << listIter -> first << " | " << uint16_t(hugeGameObjects[listIter -> first - objectNum] -> type) << " | " << hugeGameObjects[listIter -> first - objectNum] -> minPosX << " | " << hugeGameObjects[listIter -> first - objectNum] -> maxPosX << " | " << hugeGameObjects[listIter -> first - objectNum] -> minPosY << " | " << hugeGameObjects[listIter -> first - objectNum] -> maxPosY << " | " << hugeGameObjects[listIter -> first - objectNum] -> range << endl;
                    }
                }
            }
            
            return 1;
        }
    }
    
    for (int i = 0; i < hugeObjectNum; i ++) {
        if (hugeGameObjects[i] -> messageNum == hugeGameObjectsCopy2[i] -> messageNum &&
            hugeGameObjects[i] -> addMessageNum == hugeGameObjectsCopy2[i] -> addMessageNum &&
            hugeGameObjects[i] -> leaveMessageNum == hugeGameObjectsCopy2[i] -> leaveMessageNum &&
            hugeGameObjects[i] -> moveMessageNum == hugeGameObjectsCopy2[i] -> moveMessageNum) {
            
        } else {
            cout << "isRemoveHugeRight : FALSE!!! at hugeObject " << i << endl << endl;
            
            isRemoveRight = false;
            return 1;
        }
    }
    
    
    cout << "??????????????????????????????????" << endl;
    cout << "REMOVE OBJECT PASS" << endl;
    cout << "??????????????????????????????????" << endl << endl;
    
    delete world1 -> aoi;
    delete world1;
    delete world2 -> aoi;
    delete world2;
    
    return 0;
}



////test code
//cout << hugeGameObjects[i] -> id << " | " << uint16_t(hugeGameObjects[i] -> type) << " | " << hugeGameObjects[i] -> minPosX << " | " << hugeGameObjects[i] -> minPosY << " | " << hugeGameObjects[i] -> maxPosX << " | " << hugeGameObjects[i] -> maxPosY << " | " << hugeGameObjects[i] -> range << " | " << hugeGameObjects[i] -> messageNum << " | " << hugeGameObjects[i] -> addMessageNum << " | " << hugeGameObjects[i] -> moveMessageNum << " | " << hugeGameObjects[i] -> leaveMessageNum << endl;
//
//cout << i + objectNum << " | " << uint16_t(hugeGameObjectsCopy2[i] -> type) << " | " << hugeGameObjectsCopy2[i] -> posX << " | " << hugeGameObjectsCopy2[i] -> posY << " | " << hugeGameObjectsCopy2[i] -> range << " | " << hugeGameObjectsCopy2[i] -> messageNum << " | " << hugeGameObjectsCopy2[i] -> addMessageNum << " | " << hugeGameObjectsCopy2[i] -> moveMessageNum << " | " << hugeGameObjectsCopy2[i] -> leaveMessageNum << endl;
//
//map<entity_t, entity_t>::iterator listIter;
//cout << "more: " << endl;
//for (listIter = hugeGameObjectsCopy2[i] -> addMessageDetail . begin(); listIter != hugeGameObjectsCopy2[i] -> addMessageDetail . end(); listIter ++) {
//    if (hugeGameObjects[i] -> addMessageDetail . find(listIter -> first) == hugeGameObjects[i] -> addMessageDetail . end()) {
//        if (listIter -> first < objectNum) {
//            cout << listIter -> first << " | " << uint16_t(gameObjects[listIter -> first] -> type) << " | " << gameObjects[listIter -> first] -> posX << " | " << gameObjects[listIter -> first] -> posY << endl;
//        } else {
//            cout << listIter -> first << " | " << uint16_t(hugeGameObjects[listIter -> first - objectNum] -> type) << " | " << hugeGameObjects[listIter -> first - objectNum] -> minPosX << " | " << hugeGameObjects[listIter -> first - objectNum] -> maxPosX << " | " << hugeGameObjects[listIter -> first - objectNum] -> minPosY << " | " << hugeGameObjects[listIter -> first - objectNum] -> maxPosY << " | " << hugeGameObjects[listIter -> first - objectNum] -> range << endl;
//        }
//    }
//}
//
//cout << "less: " << endl;
//for (listIter = hugeGameObjects[i] -> addMessageDetail . begin(); listIter != hugeGameObjects[i] -> addMessageDetail . end(); listIter ++) {
//    if (hugeGameObjectsCopy2[i] -> addMessageDetail . find(listIter -> first) == hugeGameObjectsCopy2[i] -> addMessageDetail . end()) {
//        if (listIter -> first < objectNum) {
//            cout << listIter -> first << " | " << uint16_t(gameObjects[listIter -> first] -> type) << " | " << gameObjects[listIter -> first] -> posX << " | " << gameObjects[listIter -> first] -> posY << endl;
//        } else {
//            cout << listIter -> first << " | " << uint16_t(hugeGameObjects[listIter -> first - objectNum] -> type) << " | " << hugeGameObjects[listIter -> first - objectNum] -> minPosX << " | " << hugeGameObjects[listIter -> first - objectNum] -> maxPosX << " | " << hugeGameObjects[listIter -> first - objectNum] -> minPosY << " | " << hugeGameObjects[listIter -> first - objectNum] -> maxPosY << " | " << hugeGameObjects[listIter -> first - objectNum] -> range << endl;
//        }
//    }
//}








//test code
//cout << gameObjects[i] -> id << " | " << uint16_t(gameObjects[i] -> type) << " | " << gameObjects[i] -> posX << " | " << gameObjects[i] -> posY << " | " << gameObjects[i] -> range << " | " << gameObjects[i] -> messageNum << " | " << gameObjects[i] -> addMessageNum << " | " << gameObjects[i] -> moveMessageNum << " | " << gameObjects[i] -> leaveMessageNum << endl;
//
//cout << i << " | " << uint16_t(gameObjectsCopy2[i] -> type) << " | " << gameObjectsCopy2[i] -> posX << " | " << gameObjectsCopy2[i] -> posY << " | " << gameObjectsCopy2[i] -> range << " | " << gameObjectsCopy2[i] -> messageNum << " | " << gameObjectsCopy2[i] -> addMessageNum << " | " << gameObjectsCopy2[i] -> moveMessageNum << " | " << gameObjectsCopy2[i] -> leaveMessageNum << endl;
//
//map<entity_t, entity_t>::iterator listIter;
//cout << "more: " << endl;
//for (listIter = gameObjectsCopy2[i] -> addMessageDetail . begin(); listIter != gameObjectsCopy2[i] -> addMessageDetail . end(); listIter ++) {
//
//    if (gameObjects[i] -> addMessageDetail . find(listIter -> first) == gameObjects[i] -> addMessageDetail . end()) {
//        if (listIter -> first < objectNum) {
//            cout << listIter -> first << " | " << uint16_t(gameObjects[listIter -> first] -> type) << " | " << gameObjects[listIter -> first] -> posX << " | " << gameObjects[listIter -> first] -> posY << endl;
//        } else {
//            cout << listIter -> first << " | " << uint16_t(hugeGameObjects[listIter -> first - objectNum] -> type) << " | " << hugeGameObjects[listIter -> first - objectNum] -> minPosX << " | " << hugeGameObjects[listIter -> first - objectNum] -> maxPosX << " | " << hugeGameObjects[listIter -> first - objectNum] -> minPosY << " | " << hugeGameObjects[listIter -> first - objectNum] -> maxPosY << " | " << hugeGameObjects[listIter -> first - objectNum] -> range << endl;
//        }
//    }
//}
//
//cout << "less: " << endl;
//for (listIter = gameObjects[i] -> addMessageDetail . begin(); listIter != gameObjects[i] -> addMessageDetail . end(); listIter ++) {
//    if (gameObjectsCopy2[i] -> addMessageDetail . find(listIter -> first) == gameObjectsCopy2[i] -> addMessageDetail . end()) {
//        if (listIter -> first < objectNum) {
//            cout << listIter -> first << " | " << uint16_t(gameObjects[listIter -> first] -> type) << " | " << gameObjects[listIter -> first] -> posX << " | " << gameObjects[listIter -> first] -> posY << endl;
//        } else {
//            cout << listIter -> first << " | " << uint16_t(hugeGameObjects[listIter -> first - objectNum] -> type) << " | " << hugeGameObjects[listIter -> first - objectNum] -> minPosX << " | " << hugeGameObjects[listIter -> first - objectNum] -> maxPosX << " | " << hugeGameObjects[listIter -> first - objectNum] -> minPosY << " | " << hugeGameObjects[listIter -> first - objectNum] -> maxPosY << " | " << hugeGameObjects[listIter -> first - objectNum] -> range << endl;
//        }
//    }
//}

