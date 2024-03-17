#include "Automaton.h"

void State::addTransition(char symbol, std::shared_ptr<State> state) {
    transitions[symbol] = std::move(state);
}

std::shared_ptr<State> State::transition(char symbol) const {
    auto it = transitions.find(symbol);
    if (it != transitions.end()) {
        return it->second;
    }
    return nullptr;
}

bool State::isFinal() const {
    return isFinalState;
}

Automaton::Automaton(const std::string& word) : word(word) {
    auto q1 = std::make_shared<State>();
    auto q2 = std::make_shared<State>();
    auto q3 = std::make_shared<State>();
    auto q4 = std::make_shared<State>();
    auto qF = std::make_shared<State>(true);

    q1->addTransition('0', q4);
    q1->addTransition('1', q2);

    q2->addTransition('0', q3);
    q2->addTransition('1', qF);

    q3->addTransition('0', q2);
    q3->addTransition('1', q4);

    q4->addTransition('0', q1);
    q4->addTransition('1', q3);

    // qF is a final state and has no outgoing transitions

    // Set the initial state
    initialState = q1;
    currentState = initialState;
}

bool Automaton::checkParity() {
    for (char c : word) {
        auto nextState = currentState->transition(c);
        if (nextState != nullptr) {
            currentState = nextState;
        } else {
            // Invalid transition
            return false;
        }
    }
    return currentState->isFinal();
}
