//
//  EuDistanceAOIService.cpp
//  AOIs
//
//  Created by zklgame on 4/5/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "EuDistanceAOIService.hpp"

EuDistanceAOIService::EuDistanceAOIService(position_t worldWidth, position_t worldHeight) {
    this -> aoi = new EuDistanceAOI(worldWidth, worldHeight);
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cout << "AOI: EuDistanceAOIService" << endl;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&\n" << endl;
}

EuDistanceAOIService::~EuDistanceAOIService() {
    delete this -> aoi;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cout << "AOI: ~EuDistanceAOIService" << endl;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&\n" << endl;
}

