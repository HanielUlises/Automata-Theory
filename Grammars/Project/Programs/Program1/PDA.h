#ifndef PDA_H
#define PDA_H

#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <fstream>

class PDA {
public:
    PDA();
    void reset();
    bool transition(char input);
    bool processString(const std::string& str);
    void logIdSequence(const std::string& filename);

private:
    struct ID {
        std::string state;
        char input;
        std::stack<char> stack_before;
        std::stack<char> stack_after;
    };

    std::stack<char> stack;
    std::string state;
    std::vector<ID> id_sequence;

    std::string stackToString(const std::stack<char>& stack);
};

std::string generateRandomString(size_t max_length = 100000);

#endif // PDA_H
