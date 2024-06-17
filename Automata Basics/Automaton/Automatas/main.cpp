#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>

class NFA {
    int states;
    std::set<int> *transition[2];
    std::set<int> initial;
    std::set<int> accepting;

public:
    NFA(int states) {
        this->states = states;
        for (int i = 0; i < 2; ++i) {
            transition[i] = new std::set<int>[states];
        }
    }

    void addTransition(int state, int input, int nextState) {
        transition[input][state].insert(nextState);
    }

    void setInitial(int state) {
        initial.insert(state);
    }

    void setAccepting(int state) {
        accepting.insert(state);
    }

    std::set<int> move(std::set<int>& s, int input) {
        std::set<int> result;
        for (int state : s) {
            for (int nextState : transition[input][state]) {
                result.insert(nextState);
            }
        }
        return result;
    }

    std::set<int> epsilonClosure(std::set<int>& s) {
        std::set<int> result = s;
        std::queue<int> q;
        for (int state : s) {
            q.push(state);
        }
        while (!q.empty()) {
            int state = q.front();
            q.pop();
            for (int nextState : transition[0][state]) {
                if (result.find(nextState) == result.end()) {
                    result.insert(nextState);
                    q.push(nextState);
                }
            }
        }
        return result;
    }

    bool accepts(std::string input) {
        std::set<int> current = epsilonClosure(initial);
        for (char c : input) {
            current = epsilonClosure(move(current, c - '0'));
        }
        for (int state : current) {
            if (accepting.find(state) != accepting.end()) {
                return true;
            }
        }
        return false;
    }
};

class DFA {
    int states;
    std::map<std::pair<int, int>, int> transition;
    std::set<int> initial;
    std::set<int> accepting;

public:
    DFA(int states) {
        this->states = states;
    }

    void addTransition(int state, int input, int nextState) {
        transition[{state, input}] = nextState;
    }

    void setInitial(int state) {
        initial.insert(state);
    }

    void setAccepting(int state) {
        accepting.insert(state);
    }

    bool accepts(std::string input) {
        int currentState = *initial.begin();
        for (char c : input) {
            currentState = transition[{currentState, c - '0'}];
        }
        return accepting.find(currentState) != accepting.end();
    }
};

int main() {
    NFA nfa(3);
    nfa.addTransition(0, 1, 1);
    nfa.addTransition(1, 0, 2);
    nfa.addTransition(1, 1, 2);
    nfa.addTransition(2, 0, 2);
    nfa.addTransition(2, 1, 2);
    nfa.setInitial(0);
    nfa.setAccepting(2);

    DFA dfa(4);
    dfa.addTransition(0, 0, 1);
    dfa.addTransition(0, 1, 2);
    dfa.addTransition(1, 0, 3);
    dfa.addTransition(1, 1, 2);
    dfa.addTransition(2, 0, 2);
    dfa.addTransition(2, 1, 2);
    dfa.addTransition(3, 0, 3);
    dfa.addTransition(3, 1, 2);
    dfa.setInitial(0);
    dfa.setAccepting(2);
    dfa.setAccepting(3);

    std::cout << "NFA accepts 010? " << (nfa.accepts("010") ? "Yes" : "No") << std::endl;
    std::cout << "DFA accepts 010? " << (dfa.accepts("010") ? "Yes" : "No") << std::endl;

    return 0;
}
