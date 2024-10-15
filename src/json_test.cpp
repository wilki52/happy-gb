#include "json_test.h"
#include <fstream>

#include <iostream>


JsonTest::JsonTest(){}
JsonTest::JsonTest(Memory& ram,Cpu& cpu): ram(&ram), cpu(&cpu){

}

json JsonTest::read_json(const char path[])
{
    std::ifstream f(path);

    if (!f.is_open()){
        std::cout << "file not found" << std::endl;
        return json({});
    }
    
    json data = json::parse(f);
    //std::cout << data[0] << std::endl;
    //initialize(data[0]);

    return data;
    //set initial processor state from test;
    //set initial RAM state from test;
}

//initialize PC to given PC-1, then run one cycle.

void JsonTest::initialize(json data)
{
   json initials = data["initial"];
   cpu->af.high = initials["a"].get<int>();
   cpu->af.low = initials["f"].get<int>();
   cpu->bc.high = initials["b"].get<int>();
   cpu->bc.low= initials["c"].get<int>();
   cpu->de.high = initials["d"].get<int>();
   cpu->de.low= initials["e"].get<int>();
   cpu->hl.high = initials["h"].get<int>();
   cpu->hl.low= initials["l"].get<int>();
   cpu->pc = initials["pc"].get<int>()-1;
   cpu->sp.full = initials["sp"].get<int>();

   for (json mem : initials["ram"])
   {
        //std::cout<< mem << std::endl;
        ram->memory[mem[0].get<int>()] = mem[1].get<int>(); //acts as a write. 
   }
    //then, we run the test. do i run until i can check if PC has been reached?

    json final = data["final"];
    while (cpu->pc < (final["pc"].get<int>()-1))
    {
        uint8_t instruction = cpu->fetch();
        cpu->decode(instruction);
        cpu->handle_interrupt();
    }

    if (cpu->af.high != final["a"].get<int>() || cpu->af.low != final["f"].get<int>() || cpu->bc.high != final["b"].get<int>() || cpu->bc.low != final["c"].get<int>() || cpu->de.high != final["d"].get<int>() || cpu->de.low != final["e"].get<int>() || cpu->hl.high != final["h"].get<int>() || cpu->hl.low != final["l"].get<int>() || (cpu->pc != (final["pc"].get<int>()-1)) || cpu->sp.full != final["sp"].get<int>())
    {
        
        std::cout << "ERROR: TEST " << data["name"] << std::endl;
        return;
    }
    for (json mem : final["ram"])
   {
        //std::cout<< mem << std::endl;
        if (ram->memory[mem[0].get<int>()] != mem[1].get<int>()){
            std::cout << "ERROR: TEST " << data["name"] << std::endl;
            return;
        }
   }

    //std::cout << "test " << counter << " : " << data["name"] << "passed" << std::endl;
    counter+=1;
}
void JsonTest::run_tests(const char path[])
{
    counter = 0;
    json tests = read_json(path);

    for (json test : tests){
        run_test(test);
    }
    //how do i
    //for test in test: run_test(test);
}

void JsonTest::compare_results()
{

}
void JsonTest::run_test(json data)
{
    initialize(data);

    //set initial processor state from test;
    //set initial RAM state from test;
    //for cycle in test:
     //   cycle processor

    //compare final RAM state to test and report any errors;
    //compare final processor state to test and report any errors;
}

