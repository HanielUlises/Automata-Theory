CXX=g++
CXXFLAGS=-std=c++20 -I/usr/include/python3.10 -I/home/haniel/Libraries -I/home/haniel/.local/lib/python3.10/site-packages/numpy/core/include `sdl2-config --cflags`
LDFLAGS=`sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lpython3.10
TARGET=main

SRC_DIRS:=. Programs Programs/Program1 Programs/Program2 Programs/Program3

SOURCES:=$(shell find $(SRC_DIRS) -name '*.cpp')

OBJECTS:=$(SOURCES:.cpp=.o)

DEPS:=$(OBJECTS:.o=.d)

-include $(DEPS)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS) $(DEPS)

.PHONY: all clean
