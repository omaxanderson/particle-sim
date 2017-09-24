//
//  main.cpp
//  std_test2
//
//  Created by Max Anderson on 9/21/17.
//  Copyright © 2017 Max Anderson. All rights reserved.
//
//  TODO
//  add mass
//  particle collision
//  gravitational attraction between particles
//  multithreading
//      - separate threads can perform operations on a fraction of the
//          particles but still compare to every other particle
//      - update can go at the very end after threads have been joined
//      - can have a set of threads dedicated to performing collision
//          and another set to do gravity calculations

#include <iostream>
#include <cstdlib>
#include <vector>
#include <thread>
#include "BasicConfig.hpp"
#include "Particle.hpp"

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const double GRAVITATIONAL_CONST = 0.0667408;
const int NUM_PARTICLES = 1000000;
const int NUM_THREADS = 5;

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

void threadDrawTask(int start, int end) {
    for (int i = start; i < end; i++) {
        if (attractionOn) {
            vec2 attraction;
            attraction.x = (blackHole.x - particles[i].getX()) * GRAVITATIONAL_CONST /
                            pow(euclideanDist(blackHole, particles[i].getPos()), 2);
            attraction.y = (blackHole.y - particles[i].getY()) * GRAVITATIONAL_CONST /
                            pow(euclideanDist(blackHole, particles[i].getPos()), 2);
            particles[i].applyForce(attraction);
        } else if (gravityOn) {
            particles[i].applyForce(gravity);
        }
    }
}

void threadUpdateTask(int start, int end) {
    for (int i = start; i < end; i++) {
        particles[i].update();
        particles[i].draw(*renderer);
    }
}

void draw() {

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    
    
    
    // thread testing

    vector<thread> threads(NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = thread(threadDrawTask, i * (NUM_PARTICLES / NUM_THREADS),
                            (i + 1) * (NUM_PARTICLES / NUM_THREADS));
    }
    for (thread &t : threads) {
        t.join();
    }

    for (Particle &p : particles) {
        p.update();
        p.draw(*renderer);
    }

    
/*
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
  */  
    

    SDL_RenderPresent(renderer);
    //SDL_UpdateWindowSurface( window );
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






