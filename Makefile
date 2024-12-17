CC = g++
CFLAGS = -std=c++23 -Wall -Wextra -Wpedantic -flto -O3 -I/usr/include/postgresql
LFLAGS = -lstdc++exp -L/usr/lib/ -lecpg
SRC = $(wildcard src/*.cpp)
SRC_BUILD = $(SRC:src/%.cpp=build/%.cpp)

release: $(SRC_BUILD)
	$(CC) $(CFLAGS) $^ -o program $(LFLAGS)

build/%.cpp: src/%.cpp
	ecpg $< -o $@

clean:
	rm -rf program build