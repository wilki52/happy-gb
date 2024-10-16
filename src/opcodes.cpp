#include "cpu.h"

#include <iostream>
void nop();

void Cpu::ld_r16(uint8_t dest){
    //std::cout << "LD r16, imm16" << std::endl;
    uint8_t low = read();
    reg16[dest]->low = low; //c
            
    uint8_t hi = read();
    reg16[dest]->high = hi; //b
            

} //ld r16, imm16

void Cpu::ld_to_r16mem(uint16_t address, uint8_t &reg){
    //std::cout << "LD [R16mem], A" << std::endl;

    switch (address){
        case 0:
                    write(reg16[address]->full, reg); //bc
                    break;
        case 1:
                    write(reg16[address]->full, reg); //de
                    break;
        case 2: 
                    write(reg16[3]->full+1, reg); //hl+
                    break;
        case 3:
                    write(reg16[3]->full-1, reg); //hl-
                    break;

            }

} //ld [r16mem], a
void Cpu::ld_a_from_r16mem(uint8_t &reg, uint16_t address){
    //std::cout << "LD A, [R16mem]" << std::endl;
    switch (address){
        case 0:
                    reg = read(reg16[address]->full); //bc
                    break;
        case 1:
                    reg = read(reg16[address]->full); //de
                    break;
        case 2: 
                    reg = read(reg16[address]->full+1); //hl+
                    break;
        case 3:
                    reg = read(reg16[address]->full-1); //hl-
                    break;
    }

} //ld a, [r16mem]

void Cpu::ld_to_n16mem(){
    //std::cout << "LD [imm16], sp" << std::endl;

    uint8_t low = read();
    uint8_t hi = read();
    uint16_t address = hi <<8 | low;
    write(address, sp.low);
    write(address+1, sp.high);
}

void Cpu::inc_r16(uint8_t r_key){
    //std::cout << "inc r16" << std::endl;
    reg16[r_key]->full = reg16[r_key]->full+1;
    m_cycle+=1;
}
void Cpu::dec_r16(uint8_t r_key){
    //std::cout << "dec r16" << std::endl;
    reg16[r_key]->full = reg16[r_key]->full-1;
    m_cycle+=1;
}
void Cpu::add_hl_from_r16(uint8_t operand){
    //std::cout << "add hl, r16" << std::endl;
    uint16_t op = reg16[operand]->full;
    uint8_t &h = reg16[operand]->high;
    uint8_t &l = reg16[operand]->low;
    (((h <<8 | l) + op)>255) ? set_c(1) : set_c(0);
    ((((h <<8 | l)& 0xFFF + op & 0xFFF)&0x1000) == 0x1000) ? set_h(1): set_h(0);
    set_n(0);

    hl.full = op;
    m_cycle+=1;

}

void Cpu::inc_r8(uint8_t key){
    //std::cout << "inc r8" << std::endl;
    set_h(((((*reg8[key] & 0xF)+ (0x1)) & 0x10)== 0x10) ? 1 : 0);
    *reg8[key] = *reg8[key]+1;
    set_z(*reg8[key]==0 ? 1:0);
    set_n(0);
}
void Cpu::dec_r8(uint8_t key){
    //std::cout << "dec r8" << std::endl;
    set_half_if_overflow_8(*reg8[key], 0x1);
    *reg8[key] = *reg8[key]-1;
    set_z( (*reg8[key]==0)? 1:0);
    set_n(1);
}

void Cpu::ld_r8_n8(uint8_t key){
    //std::cout << "ld r8, imm8" << std::endl;
    uint8_t instruction = read();
    *reg8[key] = instruction;
} //ld r8, imm8
void Cpu::rlca(){
    //std::cout << "rlca" << std::endl;
    //rotate to left. msb is added to carry.
    //flag set z=0, n=0, h=0, c: set according to result.
    uint8_t carry = (af.low>> 4) & 0x1; //flag
    af.high = af.high << 1 | carry&0x1;
    set_z(0);
    set_n(0);
    set_h(0);
    set_c(carry);

}
void Cpu::rrca(){
    //std::cout << "rrca" << std::endl;
    uint8_t carry = (af.low>> 4) & 0x1; //flag
    af.high = carry<<7 & 0xFF |af.high >> 1;
    set_z(0);
    set_n(0);
    set_h(0);
    set_c(carry);
}
void Cpu::rla(){
    //std::cout << "rla" << std::endl;

    uint8_t new_carry = (af.low>> 4) & 0x1; //flag
    af.high = af.high << 1 | (af.low >>4) & 0x1;
    set_z(0);
    set_n(0);
    set_h(0);
    set_c(new_carry);
                    

}
void Cpu::rra(){
    //std::cout << "rra" << std::endl;
                    uint8_t new_carry = (af.low>> 4) & 0x1; //flag
                    af.high = (af.low <<3 & 0x80) |af.high >> 1;
                    set_z(0);
                    set_n(0);
                    set_h(0);
                    set_c(new_carry);
}
void Cpu::daa(){} //todo

void Cpu::cpl(){
    //std::cout << "cpl" << std::endl;
                    //complement accuulator
                    af.high = ~af.high;
                    set_n(1);
                    set_h(1);
}
void Cpu::scf(){

    //std::cout << "scf: set carry flag" << std::endl;
                    //set carry flag
                    set_c(1);
}
void Cpu::ccf(){
    //std::cout << "ccf" << std::endl;
                    af.low = af.low ^ 0x80; //toggles c flag.
                    set_n(0);
                    set_h(0);
}

void Cpu::jr_n8(){
    //TODO: signed? fix this.
    int8_t offset = read();
    pc+=offset;

    m_cycle+=1;
}

void Cpu::jr_cc_n8(uint8_t condition){
    //std::cout << "jr cond, imm8" << std::endl;
    int8_t imm8 = read();
    switch (condition){
                    case 0:{
                        //if z==0
                        if ((af.low& 0x80)==0){
                            pc+=imm8;
                            m_cycle+=1;
                        }
                        break;
                    }
                    case 1:{
                        //if z==1
                        if ((af.low& 0x80)==1){
                            pc+=imm8;
                            m_cycle+=1;
                        }
                        break;
                    }
                    case 2:{
                        //if c==0
                        if ((af.low& 0x10)==0){
                            //jump
                            pc+=imm8;
                            m_cycle+=1;
                        }
                    }
                        break;
                    case 3:{
                        if ((af.low& 0x10)==1){
                            //jump
                            pc+=imm8;
                            m_cycle+=1;
                        }
                    }
                        break;
                }
}


        void stop();

//block 1

void Cpu::ld_r8_r8(uint8_t dest, uint8_t src){
    //std::cout << "ld r8, r8" << std::endl;
     *reg8[dest] = *reg8[src];
}
        void Cpu::halt(){}

//block 2

void Cpu::add(uint8_t &destination, uint8_t &operand){

    set_half_if_overflow_8(destination, *reg8[operand]);
    set_carry_if_overflow_8(destination, *reg8[operand]);

    destination = destination+ *reg8[operand];

    set_z((destination==0)? 1: 0);
    set_n(0);

} //add a, r8

void Cpu::adc(uint8_t &destination, uint8_t &operand){
    set_half_if_overflow_8(destination, *reg8[operand],  (af.low & 0x10)); //include carry bit
    set_carry_if_overflow_8(destination, *reg8[operand], (af.low &0x10));
    destination = destination+ *reg8[operand] + (af.low & 0x10); //+ carry flag
    //TODO: if 0, 0, if overflow from bit3, if overflow from bit7;
    set_z((destination==0)? 1: 0);
    set_n(0);

} //add a, r8
void Cpu::sub(uint8_t &destination, uint8_t &operand){
    set_half_if_borrow(destination, *reg8[operand]);
    set_carry_if_borrow(destination, *reg8[operand]);
    destination = destination - *reg8[operand];
    
    set_z((destination==0)? 1: 0);
    set_n(1);

} //add a, r8
void Cpu::sbc(uint8_t &destination, uint8_t &operand){
    uint8_t &f = af.low;
    set_half_if_borrow(destination, *reg8[operand], (f&0x10));
    set_carry_if_borrow(destination, *reg8[operand], (f&0x10));
    destination = destination - *reg8[operand] - (f & 0x10);
    set_z((destination==0)? 1: 0);
    set_n(1);

}//add a, r8
void Cpu::opcode_and(uint8_t &destination, uint8_t &operand){
    destination = destination & *reg8[operand];
            
    set_z((destination==0)? 1: 0);
    set_n(0);
    set_h(1);
    set_c(0);
} //add a, r8
void Cpu::opcode_xor(uint8_t &destination, uint8_t &operand){

    destination = destination ^ *reg8[operand];
    //TODO: set flag 0 if zero, 0, 0, 0;
    set_z((destination==0)? 1: 0);
    set_n(0);
    set_h(0);
    set_c(0);
} //add a, r8
void Cpu::opcode_or(uint8_t &destination, uint8_t &operand){
    destination = destination | *reg8[operand];
    //TODO: set flag Z IF 0, 0 0 0
    set_z((destination==0)? 1: 0);
    set_n(0);
    set_h(0);
    set_c(0);
}//add a, r8
void Cpu::cp(uint8_t &destination, uint8_t &operand){
    //compare
    //flag setting instruction
    set_half_if_borrow(destination, *reg8[operand]);
    set_carry_if_borrow(destination, *reg8[operand]);
    set_z((destination==0)? 1: 0);
    set_n(1);
} //add a, r8


//block 3
void Cpu::add(uint8_t &destination){
    uint8_t imm8 = read();

    set_half_if_overflow_8(destination, imm8);
    set_carry_if_overflow_8(destination, imm8);

    destination = destination+imm8;

    set_z((destination==0)? 1: 0);
    set_n(0);
} //add a, imm8
void Cpu::adc(uint8_t &destination){
    uint8_t &f = af.low;
    uint8_t imm8 = read();
    set_half_if_overflow_8(destination, imm8,  (f & 0x10)); //include carry bit
    set_carry_if_overflow_8(destination, imm8, (f&0x10));

    destination = destination+ imm8 + (f & 0x10); //+ carry flag
    //TODO: if 0, 0, if overflow from bit3, if overflow from bit7;
    set_z((destination==0)? 1: 0);
    set_n(0);
} //add a, imm8
void Cpu::sub(uint8_t &destination){
    uint8_t imm8 = read();

    set_half_if_borrow(destination, imm8);
    set_carry_if_borrow(destination, imm8);
    destination = destination - imm8;
    
    set_z((destination==0)? 1: 0);
    set_n(1);
} //add a, imm8
void Cpu::sbc(uint8_t &destination){
    uint8_t imm8 = read();
    uint8_t &f = af.low;
    set_half_if_borrow(destination, imm8, (f&0x10));
    set_carry_if_borrow(destination, imm8, (f&0x10));

    destination = destination - imm8 - (f & 0x10);

    set_z((destination==0)? 1: 0);
    set_n(1);
} //add a, imm8
void Cpu::opcode_and(uint8_t &destination){
    uint8_t imm8 = read();
    destination = destination & imm8;
    set_z((destination==0)? 1: 0);
    set_n(0);
    set_h(1);
    set_c(0);
} //add a, imm8
void Cpu::opcode_xor(uint8_t &destination){
    uint8_t imm8 = read();
    destination = destination ^ imm8;

    set_z((destination==0)? 1: 0);
    set_n(0);
    set_h(0);
    set_c(0);

} //add a, imm8
void Cpu::opcode_or(uint8_t &destination){
    uint8_t imm8 = read();
    destination = destination | imm8;
    
    set_z((destination==0)? 1: 0);
    set_n(0);
    set_h(0);
    set_c(0);
} //add a, imm8
void Cpu::cp(uint8_t &destination){
    uint8_t imm8 = read();
    set_half_if_borrow(destination, imm8);
    set_carry_if_borrow(destination, imm8);
    set_z((destination==0)? 1: 0);
    set_n(1);

} //add a, imm8


void Cpu::ret_cond(uint8_t condition){
    uint8_t &f = af.low;
    switch (condition){
        case 0:
            //if z==0
            if ((f& 0x80)==0){
                //jump
                pc = (ram->memory[sp.full+1]<<8) | ram->memory[sp.full]; //hi+lo
                sp.full+=2; //pop up
                m_cycle+=4; //2 bytes + internal set pc
            }
            break;
        case 1:
            //if z==1
            if ((f& 0x80)==1){
                //jump
                pc = (ram->memory[sp.full+1]<<8) | ram->memory[sp.full]; //hi+lo
                sp.full+=2; //pop up
                m_cycle+=4; //2 bytes + internal set pc
            }
            break;
        case 2:
            //if c==0
            if ((f& 0x10)==0){
                //jump
                pc = (ram->memory[sp.full+1]<<8) | ram->memory[sp.full]; //hi+lo
                sp.full+=2; //pop up
                m_cycle+=4; //2 bytes + internal set pc

            }
            break;
        case 3:
            if ((f& 0x10)==1){
                //jump
                pc = (ram->memory[sp.full+1]<<8) | ram->memory[sp.full]; //hi+lo
                sp.full+=2; //pop up
                m_cycle+=4; //2 bytes + internal set pc
            }
            break;
        default:
            m_cycle+=1; //internal branch decision
            break;
    }
}

void Cpu::ret(){
    pc = (ram->memory[sp.full+1]<<8) | ram->memory[sp.full]; //hi+lo
    sp.full+=2; //pop up
    m_cycle+=3; //2 bytes + internal set pc

}

void Cpu::reti(){
    ei();
    ret();
}

void Cpu::jp_cond(uint8_t cond){
    uint8_t &f = af.low;

    uint8_t low = read();
    uint8_t hi = read();
    uint16_t jump = (hi << 8) | low;
    switch (cond){
        case 0:
            //if z==0
            if ((f& 0x80)==0){
                pc =jump;
                m_cycle+=1;
            }
            break;
        case 1:
            //if z==1
            if ((f& 0x80)==1){
                pc =jump;
                m_cycle+=1;
            }
            break;
        case 2: //if c==0
            if ((f& 0x10)==0){
                pc =jump;
                m_cycle+=1;
            }
            break;
        case 3:
            if ((f& 0x10)==1){
                pc =jump;
                m_cycle+=1;
            }
            break;
    }
}

void Cpu::jp(){
    uint8_t low = read();
    uint8_t hi = read();
    uint16_t jump = (hi << 8) | low;
    pc = jump;
    m_cycle+=1;
}


void Cpu::jp_hl(){
    pc = hl.full;
}

void Cpu::call_cond(uint8_t cond){
    uint8_t &f = af.low;
    std::cout << "f" << (signed)f << std::endl;
    switch (cond){
        case 0:
            //if z==0
            if ((f& 0x80)==0){
                call(); 
            }
            else {
                m_cycle+=2;
                pc+=2;
            }
                
            break;
        case 1:
            //if z==1
            if ((f& 0x80)==1){
                call();
            }
            else {
                m_cycle+=2;
                pc+=2;
            }
            break;
        case 2:
            //if c==0
            if ((f& 0x10)==0){
                call();
            }
            else {
                m_cycle+=2;
                pc+=2;
            }
            break;
        case 3:
            if ((f& 0x10)==1){
                call();
            }
            else {
                m_cycle+=2;
                pc+=2;
            }
    }
//todo
}

void Cpu::call(uint16_t address){
    sp.full-=1;
    write(sp.full, ((pc>>8) & 0xFF) );
    sp.full-=1;
    write(sp.full,(pc & 0xFF));
    pc = address;
    m_cycle+=1;

}
void Cpu::call(){
    std::cout << "calling" << std::endl;
    uint8_t lo = read();
    uint8_t hi = read();

    sp.full-=1;
    write(sp.full, ((pc>>8) & 0xFF) );
    sp.full-=1;
    write(sp.full,(pc & 0xFF));

    //jump
    pc = (hi <<8)| lo;
    m_cycle+=1; //16 bits twice. 
    std::cout << "sp and pc: " << (signed)sp.full << " " << (signed)pc << std::endl;
 }

void Cpu::rst(uint8_t target){
    call(vec[target]);
    return;

    sp.full-=1;
    write(sp.full, ((pc>>8) & 0xFF) );
    sp.full-=1;
    write(sp.full,(pc & 0xFF));

    pc = vec[target];
    m_cycle+=1;

}

void Cpu::pop(uint8_t &key){
    uint8_t &f = af.low;
    if (key==0x3){ 

        af.low = ram->memory[sp.full];
        sp.full +=1;
        af.high = ram->memory[sp.full];
        sp.full +=1;
        //flags
        set_z(f>>7 & 0x1); set_n(f>>6 & 0x1); set_h(f>>5 & 0x1); set_c(f>>4 & 0x1);
    }
    else{
        reg16[key]->low = ram->memory[sp.full]; //LD reg_low, sp
        sp.full+=1;//decrement
        reg16[key]->high = ram->memory[sp.full]; //LD reg_hi, sp
        sp.full+=1;//decrement
        //no flags for some reason
    }
    
    m_cycle+=2;

}
void Cpu::push(uint8_t &key){
    if (key==0x3){ //push af
        sp.full-=1;
        write(sp.full, af.high);
        sp.full-=1;
        write(sp.full, af.low);
        m_cycle+=1;
    }
    sp.full-=1;
    write(sp.full, reg16[key]->high); 
    sp.full-=1;
    write(sp.full, reg16[key]->low);
    m_cycle+=1;
}
        void prefix(){}

void Cpu::ldh_to_c(uint8_t &a){
    write(0xFF00+bc.low, a);

} //from a

void Cpu::ldh_to_n8(uint8_t &a){ //e0
    uint8_t imm8 = read(); //unsigned
    write((0xFF00 | imm8), a);
    
} //from a
void Cpu::ld_to_n16(uint8_t &a){
    uint8_t imm_low = read();
    uint8_t imm_hi = read();
    write((imm_hi<<8)+imm_low, a);

} //from a

void Cpu::ldh_from_c(uint8_t &a){
    a = read(0xFF00 + bc.low);
}

void Cpu::ldh_from_n8(uint8_t &a){
    uint8_t imm8 = read();
    uint8_t val = read(0xFF00|imm8);
    a = val;
} //to a

void Cpu::ld_from_n16(uint8_t &a){
    uint8_t low = read();
    uint8_t hi = read();
    a = read((hi << 8)|low);

}//to a
        
void Cpu::add_sp_from_n8(){
    int8_t imm8 = read(); //signed
    set_half_if_overflow_8(sp.low, imm8);
    set_carry_if_overflow_8(sp.low, imm8); //weird, since sp is 16 bits.
    sp.full = sp.full + imm8;
    set_z(0);
    set_n(0);

    m_cycle+=2; //4th cycle. its because sp is 2 bytes. 
}
void Cpu::ld_hl_sp_and_n8(){
    int8_t imm8 = read();

    set_half_if_overflow_8(sp.low, imm8);
    set_carry_if_overflow_8(sp.low, imm8); //might need to switch
    set_z(0);
    set_n(0);

    sp.full = sp.full + imm8;
    hl.full = sp.full;

    m_cycle+=1;

}
void Cpu::ld_sp_from_hl(){
    sp.full = hl.full;
    m_cycle+=1;
}

void Cpu::di(){
    ime = 0;
}

void Cpu::ei(){
    ime_hold =2 ;
    //only set after 
}

uint8_t& Cpu::get_r8_hl(){
    uint16_t address = (hl.high << 7 | hl.low);
    m_cycle+=2;
    return ram->memory[address];
    

}

//unused
void Cpu::set_r8_hl(uint8_t val){
    uint16_t address = (hl.high << 7 | hl.low);
    ram->memory[address] = val;
}

void Cpu::rlc(uint8_t &key){
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);

    uint8_t msb = (r8 >> 7) & 0x1; //msb

    r8 = (r8<<1)| msb;

    set_z(r8);
    set_n(0);
    set_h(0);
    set_c(msb);

    m_cycle+=1;

}

void Cpu::rrc(uint8_t &key){
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);

    uint8_t lsb = r8 & 0x1;

    r8 = (lsb<<7) | (r8>>1); 

    set_z(r8);
    set_n(0);
    set_h(0);
    set_c(lsb);
    m_cycle+=1;

}

void Cpu::rl(uint8_t &key){
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);
    uint8_t carry = (r8 >> 7) & 0x1; //flag

    r8 = (r8<<1) | carry;

    set_z(r8);
    set_n(0);
    set_h(0);
    set_c(carry);
    m_cycle+=1;
}

void Cpu::rr(uint8_t &key){
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);

    uint8_t carry = (r8) & 0x1; //flag

    r8 = (carry << 7) | (r8>>1);
    set_z(r8);
    set_n(0);
    set_h(0);
    set_c(carry);
    m_cycle+=1;

}

void Cpu::sla(uint8_t &key){
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);
    
    uint8_t carry = (r8 >>7) * 0x1;

    r8 = r8<<1;
    set_z(r8);
    set_n(0);
    set_h(0);
    set_c(carry);
    m_cycle+=1;

}
void Cpu::sra(uint8_t &key){
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);
    
    uint8_t carry = r8 & 0x1;
    uint8_t msb = (r8 >> 7)  & 0x1;
    r8 = r8 & 0x1;
    r8 = (msb<<7) | (r8>>1);
    set_z(r8);
    set_n(0);
    set_h(0);
    set_c(carry);
    m_cycle+=1;
}

void Cpu::swap(uint8_t &key){
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);

    uint8_t low = r8 & 0xF;
    r8 = (low<<4) | (r8 >>4);
    set_z(r8);
    set_n(0);
    set_h(0);
    set_c(0);
    m_cycle+=1;

}

void Cpu::srl(uint8_t &key){
    //shift right logically register r8
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);

    uint8_t carry = r8 * 0x1;
    r8 = r8>>1;
    set_z(r8);
    set_n(0);
    set_h(0);
    set_c(carry);
    m_cycle+=1;

}

void Cpu::bit(uint8_t &bit_index, uint8_t &op_key){
    uint8_t& r8 = ((op_key==0x6) ? get_r8_hl() : *reg8[op_key]);

    uint8_t bit = (r8 >> bit_index) &0x1;
    set_z(bit);
    set_n(0);
    set_h(1);
    m_cycle+=1;


}

void Cpu::res(uint8_t &bit_index, uint8_t &op_key){
    uint8_t& r8 = ((op_key==0x6) ? get_r8_hl() : *reg8[op_key]);
    r8 = (r8) & ~(0x1 << bit_index); 
    m_cycle+=1;
}
        
void Cpu::set(uint8_t &bit_index, uint8_t &op_key){
    uint8_t& r8 = ((op_key==0x6) ? get_r8_hl() : *reg8[op_key]);
    r8 = (r8) | (0x1 << bit_index);
    m_cycle+=1;
}

