
#pragma once

#ifndef AUTOMATON_H
#define AUTOMATON_H 

#include <iostream>
#include <string>
#include <memory>

struct State{
    std::unique_ptr<State> transition;

};

class Automaton{
    public:
    Automaton (std::string word, State qo);


    private:

    std::string word;
    State initial_state;
};

#endif // AUTOMATON_H