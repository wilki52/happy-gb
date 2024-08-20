#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "memory.h"
class Cpu{
    private:
        uint8_t a; //af = a + flag
        uint8_t b,c;
        uint8_t d,e;
        uint8_t h,l;
        uint16_t sp; //stack pointer
        uint16_t pc; //16 bits to point to 32kb memory.
        //flags
        uint8_t zf; //zero flag
        uint8_t nf; //subtraction flag (bcd)//used by DAA instruction only
        uint8_t hf; //half carry flag (bcd) //used by DAA instruction only
        uint8_t cf; //carry flag

        void decode_block0(uint8_t instruction);
        void decode_block1(uint8_t instruction);
        void decode_block2(uint8_t instruction);
        void decode_block3(uint8_t instruction);
    public:
        Cpu();
        uint8_t fetch(Memory ram);
        uint8_t decode(uint8_t instruction);
        uint8_t load_rom(Memory ram);
};

#endif