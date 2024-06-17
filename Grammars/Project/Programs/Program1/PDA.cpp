#include "PDA.h"
#include <ctime>
#include <cstdlib>

PDA::PDA() {
    reset();
}

void PDA::reset() {
    while (!stack.empty()) stack.pop();
    stack.push('Z');
    state = "q0";
    id_sequence.clear();
}

bool PDA::transition(char input) {
    std::stack<char> current_stack = stack;
    if (state == "q0") {
        if (input == '0') {
            stack.push('0');
        } else if (input == '1' && stack.top() == '0') {
            stack.pop();
        } else {
            state = "reject";
            return false;
        }
        id_sequence.push_back({state, input, current_stack, stack});
        return true;
    }
    return false;
}

bool PDA::processString(const std::string& str) {
    reset();
    for (char ch : str) {
        if (!transition(ch)) {
            break;
        }
    }
    if (stack.size() == 1 && stack.top() == 'Z') {
        state = "accept";
    } else {
        state = "reject";
    }
    id_sequence.push_back({state, '\0', stack, stack});
    return state == "accept";
}

void PDA::logIdSequence(const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& id : id_sequence) {
        file << "State: " << id.state << ", Input: " << id.input 
             << ", Stack before: " << stackToString(id.stack_before) 
             << ", Stack after: " << stackToString(id.stack_after) << "\n";
    }
}

std::string PDA::stackToString(const std::stack<char>& stack) {
    std::stack<char> temp = stack;
    std::string result;
    while (!temp.empty()) {
        result += temp.top();
        temp.pop();
    }
    return result;
}

std::string generateRandomString(size_t max_length) {
    srand(time(0));
    size_t n = rand() % (max_length / 2) + 1;
    return std::string(n, '0') + std::string(n, '1');
}
