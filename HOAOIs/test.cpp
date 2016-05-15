//
//  test.cpp
//  HOAOIs
//
//  Created by zklgame on 5/12/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include <iostream>
using namespace std;

#include "GameBasics/HOGameObject.hpp"

int main_test() {
    
    HOGameObject *test = new HOGameObject(1, 2, 3, 4, 5, 6, 7);
    cout << test -> id << endl;
    cout << uint16_t(test -> type) << endl;
    cout << test -> posX << endl;
    cout << test -> posY << endl;
    cout << test -> minPosX << endl;
    cout << test -> minPosY << endl;
    cout << test -> maxPosX << endl;
    cout << test -> maxPosY << endl;
    cout << test -> range << endl;
    cout << test -> tree << endl;
    
    return 0;
}