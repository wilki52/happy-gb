#include <iostream>
#include "happy-gb.h"
#include <stdint.h>
int main(){
    std::cout<< "hello world" << std::endl;

    //Happy happygb;

    /*
    uint8_t a = 255;
    uint8_t b= 255;
    uint8_t c = a+b;
    //check a> 255-b
    std::cout << ((a+b)>255) << std::endl;
    */
    uint8_t d = 253;
    uint8_t e= 255;
    uint8_t f = d-e;
    //check a> 255-b
    std::cout << ((d-e)<0) << std::endl;
    
    //set i fborrow from bit 4 for halfcarry

    //set if borrow if r8> A, where 
    //A-r8. So set c if underflow basically.





    //happygb.decode(0x32);
    //happygb.decode(0x15);
    return 0;
};