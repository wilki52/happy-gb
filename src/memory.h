#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>
#include <vector>

class Memory{
    private:
        //RAM
        friend class Cpu;
        uint8_t memory[65535]; //up to FFFF
       //stack is at the top of memory, and decrements down. REMEMBER THIS!!
        //GRAPHICAL RAM
        //uint16_t video_memory[16000];
        uint8_t pixels [160*144]; //resolution //pixel might need to be 16bit to support 15bit rgb <-idk how this works lol

        //addresses
        uint16_t cgb_flag = 0x0143; //x80 = bc, xC0 = cgb only

        uint16_t DIV = 0xFF04; //increment at rate of 16384Hz. anytime this is written in, write 0!!!.
            //stop resets DIV. ticks only when stop mode ends.
            //affected by cgb doublespeed
        uint16_t TIMA = 0xFF05;
        uint16_t TMA = 0xFF06;
        uint16_t TAC = 0xFF07;


        void pop_stack(uint16_t pointer);
        void push_stack(uint16_t pointer);
    public:
        
        Memory();
};

#endif