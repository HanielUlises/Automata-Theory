#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>

class State {
public:
    explicit State(bool isFinal = false) : isFinalState(isFinal) {}
    void addTransition(char symbol, std::shared_ptr<State> state);
    std::shared_ptr<State> transition(char symbol) const;
    bool isFinal() const;

private:
    bool isFinalState;
    std::unordered_map<char, std::shared_ptr<State>> transitions;
};

class Automaton {
public:
    explicit Automaton(const std::string& word);
    bool checkParity();

private:
    std::string word;
    std::shared_ptr<State> initialState;
    std::shared_ptr<State> currentState;
};
