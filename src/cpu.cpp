#include "cpu.h"
#include <iostream>
Cpu::Cpu(){
    
}
Cpu::Cpu(Memory& ram): ram(&ram){
    //init sp
    //sp = {0};
    bc.full = 0;
    de.full = 0;
    hl.full = 0;
    counter.full = 0;
    sp.full = 0xFFFE; //starts at top of memory

    //init map
    reg8[0x0] = &b;
    reg8[0x1] = &c;
    reg8[0x2] = &d;
    reg8[0x3] = &e;
    reg8[0x4] = &h;
    reg8[0x5] = &l;
    //reg8[0x8] = &; supposed to be [hl]
    reg8[0x7] = &a;

    //init reg16
    uint8_t *bcc[2] = {&b, &c};
    reg16[0x0] = *bcc;
    uint8_t *dee[2] = {&d, &e};
    reg16[0x1] = *dee;
    uint8_t *hll[2] = {&h, &l};
    reg16[0x2] = *hll;

    uint8_t *spp[2] = {&sp.s, &sp.p};
    reg16[0x3] = *spp;
    //buy a lock
    reg16[0x0][0] = 2;
    reg16[0x0][1] = 3;
    //sp.s = 0;
    //sp.p = 0;
    std::cout << unsigned(b) << std::endl;
    std::cout << unsigned(c) << std::endl;
    //uint16 bc.
    //b = bc >> 8 & 0xFF;
    //b = new_hi;
    //bc = new_hi | bc&0xFF;
    std::cout << "t" << std::endl;
        
    reg1[0] = &bc;
    reg1[1] = &de;
    reg1[2] = &hl;
    reg1[3] = &sp;
    std::cout << "ts" << std::endl;
    reg1[0]->s = 1;
    reg1[0]->p = 1;
    std::cout << "1: " << unsigned(bc.full) << std::endl;
    bc.s = 2;
    bc.p = 0;
    std::cout << "2: " << unsigned(reg1[0]->full) << std::endl;

    //option 1
    //uint8* no = {$n, $o};


    //option 2 : 
    //union for all. then use map<uint8_t, reg> reg16.

}

void Cpu::set_z(uint8_t z){
    (z==1) ? (f= f| 0x80) : (f = f & 0x7F);
}
void Cpu::set_n(uint8_t n){
    (n==1) ? (f = f | 0x40) : (f = f & 0xBF);
}
void Cpu::set_h(uint8_t h){
    (h==1) ? (f = f | 0x20): (f = f & 0xDF);
}
void Cpu::set_c(uint8_t c){
    (c==1) ? (f = f | 0x10): (f = f & 0xEF);
}
void Cpu::set_flags(uint8_t z, uint8_t n, uint8_t h, uint8_t c){

}

//3 bit overflow
void Cpu::set_half_if_overflow_8(uint8_t op1, uint8_t op2, uint8_t carry){
    uint8_t overflow_bit = (((op1 & 0xF)+(op2 & 0xF)+(carry&0xF)) & 0x10);
    set_h((overflow_bit == 0x10) ? 1 : 0);
} 
//check overflow bit7
void Cpu::set_carry_if_overflow_8(uint8_t op1, uint8_t op2, uint8_t carry){
    bool overflow = ((op1+op2+carry)>255);
    set_c(overflow ? 1:0);
}

 //11 bit overflow
void Cpu::set_half_if_overflow_16(uint16_t op1, uint16_t op2, uint8_t carry){
    uint8_t overflow_bit = (((op1 & 0xFFF)+(op2 & 0xFFF)+ (carry & 0xFFF)) & 0x1FFF);
    set_h((overflow_bit== 0x1FFF) ? 1 : 0);
} 
//check overflow bit15
void Cpu::set_carry_if_overflow_16(uint16_t op1, uint16_t op2, uint8_t carry){
    bool overflow = ((op1+op2+carry)>65535) ;
    set_c(overflow ? 1 : 0);
} 

void Cpu::set_half_if_borrow(uint8_t minuend, uint8_t subtrahend, uint8_t carry){
    //((subtrahend & 0xF) > (minuend & 0xF)) ? set_h(1) : set_h(0);
    bool borrow = (((subtrahend & 0xF)+(carry&0xF)) > ((minuend & 0xF)));
    set_h(borrow ? 1 : 0);
} 
void Cpu::set_carry_if_borrow(uint8_t minuend, uint8_t subtrahend, uint8_t carry){
    ((subtrahend + carry)> minuend) ? set_c(1) : set_c(0);
}

//return 8bit instruction
uint8_t Cpu::fetch(){
    uint8_t instruction = ram->memory[pc];
    
    pc= pc+1;
    m_cycle+=1; //FETCH IS +1 m_cycle
    return instruction;
}

//read from memory, M_CYCLE+=1
uint8_t Cpu::read(){
    uint8_t int_8 = ram->memory[pc];
    pc= pc+1;
    m_cycle+=1; //FETCH IS +1 m_cycle
    return int_8;
}
uint8_t Cpu::read(uint16_t address){
    uint8_t val = ram->memory[address];
    m_cycle+=1; //FETCH IS +1 m_cycle
    return val;
}



//write to memory
uint8_t Cpu::write(uint16_t address, uint8_t data){
    ram->memory[address] = data;

    pc= pc+1;
    m_cycle+=1; //writing IS +1 m_cycle
}


uint8_t Cpu::decode(uint8_t instruction){
    std::cout << "instruction: "  << unsigned(instruction) << std::endl;
    uint8_t opcode = (instruction >> 6) & 0x3; //2 bits
    std::cout << unsigned(opcode) << std::endl; //this works.
    //parse(opcode);
    switch (opcode){
        case 0: //block 0
            decode_block0(instruction);
            break;
        case 1: //block 1
            decode_block1(instruction);
            break;
        case 2: //block2
            decode_block2(instruction);
            break;
        case 3: //block3
            decode_block3(instruction);
            break;
    }
}

void Cpu::decode_block0(uint8_t instruction){
    uint8_t opcode = instruction & 0xF; //gets first 4 bits,s tarting from lsb
    std::cout << unsigned(opcode) << std::endl;

    switch (opcode){
        case 0:

            if ((instruction >> 5 & 0x1)==1){ //if bit5==1
                std::cout << "jr cond, imm8" << std::endl;
                //relative jump to address n16 if cond is met.
                //COND : nz (not zero), z (zero), nc (not carry), c (carry)
                uint8_t condition = instruction >> 3 & 0x3;

                //READ IMM8
                uint8_t imm8 = read();
                switch (condition){
                    case 0:{
                        //if z==0
                        if ((f& 0x80)==0){
                            //jump
                            pc+=imm8;
                            m_cycle+=1;
                        }
                    }
                        break;
                    case 1:{
                        //if z==1
                        if ((f& 0x80)==1){
                            //jump
                            pc+=imm8;
                            m_cycle+=1;

                        }
                    }
                        break;
                    case 2:{
                        //if c==0
                        if ((f& 0x10)==0){
                            //jump
                            pc+=imm8;
                            m_cycle+=1;

                        }

                    }
                        break;
                    case 3:{
                        if ((f& 0x10)==1){
                            //jump
                            pc+=imm8;
                            m_cycle+=1;

                        }
                    }
                        break;
                }

            }
            else{
                uint8_t bits43 = instruction >> 3 & 0x2;
                switch (bits43){
                    case 3:
                        std::cout << "jr imm8" << std::endl;
                        pc+= read();
                        break;
                    case 2:
                        std::cout << "stop" << std::endl;
                        //TODO: enter low power mode. use to swtich between double/normal speed CPU in gbc.
                        break;
                    case 0:
                        std::cout << "NOP" << std::endl;
                        //no operation, just use to skip pc. already done with fetch.
                        break;
                }
            }
            
            break;
        case 1: {
            std::cout << "LD r16, imm16" << std::endl;
            uint8_t dest = instruction >> 4 & 0x3;
            uint8_t low = read();
            reg16[dest][1] = low; //c
            
            uint8_t hi = read();
            reg16[dest][0] = hi; //b
            
            //no flag
            break;
        }
        case 2: {
        //use hl for indiredct access i think
            std::cout << "LD [R16mem], A" << std::endl;
            //load A into byte at ram[r16mem].
            uint8_t address = instruction >> 4 & 0x3;
            switch (address){
                case 0:
                    write(*reg16[address], a); //bc
                    break;
                case 1:
                    write(*reg16[address], a); //de
                    break;
                case 2: 
                    write(*reg16[3]+1, a); //hl+
                    break;
                case 3:
                    write(*reg16[3]-1, a); //hl-
                    break;

            }
            break;
        }
        case 10:{
            std::cout << "LD A, [R16mem]" << std::endl;
            //get value in address, load in A. 
            uint8_t address = instruction >> 4 & 0x3;

            switch (address){
                case 0:
                    a = read(*reg16[address]); //bc
                    break;
                case 1:
                    a = read(*reg16[address]); //de
                    break;
                case 2: 
                    a = read(*reg16[3]+1); //hl+
                    break;
                case 3:
                    a = read(*reg16[3]-1); //hl-
                    break;
            }
            break;
            
        }
        case 8:{
            std::cout << "LD [imm16], sp" << std::endl;
            //load value in sp (hi lo) to (imm16), aka the address that valimm16 points to
            uint8_t instruction_low = read();
            uint8_t instruction_hi = read();
            write(instruction_low, sp.p);
            write(instruction_hi, sp.s);
            //write(pc, s), pc++
            break;
        }
            

        case 3:{
            std::cout << "inc r16" << std::endl;
            //read reg16, add one.
            uint8_t operand = instruction >> 4 & 0x3;
            uint16_t val = reg16[operand][0] <<4|reg16[operand][1];
            val = val+1;
            reg16[operand][0] = val>>4 & 0x3;
            reg16[operand][1] = val & 0x3;

            m_cycle+=1; //16 bit increments need 1 more cycle.
        
            break;

        }
            
        case 11:{
            
            std::cout << "dec r16" << std::endl;
            uint8_t operand = instruction >> 4 & 0x3;
            uint16_t val = reg16[operand][0] <<4|reg16[operand][1];
            val = val-1;
            reg16[operand][0] = val>>4 & 0x3; 
            reg16[operand][1] = val & 0x3;
            //
            m_cycle+=1; //16 bit increments need 1 more cycle.
            break;
        }
        case 9:{
            std::cout << "add hl, r16" << std::endl;
            uint8_t operand = instruction >> 4 & 0x3;
            //add value of r16 into address of HL?
            //fetched already.
            //read hi lo of reg16, into address of 
            uint16_t add = (reg16[operand][0]<<8 | reg16[operand][1]);
            //get 11 bits of hl and r16. check for overflow
            ((((h <<8 | l)& 0xFFF + add&0xFFF)&0x1000) == 0x1000) ? set_h(1): set_h(0);
            
            //now check for overflow
            uint16_t new_hl = (h <<8 | l) + add;

            //check for overflow carry bit
            (((h <<8 | l) + add)>255) ? set_c(1) : set_c(0);

            //set hf if overflow after bit11
            ((((h <<8 | l)& 0xFFF + add&0xFFF)&0x1000) == 0x1000) ? set_h(1): set_h(0);
            
            set_n(0);

            h = new_hl >> 4 & 0xF;
            l = new_hl & 0xF;

            m_cycle+=1;
            break;
        }
        
        
        case 4:{
            std::cout << "inc r8" << std::endl;
            uint8_t operand = instruction >> 3 & 0x7; //3 bits
            //overflow from bit3
            ((((*reg8[operand] & 0xF)+ (0x1)) & 0x10)== 0x10) ? set_h(1) : set_h(0);
            
            *reg8[operand] = *reg8[operand]+1;

            (*reg8[operand]==0) ? set_z(1) : set_z(0);

            set_n(0);
                       
            //no extra m_cycles needed.
            break;
        }
        
        case 5:{
            std::cout << "dec r8" << std::endl;
            uint8_t operand = instruction >> 3 & 0x7; //3 bits

            //set h if borrow from bit4 //aka if subtractor > subtract from.
            set_half_if_overflow_8(*reg8[operand], 0x1);

            *reg8[operand] = *reg8[operand]-1;

            //BORROW FROM BIT4
            
            if (*reg8[operand]==0) set_z(1);
            else set_z(0);
            set_n(1);
            break;
        }
        case 6:{
            std::cout << "ld r8, imm8" << std::endl;
            //read immediate.
            uint8_t operand = instruction >> 3 & 0x7; //3 bits
            uint8_t instruction = read();

            *reg8[operand] = instruction;
            //load this into r8
            break;
        }
        
        case 7:{
            uint8_t mid = (instruction >>3) & 0x7;
            switch (mid){
                case 0:{
                    std::cout << "rlca" << std::endl;
                    //rotate to left. msb is added to carry.
                    //flag set z=0, n=0, h=0, c: set according to result.
                    uint8_t carry = (a>> 7) & 0x1; //flag
                    a = a << 1 | carry&0x1;
                    set_z(0);
                    set_n(0);
                    set_h(0);
                    set_c(carry);
                    break;

                }
                    
                case 1:{
                    std::cout << "rrca" << std::endl;

                    uint8_t carry = (a) & 0x1; //flag
                    a = carry<<7 & 0xFF |a >> 1;
                    set_z(0);
                    set_n(0);
                    set_h(0);
                    set_c(carry);
                    
                
                    break;
                }
                    
                    
                case 2:{
                    std::cout << "rla" << std::endl;

                    uint8_t new_carry = (a>> 7) & 0x1; //flag
                    a = a << 1 | (f >>4) & 0x1;
                    set_z(0);
                    set_n(0);
                    set_h(0);
                    set_c(new_carry);
                    
                    break;
                }
                    
                case 3:{
                    std::cout << "rra" << std::endl;
                    uint8_t new_carry = (a) & 0x1; //flag
                    a = (f <<3 & 0x80) |a >> 1;
                    set_z(0);
                    set_n(0);
                    set_h(0);
                    set_c(new_carry);
                    break;
                }
                    
                case 4:{
                    std::cout << "daa" << std::endl;
                    //TODO: DAA WTF

                    break;
                }
                    
                case 5:{

                }
                    std::cout << "cpl" << std::endl;
                    //complement accuulator
                    a = ~a;
                    set_n(1);
                    set_h(1);
                    break;
                case 6:{

                }
                    std::cout << "scf: set carry flag" << std::endl;
                    //set carry flag
                    set_c(1);
                    break;
                case 7:{
                    std::cout << "ccf" << std::endl;
                    f = f ^ 0x80; //toggles c flag.
                    set_n(0);
                    set_h(0);
                    break;

                }
                    
                
            }
            break;
        }
    }
}

void Cpu::decode_block1(uint8_t instruction){
    uint8_t identifier = instruction & 0b111111;
    
    if (identifier == 0b110110){
        std::cout << "halt" << std::endl;
        //IME flag, for interrupts
        //TODO: basically stops PC iteration until an interrupt occurs. interesting! complete later
    }

    else{ //ld: 0b01XXXYYY //one cycle from fetching...fetching instruction...i see.
        std::cout << "ld r8, r8" << std::endl;
        //store value of reg8 yyy into reg8 xxx;
        uint8_t dest = (instruction >> 3) & 0x7;
        uint8_t source = (instruction) & 0x7;
        *reg8[dest] = *reg8[source];
        //no flags altered
    }
}

//8 bit arithmetic
void Cpu::decode_block2(uint8_t instruction){
    uint8_t bits_5_4_3 = instruction >> 3 & 0x7;
    uint8_t operand = instruction & 0x7;
    switch (bits_5_4_3){
        case 0:
            std::cout << "ADD a, r8" << std::endl;
            //write r8 into a.
            set_half_if_overflow_8(a, *reg8[operand]);
            set_carry_if_overflow_8(a, *reg8[operand]);

            a = a+ *reg8[operand];

            set_z((a==0)? 1: 0);
            set_n(0);
            break;
        case 1:
            std::cout << "ADC a, r8" << std::endl;

            //need to check 
            set_half_if_overflow_8(a, *reg8[operand],  (f & 0x10)); //include carry bit
            set_carry_if_overflow_8(a, *reg8[operand], (f&0x10));
            a = a+ *reg8[operand] + (f & 0x10); //+ carry flag
            //TODO: if 0, 0, if overflow from bit3, if overflow from bit7;
            set_z((a==0)? 1: 0);
            set_n(0);
            break;
        case 2:
            std::cout << "SUB a, r8" << std::endl;

            set_half_if_borrow(a, *reg8[operand]);
            set_carry_if_borrow(a, *reg8[operand]);
            a = a - *reg8[operand];
            
            set_z((a==0)? 1: 0);
            set_n(1);
            break;
        case 3:
            std::cout << "SBC a, r8" << std::endl;

            set_half_if_borrow(a, *reg8[operand], (f&0x10));
            set_carry_if_borrow(a, *reg8[operand], (f&0x10));

            a = a - *reg8[operand] - (f & 0x10);

            set_z((a==0)? 1: 0);
            set_n(1);
            break;

        case 4:
            std::cout << "AND a, r8" << std::endl;
            a = a & *reg8[operand];
            
            set_z((a==0)? 1: 0);
            set_n(0);
            set_h(1);
            set_c(0);
            break;
        case 5:
            std::cout << "XOR a, r8" << std::endl;
            a = a ^ *reg8[operand];
             //TODO: set flag 0 if zero, 0, 0, 0;
            set_z((a==0)? 1: 0);
            set_n(0);
            set_h(0);
            set_c(0);

            break;
        case 6:
            std::cout << "OR a, r8" << std::endl;
            a = a | *reg8[operand];
            //TODO: set flag Z IF 0, 0 0 0
            set_z((a==0)? 1: 0);
            set_n(0);
            set_h(0);
            set_c(0);
            break;
        case 7:
            std::cout << "CP a, r8" << std::endl;
            //compare
            //flag setting instruction
            set_half_if_borrow(a, *reg8[operand]);
            set_carry_if_borrow(a, *reg8[operand]);
            set_z((a==0)? 1: 0);
            set_n(1);
            break;
        

    }
    
}
void Cpu::decode_block3(uint8_t instruction){
    uint8_t opcode = instruction & 0x7; //first 3 bits

    switch (opcode){
        case 6:
            std::cout << "logic gates here" << std::endl;
            switch (instruction){
                case 0xC6:{
                    std::cout << "add a, imm8" << std::endl;
                    //add imm8 to a
                    uint8_t imm8 = read();

                    set_half_if_overflow_8(a, imm8);
                    set_carry_if_overflow_8(a, imm8);

                    a = a+imm8;

                    set_z((a==0)? 1: 0);
                    set_n(0);
                    break;

                }
                case 0xCE:{
                    std::cout << "adc a, imm8" << std::endl;

                    uint8_t imm8 = read();
                    set_half_if_overflow_8(a, imm8,  (f & 0x10)); //include carry bit
                    set_carry_if_overflow_8(a, imm8, (f&0x10));

                    a = a+ imm8 + (f & 0x10); //+ carry flag
                    //TODO: if 0, 0, if overflow from bit3, if overflow from bit7;
                    set_z((a==0)? 1: 0);
                    set_n(0);
                    break;
                }
                case 0xD6:{
                    std::cout << "sub a, imm8" << std::endl;

                    uint8_t imm8 = read();

                    set_half_if_borrow(a, imm8);
                    set_carry_if_borrow(a, imm8);
                    a = a - imm8;
                    
                    set_z((a==0)? 1: 0);
                    set_n(1);
                    break;
                }
                case 0xDE:{
                    std::cout << "sbc a, imm8" << std::endl;
                    uint8_t imm8 = read();

                    set_half_if_borrow(a, imm8, (f&0x10));
                    set_carry_if_borrow(a, imm8, (f&0x10));

                    a = a - imm8 - (f & 0x10);

                    set_z((a==0)? 1: 0);
                    set_n(1);

                    break;
                }
                case 0xE6:{
                    std::cout << "and a, imm8" << std::endl;
                    uint8_t imm8 = read();
                    a = a & imm8;
            
                    set_z((a==0)? 1: 0);
                    set_n(0);
                    set_h(1);
                    set_c(0);

                    break;
                }
                case 0xEE:{
                    std::cout << "xor a, imm8" << std::endl;

                    uint8_t imm8 = read();
                    a = a ^ imm8;
    
                    set_z((a==0)? 1: 0);
                    set_n(0);
                    set_h(0);
                    set_c(0);
                    break;
                }
                case 0xF6:{
                    std::cout << "or a, imm8" << std::endl;
                    uint8_t imm8 = read();
                    a = a | imm8;
                   
                    set_z((a==0)? 1: 0);
                    set_n(0);
                    set_h(0);
                    set_c(0);
                    break;
                }
                case 0xFE:{
                    std::cout << "cp a, imm8" << std::endl;
                    uint8_t imm8 = read();
                    set_half_if_borrow(a, imm8);
                    set_carry_if_borrow(a, imm8);
                    set_z((a==0)? 1: 0);
                    set_n(1);

                    break;
                }
            }

            break;
        
        case 0:
            switch (instruction){
                case 0xE0:{
                    std::cout << "LDH [imm8], A" << std::endl;
                    //load val of A into ldh
                    uint8_t imm8 = read();
                    write((0xFF00 | imm8), a);
                    //no flag
                    break;

                }
                    
                case 0xF0:{
                    std::cout << "LDH A, [imm8]" << std::endl;
                    //read imm8
                    uint8_t imm8 = read();
                    uint8_t val = read(0xFF00|imm8);
                    a = val;
                    break;
                }
                case 0xE8:{
                    //TODO: SIGNED IMM8
                    std::cout << "ADD sp, imm8" << std::endl;
                    //add imm8 to sp.
                    
                    //sp is split into 2, so this is going to be a pain in the ass.
                    uint8_t imm8 = read();
                    set_half_if_overflow_8(sp.p, imm8);
                    set_carry_if_overflow_8(sp.p, imm8); //weird, since sp is 16 bits.

                    sp.full = sp.full + imm8;

                    set_z(0);
                    set_n(0);

                    m_cycle+=2; //4th cycle. its because sp is 2 bytes.
                    
                    break;

                }
                    
                case 0xF8:{
                    //TODO: SIGNED IMM8
                    std::cout << "ld hl, sp + imm8" << std::endl;
                    //
                    uint8_t imm8 = read();

                    set_half_if_overflow_8(sp.p, imm8);
                    set_carry_if_overflow_8(sp.p, imm8); //might need to switch

                    sp.full = sp.full + imm8;

                    set_z(0);
                    set_n(0);


                    h = sp.s;
                    l = sp.p;


                    //add e8 to SP, store result in hl. How is this lesss clock cycles lol

                    break;

                }
                    
                default:
                    std::cout << "RET COND" << std::endl;
                    //return from subroutine. aka stackpointer bruv.
                    uint8_t condition = instruction >> 3 & 0x3;

                    switch (condition){
                    case 0:{
                        //if z==0
                        if ((f& 0x80)==0){
                            //jump
                            pc = (ram->memory[sp.full+1]<<8) | ram->memory[sp.full]; //hi+lo
                            sp.full+=2; //pop up
                            m_cycle+=3; //2 bytes + internal set pc
                        }
                    }
                        break;
                    case 1:{
                        //if z==1
                        if ((f& 0x80)==1){
                            //jump
                            pc = (ram->memory[sp.full+1]<<8) | ram->memory[sp.full]; //hi+lo
                            sp.full+=2; //pop up
                            m_cycle+=3; //2 bytes + internal set pc

                        }
                    }
                        break;
                    case 2:{
                        //if c==0
                        if ((f& 0x10)==0){
                            //jump
                            pc = (ram->memory[sp.full+1]<<8) | ram->memory[sp.full]; //hi+lo
                            sp.full+=2; //pop up
                            m_cycle+=3; //2 bytes + internal set pc

                        }

                    }
                        break;
                    case 3:{
                        if ((f& 0x10)==1){
                            //jump
                            pc = (ram->memory[sp.full+1]<<8) | ram->memory[sp.full]; //hi+lo
                            sp.full+=2; //pop up
                            m_cycle+=3; //2 bytes + internal set pc
                        }
                    }
                        m_cycle+=1; //internal branch decision
                        break;
                    }
                    break;    
            }
            break;
        case 1:
            switch (instruction){
                case 0xC9:
                    std::cout << "ret" << std::endl;
                    pc = (ram->memory[sp.full+1]<<8) | ram->memory[sp.full]; //hi+lo
                    sp.full+=2; //pop up
                    m_cycle+=3; //2 bytes + internal set pc
                    break;
                case 0xD9:
                    std::cout << "reti" << std::endl;
                    //TODO: IME related stuff. basically does EI then RET. wait for interrupts.
                    break;
                case 0xE9:
                    std::cout << "jp hl" << std::endl;
                    pc = (h << 8)| l;
                    break;
                case 0xFA:
                    std::cout << "ld sp, hl" << std::endl;
                    sp.s = h;
                    sp.p = l;
                    m_cycle+=1; //internal: reg16 low, hi.
                    break;
                default:{
                    std::cout << "pop r16stk" << std::endl;
                    uint8_t dest =  instruction >> 4 & 0x3;
                    
                    //pop af
                    if (dest==0x3){
                        //af
                        f = ram->memory[sp.full];
                        sp.full +=1;
                        a = ram->memory[sp.full];
                        sp.full +=1;

                        m_cycle+=2;

                        //flags
                        set_z(f>>7 & 0x1);
                        set_n(f>>6 & 0x1);
                        set_h(f>>5 & 0x1);
                        set_c(f>>4 & 0x1);
                        break;
                    }
                    //else: pop r16

                    //get 16 bit address, put into r16.
                    //pop 
                    //
                    reg16[dest][1] = ram->memory[sp.full]; //LD reg_low, sp
                    sp.full+=1;//decrement
                    reg16[dest][0] = ram->memory[sp.full]; //LD reg_hi, sp
                    sp.full+=1;//decrement

                    m_cycle+=2;
                    break;

                }
                    
                
            }
            break;
        case 2:
            switch (instruction){
                case 0xE2:
                    std::cout << "ldh [c], a" << std::endl;
                    //store a into address w/ vallue c
                    //ld [0xff00+c], a
                    write(0xFF00+c, a);
                    break;
                case 0xEA:{
                    std::cout << "ld [imm16], a" << std::endl;
                    uint8_t imm_low = read();
                    uint8_t imm_hi = read();
                    write(imm_hi<<8+imm_low, a);
                    break;
                    

                }
                    
                case 0xF2:{
                    std::cout << "ldh a, [c]" << std::endl;
                    //a = read(0xff00+c)
                    a = read(0xFF00 + c);
                    break;
                }
                    
                case 0xFA:{
                    std::cout << "ld a, [imm16]" << std::endl;
                    uint8_t low = read();
                    uint8_t hi = read();
                    a = read((hi << 8)|low);
                    break;

                }
                    
                default:{
                    std::cout << "jp cond, imm16" << std::endl;
                    //jumpto imm16 on condition flag.
                    uint8_t condition = instruction >> 3 & 0x3;

                    //READ IMM8
                    uint8_t low = read();
                    uint8_t hi = read();
                    uint16_t jump = (hi << 8) | low;
                    
                    switch (condition){
                        case 0:{
                            //if z==0
                            if ((f& 0x80)==0){
                                //jump
                                pc =jump;
                                m_cycle+=1;
                            }
                        }
                            break;
                        case 1:{
                            //if z==1
                            if ((f& 0x80)==1){
                                //jump
                                pc =jump;
                                m_cycle+=1;

                            }
                        }
                            break;
                        case 2:{
                            //if c==0
                            if ((f& 0x10)==0){
                                //jump
                                pc =jump;
                                m_cycle+=1;

                            }

                        }
                            break;
                        case 3:{
                            if ((f& 0x10)==1){
                                //jump
                                pc =jump;
                                m_cycle+=1;

                            }
                        }
                            break;
                    }

                    //

                    break;
            }
            break;
        case 3:
            switch (instruction){
                case 0xC3:{
                    std::cout << "jp imm16" << std::endl;
                        uint8_t low = read();
                        uint8_t hi = read();
                        uint16_t jump = (hi << 8) | low;
                        pc = jump;
                        m_cycle+=1;
                    
                    break;

                }
                    
                case 0xCB:
                    std::cout << "PREFIX" << std::endl;
                    //TODO
                    break;
                case 0xF3:
                    std::cout << "di" << std::endl;
                    //clear IME flag
                    //todo
                    break;
                case 0xFB:
                    std::cout << "ei" << std::endl;
                    //enable ime flag
                    //todo
                    break;
                
 
                
            }
            break;
        case 4:{
            std::cout << "call cond, imm16" << std::endl;
            uint8_t condition = instruction >> 3 & 0x3;
            uint8_t lo = read();
            uint8_t hi = read();

            switch (condition){
                        case 0:{
                            //if z==0
                            if ((f& 0x80)==0){
                                //jump
                                sp.full-=1;
                                write(sp.full, ((pc>>8) & 0xFF) );
                                sp.full-=1;
                                write(sp.full,(pc & 0xFF));
                                
                                //jump
                                pc = (hi <<8)| lo;
                                m_cycle+=3; //16 bits twice. 
                            }
                        }
                            break;
                        case 1:{
                            //if z==1
                            if ((f& 0x80)==1){
                                //jump
                                sp.full-=1;
                                write(sp.full, ((pc>>8) & 0xFF) );
                                sp.full-=1;
                                write(sp.full,(pc & 0xFF));
                                
                                //jump
                                pc = (hi <<8)| lo;
                                m_cycle+=3; //16 bits twice. 

                            }
                        }
                            break;
                        case 2:{
                            //if c==0
                            if ((f& 0x10)==0){
                                //jump
                                sp.full-=1;
                                write(sp.full, ((pc>>8) & 0xFF) );
                                sp.full-=1;
                                write(sp.full,(pc & 0xFF));
                                
                                //jump
                                pc = (hi <<8)| lo;
                                m_cycle+=3; //16 bits twice. 

                            }

                        }
                            break;
                        case 3:{
                            if ((f& 0x10)==1){
                                //jump
                                sp.full-=1;
                                write(sp.full, ((pc>>8) & 0xFF) );
                                sp.full-=1;
                                write(sp.full,(pc & 0xFF));
                      
                                //jump
                                pc = (hi <<8)| lo;
                                m_cycle+=3; //16 bits twice. 

                            }
                        }
                            break;
                    }
            //todo
            break;
        }
            
        case 5:
            switch (instruction){
                case 0xCD:{
                    std::cout << "CALL imm16" << std::endl;
                    uint8_t lo = read();
                    uint8_t hi = read();

                    //store instruction to return to.
                    sp.full-=1;
                    write(sp.full, ((pc>>8) & 0xFF) );
                    sp.full-=1;
                    write(sp.full,(pc & 0xFF));

                    //jump
                    pc = (hi <<8)| lo;

                    m_cycle+=1; //16 bits twice. 
                    break;
                }
                    
                default:
                    std::cout << "push r16stk" << std::endl;

                    uint8_t reg = (instruction >> 4) & 0x3;

                    if (reg==0x3){ //push af
                        sp.full-=1;
                        write(sp.full, a);
                        sp.full-=1;
                        write(sp.full, f);
                        m_cycle+=1;
                        break;
                    }

                    sp.full-=1;
                    write(sp.full, reg16[reg][0]); 
                    sp.full-=1;
                    write(sp.full, reg16[reg][1]);
                    m_cycle+=1;

                    break;
            }
            break;
        case 7:{
            std::cout << "rst tgt3" << std::endl;
            
            uint8_t target = (instruction >> 3) & 0x7;
            //save pc to stack.
            sp.full -=1;
            write(sp.full, (pc >>8) & 0xff);
            sp.full-=1;
            write(sp.full, pc & 0xff );
            //pc = target
            pc = target;

            break;    
        }

            }
            
        
        
    
    
    }

    
}