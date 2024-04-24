// AUTOMATON_H
#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iostream>

struct Position {
    int line;
    int column;
};

// Represents a state in the NFA.
struct Automaton_State {
    // Maps an input character to a set of states.
    std::unordered_map<char, std::unordered_set<Automaton_State*>> transitions;
    bool isFinal; // True if this state is an accepting state.

    Automaton_State(bool isFinalState = false) : isFinal(isFinalState) {}

    // Adds a transition from this state to the given state on the given symbol.
    void addTransition(char symbol, Automaton_State* state) {
        transitions[symbol].insert(state);
    }

    // Returns the set of states that can be reached from this state given the input symbol.
    std::unordered_set<Automaton_State*> getTransitions(char symbol) {
        if (transitions.find(symbol) != transitions.end()) {
            return transitions[symbol];
        }
        return {};
    }
};

class Automaton {
private:
    Automaton_State* startState; // The start state of the NFA.
    std::vector<Automaton_State*> states; // All states in the NFA.

public:
    Automaton() : startState(nullptr) {}

    ~Automaton() {
        for (Automaton_State* state : states) {
            delete state;
        }
    }

    // Sets the start state of the automaton.
    void setStartState(Automaton_State* state) {
        startState = state;
    }

    // Adds a new state to the automaton.
    void addState(Automaton_State* state) {
        states.push_back(state);
    }

    // Checks whether the automaton accepts the given string.
    bool accepts(const std::string& input) {
    std::cout << "Checking input: " << input << std::endl; // Debugging print
    return checkAccepts(startState, input, 0);
    }

private:
    // Check if the automaton accepts the input string.
    bool checkAccepts(Automaton_State* state, const std::string& input, size_t position) {
        if (position == input.length()) {
            return state->isFinal;
        }

        char currentSymbol = input[position];
        if (state->transitions.find(currentSymbol) != state->transitions.end()) {
            for (Automaton_State* nextState : state->getTransitions(currentSymbol)) {
                if (checkAccepts(nextState, input, position + 1)) {
                    return true;
                }
            }
        }
        return false;
    }
};

void run_program_3();
#endif
