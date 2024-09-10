SDLFLAGS = $(shell sdl2-config --libs --cflags)

all: gb

gb: src/main.cpp
	g++ -o happygb src/display.cpp src/main.cpp src/happy-gb.cpp src/cpu.cpp src/memory.cpp src/opcodes.cpp $(SDLFLAGS)