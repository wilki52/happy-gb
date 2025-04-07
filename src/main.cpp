#include <iostream>
#include "happy-gb.h"
#include <stdint.h>
int main(){
    std::cout<< "hello cmake" << std::endl;

    Happy gb;
    
    gb.load_rom("../roms/tetris.gb");
    //gb.load_rom("../roms/doctor.gb");

    //gb.load_rom("../test_roms/cpu_instrs.gb");
    gb.run_program();
    //gb.run_tests();

    return 0;
};