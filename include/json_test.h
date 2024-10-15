#ifndef JSON_TEST_H
#define JSON_TEST_H

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <stdint.h>
#include "cpu.h"
#include "memory.h"
//read json test
//sets memory and cpu with init

//tests cpu and mem with json.
//best practice??
//FUCK IT, ignoring cycles for now. just check before and after
class JsonTest{

    public:
        JsonTest();
        JsonTest(Memory&, Cpu&);
        json read_json(const char* path); 

        void run_all_tests(const char path[]);
        void run_tests(std::string path);
        void run_test(json data); 
        void compare_results();
    private:
        int counter = 0;
        void initialize(json data);
        Memory* ram;
        Cpu* cpu;
};



#endif