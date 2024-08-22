#include "cpu.h"
#include <iostream>
Cpu::Cpu(){
    
}
Cpu::Cpu(Memory& ram): ram(&ram){
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
    uint8_t bc[2] = {b, c};
    reg16[0x0] = bc;
    uint8_t de[2] = {d, e};
    reg16[0x1] = de;
    uint8_t hl[2] = {h, l};
    reg16[0x2] = hl;

    uint8_t sp[2] = {s, p};
    reg16[0x3] = sp;
    
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

            }
            else{
                uint8_t bits43 = instruction >> 3 & 0x2;
                switch (bits43){
                    case 3:
                        std::cout << "jr imm8" << std::endl;
                        break;
                    case 2:
                        std::cout << "stop" << std::endl;
                        break;
                    case 0:
                        std::cout << "NOP" << std::endl;
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
            
            break;
            }
        case 8:{
            std::cout << "LD [imm16], sp" << std::endl;
            //load value in sp (hi lo) to (imm16), aka the address that valimm16 points to
            uint8_t instruction_low = read();
            uint8_t instruction_hi = read();
            write(instruction_low, p);
            write(instruction_hi, s);
            //write(pc, s), pc++
            break;
        }
            

        case 3:
            std::cout << "inc r16" << std::endl;
            break;
        case 11:
            std::cout << "dec r16" << std::endl;
            break;
        case 9:
            std::cout << "add hl, r16" << std::endl;
            break;
        
        case 4:
            std::cout << "inc r8" << std::endl;
            break;
        case 5:
            std::cout << "dec r8" << std::endl;
            break;
        
        case 6:
            std::cout << "ld r8, imm8" << std::endl;
            break;
        case 7:
            uint8_t mid = (instruction >>3) & 0x7;
            switch (mid){
                case 0:
                    std::cout << "rlca" << std::endl;
                    break;
                case 1:
                    std::cout << "rrca" << std::endl;
                    break;
                case 2:
                    std::cout << "rla" << std::endl;
                    break;
                case 3:
                    std::cout << "rra" << std::endl;
                    break;
                case 4:
                    std::cout << "daa" << std::endl;
                    break;
                case 5:
                    std::cout << "cpl" << std::endl;
                    break;
                case 6:
                    std::cout << "scf" << std::endl;
                    break;
                case 7:
                    std::cout << "ccf" << std::endl;
                    break;
                
            }
            break;
    }
}

void Cpu::decode_block1(uint8_t instruction){
    uint8_t identifier = instruction & 0b111111;
    
    if (identifier == 0b110110){
        std::cout << "halt" << std::endl;
        //IME flag, for interrupts
    }

    else{ //ld: 0b01XXXYYY //one cycle from fetching...fetching instruction...i see.
        std::cout << "ld r8, r8" << std::endl;
        //store value of reg8 yyy into reg8 xxx;
        uint8_t dest = (instruction >> 3) & 0x7;
        uint8_t source = (instruction) & 0x7;

        *reg8[dest] = *reg8[source];
    }
}



void Cpu::set_flags(uint8_t z, uint8_t n, uint8_t h, uint8_t c){

}

//8 bit arithmetic
void Cpu::decode_block2(uint8_t instruction){
    uint8_t bits_5_4_3 = instruction >> 3 & 0x7;
    uint8_t operand = instruction & 0x7;
    switch (bits_5_4_3){
        case 0:
            std::cout << "ADD a, r8" << std::endl;
            //write r8 into a.
            a = a+ *reg8[operand];
            //set if result is 0, n=0, h set overflow from bit3, c if overfow from bit7
            break;
        case 1:
            std::cout << "ADC a, r8" << std::endl;
            a = a+ *reg8[operand] + (f & 0x10); //+ carry flag
            //TODO: if 0, 0, if overflow from bit3, if overflow from bit7;
            break;
        case 2:
            std::cout << "SUB a, r8" << std::endl;
            a = a - *reg8[operand];
            //TODO: set flag 0 if zero, 1, set if borrow from bit4, if borrow (set if r8>A);
            break;
        case 3:
            std::cout << "SBC a, r8" << std::endl;
            a = a - *reg8[operand] - (f & 0x10);
            //TODO: set flag 0 if zero, 1, if borrow, if borrow;
            break;

        case 4:
            std::cout << "AND a, r8" << std::endl;
            a = a & *reg8[operand];
            //TODO: set flag 0 if zero, 0, 1, 0;
            break;
        case 5:
            std::cout << "XOR a, r8" << std::endl;
            a = a ^ *reg8[operand];
             //TODO: set flag 0 if zero, 0, 0, 0;

            break;
        case 6:
            std::cout << "OR a, r8" << std::endl;
            a = a | *reg8[operand];
            //TODO: set flag Z IF 0, 0 0 0
            break;
        case 7:
            std::cout << "CP a, r8" << std::endl;
            //compare
            uint8_t temp = a - *reg8[operand];
            //TODO: set flag 0, 1, set if borrow, set if borrow
            break;
        

    }
    
}
void Cpu::decode_block3(uint8_t instruction){
    uint8_t opcode = instruction & 0x7; //first 3 bits

    switch (opcode){
        case 6:
            std::cout << "logic gates here" << std::endl;
            switch (instruction){
                case 0xC6:
                    std::cout << "add a, imm8" << std::endl;
                    break;
                case 0xCE:
                    std::cout << "adc a, imm8" << std::endl;
                    break;
                case 0xD6:
                    std::cout << "sub a, imm8" << std::endl;
                    break;
                case 0xDE:
                    std::cout << "sbc a, imm8" << std::endl;
                    break;
                case 0xE6:
                    std::cout << "and a, imm8" << std::endl;
                    break;
                case 0xEE:
                    std::cout << "xor a, imm8" << std::endl;
                    break;
                case 0xF6:
                    std::cout << "or a, imm8" << std::endl;
                    break;
                case 0xFE:
                    std::cout << "cp a, imm8" << std::endl;
                    break;
                
            }

            break;
        
        case 0:
            switch (instruction){
                case 0xE0:
                    std::cout << "LDH [imm8], A" << std::endl;
                    break;
                case 0xF0:
                    std::cout << "LDH A, [imm8]" << std::endl;
                    break;
                case 0xE8:
                    std::cout << "ADD sp, imm8" << std::endl;
                    break;
                case 0xF8:
                    std::cout << "ld hl, sp + imm8" << std::endl;
                    break;
                default:
                    std::cout << "RET COND" << std::endl;
                    break;

                
            }
            break;
        case 1:
            switch (instruction){
                case 0xC9:
                    std::cout << "ret" << std::endl;
                    break;
                case 0xD9:
                    std::cout << "reti" << std::endl;
                    break;
                case 0xE9:
                    std::cout << "jp hl" << std::endl;
                    break;
                case 0xFA:
                    std::cout << "ld sp, hl" << std::endl;
                    break;
                default:
                    std::cout << "pop r16stk" << std::endl;
                    break;
                
            }
            break;
        case 2:
            switch (instruction){
                case 0xE2:
                    std::cout << "ldh [c], a" << std::endl;
                    break;
                case 0xEA:
                    std::cout << "ld [imm16], a" << std::endl;
                    break;
                case 0xF2:
                    std::cout << "ld [imm16], a" << std::endl;
                    break;
                case 0xFA:
                    std::cout << "ld a, [imm16]" << std::endl;
                    break;
                default:
                    std::cout << "jp cond, imm16" << std::endl;
                    break;
                
            }
            break;
        case 3:
            switch (instruction){
                case 0xC3:
                    std::cout << "jp imm16" << std::endl;
                    break;
                case 0xCB:
                    std::cout << "PREFIX" << std::endl;
                    break;
                case 0xF3:
                    std::cout << "di" << std::endl;
                    break;
                case 0xFB:
                    std::cout << "ei" << std::endl;
                    break;
                
 
                
            }
            break;
        case 4:
            std::cout << "call cond, imm16" << std::endl;
            break;
        case 5:
            switch (instruction){
                case 0xCD:
                    std::cout << "CALL imm16" << std::endl;
                    break;
                default:
                    std::cout << "push r16stk" << std::endl;
                    break;
            }
            break;
        case 7:
            std::cout << "rst tgt3" << std::endl;
            break;    
        
        
    
    
    }

    
}