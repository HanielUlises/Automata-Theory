#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <iostream>
#include <string>

enum class State {
    q0, // Even 0's, Even 1's
    q1, // Odd 0's, Even 1's
    q2, // Even 0's, Odd 1's
    q3  // Odd 0's, Odd 1's
};

class Automaton {
private:
    State currentState;

public:
    Automaton();

    void transition(char input);
    bool isAccepting() const;
};

bool processString(const std::string& input);

#endif