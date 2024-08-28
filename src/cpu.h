#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "memory.h"
#include <map>

typedef union {
            uint16_t full;
            struct {
                uint8_t low;
                uint8_t high;
            };
        } Register;

class Cpu{
    public:
        Cpu();
        Cpu(Memory& ram);
        uint8_t fetch();
        uint8_t decode(uint8_t instruction);
        uint8_t load_rom(Memory ram);

    private:
        uint8_t ime;
        uint8_t ime_hold;

        //will not need a variable since it has fixed spot in memory. so...delete it when u implement
        uint8_t IE; //0xFFFF
        uint8_t IF; //0xFF0F




        //uint8_t a, f; //af = a + flag
                            //flags: bit 7: z, 6: n, 5: h, 4: c
        //uint8_t b,c;
        //uint8_t d,e;
        //uint8_t h,l;
        //uint8_t s,p; //usually 16 bits, but i put into hi/lo to help with maps. trust.

        //
        Register sp;
        Register af;
        Register bc;
        Register de;
        Register hl;
        Register counter;

        std::map<uint8_t, Register*> reg16;
        std::map<uint8_t, uint8_t> vec; //rst vectors
        //std::map<uint8_t, uint8_t*> map to functions? get_z, get_c.
        uint16_t pc; //16 bits to point to 32kb memory.
        //flags
        //uint8_t zf; //zero flag
        //uint8_t nf; //subtraction flag (bcd)//used by DAA instruction only
        //uint8_t hf; //half carry flag (bcd) //used by DAA instruction only
        //uint8_t cf; //carry flag
        //uint8_t a, b, c, d, e, h, l, f;

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
        std::map<uint8_t, uint8_t*> r16mem; //use so we dont need absurd switch statement

        //COND : nz (not zero), z (zero), nc (not carry), c (carry)

        //OPCODES
        void nop();

        void ld_r16(uint8_t dest); //ld r16, imm16
        void ld_to_r16mem(uint16_t address, uint8_t &reg); //ld [r16mem], a
        void ld_a_from_r16mem(uint8_t &reg, uint16_t address); //ld a, [r16mem]
        void ld_to_n16mem(); //ld [imm16], sp

        void inc_r16(uint8_t);
        void dec_r16(uint8_t);
        void add_hl_from_r16(uint8_t);

        void inc_r8(uint8_t);
        void dec_r8(uint8_t);

        void ld_r8_n8(uint8_t); //ld r8, imm8

        void rlca();
        void rrca();
        void rla();
        void rra();
        void daa();
        void cpl();
        void scf();
        void ccf();

        void jr_n8();
        void jr_cc_n8(uint8_t condition);
        
        void stop();

        //block 1

        void ld_r8_r8(uint8_t dest, uint8_t src);
        void halt();

        //block 2
        void add(uint8_t &destination, uint8_t &operand); //add a, r8
        void adc(uint8_t &destination, uint8_t &operand); //add a, r8
        void sub(uint8_t &destination, uint8_t &operand); //add a, r8
        void sbc(uint8_t &destination, uint8_t &operand); //add a, r8
        void opcode_and(uint8_t &destination, uint8_t &operand); //add a, r8
        void opcode_xor(uint8_t &destination, uint8_t &operand); //add a, r8
        void opcode_or(uint8_t &destination, uint8_t &operand); //add a, r8
        void cp(uint8_t &destination, uint8_t &operand); //add a, r8
    
        //block 3
        void add(uint8_t &destination); //add a, imm8
        void adc(uint8_t &destination); //add a, imm8
        void sub(uint8_t &destination); //add a, imm8
        void sbc(uint8_t &destination); //add a, imm8
        void opcode_and(uint8_t &destination); //add a, imm8
        void opcode_xor(uint8_t &destination); //add a, imm8
        void opcode_or(uint8_t &destination); //add a, imm8
        void cp(uint8_t &destination); //add a, imm8

        void ret_cond(uint8_t condition);
        void ret();
        void reti();
        void jp_cond(uint8_t cond);
        void jp();
        void jp_hl();

        void call_cond(uint8_t cond);
        void call();
        void rst(uint8_t target);

        void pop(uint8_t &);
        void push(uint8_t &);

        void prefix();

        void ldh_to_c(uint8_t &); //from a
        void ldh_to_n8(uint8_t &); //from a
        void ld_to_n16(uint8_t &); //from a
        void ldh_from_c(uint8_t &); //from c
        void ldh_from_n8(uint8_t &); //to a
        void ld_from_n16(uint8_t &);//to a
        
        void add_sp_from_n8();
        void ld_hl_sp_and_n8();
        void ld_sp_from_hl();

        void di();
        void ei();

        //prefixes
        void rlc(uint8_t &reg);
        void rrc(uint8_t &reg);
        void rl(uint8_t &reg);
        void rr(uint8_t &reg);
        void sla(uint8_t &reg);
        void sra(uint8_t &reg);
        void swap(uint8_t &reg);
        void srl(uint8_t &reg);

        void bit(uint8_t &bit, uint8_t &operand);
        void res(uint8_t &bit, uint8_t &operand);
        void set(uint8_t &bit, uint8_t &operand);




        




    
};

#endif