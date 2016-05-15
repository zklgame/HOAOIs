//
//  BasePerformanceTest.hpp
//  AOIs
//
//  Created by zklgame on 5/3/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef BasePerformanceTest_hpp
#define BasePerformanceTest_hpp

#include <stdio.h>
#include "../../GameBasics/GameWorld.hpp"

class BasePerformanceTest {
public:
    BasePerformanceTest();
    virtual ~BasePerformanceTest();
    
    GameWorld *world;
    
    void test(int i);
    
private:
    // basic
    FILE *fp;
    
    int width = 50000;
    int height = 50000;
    
    int objectNum = 50000;
    int movedNum = 25000;
    int leaveNum = 25000;
    int maxRange = 1000;
    int maxMove = 500;
    
    int hugeObjectNum = 5000;
    int hugeMovedNum = 2500;
    int hugeLeaveNum = 2500;
    int hugeSizeBase = 600;
    int hugeSizeRange = 600;
    
    // to set
    vector<GameObject *> gameObjects, movedObjects, leavedObjects;
    vector<HOGameObject *> hugeGameObjects, hugeMovedObjects, hugeLeavedObjects;
    vector<int> movedPosX, movedPosY;
    vector<int> hugeMovedMinPosX, hugeMovedMinPosY, hugeMovedMaxPosX, hugeMovedMaxPosY;
    
    void testAdd(int i);
    void testMove(int i);
    void testLeave(int i);
    
    void initGame(int i);
    void deInitGame();
    
    void realTest(int i);
};


#endif /* BasePerformanceTest_hpp */
