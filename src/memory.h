#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>

class Memory{
    private:
        //RAM
        friend class Cpu;
        uint16_t memory[65535]; //up to FFFF
        


        //GRAPHICAL RAM
        //uint16_t video_memory[16000];
        uint16_t pixels [160*144]; //resolution //pixel might need to be 16bit to support 15bit rgb <-idk how this works lol
    public:
        
        Memory();
};

#endif