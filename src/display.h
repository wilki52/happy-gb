#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL2/SDL.h>

class Display{
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
    public:
        Display();
        bool open();
};


#endif