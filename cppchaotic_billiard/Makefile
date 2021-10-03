cxx := g++
cxx_flags := -Wall -Wextra -std=c++17 -O3

bin := ./bin
src := ./src
build := ./build
include := -I./include -I../physics/include -I./lib
libraries := -lsfml-graphics -lsfml-window -lsfml-system -lGL
objs := build/curve.o build/collider.o build/globals.o build/logger.o

VPATH := ../physics
.PHONY: dirs run clean all

all: dirs $(bin)/app.out $(bin)/export_world_json.out

run: clean all
	./$(bin)/app.out

$(build)/%.o: $(src)/%.cpp
	$(cxx) $(cxx_flags) $(include) $(libraries) -c $< -o $@

$(bin)/app.out: build/app.o $(objs)
	$(cxx) $(cxx_flags) $(include) $(libraries) $^ -o $@

$(bin)/export_world_json.out: build/export_world_json.o $(objs)
	$(cxx) $(cxx_flags) $(include) $(libraries) $^ -o $@

clean:
	rm -f $(build)/*
	rm -f $(bin)/*
	
dirs:
	mkdir -p $(build)
	mkdir -p $(bin)
