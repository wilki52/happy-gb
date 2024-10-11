#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>
#include <vector>

class Memory{
    private:
        //RAM
        friend class Cpu;




        void pop_stack(uint16_t pointer);
        void push_stack(uint16_t pointer);
    public:
        
        uint8_t memory[65535]; //up to FFFF
       //stack is at the top of memory, and decrements down. REMEMBER THIS!!
        //GRAPHICAL RAM
        //uint16_t video_memory[16000];
        uint8_t pixels [160*144]; //resolution //pixel might need to be 16bit to support 15bit rgb <-idk how this works lol
        //addresses
        uint16_t cgb_flag; //x80 = bc, xC0 = cgb only


        uint16_t JOYP;
        uint16_t DIV;//increment at rate of 16384Hz. anytime this is written in, write 0!!!.
            //stop resets DIV. ticks only when stop mode ends.
            //affected by cgb doublespeed
        uint16_t TIMA;
        uint16_t TMA;
        uint16_t TAC;

        //corresponding handler in IF/IE then calls call<address>. 
        //PC pushed on stack, and set address of corresponding interrupt handler.
        //5 m cycles
        //bit 0: has highest priority
        //bit 4: lowest priority.

        uint16_t IF; //controls if handler is being requested.
                //1 means it is being requested from X source.
                //only means it is an interrupt. If IF is set, then it checks
                    //IME=1 and IE's corresponding bit is also 1. then, interrupt occurs.

        uint16_t IE; //controls if X interrupt handler can be called.
        //if interrupt occurs: IF corresponding bit set to 0, then IME reset also.
            //this means iinterrupt request was acknowledged. IME being set to 0 means no interupt can interupt our interupt.
        //interrupt sources
        //40
        //48
        //50
        //58
        //60
        
        
        //BACKGROUND 


        uint16_t SCY;
        uint16_t SCX;
        //LCD
        uint16_t LCDC; //LCD CONTROL
        uint16_t LY; //
        uint16_t LYC; //
        uint16_t STAT; //

        uint16_t BGP;
        //graphics
        
        //SERIAL TRANSFER
        uint16_t SB;
        uint16_t SC;

        Memory();
};

#endif