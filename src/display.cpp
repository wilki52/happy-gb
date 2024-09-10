#include "display.h"
#include <iostream>

Display::Display(){
    
}

bool Display::open(){
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) >=0){
        std::cout << "window opened" << std::endl;
        SDL_CreateWindowAndRenderer(640, 320, 0, &window, &renderer);
        SDL_RenderSetScale(renderer, 10, 10);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        //SDL_RenderClear(global_renderer);

        
        SDL_RenderPresent(renderer);
         return success;
    }
    std::cout << "failed to open" << std::endl;
    return false;
   

}