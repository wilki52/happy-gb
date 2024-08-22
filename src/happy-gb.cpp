#include "happy-gb.h"


//INIT REG MAPPING HERE
Happy::Happy(): ram(), cpu(ram){ //init ram, then init cpu constructor
    //register setup
    //decode(0b01111111);

    cycle();

}

int Happy::cycle(){
    uint8_t instruction = cpu.fetch();
    cpu.decode(0b00111000); 
    0b0101;

}

int Happy::decode(uint8_t instruction){
   
}
//arbitrary decode blocks following pandocs, based on opcode bits 7,6

int Happy::load_rom(unsigned char path[]){

}