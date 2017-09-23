//
//  BasicConfig.cpp
//  std_test2
//
//  Created by Max Anderson on 9/22/17.
//  Copyright © 2017 Max Anderson. All rights reserved.
//

#include "BasicConfig.hpp"

float euclideanDist(vec2 a, vec2 b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
