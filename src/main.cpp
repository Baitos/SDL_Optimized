#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <format>

#include "../include/state.h"
#include "../include/gameState.h"
#include "../include/resources.h"
#include "../include/globals.h"



int main(int argc, char** argv) { // SDL needs to hijack main to do stuff; include argv/argc
    SDLState state;
    // load game assets
    Resources res;

    state.width = 1600;
    state.height = 900;
    state.logW = 800;
    state.logH = 450;

    state.xRatio = (float)state.logW / state.width; 
    state.yRatio = (float)state.logH / state.height;

    if (!initialize(state)) {
        return 1;
    }
    
    res.load(state);

    // set window icon
    /*SDL_Surface *icon = IMG_Load("data/logo.png");
    SDL_SetWindowIcon(state.window, icon);
    SDL_DestroySurface(icon);*/
    
    srand(time(0)); // randomize item generation

    //Initial Game State

    // setup game data
    GameState gs(state);
    
    uint64_t prevTime = SDL_GetTicks();

    uint64_t frames = 0;
    uint64_t FPS = 0;
    uint64_t lastTime = 0;


    // start game loop
    while (running) {
        uint64_t nowTime = SDL_GetTicks(); // take time from previous frame to calculate delta
        frames++;
        if (nowTime > lastTime + 1000) { // fps counter
            lastTime = nowTime;
            FPS = frames;           
            frames = 0;
        }
        float deltaTime = (nowTime - prevTime) / 1000.0f; // convert to seconds from ms
        
        SDL_Event event { 0 };
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                {
                    running = false;
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED: 
                {
                    state.width = event.window.data1;
                    state.height = event.window.data2;
                    
                    //printf("Width = %d, Height = %d", state.width, state.height);
                    break;
                }
                case SDL_EVENT_KEY_DOWN:
                {
                    if (event.key.scancode == SDL_SCANCODE_F12) {
                        gs.debugMode = !gs.debugMode;
                    }
                    else if (event.key.scancode == SDL_SCANCODE_F1) {
                        running = false;
                    }
                    break;
                }
                case SDL_EVENT_KEY_UP:
                {
                    
                    break;
                }
            }
        }


        SDL_SetRenderDrawColor(state.renderer, 64, 51, 83, 255);
        SDL_RenderClear(state.renderer);

        if (gs.debugMode) {
        // debug info
            SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
            SDL_RenderDebugText(state.renderer, 5, 5,
                            std::format("FPS: {}", 
                            static_cast<int>(FPS)).c_str());
        }
        //swap buffers and present
        SDL_RenderPresent(state.renderer);
        prevTime = nowTime;
    }
    res.unload();
    cleanup(state);
    return 0;
}