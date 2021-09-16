cxx := g++
cxx_flags := -Wall -Wextra -std=c++17

bin := ./bin
src := ./src
build := ./build
include := ./include
lib := ./lib
libraries := -lsfml-graphics -lsfml-window -lsfml-system -lGL
executable := app
objs := build/collider.o build/curve.o build/app.o

.PHONY: dirs run clean all

all: dirs $(bin)/$(executable)

run: clean all
	./$(bin)/$(executable)

$(build)/%.o: $(src)/%.cpp
	$(cxx) $(cxx_flags) -I$(include) -L$(lib) $(libraries) -c $< -o $@

$(bin)/$(executable): $(objs)
	$(cxx) $(cxx_flags) -I$(include) -L$(lib) $(libraries) $^ -o $@

clean:
	rm -f $(build)/*
	rm -f $(bin)/*
	
dirs:
	mkdir -p $(build)
	mkdir -p $(bin)
