#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL2/SDL.h>
#include "memory.h"

class Display{
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;

        Memory* ram;
    public:
        Display();
        Display(Memory& ram);
        bool open();
        void render_background();
        void render_scanline();
        void render_window();
        void render_object();

        //DDEBUG

        void view_vram();
        void view_map();
};


#endif