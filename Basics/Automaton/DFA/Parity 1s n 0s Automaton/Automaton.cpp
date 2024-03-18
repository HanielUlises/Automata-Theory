#include "Automaton.h"

Automaton::Automaton() : currentState(State::q0) {}

void Automaton::transition(char input) {
        switch (currentState) {
            case State::q0:
                currentState = (input == '0') ? State::q1 : State::q2;
                break;
            case State::q1:
                currentState = (input == '0') ? State::q0 : State::q3;
                break;
            case State::q2:
                currentState = (input == '0') ? State::q3 : State::q0;
                break;
            case State::q3:
                currentState = (input == '0') ? State::q2 : State::q1;
                break;
        }
    }

bool Automaton::isAccepting() const {
    return currentState == State::q0;
}

bool processString(const std::string& input) {
    Automaton automaton;
    for (char c : input) {
        automaton.transition(c);
    }
    return automaton.isAccepting();
}