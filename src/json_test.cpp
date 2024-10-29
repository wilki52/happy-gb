#include "json_test.h"
#include <fstream>

#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

JsonTest::JsonTest(){}
JsonTest::JsonTest(Memory& ram,Cpu& cpu): ram(&ram), cpu(&cpu){

}

json JsonTest::read_json(const char* path)
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
    while (cpu->pc != (final["pc"].get<int>()-1))
    {
        uint8_t instruction = cpu->fetch();
        cpu->decode(instruction);
        cpu->handle_interrupt();
        //std::cout << cpu->pc << " <? " << final["pc"].get<int>()-1 << std::endl;
    }

    if (cpu->af.high != final["a"].get<int>() || cpu->af.low != final["f"].get<int>() || cpu->bc.high != final["b"].get<int>() || cpu->bc.low != final["c"].get<int>() || cpu->de.high != final["d"].get<int>() || cpu->de.low != final["e"].get<int>() || cpu->hl.high != final["h"].get<int>() || cpu->hl.low != final["l"].get<int>() || (cpu->pc != (final["pc"].get<int>()-1)) || cpu->sp.full != final["sp"].get<int>())
    {
        
        std::cout << "reg ERROR: TEST " << data["name"] << std::endl;
        print_error_state(data);
        return;
    }
    for (json mem : final["ram"])
   {
        //std::cout<< mem << std::endl;
        if (ram->memory[mem[0].get<int>()] != mem[1].get<int>()){
            std::cout << "memory ERROR: TEST " << data["name"] << std::endl;
            print_error_state(data);

            return;
        }
   }

    //std::cout << "test " << counter << " : " << data["name"] << "passed" << std::endl;
}


void JsonTest::print_error_state(json data)
{
    json log = data["final"];
    json init = data["initial"];
    std::cout << "init log: " << init << std::endl;
    std::cout << "final log: " << log << std::endl;
    //std::cout << "'a':" (signed)cpu->af.high << std::endl;

    if (cpu->af.high != log["a"].get<int>())
    {
        std::cout << "'a:'" << (signed)cpu->af.high << std::endl;
    }
    if (cpu->af.low != log["f"].get<int>())
    {
        std::cout << "f: 0d" << std::dec << (signed)cpu->af.low << std::endl;
    }
    if (cpu->bc.high != log["b"].get<int>())
    {
        std::cout << "'b:'" << (signed)cpu->bc.high << std::endl;
    }
    if (cpu->bc.low != log["c"].get<int>())
    {
        std::cout << "'c:'" << (signed)cpu->bc.low << std::endl;
    }
    if (cpu->de.high != log["d"].get<int>())
    {
        std::cout << "'d:'" << (signed)cpu->de.high << std::endl;
    }
    if (cpu->de.low != log["e"].get<int>())
    {
        std::cout << "'e:'" << (signed)cpu->de.low << std::endl;
    }
    if (cpu->hl.high != log["h"].get<int>())
    {
        std::cout << "'h:'" << (signed)cpu->hl.high << std::endl;
    }
    if (cpu->hl.low != log["l"].get<int>())
    {
        std::cout << "'l:'" << (signed)cpu->hl.low << std::endl;
    }

    if (cpu->sp.full != log["sp"].get<int>())
    {
        std::cout << "sp: 0d" << std::dec  <<(signed)cpu->sp.full << std::endl;
    }
    if (cpu->pc != log["pc"].get<int>()-1)
    {
        std::cout << "'pc:'" << (signed)cpu->pc << std::endl;
    }

    for (json mem : log["ram"])
    {
        if (ram->memory[mem[0].get<int>()] != mem[1].get<int>())
            std::cout << "memory address: 0d"<< std::dec << (signed)mem[0].get<int>()<< ": 0d" << std::dec << (signed)ram->memory[mem[0].get<int>()] << std::endl;
    }

    abort();
}
void JsonTest::run_all_tests(const char path[])
{
    //
    for (const auto & entry : fs::directory_iterator(path))
    {
        std::cout << "running file " << counter << ": " << entry.path().string() << std::endl;
        counter++;
        run_tests(entry.path().string());
    }

    std::cout << "finished testing" << std::endl;
        

}
void JsonTest::run_tests(std::string path)
{
    json tests = read_json(path.c_str());

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
    std::cout << "starting new test : "<< data["name"] << std::endl;
    initialize(data);

    //set initial processor state from test;
    //set initial RAM state from test;
    //for cycle in test:
     //   cycle processor

    //compare final RAM state to test and report any errors;
    //compare final processor state to test and report any errors;
}

