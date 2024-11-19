#include "cpu.h"

#include <iostream>
void Cpu::nop(){
    //nothing happens here lil bro

}

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
                    write(reg16[2]->full, reg); //hl+
                    reg16[2]->full++;
                    break;
        case 3:
                    write(reg16[2]->full, reg); //hl-
                    reg16[2]->full--;
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
                    reg = read(reg16[2]->full); //hl+
                    reg16[2]->full+=1;
                    break;
        case 3:
                    reg = read(reg16[2]->full); //hl-
                    reg16[2]->full-=1;
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
    
    // if (r_key == 0x2){
    //     uint8_t& mem = get_r8_hl();
    //     mem = mem + 1;
    //     return;
    // }

    reg16[r_key]->full = reg16[r_key]->full+1;
    tick();;
}
void Cpu::dec_r16(uint8_t r_key){
    //std::cout << "dec r16" << std::endl;
    reg16[r_key]->full = reg16[r_key]->full-1;
    tick();;
}
void Cpu::add_hl_from_r16(uint8_t operand){
    //std::cout << "add hl, r16" << std::endl;
    uint16_t op = reg16[operand]->full;
    
    set_carry_if_overflow_16(hl.full, op, (af.low & 0x10)>>4);
    set_half_if_overflow_16(hl.full, op, (af.low & 0x20)>>5);
    hl.full +=op;
    
    set_n(0);

    tick();;

}

void Cpu::inc_r8(uint8_t key){
    //std::cout << "inc r8" << std::endl;
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);


    set_h(((((r8 & 0xF)+ (0x1)) & 0x10)== 0x10) ? 1 : 0);

    
    r8 = r8+1;

    set_z(r8==0 ? 1:0);
    set_n(0);
}
void Cpu::dec_r8(uint8_t key){
    //std::cout << "dec r8" << std::endl;
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);

    //set_half_if_overflow_8(*reg8[key], 0x1);
    set_half_if_borrow(r8, 1, 0);
    r8 = r8-1;
    set_z( (r8==0)? 1:0);
    set_n(1);
}

void Cpu::ld_r8_n8(uint8_t key){
    //std::cout << "ld r8, imm8" << std::endl;
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);

    uint8_t instruction = read();
    r8 = instruction;
} //ld r8, imm8
void Cpu::rlca(){
    //std::cout << "rlca" << std::endl;
    //rotate to left. msb is added to carry.
    //flag set z=0, n=0, h=0, c: set according to result.
    uint8_t new_carry = (af.high >> 7) & 0x1; //flag
    af.high = af.high << 1 | new_carry;
    set_z(0);
    set_n(0);
    set_h(0);
    set_c(new_carry);

}
void Cpu::rrca(){
    //std::cout << "rrca" << std::endl;
    uint8_t new_carry = (af.high) & 0x1; //flag
    af.high = new_carry<<7 |af.high >> 1;
    set_z(0);
    set_n(0);
    set_h(0);
    set_c(new_carry);
}
void Cpu::rla(){
    //std::cout << "rla" << std::endl;

    uint8_t new_carry = af.high >> 7 & 0x1;
    //uint8_t new_carry = (af.low>> 4) & 0x1; //flag
    af.high = af.high << 1 | (af.low >>4) & 0x1;
    set_z(0);
    set_n(0);
    set_h(0);
    set_c(new_carry);
                    

}
void Cpu::rra(){
    uint8_t new_carry = af.high & 0x01;
                    //uint8_t new_carry = (af.low>> 4) & 0x1; //flag
    af.high = ((af.low & 0x10) << 3) |(af.high >> 1);
    
    set_z(0);
    set_n(0);
    set_h(0);
    set_c(new_carry);
}
void Cpu::daa(){
    if ((af.low & 0x40)>0){ //if n=1
         if ((af.low & 0x10)>0){
            af.high -= 0x60;
         }
         if ((af.low & 0x20) > 0){
            af.high -= 6;
         }
    }
    else{ //n=0
        if (((af.low & 0x20)>0)||((af.high & 0xF) > 0x9)){

            af.high = af.high + 6;
        //std::cout << signed(af.full) << std::endl;
        }
        if (((af.high & 0xF0) >> 4) > 9 || (af.low & 0x10)>0)
        {
            af.high = af.high+=0x60;
            set_c(1);
        //af.full +=0x60;
        }
         
    }
    set_z((af.high==0) ? 1 : 0);
    set_h(0);
} 

void Cpu::cpl(){
    //std::cout << "cpl" << std::endl;
                    //complement accuulator
                    af.high = ~af.high;
                    //std::cout << "a: 0b" << std::bitset<8>(af.high)<< std::endl;
                    set_n(1);
                    set_h(1);
}
void Cpu::scf(){

    //std::cout << "scf: set carry flag" << std::endl;
                    //set carry flag
                    set_n(0);
                    set_h(0);
                    set_c(1);
}
void Cpu::ccf(){
    //std::cout << "ccf" << std::endl;
                    af.low = af.low ^ 0x10; //toggles c flag.
                    set_n(0);
                    set_h(0);
}

void Cpu::jr_n8(){
    int8_t offset = read();
    pc+=offset;

    tick();;
}

void Cpu::jr_cc_n8(uint8_t condition){
    //std::cout << "jr cond, imm8" << std::endl;
    int8_t imm8 = read();
    //std::cout << "pc : 0d" << std::dec << (signed)pc << std::endl;
    //std::cout <<"imm8: 0b" <<  std::bitset<8>((signed)imm8) << std::endl;

    //std::cout <<"imm8: 0d" <<  std::dec << (signed)imm8 << std::endl;
    switch (condition){
                    case 0:{
                        //if z==0
                        if ((af.low& 0x80)==0){
                            pc+=imm8;
                            tick();;
                        }
                        break;
                    }
                    case 1:{
                        //if z==1
                        if ((af.low& 0x80) > 0){
                            pc+=(signed)imm8;
                            tick();;
                            //std::cout << "yes in"<<std::endl;
                            //std::cout << "pc: 0d" << std::dec << (signed)pc << std::endl;
                            //abort();
                        }

                        //std::cout << "pc: 0d" << std::dec << (signed)pc << std::endl;
                        //abort();
                        break;
                    }
                    case 2:{
                        //if c==0
                        if ((af.low& 0x10)==0){
                            //jump
                            pc+=imm8;
                            tick();;

                        }
                        break;
                    }
                        
                    case 3:{
                        if ((af.low& 0x10)>0){
                            //jump
                            pc+=imm8;
                            tick();;
                        }
                        break;
                    }
                       
                }
}

//todo
void Cpu::stop(){

}

//block 1

void Cpu::ld_r8_r8(uint8_t dest, uint8_t src){
    //std::cout << "ld r8, r8" << std::endl;
    uint8_t& r8_dest = ((dest==0x6) ? get_r8_hl() : *reg8[dest]);
    uint8_t& r8_src = ((src==0x6) ? get_r8_hl() : *reg8[src]);
    
    r8_dest = r8_src;
    //*reg8[dest] = *reg8[src];
}

//TODO
void Cpu::halt(){}

//block 2

void Cpu::add(uint8_t &destination, uint8_t &operand){
    uint8_t& r8 = ((operand==0x6) ? get_r8_hl() : *reg8[operand]);


    set_half_if_overflow_8(destination, r8);
    set_carry_if_overflow_8(destination, r8);

    destination = destination+ r8;

    set_z((destination==0)? 1: 0);
    set_n(0);

} //add a, r8

void Cpu::adc(uint8_t &destination, uint8_t &operand){
    uint8_t& r8 = ((operand==0x6) ? get_r8_hl() : *reg8[operand]);
    
    //std::cout << "a: 0d" << std::dec << (signed)destination << "  r8: 0d" << std::dec << (signed)r8 << std::endl;
    
    uint8_t old = destination;
    uint8_t old_op = r8;
    destination = destination + r8 + ((af.low & 0x10)>>4); //+ carry flag
    //std::cout << "new a: 0d" << std::dec << (signed)destination << std::endl;
    set_half_if_overflow_8(old, old_op,  (af.low & 0x10)>>4); //include carry bit
    set_carry_if_overflow_8(old, old_op, (af.low &0x10)>>4);
    
    set_z((destination==0)? 1: 0);
    set_n(0);

    //std::cout << "a: 0d" << std::dec << (signed)destination << std::endl;
    //std::cout << "f: 0d" << std::dec << (signed)af.low << std::endl;

    //abort();

} //add a, r8
void Cpu::sub(uint8_t &destination, uint8_t &operand){
    uint8_t& r8 = ((operand==0x6) ? get_r8_hl() : *reg8[operand]);

    set_half_if_borrow(destination, r8);
    set_carry_if_borrow(destination, r8);
    destination = destination - r8;
    
    set_z((destination==0)? 1: 0);
    set_n(1);

} //add a, r8
void Cpu::sbc(uint8_t &destination, uint8_t &operand){
    uint8_t &f = af.low;
    uint8_t& r8 = ((operand==0x6) ? get_r8_hl() : *reg8[operand]);
    uint8_t old = destination;
    uint8_t old_op= r8;

    


    //std::cout << "a: " << std::dec <<(signed)destination << "reg:  " << std::dec <<(signed)*reg8[operand]<< "carry flag"<<  std::dec << (signed)(f&0x10) << std::endl;
    destination = destination - r8 - ((f & 0x10)>>4);
    //std::cout << "new a: " << std::dec <<(signed)destination << std::endl;
    
    set_half_if_borrow(old, old_op, (f&0x10)>>4);
    set_carry_if_borrow(old, old_op, (f&0x10)>>4); //should be zero for ba 5e
    
    set_z((destination==0)? 1: 0);
    set_n(1);

}//add a, r8
void Cpu::opcode_and(uint8_t &destination, uint8_t &operand){
    uint8_t& r8 = ((operand==0x6) ? get_r8_hl() : *reg8[operand]);

    destination = destination & r8;
            
    set_z((destination==0)? 1: 0);
    set_n(0);
    set_h(1);
    set_c(0);
} //add a, r8
void Cpu::opcode_xor(uint8_t &destination, uint8_t &operand){
    uint8_t& r8 = ((operand==0x6) ? get_r8_hl() : *reg8[operand]);

    destination = destination ^ r8;
    
    set_z((destination==0)? 1: 0);
    set_n(0);
    set_h(0);
    set_c(0);
} //add a, r8
void Cpu::opcode_or(uint8_t &destination, uint8_t &operand){
    uint8_t& r8 = ((operand==0x6) ? get_r8_hl() : *reg8[operand]);
    
    destination = destination | r8;
    
    set_z((destination==0)? 1: 0);
    set_n(0);
    set_h(0);
    set_c(0);
}//add a, r8

void Cpu::cp(uint8_t &destination, uint8_t &operand){
    //compare
    //flag setting instruction
    
    uint8_t& r8 = ((operand==0x6) ? get_r8_hl() : *reg8[operand]);

    set_half_if_borrow(destination, r8);
    set_carry_if_borrow(destination, r8);
    set_z(((destination-r8)==0)? 1: 0);
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
    uint8_t old = destination;
    destination = destination+ imm8 + ((f & 0x10)>>4); //+ carry flag
    set_half_if_overflow_8(old, imm8,  (f & 0x10)>>4); //include carry bit
    set_carry_if_overflow_8(old, imm8, (f&0x10)>>4);

    
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
    
    uint8_t original = destination;

    destination = destination - imm8 - ((f>>4) & 0x1);

    set_half_if_borrow(original, imm8, ((f>>4) & 0x1));
    set_carry_if_borrow(original, imm8, ((f>>4) & 0x1));
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
    set_z(((destination-imm8)==0)? 1: 0);
    set_n(1);

} //add a, imm8


void Cpu::ret_cond(uint8_t condition){
    uint8_t &f = af.low;
    //std::cout << "cond: 0d" << std::dec << (signed)condition << std::endl;

    switch (condition){
        case 0:
            //abort();
            //if z==0
            //std::cout << "case 0" << std::endl;
            if (((f& 0x80) >> 7)==0){
                //jump
                pc = (ram->memory[sp.full+1]<<8) | ram->memory[sp.full]; //hi+lo
                sp.full+=2; //pop up
                 //2 bytes + internal set pc
                tick();
                tick();
                tick();
                tick();
            }
            break;
        case 1:
            //if z==1
            
            if (((f& 0x80)>> 7)==1){
                //jump
                pc = (ram->memory[sp.full+1]<<8) | ram->memory[sp.full]; //hi+lo
                sp.full+=2; //pop up
                
                tick();
                tick();
                tick();
                tick();
            }
            break;
        case 2:
            //if c==0
            
            if (((f >> 4 )& 0x1)==0){
                //jump
                pc = (ram->memory[sp.full+1]<<8) | ram->memory[sp.full]; //hi+lo
                sp.full+=2; //pop up
                
                tick();
                tick();
                tick();
                tick();
                

            }
            break;
        case 3:
            
            
            if (((f>> 4)& 0x1)==1){
                //jump
                
                pc = ((ram->memory[sp.full+1]<<8) | ram->memory[sp.full]); //hi+lo
                sp.full+=2; //pop up
               
                tick();
                tick();
                tick();
                tick();
                
            }
            break;
        default:
            
            tick();; //internal branch decision
            break;
    }
}

void Cpu::ret(){
    pc = (ram->memory[sp.full+1]<<8) | ram->memory[sp.full]; //hi+lo
    
    sp.full+=2; //pop up
    
    tick();

    tick();

    tick();


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
                tick();;
            }
            break;
        case 1:
            //if z==1
            if ((f& 0x80)>0){
                pc =jump;
                tick();;
            }
            break;
        case 2: //if c==0
            if ((f& 0x10)==0){
                pc =jump;
                tick();;
            }
            break;
        case 3:
            if ((f& 0x10)>0){
                pc =jump;
                tick();;
            }
            break;
    }
}

void Cpu::jp(){
    uint8_t low = read();
    uint8_t hi = read();
    uint16_t jump = (hi << 8) | low;
    pc = jump;
    tick();;
}


void Cpu::jp_hl(){
    pc = hl.full;
}

void Cpu::call_cond(uint8_t cond){
    uint8_t &f = af.low;
    //std::cout << "f" << (signed)f << std::endl;
    switch (cond){
        case 0:
            //if z==0
            if (((f& 0x80)>>7)==0){
                call(); 
            }
            else {
                tick();
                tick();

                pc+=2;
            }
                
            break;
        case 1:
            //if z==1
            if (((f& 0x80)>>7)==1){
                call();
            }
            else {
                tick();
                tick();

                pc+=2;
            }
            break;
        case 2:
            //if c==0
            if (((f& 0x10)>>4)==0){
                call();
            }
            else {
                //todo: what is the order here? should pc+2 be in a function instead?
                tick();
                tick();

                pc+=2;
            }
            break;
        case 3:
            if (((f& 0x10)>>4)==1){
                call();
            }
            else {
                tick();
                tick();

                pc+=2;
            }
    }
}

void Cpu::call(uint16_t address){
    sp.full-=1;
    write(sp.full, ((pc>>8) & 0xFF) );
    sp.full-=1;
    write(sp.full,(pc & 0xFF));
    pc = address;
    tick();;

}
void Cpu::call(){
    //std::cout << "calling" << std::endl;
    uint8_t lo = read();
    uint8_t hi = read();

    sp.full-=1;
    write(sp.full, ((pc>>8) & 0xFF) );
    sp.full-=1;
    write(sp.full,(pc & 0xFF));

    //jump
    pc = (hi <<8)| lo;
    tick();; //16 bits twice. 
    //std::cout << "sp and pc: " << (signed)sp.full << " " << (signed)pc << std::endl;
 }

void Cpu::rst(uint8_t target){
    call(vec[target]);
    return;

    sp.full-=1;
    write(sp.full, ((pc>>8) & 0xFF) );
    sp.full-=1;
    write(sp.full,(pc & 0xFF));

    pc = vec[target];
    tick();;

}

void Cpu::pop(uint8_t &key){
    uint8_t &f = af.low;
    if (key==0x3){ 

        af.low = ram->memory[sp.full] & 0xF0;
        sp.full +=1;
        af.high = ram->memory[sp.full];
        sp.full +=1;
        //flags
        //set_z(f>>7 & 0x1); set_n(f>>6 & 0x1); set_h(f>>5 & 0x1); set_c(f>>4 & 0x1);
        
    }
    else{
        reg16[key]->low = ram->memory[sp.full]; //LD reg_low, sp
        sp.full+=1;//decrement
        reg16[key]->high = ram->memory[sp.full]; //LD reg_hi, sp
        sp.full+=1;//decrement
        //no flags for some reason
    }
    
    tick();
    tick();


}
void Cpu::push(uint8_t &key){
    if (key==0x3){ //push af
        sp.full-=1;
        write(sp.full, af.high);
        sp.full-=1;
        write(sp.full, af.low);
        tick();;
        return;
    }
    sp.full-=1;
    write(sp.full, reg16[key]->high); 
    sp.full-=1;
    write(sp.full, reg16[key]->low);
    tick();;
}


void Cpu::ldh_to_c(uint8_t &a){
    write(0xFF00+bc.low, a);
}

void Cpu::ldh_to_n8(uint8_t &a){ //e0
    uint8_t imm8 = read(); //unsigned
    write((0xFF00 | imm8), a);  
} 

void Cpu::ld_to_n16(uint8_t &a){
    uint8_t imm_low = read();
    uint8_t imm_hi = read();
    write((imm_hi<<8)+imm_low, a);
}

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

    tick();
    tick(); //4th cycle. its because sp is 2 bytes. 
}
void Cpu::ld_hl_sp_and_n8(){
    int8_t imm8 = read();

    set_half_if_overflow_8(sp.low, imm8);
    set_carry_if_overflow_8(sp.low, imm8); //might need to switch
    set_z(0);
    set_n(0);

    hl.full = sp.full + imm8;

    tick();;

}
void Cpu::ld_sp_from_hl(){
    sp.full = hl.full;
    tick();;
}

void Cpu::di(){
    ime = 0;
    ime_hold = 0;
}

void Cpu::ei(){
    ime_hold =2 ;
    //only set after 
}


void Cpu::rlc(uint8_t &key){
    std::cout << "rlc :key 0d" << std::dec << (signed)key << std::endl;
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);
 //h
    std::cout << "r8: 0d" << std::dec << (signed)r8 << std::endl;
    uint8_t msb = (r8 >> 7) & 0x1; //msb


    r8 = (r8<<1)| msb;
    std::cout << "r8 after: 0d" << std::dec << (signed)r8 << std::endl;


    set_z((r8==0) ? 1 : 0);
    set_n(0);
    set_h(0);
    set_c(msb);

    tick();;

}

void Cpu::rrc(uint8_t &key){
    std::cout << "rrc :key 0d" << std::dec << (signed)key << std::endl;

    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);

    uint8_t lsb = r8 & 0x1;

    r8 = (lsb<<7) | (r8>>1); 

    set_z((r8==0) ? 1 : 0);
    set_n(0);
    set_h(0);
    set_c(lsb);
    tick();;

}

void Cpu::rl(uint8_t &key){
    //std::cout << "rl r8" << std::endl;
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);
    uint8_t carry = (r8 >> 7) & 0x1; //flag
    //std::cout << "dec: 0d" << std::dec << (signed)carry << std::endl;
    //std::cout << "flag: 0d" << std::dec << (signed)(af.low & 0x10 >>4) << std::endl;
    r8 = (r8<<1) | ((af.low & 0x10) >> 4);

    set_z((r8==0) ? 1 : 0);
    set_n(0);
    set_h(0);
    set_c(carry);
    tick();;
}

void Cpu::rr(uint8_t &key){
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);

    uint8_t carry = (r8) & 0x1; //flag

    r8 = ((af.low >>4 &0x1) << 7) | (r8>>1);
    set_z((r8==0) ? 1 : 0);
    set_n(0);
    set_h(0);
    set_c(carry);
    tick();;

}

void Cpu::sla(uint8_t &key){
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);
    
    uint8_t carry = (r8 >>7) & 0x1;

    r8 = r8<<1;
    set_z((r8==0) ? 1 : 0);
    set_n(0);
    set_h(0);
    set_c(carry);
    tick();;

}
void Cpu::sra(uint8_t &key){
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);
    
    uint8_t carry = r8 & 0x1;
    uint8_t msb = r8 >>0x7 & 0x1;
    r8 = msb<<7 | (r8>>1);

    set_z((r8==0) ? 1 : 0);
    set_n(0);
    set_h(0);
    set_c(carry);
    tick();;
}

void Cpu::swap(uint8_t &key){
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);

    uint8_t low = r8 & 0xF;
    r8 = (low<<4) | (r8 >>4);
    set_z((r8==0) ? 1 : 0);
    set_n(0);
    set_h(0);
    set_c(0);
    tick();;
}

void Cpu::srl(uint8_t &key){
    //shift right logically register r8
    uint8_t& r8 = ((key==0x6) ? get_r8_hl() : *reg8[key]);

    uint8_t carry = r8 & 0x1;
    r8 = r8>>1;
    set_z((r8==0) ? 1 : 0);
    set_n(0);
    set_h(0);
    set_c(carry);
    tick();;
}

void Cpu::bit(uint8_t &bit_index, uint8_t &op_key){
    uint8_t& r8 = ((op_key==0x6) ? get_r8_hl() : *reg8[op_key]);

    uint8_t bit = (r8 >> bit_index) &0x1;
    set_z((bit==0) ? 1 : 0);
    set_n(0);
    set_h(1);
    tick();;
}

void Cpu::res(uint8_t &bit_index, uint8_t &op_key){
    uint8_t& r8 = ((op_key==0x6) ? get_r8_hl() : *reg8[op_key]);
    r8 = (r8) & ~(0x1 << bit_index); 
    tick();;
}
        
void Cpu::set(uint8_t &bit_index, uint8_t &op_key){
    uint8_t& r8 = ((op_key==0x6) ? get_r8_hl() : *reg8[op_key]);
    r8 = (r8) | (0x1 << bit_index);
    tick();;
}

