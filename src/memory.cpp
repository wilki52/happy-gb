#include "memory.h"
#include <iostream>

Memory::Memory(){
    cgb_flag = 0x0143;
    //joypad
    JOYP = 0xFF00;

    //serial transfer
    SB = 0xFF01;
    SC = 0xFF02;

    //time
    DIV = 0xFF04;
    TIMA = 0xFF05;
    TMA = 0xFF06;
    TAC = 0xFF07;

    //interrupts
    IF = 0xFF0F;
    IE = 0xFFFF;
    

    LCDC = 0xFF40;
    LY = 0xFF44;
    LYC = 0xFF45;
    STAT = 0xFF41;
    BGP = 0xFF47;


}