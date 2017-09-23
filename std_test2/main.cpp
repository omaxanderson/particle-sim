//
//  main.cpp
//  std_test2
//
//  Created by Max Anderson on 9/21/17.
//  Copyright © 2017 Max Anderson. All rights reserved.
//
//  TODO
//  particle collision
//  gravitational attraction between particles
//  multithreading

#include <iostream>
#include <cstdlib>
#include <vector>
#include "BasicConfig.hpp"
#include "Particle.hpp"

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const double GRAVITATIONAL_CONST = 0.0667408;
const int NUM_PARTICLES = 50000;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* img = NULL;

vec2 gravity;
vec2 blackHole;
bool gravityOn = false;
bool attractionOn = false;

vector<Particle> particles;

void keyPressCallback(SDL_Event e) {
    switch(e.key.keysym.sym) {
        case SDLK_a: {

            break;
        }
        case SDLK_g:
            gravityOn = true;
            cout << "gravity on" << endl;
            break;
        default: cout << "key doesn't do anything dude" << endl;
    }
}

void mousePressCallback(SDL_Event e) {
    switch (e.button.button) {
        case SDL_BUTTON_LEFT: {
            attractionOn = true;
            blackHole = vec2(e.motion.x, e.motion.y);
            break;
        }
        case SDL_BUTTON_RIGHT:
            cout << "button right" << endl;
            break;
        default: cout << "some mouse event" << endl;
    }
}

bool initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        return false;
    }
    
    if (IMG_Init(IMG_INIT_JPG) == -1) {
        return false;
    }
    
    // initialize window
    window = SDL_CreateWindow("Tester", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        cout << "Error creating window" << endl;
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        cout << "Error creating renderer" << endl;
        return false;
    }
    
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(renderer);
    
    // initialize screen surface
    //screenSurface = SDL_GetWindowSurface(window);
    //if (screenSurface == NULL) {
    //    cout << "Error creating surface" << endl;
    //    return false;
    //}
    gravity = vec2(0, .001);
    
    // SDL_SetRenderDrawColor(<#SDL_Renderer *renderer#>, <#Uint8 r#>, <#Uint8 g#>, <#Uint8 b#>, <#Uint8 a#>)
    for (int i = 0; i < NUM_PARTICLES; i++) {
        particles.push_back(Particle(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT));
    }
    
    return true;
}

void draw() {
    //Fill the surface white
    //SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xAA, 0xAA, 0xFF ) );
    
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    
    //SDL_Rect rect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
    //SDL_RenderDrawRect(renderer, &rect);
    
    
    //for (size_t i = 0; i < particles.size(); i++) {
    //    particles[i].applyForce(gravity);
    //    particles[i].update();
    //    particles[i].draw(*renderer);
    //}
    
    for (Particle &p : particles) {
        if (attractionOn) {
            vec2 attraction;
            attraction.x = (blackHole.x - p.getX()) * GRAVITATIONAL_CONST / pow(euclideanDist(blackHole, p.getPos()), 2);
            attraction.y = (blackHole.y - p.getY()) * GRAVITATIONAL_CONST / pow(euclideanDist(blackHole, p.getPos()), 2);
            p.applyForce(attraction);
        } else if (gravityOn) {
            p.applyForce(gravity);
        }
        //p.checkCollisionAndGravitation(particles);
        p.update();
        p.draw(*renderer);
    }
    
    /* 
    img = IMG_Load("images/dany.jpg");
    if (img == NULL) {
        cout << "error loading image" << endl;
    } else {
        cout << "worked" << endl;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, img);
    
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    */
     
    SDL_RenderPresent(renderer);

    //SDL_DestroyTexture(texture);
    SDL_UpdateWindowSurface( window );
}

void close() {
    //Destroy window
    SDL_DestroyWindow( window );
    
    SDL_DestroyRenderer(renderer);
    
    //Quit SDL subsystems
    SDL_Quit();
}

int main(int argc, const char * argv[]) {
    
    // Set up SDL
    initialize();
    
    bool quit = false;
    SDL_Event e;
    int i = 0;
    while (!quit) {
        //SDL_Delay(10);
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_KEYDOWN: keyPressCallback(e); break;
                case SDL_MOUSEBUTTONDOWN: mousePressCallback(e); break;
                case SDL_QUIT: quit = true; break;
            }
        
        }
        
        draw();
    }
    
    // Quit SDL
    close();
    return 0;
}






