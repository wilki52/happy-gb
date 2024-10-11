#ifndef JSON_TEST_H
#define JSON_TEST_H


#include <stdint.h>
#include "Cpu.h"
#include "Memory.h"
//read json test
//sets memory and cpu with init

//tests cpu and mem with json.
//best practice??
class JsonTest{

    public:
        //JsonTest();
        JsonTest(Memory&, Cpu&);
        void read_json(); 
        void load_test(); 
        void compare_results();
    private:
         Memory* ram;
         Cpu* cpu;
};



#endif