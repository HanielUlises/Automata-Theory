#ifndef TURINGMACHINE_H
#define TURINGMACHINE_H

#include <string>
#include <vector>
#include <map>
#include <set>

class TuringMachine {
public:
    TuringMachine();
    bool process(const std::string& input);
    void printInstantaneousDescriptions(const std::string& input, const std::string& filename);
    void visualize(const std::string& input);

private:
    struct Transition {
        char write;
        char move; // 'L' for left, 'R' for right
        std::string nextState;
    };

    std::map<std::string, std::map<char, Transition>> transitions;
    std::string initialState;
    std::set<std::string> acceptStates;
    std::set<std::string> rejectStates;
};

#endif // TURINGMACHINE_H
