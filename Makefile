SDLFLAGS = $(shell sdl2-config --libs --cflags)

all: gb

gb: src/main.cpp
	g++ -o happygb src/main.cpp src/happy-gb.cpp src/cpu.cpp src/memory.cpp $(SDLFLAGS)