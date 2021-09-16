cxx := g++
cxx_flags := -Wall -Wextra -std=c++20

bin := ./bin
src := ./src
build := ./build
include := -I./include -I../physics/include -I./lib
libraries := -lsfml-graphics -lsfml-window -lsfml-system -lGL
executable := app.out
objs := build/app.o build/curve.o build/collider.o

VPATH := ../physics
.PHONY: dirs run clean all

all: dirs $(bin)/$(executable)

run: clean all
	./$(bin)/$(executable)

$(build)/%.o: $(src)/%.cpp
	$(cxx) $(cxx_flags) $(include) $(libraries) -c $< -o $@

$(bin)/$(executable): $(objs)
	$(cxx) $(cxx_flags) $(include) $(libraries) $^ -o $@

clean:
	rm -f $(build)/*
	rm -f $(bin)/*
	
dirs:
	mkdir -p $(build)
	mkdir -p $(bin)
