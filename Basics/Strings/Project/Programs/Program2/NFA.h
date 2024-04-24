#ifndef NFA_H
#define NFA_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>

struct State {
    State(int id, bool is_state = false) : id(id), accepted_state(is_state) {}
    void add_transition(char symbol, State* state);

    
    std::map<char, std::vector<State*>> transitions;
    std::vector<State*> next_states(char symbol);

    int id;
    bool accepted_state;
};

class NFA {
public:
    
    NFA() {};
    NFA(State* start, State* final) : q0(start) , qf (final){}

    State* q0;
    State* qf;

    std::vector<State*> states;
    bool is_accepted(const std::string& input);

    void find_paths(State* current, const std::string& input, size_t index,
                     std::vector<State*>& current_path,
                     std::vector<std::vector<State*>>& all_paths,
                     bool is_winner);

    bool print_paths(const std::string& input, std::string id);

    std::vector<std::vector<State*>> get_possible_moves();
    std::vector<std::vector<State*>> get_winner_moves();
    std::vector<std::vector<State*>> get_alternate_winner_moves(std::vector<State*>& current_route);
    
private:
    std::vector<std::vector<State*>> possible_moves;
    std::vector<std::vector<State*>> winner_moves;

    bool check_state(State* currentState, const std::string& input, size_t index);
};

void test_automatons();

#endif // NFA_H
