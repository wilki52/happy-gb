#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "memory.h"
#include <map>
class Cpu{
    private:
        uint8_t a, f; //af = a + flag
                            //flags: bit 7: z, 6: n, 5: h, 4: c
        uint8_t b,c;
        uint8_t d,e;
        uint8_t h,l;
        //uint8_t s,p; //usually 16 bits, but i put into hi/lo to help with maps. trust.

        //
        typedef union {
            uint16_t full;
            struct {
                uint8_t p;
                uint8_t s;

            };
        } stack_pointer;
        stack_pointer sp;

        uint16_t pc; //16 bits to point to 32kb memory.
        //flags
        //uint8_t zf; //zero flag
        //uint8_t nf; //subtraction flag (bcd)//used by DAA instruction only
        //uint8_t hf; //half carry flag (bcd) //used by DAA instruction only
        //uint8_t cf; //carry flag


        Memory* ram;
        int m_cycle;
        int t_state; 

        void set_flags(uint8_t z=0, uint8_t n=0, uint8_t h=0, uint8_t c=0);

        void set_half_if_overflow_8(uint8_t op1, uint8_t op2, uint8_t carry=0); //3 bit overflow
        void set_half_if_overflow_16(uint16_t op1, uint16_t op2, uint8_t carry=0);  //11 bit overflow

        void set_carry_if_overflow_8(uint8_t op1, uint8_t op2, uint8_t carry=0); //overflow bit7
        void set_carry_if_overflow_16(uint16_t op1, uint16_t op2, uint8_t carry=0); //oerflow bit15

        void set_half_if_borrow(uint8_t minuend, uint8_t subtrahend, uint8_t carry=0); //min - sub
        void set_carry_if_borrow(uint8_t minuend, uint8_t subtrahend, uint8_t carry=0);

        void set_z(uint8_t result);
        void set_n(uint8_t n);
        void set_h(uint8_t h);
        void set_c(uint8_t c);

        void decode_block0(uint8_t instruction);
        void decode_block1(uint8_t instruction);
        void decode_block2(uint8_t instruction);
        void decode_block3(uint8_t instruction);

        uint8_t read();
        uint8_t read(uint16_t address);
        uint8_t write(uint16_t address, uint8_t data);

        std::map<uint8_t, uint8_t*> reg8;
        std::map<uint8_t, uint8_t*> reg16;
        

        std::map<uint8_t, uint8_t*> r16mem; //use so we dont need absurd switch statement

        //COND : nz (not zero), z (zero), nc (not carry), c (carry)
    public:
        Cpu();
        Cpu(Memory& ram);
        uint8_t fetch();
        uint8_t decode(uint8_t instruction);
        uint8_t load_rom(Memory ram);
};

#endif