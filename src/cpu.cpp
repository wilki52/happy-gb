#include "cpu.h"
#include <iostream>
Cpu::Cpu(){

}

//return 8bit instruction
uint8_t Cpu::fetch(Memory ram){
    uint8_t instruction = ram.memory[pc];
    pc= pc+1;
    return instruction;
}
uint8_t Cpu::decode(uint8_t instruction){
    std::cout << "instruction: "  << unsigned(instruction) << std::endl;
    uint8_t opcode = (instruction >> 6) & 0x3;
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
            //uint8_t bit5 = 


            if ((instruction >> 5 & 0x1)==1){
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
        case 1:
            std::cout << "LD r16, imm16" << std::endl;
            break;
        case 2:
            std::cout << "LD [R16mem], A" << std::endl;
            break;
        case 10:
            std::cout << "LD A, [R16mem]" << std::endl;
            break;
        case 8:
            std::cout << "LD [imm16], sp" << std::endl;
            break;

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
    }
    else{
        std::cout << "ld r8, r8" << std::endl;
    }
}


void Cpu::decode_block2(uint8_t instruction){
    uint8_t bits_5_4_3 = instruction >> 3 & 0x7;
    switch (bits_5_4_3){
        case 0:
            std::cout << "ADD a, r8" << std::endl;
            break;
        case 1:
            std::cout << "ADC a, r8" << std::endl;
            break;
        case 2:
            std::cout << "SUB a, r8" << std::endl;
            break;
        case 3:
            std::cout << "SBC a, r8" << std::endl;
            break;
        case 4:
            std::cout << "AND a, r8" << std::endl;
            break;
        case 5:
            std::cout << "XOR a, r8" << std::endl;
            break;
        case 6:
            std::cout << "OR a, r8" << std::endl;
            break;
        case 7:
            std::cout << "CP a, r8" << std::endl;
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