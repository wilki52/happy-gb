#include "happy-gb.h"
#include <SDL2/SDL.h>

//INIT REG MAPPING HERE
Happy::Happy(): ram(), cpu(ram){ //init ram, then init cpu constructor
    //register setup
    //decode(0b01111111);

    //cycle();

}

int Happy::cycle(){
    uint8_t instruction = cpu.fetch();
    
   // std::cout<< "instruction: "<< unsigned(instruction) << std::endl;
    SDL_Delay(10);
    //cpu.decode(instruction); 
}

int Happy::decode(uint8_t instruction){
   
}
//arbitrary decode blocks following pandocs, based on opcode bits 7,6

int Happy::load_rom(const char path[]){
    cpu.read_rom(path);
}