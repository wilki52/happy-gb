#include "happy-gb.h"
#include <SDL2/SDL.h>

//INIT REG MAPPING HERE
Happy::Happy(): ram(), cpu(ram){ //init ram, then init cpu constructor
    //register setup
    //decode(0b01111111);

    //cycle();

}

void Happy::cycle(){
    uint8_t instruction = cpu.fetch();
    
    //std::cout<< instruction;
    SDL_Delay(10);
    cpu.decode(instruction); 

}

//arbitrary decode blocks following pandocs, based on opcode bits 7,6

int Happy::load_rom(const char path[]){
    int pass = cpu.read_rom(path);
    return pass;
}