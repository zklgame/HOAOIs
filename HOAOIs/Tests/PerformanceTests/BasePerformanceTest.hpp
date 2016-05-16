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
    
    int width = 25000;
    int height = 25000;
    
    int objectNum = 25000;
    int movedNum = 12500;
    int leaveNum = 12500;
    int maxRange = 500;
    int maxMove = 250;
    
    int hugeObjectNum = 2500;
    int hugeMovedNum = 1250;
    int hugeLeaveNum = 1250;
    int hugeSizeBase = 300;
    int hugeSizeRange = 300;
    
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
