CXX = clang++
CXXFLAGS = -std=c++20 -Wall
DEPS = Automaton.h
OBJ = main.o Automaton.o

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

parity_automaton.exe: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) parity_automaton.exe

.PHONY: clean
