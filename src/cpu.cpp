#include "cpu.h"
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
Cpu::Cpu(){
    
}
Cpu::Cpu(Memory& ram): ram(&ram){
    //init sp
    //sp = {0};
    ime_hold = 0;
    ime = 0;
    pc = 0;
    bc.full = 0;
    de.full = 0;
    hl.full = 0;
    counter.full = 0;
    sp.full = 0xFFFE; //starts at top of memory

    //init reg8 new
    reg8[0x0] = &bc.high;
    reg8[0x1] = &bc.low;
    reg8[0x2] = &de.high;
    reg8[0x3] = &de.low;
    reg8[0x4] = &hl.high;
    reg8[0x5] = &hl.low;
    //reg8_new[0x6] = &h;//8 bits at [hl]
    reg8[0x6] = &ram.memory[hl.full]; //this doesnt work since it doesnt get updated.
    reg8[0x7] = &af.high;
        

    reg16[0] = &bc;
    reg16[1] = &de;
    reg16[2] = &hl;
    reg16[3] = &sp;

    //rst vecs
    vec[0x0] = 0x00; //11 00 0 111 //C7
    vec[0x1] = 0x08; //11 00 1 111 //CF
    vec[0x2] = 0x10; //11 01 0 111 //D7
    vec[0x3] = 0x18; //11 01 1 111 //DF
    vec[0x4] = 0x20; //11 10 0 111 //E7
    vec[0x5] = 0x28; //11 10 1 111 //EF
    vec[0x6] = 0x30; //11 11 0 111 /F7
    vec[0x7] = 0x38; //11 11 1 111 //FF

}


int Cpu::read_rom(const char path[]){
    std::ifstream reader;
    reader.open(path, std::ios::binary);
    if (!reader){
        std::cout << "cant open ROM" << std::endl;
        return 0;
    }
    //reader.seekg(0, reader.end);
    //int length = reader.tellg();
    //reader.seekg(0, reader.beg);

    uint8_t instruction;
    int point = 0;
    while (!reader.eof()){
       // memory

        while (reader.read((char*)(&instruction), sizeof(instruction))){
            
            //std::cout << "address: " << unsigned(point) << " hex: "<< std::hex << (int)(instruction) << " " << std::endl;
            ram->memory[point] = instruction;
            //std::cout << "address: " << unsigned(point) << " hex: "<< std::hex << (int)(ram->memory[point]) << " " << std::endl;
            std::cout << "address: " <<unsigned(ram->memory[point]) << std::endl; //it doesnt get saved! figure out different solution.
            point+=1;
            //SDL_Delay(1);
        }
        
    }
    //where to start, where to end?


    return 1;
}

void Cpu::set_z(uint8_t z){
    uint8_t &f = af.low;
    (z==1) ? (f= f| 0x80) : (f = f & 0x7F);
}
void Cpu::set_n(uint8_t n){
     uint8_t &f = af.low;
    (n==1) ? (f = f | 0x40) : (f = f & 0xBF);
}
void Cpu::set_h(uint8_t h){
     uint8_t &f = af.low;
    (h==1) ? (f = f | 0x20): (f = f & 0xDF);
}
void Cpu::set_c(uint8_t c){
     uint8_t &f = af.low;
    (c==1) ? (f = f | 0x10): (f = f & 0xEF);
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
    uint16_t overflow_bit = (((op1 & 0xFFF)+(op2 & 0xFFF)+ (carry & 0xFFF)) & 0x1FFF);
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
void Cpu::write(uint16_t address, uint8_t data){
    ram->memory[address] = data;

    pc= pc+1;
    m_cycle+=1; //writing IS +1 m_cycle
}


void Cpu::decode(uint8_t instruction){
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

    //ime check
    
    if (ime_hold ==1){
        ime=ime_hold;
    }
    if (ime_hold >0){
        ime_hold-=1;
    }
}

void Cpu::decode_block0(uint8_t instruction){
    uint8_t opcode = instruction & 0xF; //gets first 4 bits,s tarting from lsb
    std::cout << unsigned(opcode) << std::endl;

    switch (opcode){
        case 0:

            if ((instruction >> 5 & 0x1)==1){ //if bit5==1
                uint8_t condition = instruction >> 3 & 0x3;
                jr_cc_n8(condition);

            }
            else{
                uint8_t bits43 = instruction >> 3 & 0x2;
                switch (bits43){
                    case 3:
                        std::cout << "jr imm8" << std::endl;
                        jr_n8();
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
        case 1:
            ld_r16(((instruction >> 4) & 0x3));
            break;
        case 2:
            ld_to_r16mem((instruction >> 4) & 0x3, af.high);
            break;
        case 10:
            ld_a_from_r16mem(af.high, instruction >> 4 & 0x3);
            break; 
        case 8:
            ld_to_n16mem();
            break;
        case 3:{
            uint8_t operand = instruction >> 4 & 0x3;
            inc_r16(operand);
            break;
        }
        case 11:{
            uint8_t operand = instruction >> 4 & 0x3;
            dec_r16(operand);
            break;
        }
        case 9:{
            uint8_t operand = instruction >> 4 & 0x3;
            add_hl_from_r16(operand);
            break;
        }
        case 4:{
            uint8_t operand = instruction >> 3 & 0x7; //3 bits
            inc_r8(operand);
            break;
        }
        case 5:{
            uint8_t operand = instruction >> 3 & 0x7; //3 bits
            dec_r8(operand);
            break;
        }
        case 6:{
            uint8_t operand = instruction >> 3 & 0x7; //3 bits
            ld_r8_n8(operand);
            break;
        }
        case 7:{
            uint8_t mid = (instruction >>3) & 0x7;
            switch (mid){
                case 0: rlca(); break;
                case 1: rrca(); break;
                case 2: rla(); break;
                case 3: rra(); break;  
                case 4:{
                    std::cout << "daa" << std::endl;
                    //TODO: DAA WTF
                    break;
                }  
                case 5: cpl(); break;
                case 6:scf(); break;
                case 7:ccf(); break;
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
    else{
        uint8_t dest = (instruction >> 3) & 0x7;
        uint8_t source = (instruction) & 0x7;
        ld_r8_r8(dest, source);
    }
}

//8 bit arithmetic
void Cpu::decode_block2(uint8_t instruction){
    uint8_t bits_5_4_3 = instruction >> 3 & 0x7;
    uint8_t &a = af.high;
    uint8_t operand = instruction & 0x7;
    switch (bits_5_4_3){
        case 0:
            std::cout << "ADD a, r8" << std::endl;
            add(a, operand);
            break;
        case 1:
            std::cout << "ADC a, r8" << std::endl;
            adc(a, operand);
            break;
        case 2:
            std::cout << "SUB a, r8" << std::endl;
            sub(a, operand);
            break;
        case 3:
            std::cout << "SBC a, r8" << std::endl;
            sbc(a, operand);
            break;
        case 4:
            std::cout << "AND a, r8" << std::endl;
            opcode_and(a, operand);
            break;
        case 5:
            std::cout << "XOR a, r8" << std::endl;
            opcode_xor(a, operand);
            break;
        case 6:
            std::cout << "OR a, r8" << std::endl;
            opcode_or(a, operand);
            break;
        case 7:
            std::cout << "CP a, r8" << std::endl;
            cp(a, operand);
            break;
    }
    
}
void Cpu::decode_block3(uint8_t instruction){
    uint8_t opcode = instruction & 0x7; //first 3 bits

    switch (opcode){
        case 6:{
            std::cout << "logic gates here" << std::endl;
            uint8_t &a = af.high;
            switch (instruction){
                case 0xC6:{
                    std::cout << "add a, imm8" << std::endl;
                    //add imm8 to a
                    add(a);
                    break;

                }
                case 0xCE:{
                    std::cout << "adc a, imm8" << std::endl;

                    adc(a);
                    break;
                }
                case 0xD6:{
                    std::cout << "sub a, imm8" << std::endl;
                    sub(a);
                    break;
                }
                case 0xDE:{
                    std::cout << "sbc a, imm8" << std::endl;
                    sbc(a);

                    break;
                }
                case 0xE6:{
                    std::cout << "and a, imm8" << std::endl;
                    opcode_and(a);
                    break;
                }
                case 0xEE:{
                    std::cout << "xor a, imm8" << std::endl;
                    opcode_xor(a);
                    break;
                }
                case 0xF6:{
                    std::cout << "or a, imm8" << std::endl;
                    opcode_or(a);
                    break;
                }
                case 0xFE:{
                    std::cout << "cp a, imm8" << std::endl;
                    cp(a);
                    break;
                }
            }

            break;
        }
        case 0:{
            uint8_t &a = af.high;
            switch (instruction){
                case 0xE0:
                    std::cout << "LDH [imm8], A" << std::endl;
                    ldh_to_n8(a);
                    break;
                case 0xF0:
                    std::cout << "LDH A, [imm8]" << std::endl;
                    ldh_from_n8(a);
                    break;
                case 0xE8:
                    std::cout << "ADD sp, s_imm8" << std::endl;
                    add_sp_from_n8();
                    break;
                case 0xF8:
                    //TODO: SIGNED IMM8
                    std::cout << "ld hl, sp + imm8" << std::endl;
                    ld_hl_sp_and_n8();
                    break;
                default:
                    std::cout << "RET COND" << std::endl;
                    uint8_t condition = instruction >> 3 & 0x3;
                    ret_cond(condition);
                    break;    
            }
            break;
        }
        case 1:
            switch (instruction){
                case 0xC9:
                    std::cout << "ret" << std::endl;
                    ret();
                    break;
                case 0xD9:
                    std::cout << "reti" << std::endl;
                    reti();
                    //TODO: IME related stuff. basically does EI then RET. wait for interrupts.
                    break;

                case 0xE9:
                    std::cout << "jp hl" << std::endl;
                    jp_hl();
                    break;

                case 0xFA:
                    std::cout << "ld sp, hl" << std::endl;
                    ld_sp_from_hl();
                    break;
                default:{
                    std::cout << "pop r16stk" << std::endl;
                    uint8_t key =  instruction >> 4 & 0x3;
                    pop(key);
                    break;
                }
            }
            break;
        case 2:
            switch (instruction){
                case 0xE2:
                    std::cout << "ldh [c], a" << std::endl;
                    ldh_to_c(af.high);
                    break;
                case 0xEA:
                    std::cout << "ld [imm16], a" << std::endl;
                    ld_to_n16(af.high);
                    break;
                case 0xF2:
                    std::cout << "ldh a, [c]" << std::endl;
                    ldh_from_c(af.high);
                    break;
                case 0xFA:
                    std::cout << "ld a, [imm16]" << std::endl;
                    ld_from_n16(af.high);
                    break;
                default:
                    std::cout << "jp cond, imm16" << std::endl;
                    uint8_t condition = instruction >> 3 & 0x3;
                    jp_cond(condition);
                    break;
            }
            break;
        case 3:
            switch (instruction){
                case 0xC3:
                    std::cout << "jp imm16" << std::endl;
                    jp();
                    break;
                case 0xCB: //prefix
                    std::cout << "PREFIX" << std::endl;

                    //TODO
                    break;
                case 0xF3:
                    std::cout << "di" << std::endl;
                    di();
                    break;
                case 0xFB:
                    std::cout << "ei" << std::endl;
                    ei();
                    break;
            }
            break;
        case 4:{
            std::cout << "call cond, imm16" << std::endl;
            uint8_t condition = instruction >> 3 & 0x3;
            call_cond(condition);
            break;
        }
        case 5:
            switch (instruction){
                case 0xCD:
                    std::cout << "CALL imm16" << std::endl;
                    call();
                    break;
                default:
                    std::cout << "push r16stk" << std::endl;
                    uint8_t reg = (instruction >> 4) & 0x3;
                    push(reg);
                    break;
            }
            break;
        case 7:{
            std::cout << "rst tgt3" << std::endl;     
            uint8_t target = (instruction >> 3) & 0x7;
            rst(target);
            break;    
        }
    }
    
}

void Cpu::cb(){
    uint8_t bits = fetch();

    uint8_t opcode = (bits >> 3) &0x1F;

    switch (opcode){
        case 0x0:
            
            break;
        case 0x1:
            break;
        case 0x2:
            break;
        case 0x3:
            break;
        case 0x4:
            break;
        case 0x5:
            break;
        case 0x6:
            break;
        case 0x7:
            break;
        
    }

}