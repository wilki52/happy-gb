#ifndef HAPPY_GB_H
#define HAPPY_GB_H
#include <stdint.h>
#include <iostream>
#include <map>

#include "memory.h"
#include "cpu.h"
#include "display.h"
class Happy{
    private:
        //CPU specifications
        Memory ram;
        Cpu cpu;
        Display display;

        
        //display here. pointer to a sdl display class probably
        uint16_t rgb;
        uint16_t bus; //named bus, but will be renamed appropriately for future pointers
        
        
        
    public:
        Happy();
        void cycle();
        int load_rom(const char path[]);
        void run_program();
};


#endif
