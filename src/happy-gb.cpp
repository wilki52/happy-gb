#include "happy-gb.h"
#include <SDL2/SDL.h>

//INIT REG MAPPING HERE
Happy::Happy(): ram(), cpu(ram), display(){ //init ram, then init cpu constructor
    //display.open();
    display.open();


}

void Happy::run_program(){
    SDL_Event e;
    std::cout << "run program" << std::endl;
    bool running = true;

    while (running){
        if (SDL_PollEvent(&e)!=0){
            SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
            if (e.type==SDL_QUIT){
                running = false;
            }
            cpu.handle_input(e);

        }

        //cycle();

    }
}

void Happy::cycle(){
    uint8_t instruction = cpu.fetch();
    

    //std::cout<< instruction;
    SDL_Delay(10);
    cpu.decode(instruction); 

    SDL_Event e;


}

//arbitrary decode blocks following pandocs, based on opcode bits 7,6

int Happy::load_rom(const char path[]){
    int pass = cpu.read_rom(path);
    return pass;
}