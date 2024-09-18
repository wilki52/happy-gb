#include "display.h"
#include <iostream>

Display::Display(){
    
}
Display::Display(Memory& ram): ram(&ram){ //var ram= parameter ram
    
}

bool Display::open(){
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) >=0){
        std::cout << "window opened" << std::endl;
        SDL_CreateWindowAndRenderer(800, 720, 0, &window, &renderer);
        SDL_RenderSetScale(renderer, 5, 5);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        //SDL_RenderClear(global_renderer);

        
        SDL_RenderPresent(renderer);
        return success;
    }
    std::cout << "failed to open" << std::endl;
    return false;
   

}

void Display::render_background(){
    
}

void Display::render_scanline(){
    SDL_SetRenderDrawColor(renderer, 155, 188, 15, 255);
    for (int i= 0; i<160; i++){
        SDL_Rect pixel;
        pixel.x = i;
        pixel.y = 143;

        pixel.h = 1;
        pixel.w = 1;
        SDL_RenderFillRect(renderer, &pixel);

    }
    SDL_RenderPresent(renderer);

    SDL_RenderDrawPoint(renderer, 5, 5);
        // SDL_Rect pixel;
        // pixel.x = 5;
        // pixel.y = 5;
        // pixel.h = 1;
        // pixel.w = 1;
        //SDL_RenderFillRect(renderer, &pixel);


    SDL_RenderPresent(renderer);
    

}
void Display::view_vram(){
    //from 8000 to 87FF
    //from there, depict each as a palletee.
    uint8_t y = 0;
    uint8_t x = 0;
    for (int i = 0x8000; i< 0x8020; i+=2){
        
        uint8_t lo = ram->memory[i]; //read...
        uint8_t hi = ram->memory[i+1];
        
        //with this, create a line.
       
        for (int p = 7; p>-1; p--){
            uint8_t col_id = (hi >> p-1) | (lo >> p);
            switch (col_id){
                case 0:
                    SDL_SetRenderDrawColor(renderer, 15, 56, 15, 255);
                    break;
                case 1:
                    SDL_SetRenderDrawColor(renderer,  48, 98, 48, 255);
                    break;
                case 2:
                    SDL_SetRenderDrawColor(renderer, 139, 172, 15, 255);
                    break;
                case 3:
                    SDL_SetRenderDrawColor(renderer, 155, 188, 15, 255);
                    break;
                default:
                    SDL_SetRenderDrawColor(renderer, 155, 188, 15, 255);
            }
            //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            SDL_Rect pixel;
            pixel.x = x+p;
            pixel.y = y;
            pixel.w = 1;
            pixel.h = 1;
            SDL_RenderFillRect(renderer, &pixel);

        
        }
 
        


    }
    SDL_RenderPresent(renderer);
    


}
void Display::view_map(){

}