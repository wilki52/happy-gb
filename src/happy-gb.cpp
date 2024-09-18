#include "happy-gb.h"
#include <SDL2/SDL.h>

//INIT REG MAPPING HERE
Happy::Happy(): ram(), cpu(ram), display(ram){ //init ram, then init cpu constructor
    //display.open();
    display.open();


}

void Happy::run_program(){
    SDL_Event e;
    std::cout << "run program" << std::endl;
    bool running = true;

    display.view_vram();
    uint8_t draw = 0;
    while (running){
        if (SDL_PollEvent(&e)!=0){
            SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
            if (e.type==SDL_QUIT){
                running = false;
            }
            cpu.handle_input(e);

        }

        cycle();
        draw+=1;

        if (draw==100){
            display.view_vram();
            draw = 0;
        }

        

    }
    display.print_vram();
    // for (int i = 0x8000; i<0x8020; i++){
    //     if (true){
    //         std::cout << std::hex << i << ": " << signed(ram.memory[i]) <<  std::endl;
    //     }
    // }
    
    //for (int i = 0; i < sizeof(ram.memory); i++){
    //    std::cout << ram.memory[i];
    //}
    

}

void Happy::cycle(){
    uint8_t instruction = cpu.fetch();
    
    cpu.decode(instruction); 

    cpu.handle_interrupt();

    SDL_Delay(10);

    //if LCDC.7: PPU render
    if (ram.memory[(ram.LCDC >> 7)&0x1]==1){
        display.render_background();
    }



}

//arbitrary decode blocks following pandocs, based on opcode bits 7,6

int Happy::load_rom(const char path[]){
    int pass = cpu.read_rom(path);

    return pass;
}