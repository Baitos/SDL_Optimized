#pragma once

#include <SDL3/SDL.h> // don't include SDL3_main in anything other than the main cpp file
#include <vector>
#include <string>
#include "../ext/glm/glm.hpp"
#include "animation.h"

struct SDLState; // forward def

struct Resources { 
    std::vector<SDL_Texture *> textures;

    SDL_Texture *loadTexture(SDL_Renderer *renderer, const std::string &filepath);
    void load(SDLState &state);
    void unload();

    Resources(){};
};