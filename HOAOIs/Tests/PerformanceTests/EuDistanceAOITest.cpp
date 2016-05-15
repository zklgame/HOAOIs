//
//  EuDistanceAOITest.cpp
//  AOIs
//
//  Created by zklgame on 5/3/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "BasePerformanceTest.hpp"
#include "../../AOIServices/EuDistanceAOIService.hpp"

int main11() {
    
    int i = 1;
    
    cout << "Test " << i + 1 << " :" << endl;
    BasePerformanceTest *test = new BasePerformanceTest();
    test -> world -> aoi = new EuDistanceAOIService(test -> world -> width, test -> world -> length);
    test -> test(i + 1);
    delete test -> world -> aoi;
    delete test;
    
    return 0;
}