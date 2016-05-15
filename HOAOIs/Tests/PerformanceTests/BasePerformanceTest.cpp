//
//  BasePerformanceTest.cpp
//  AOIs
//
//  Created by zklgame on 5/3/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "BasePerformanceTest.hpp"

BasePerformanceTest::BasePerformanceTest() {
    this -> world = new GameWorld(width, height);
    
    // !!!!
    // aoi is set out of the class !!!
}

BasePerformanceTest::~BasePerformanceTest() {
    delete world;
}

void BasePerformanceTest::test(int i) {
    initGame(i);
    
    realTest(i);
    
    deInitGame();
}

void BasePerformanceTest::testAdd(int i) {
    vector<GameObject *>::iterator iter;
    vector<HOGameObject *>::iterator hugeIter;

    for (iter = gameObjects . begin(); iter != gameObjects . end(); iter ++) {
        world -> addObject(*iter);
    }
    for (hugeIter = hugeGameObjects . begin(); hugeIter != hugeGameObjects . end(); hugeIter ++) {
        world -> addHugeObject(*hugeIter);
    }
    cout << "add:" << endl;
    cout << "\taddMsgNum: " << world -> addMessageNum << endl;
    cout << "\tmoveMsgNum: " << world -> moveMessageNum << endl;
    cout << "\tleaveMsgNum: " << world -> leaveMessageNum << endl << endl;
}

void BasePerformanceTest::testMove(int i) {
    for (int i = 0; i < movedObjects.size(); i ++) {
        movedObjects[i] -> move(movedPosX[i], movedPosY[i]);
    }
    for (int i = 0; i < hugeMovedObjects.size(); i ++) {
        hugeMovedObjects[i] -> move(hugeMovedMinPosX[i], hugeMovedMinPosY[i], hugeMovedMaxPosX[i], hugeMovedMaxPosY[i]);
    }
    cout << "move:" << endl;
    cout << "\taddMsgNum: " << world -> addMessageNum << endl;
    cout << "\tmoveMsgNum: " << world -> moveMessageNum << endl;
    cout << "\tleaveMsgNum: " << world -> leaveMessageNum << endl << endl;
}

void BasePerformanceTest::testLeave(int i) {
    for (int i = 0; i < leavedObjects.size(); i ++) {
        world -> removeObject(leavedObjects[i]);
    }
    for (int i = 0; i < hugeLeavedObjects.size(); i ++) {
        world -> removeHugeObject(hugeLeavedObjects[i]);
    }
    cout << "leave:" << endl;
    cout << "\taddMsgNum: " << world -> addMessageNum << endl;
    cout << "\tmoveMsgNum: " << world -> moveMessageNum << endl;
    cout << "\tleaveMsgNum: " << world -> leaveMessageNum << endl << endl;
}

void BasePerformanceTest::initGame(int i) {
    gameObjects.clear();
    movedObjects.clear();
    leavedObjects.clear();
    hugeGameObjects.clear();
    hugeMovedObjects.clear();
    hugeLeavedObjects.clear();
    
    movedPosX.clear();
    movedPosY.clear();
        
    int id, type, posX, posY, range, minPosX, minPosY, maxPosX, maxPosY;
    
    char num[2];
    sprintf(num, "%d", i);
    // read init data
    char path[100] = "/Users/zklgame/WorkPlace/HOAOIs/HOAOIs/Tests/Datas/data";
    strcat(path, num);
    strcat(path, "/data_init.txt");
    fp = fopen(path, "r+");
    if (NULL == fp) {
        printf("ERROR! read init error\n");
        printf("%s\n", path);
        return;
    }
    
    while (fscanf(fp, "%d %d %d %d %d\n", &id, &type, &posX, &posY, &range) != EOF) {
        GameObject *obj = new GameObject(id, type, posX, posY, range);
        gameObjects.push_back(obj);
    }
    
    // read huge init data
    char hugePath[100] = "/Users/zklgame/WorkPlace/HOAOIs/HOAOIs/Tests/Datas/data";
    strcat(hugePath, num);
    strcat(hugePath, "/huge_data_init.txt");
    fp = fopen(hugePath, "r+");
    if (NULL == fp) {
        printf("ERROR! read huge init error\n");
        printf("%s\n", hugePath);
        return;
    }
    
    while (fscanf(fp, "%d %d %d %d %d %d %d\n", &id, &type, &minPosX, &minPosY, &maxPosX, &maxPosY, &range) != EOF) {
        HOGameObject *obj = new HOGameObject(id, type, minPosX, minPosY, maxPosX, maxPosY, range);
        hugeGameObjects.push_back(obj);
    }
    
    // read move data
    char movePath[100] = "/Users/zklgame/WorkPlace/HOAOIs/HOAOIs/Tests/Datas/data";
    strcat(movePath, num);
    strcat(movePath, "/data_move.txt");
    fp = fopen(movePath, "r+");
    if (NULL == fp) {
        printf("ERROR! read move error\n");
        printf("%s\n", movePath);
        return;
    }
    
    int tmpX, tmpY;
    while (fscanf(fp, "%d %d %d %d\n", &i, &id, &posX, &posY) != EOF) {
        id -= 1;
        movedObjects.push_back(gameObjects[id]);
        if (i < movedNum * 0.2) {
            movedPosX.push_back(posX);
            movedPosY.push_back(posY);
        } else {
            tmpX = gameObjects[id] -> posX + posX;
            tmpY = gameObjects[id] -> posY + posY;
            if (tmpX < 0) {
                tmpX = 0;
            } else if (tmpX >= width) {
                tmpX = width - 1;
            }
            if (tmpY < 0) {
                tmpY = 0;
            } else if (tmpY >= height) {
                tmpY = height - 1;
            }
            movedPosX.push_back(tmpX);
            movedPosY.push_back(tmpY);
        }
    }
    
    // read huge move data
    char hugeMovePath[100] = "/Users/zklgame/WorkPlace/HOAOIs/HOAOIs/Tests/Datas/data";
    strcat(hugeMovePath, num);
    strcat(hugeMovePath, "/huge_data_move.txt");
    fp = fopen(hugeMovePath, "r+");
    if (NULL == fp) {
        printf("ERROR! read huge move error\n");
        printf("%s\n", hugeMovePath);
        return;
    }
    
    while (fscanf(fp, "%d %d %d %d\n", &i, &id, &posX, &posY) != EOF) {
        id -= 1;
        hugeMovedObjects.push_back(hugeGameObjects[id - objectNum]);
        if (i < hugeMovedNum * 0.2) {
            hugeMovedMinPosX.push_back(posX);
            hugeMovedMinPosY.push_back(posY);
            hugeMovedMaxPosX.push_back(hugeGameObjects[id - objectNum] -> maxPosX - hugeGameObjects[id - objectNum] -> minPosX + posX);
            hugeMovedMaxPosY.push_back(hugeGameObjects[id - objectNum] -> maxPosY - hugeGameObjects[id - objectNum] -> minPosY + posY);
        } else {
            if (posX >= 0) {
                tmpX = hugeGameObjects[id - objectNum] -> maxPosX + posX;
                if (tmpX >= width) {
                    tmpX = width - 1;
                }
                hugeMovedMaxPosX.push_back(tmpX);
                hugeMovedMinPosX.push_back(hugeGameObjects[id - objectNum] -> minPosX - hugeGameObjects[id - objectNum] -> maxPosX + tmpX);
            } else {
                tmpX = hugeGameObjects[id - objectNum] -> minPosX + posX;
                if (tmpX < 0) {
                    tmpX = 0;
                }
                hugeMovedMinPosX.push_back(tmpX);
                hugeMovedMaxPosX.push_back(hugeGameObjects[id - objectNum] -> maxPosX - hugeGameObjects[id - objectNum] -> minPosX + tmpX);
            }
            if (posY >= 0) {
                tmpY = hugeGameObjects[id - objectNum] -> maxPosY + posY;
                if (tmpY >= height) {
                    tmpY = height - 1;
                }
                hugeMovedMaxPosY.push_back(tmpY);
                hugeMovedMinPosY.push_back(hugeGameObjects[id - objectNum] -> minPosY - hugeGameObjects[id - objectNum] -> maxPosY + tmpY);
            } else {
                tmpY = hugeGameObjects[id - objectNum] -> minPosY + posY;
                if (tmpY < 0) {
                    tmpY = 0;
                }
                hugeMovedMinPosY.push_back(tmpY);
                hugeMovedMaxPosY.push_back(hugeGameObjects[id - objectNum] -> maxPosY - hugeGameObjects[id - objectNum] -> minPosY + tmpY);
            }
        }
    }

    // read leave data
    char leavePath[100] = "/Users/zklgame/WorkPlace/HOAOIs/HOAOIs/Tests/Datas/data";
    strcat(leavePath, num);
    strcat(leavePath, "/data_leave.txt");
    fp = fopen(leavePath, "r+");
    if (NULL == fp) {
        printf("ERROR! read leave error\n");
        printf("%s\n", leavePath);
        return;
    }
    
    while (fscanf(fp, "%d\n", &id) != EOF) {
        id -= 1;
        leavedObjects.push_back(gameObjects[id]);
    }
    
    // read huge leave data
    char hugeLeavePath[100] = "/Users/zklgame/WorkPlace/HOAOIs/HOAOIs/Tests/Datas/data";
    strcat(hugeLeavePath, num);
    strcat(hugeLeavePath, "/huge_data_leave.txt");
    fp = fopen(hugeLeavePath, "r+");
    if (NULL == fp) {
        printf("ERROR! read huge leave error\n");
        printf("%s\n", hugeLeavePath);
        return;
    }
    
    while (fscanf(fp, "%d\n", &id) != EOF) {
        id -= 1;
        hugeLeavedObjects.push_back(hugeGameObjects[id - objectNum]);
    }
}

void BasePerformanceTest::deInitGame() {
    for (int i = 0; i < objectNum; i ++) {
        delete gameObjects[i];
    }
    for (int i = 0; i < hugeObjectNum; i ++) {
        delete hugeGameObjects[i];
    }
}

void BasePerformanceTest::realTest(int i) {
    testAdd(i);
    testMove(i);
    testLeave(i);
}