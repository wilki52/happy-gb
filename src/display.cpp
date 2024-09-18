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
    //x, y
    //while i < 0x80X0;
    //build_tile
    //grab 16 bytes. 
    //for i in 16, write out a line one by one starting at x, y.
    //once tile is built, increment x and start over.
    

    SDL_SetRenderDrawColor(renderer,  255, 255, 255, 255);

    SDL_RenderClear(renderer);

    int i = 0x8000;
    uint16_t y = 0;
    uint16_t x = 0;

    while (i < 0x9000){
        //build tile
        int byte_count = 0;
        while (byte_count < 16){
            uint8_t lo = ram->memory[i]; //read...
            uint8_t hi = ram->memory[i+1];

            //biuld line
            for (int p = 7; p>-1; p--){
                uint8_t col_id = ((hi >> p)<<1) | (lo >> p);
                
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

                SDL_Rect pixel;
                pixel.x = ((x%50)*8)+(7-p);
                pixel.y = (x/50)*8 + byte_count/2;
                pixel.w = 1;
                pixel.h = 1;
                SDL_RenderFillRect(renderer, &pixel);
            }

            byte_count +=2;
            i+=2;

        }

        x+=1;
    

    }

    for (int j = 0; j<8; j++){
        SDL_SetRenderDrawColor(renderer,  252, 3, 3, 255);
        SDL_RenderDrawPoint(renderer, 150, j);

    }
    SDL_RenderPresent(renderer);

}
void Display::view_map(){

}