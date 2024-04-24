#include "NFA.h"

void State::add_transition (char symbol, State* state){
    transitions[symbol].push_back(state);
}

bool NFA::is_accepted(const std::string& input) {
    return check_state(q0, input, 0);
}

// Set of next states from a current symbol
std::vector<State*> State::next_states(char symbol) {
    auto it = transitions.find(symbol);
    if (it != transitions.end()) {
        return it->second;
    }
    return {};
}


// This both prints the Automaton instance player paths and saves them into a txt file
// Prints all the possible paths (including those who are not accepted by the NFA)
// And the winner paths (those who are accepted by the NFA)
bool NFA::print_paths(const std::string& input, std::string id) {
    std::vector<std::vector<State*>> paths;
    std::vector<State*> initial_path = {q0};

    std::string name_possible = "possible_moves";
    std::string name_winners = "winner_moves";

    find_paths(q0, input, 0, initial_path, paths, true);
    find_paths(q0, input, 0, initial_path, paths, false);

    name_possible = name_possible.append(id).append(".txt");
    std::ofstream file_pos(name_possible);
    name_possible = "";
    name_winners = name_winners.append(id).append(".txt");
    std::ofstream file_win(name_winners);
    name_winners = "";

    for (const auto& path : possible_moves) {
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << (i == 0 ? "S" : "->S") << path[i]->id;
            file_pos << (i == 0 ? "S" : "->S") << path[i]->id;
        }
        std::cout << std::endl;
        file_pos << std::endl;
    }
    file_pos.close();

    for (const auto& path : winner_moves) {
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << (i == 0 ? "S" : "->S") << path[i]->id;
            file_win << (i == 0 ? "S" : "->S") << path[i]->id;
        }
        std::cout << std::endl;
        file_win << std::endl;
    }
    file_win.close();

    if(winner_moves.size()==0){
        return false;
    }else{
        return true;
    }
}

// Just to fill the vectors of both possible and actually winner paths
void NFA::find_paths(State* current, const std::string& input, size_t index,
                     std::vector<State*>& current_path,
                     std::vector<std::vector<State*>>& all_paths,
                     bool is_winner) {
    
    if (index == input.size()) {
        if(!is_winner){
                possible_moves.push_back(current_path);
        }else{
            if (current == qf) {
                winner_moves.push_back(current_path);
            }
        }
        return;
    }

    auto next_states = current->next_states(input[index]);
    for (auto next : next_states) {
        current_path.push_back(next);
        find_paths(next, input, index + 1, current_path, all_paths, is_winner);
        current_path.pop_back();
    }
}

// Check if a state is ought to be considered within the NFA language
bool NFA::check_state(State* currentState, const std::string& input, size_t index) {
    if (currentState == qf && index == input.size()) {
        return true;
    }

    if (index < input.size()) {
        char currentSymbol = input[index];
        if (currentState->transitions.find(currentSymbol) != currentState->transitions.end()) {
            for (State* nextState : currentState->transitions[currentSymbol]) {
                if (check_state(nextState, input, index + 1)) {
                    return true;
                }
            }
        }
    }
    return false;
}

std::vector<std::vector<State*>> NFA::get_possible_moves(){
    return possible_moves;
}

std::vector<std::vector<State*>> NFA::get_winner_moves(){
    return winner_moves;
}

// Important for reconfiguration
std::vector<std::vector<State*>> NFA::get_alternate_winner_moves(std::vector<State*>& current_route) {
    std::vector<std::vector<State*>> alternate_routes;
    for (auto& route : winner_moves) {
        if (route != current_route) {
            alternate_routes.push_back(route);
        }
    }
    return alternate_routes;
}