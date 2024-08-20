#ifndef HAPPY_GB_H
#define HAPPY_GB_H
#include <stdint.h>
#include <iostream>
#include <map>

#include "memory.h"
#include "cpu.h"
class Happy{
    private:
        //CPU specifications
        Memory ram;
        Cpu cpu;

        
        //display here. pointer to a sdl display class probably
        uint16_t rgb;
        uint16_t bus; //named bus, but will be renamed appropriately for future pointers
        
        
        int load_rom(unsigned char path[]);
    public:
        int decode(uint8_t instruction); //instruction is 8bits...
        Happy();
        int cycle();
};


#endif
