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
        //std::cout << "hello";
        //if sc is 81, shift sb. and output.
        std::cout << ram.memory[ram.SB];

        if ((ram.memory[ram.SC] >> 7) ==0x1 ){
        //if (cpu.sb_count <8){
            //std::cout << ram.memory[ram.SB];
             //SHIFT SB.
             cpu.shift_sb();
             cpu.sb_count+=1;

             if (cpu.sb_count >=8)
             {
                //SC set to 0
                ram.memory[ram.SC] = 0x01;
                //Signal interrupt
                ram.memory[ram.IF] = (ram.memory[ram.IF]) | 0x8;
                cpu.sb_count =0;

             }


         }

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

    SDL_Delay(100);

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