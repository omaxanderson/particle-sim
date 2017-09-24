//
//  BasicConfig.hpp
//  std_test2
//
//  Created by Max Anderson on 9/22/17.
//  Copyright © 2017 Max Anderson. All rights reserved.
//

#ifndef BasicConfig_hpp
#define BasicConfig_hpp

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
//#include <SDL2_thread/SDL_thread.h>
#include <cmath>

const extern int SCREEN_WIDTH;
const extern int SCREEN_HEIGHT;
const extern double GRAVITATIONAL_CONST;

struct vec2 {
    vec2() {}
    vec2(float x, float y) : x(x), y(y) {}
    float x, y;
    void add(vec2 other) { x += other.x; y += other.y; }
    vec2 mult(float i) { x *= i; y *= i; return *this; }
};

float euclideanDist(vec2 a, vec2 b);

#endif /* BasicConfig_hpp */
