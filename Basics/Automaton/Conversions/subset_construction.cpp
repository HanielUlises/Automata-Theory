#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>

// Nondeterministic Finite Automaton
struct NFA {
    int states;
    std::set<int> **transition; // Array of sets for transition functions
    std::set<int> initial; // Initial states
    std::set<int> accepting; // Accepting states

    NFA(int states) {
        this->states = states;
        transition = new std::set<int>*[2]; // Two transition functions for binary alphabet
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

    std::set<int> move(const std::set<int>& s, int input) const {
        std::set<int> result;
        for (int state : s) {
            for (int nextState : transition[input][state]) {
                result.insert(nextState);
            }
        }
        return result;
    }

    std::set<int> epsilonClosure(const std::set<int>& s) const {
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

    bool accepts(const std::string& input) const {
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

    const std::set<int>& getInitial() const {
        return initial;
    }

    const std::set<int>& getAccepting() const {
        return accepting;
    }
};

// Deterministic Finite Automaton
struct DFA {
    int states;
    std::map<std::pair<int, int>, int> transition;
    std::set<int> initial;
    std::set<int> accepting;

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

    int getStates() const {
        return states;
    }

    bool accepts(const std::string& input) const {
        int currentState = *initial.begin();
        for (char c : input) {
            currentState = transition[{currentState, c - '0'}];
        }
        return accepting.find(currentState) != accepting.end();
    }
};

// Converts NFA to DFA using subset construction algorithm
DFA subsetConstruction(const NFA& nfa) {
    DFA dfa(1); // Start with one state
    std::queue<std::set<int>> unmarkedStates;
    std::map<std::set<int>, int> stateMapping;

    std::set<int> initialClosure = nfa.epsilonClosure(nfa.getInitial());
    stateMapping[initialClosure] = 0;
    dfa.setInitial(0);
    unmarkedStates.push(initialClosure);

    while (!unmarkedStates.empty()) {
        std::set<int> currentState = unmarkedStates.front();
        unmarkedStates.pop();

        for (int i = 0; i < 2; ++i) {
            std::set<int> nextState;
            for (int state : currentState) {
                std::set<int> moveResult = nfa.move({state}, i);
                nextState.insert(moveResult.begin(), moveResult.end());
            }
            std::set<int> nextClosure = nfa.epsilonClosure(nextState);
            if (stateMapping.find(nextClosure) == stateMapping.end()) {
                stateMapping[nextClosure] = dfa.getStates();
                dfa.addTransition(stateMapping[currentState], i, dfa.getStates());
                unmarkedStates.push(nextClosure);
                dfa.states++; // Update number of states
            } else {
                dfa.addTransition(stateMapping[currentState], i, stateMapping[nextClosure]);
            }
        }
    }

    // Mark accepting states
    for (auto entry : stateMapping) {
        for (int state : entry.first) {
            if (nfa.getAccepting().find(state) != nfa.getAccepting().end()) {
                dfa.setAccepting(entry.second);
                break;
            }
        }
    }

    return dfa;
}

int main() {
    NFA nfa(3);
    nfa.addTransition(0, 1, 1);
    nfa.addTransition(1, 0, 2);
    nfa.addTransition(1, 1, 2);
    nfa.addTransition(2, 0, 2);
    nfa.addTransition(2, 1, 2);
    nfa.setInitial(0);
    nfa.setAccepting(2);

    DFA dfa = subsetConstruction(nfa);

    // Check if NFA and DFA accept the same input
    std::cout << "NFA accepts 010? " << (nfa.accepts("010") ? "Yes" : "No") << std::endl;
    std::cout << "DFA accepts 010? " << (dfa.accepts("010") ? "Yes" : "No") << std::endl;

    return 0;
}
