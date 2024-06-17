CXX = clang++
CXXFLAGS = -std=c++20 -Wall
DEPS = Automaton.h
OBJ = main.o Automaton.o

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

parity_0_1.exe: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) parity_0_1.exe

.PHONY: clean
