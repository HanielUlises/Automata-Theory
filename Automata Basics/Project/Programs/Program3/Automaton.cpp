#include "Automaton.h"

#include <fstream>
#include <iostream>
#include <map>

void buildAutomaton(Automaton& nfa) {
    // Estado inicial
    Automaton_State* start = new Automaton_State();
    nfa.setStartState(start);

    // Para "ferminal"
    Automaton_State* f1 = new Automaton_State(); Automaton_State* fe1 = new Automaton_State(); Automaton_State* fer1 = new Automaton_State();
    Automaton_State* ferm1 = new Automaton_State(); Automaton_State* fermi1 = new Automaton_State(); Automaton_State* fermin = new Automaton_State();
    Automaton_State* ferminal = new Automaton_State(true); // Estado final para "ferminal"
    start->addTransition('f', f1); f1->addTransition('e', fe1); fe1->addTransition('r', fer1);
    fer1->addTransition('m', ferm1); ferm1->addTransition('i', fermi1); fermi1->addTransition('n', fermin);
    fermin->addTransition('a', ferminal); ferminal->addTransition('l', ferminal);

    // Para "feminazi"
    Automaton_State* fe2 = new Automaton_State(); Automaton_State* fem2 = new Automaton_State(); Automaton_State* femi2 = new Automaton_State();
    Automaton_State* femin = new Automaton_State(); Automaton_State* femina = new Automaton_State(); Automaton_State* feminaz = new Automaton_State();
    Automaton_State* feminazi = new Automaton_State(true); // Estado final para "feminazi"
    f1->addTransition('e', fe2); fe2->addTransition('m', fem2); fem2->addTransition('i', femi2);
    femi2->addTransition('n', femin); femin->addTransition('a', femina); femina->addTransition('z', feminaz);
    feminaz->addTransition('i', feminazi);

    // Para "tonta"
    Automaton_State* t = new Automaton_State(); Automaton_State* to = new Automaton_State(); Automaton_State* ton = new Automaton_State();
    Automaton_State* tont = new Automaton_State(); Automaton_State* tonta = new Automaton_State(true); // Estado final para "tonta"
    start->addTransition('t', t); t->addTransition('o', to); to->addTransition('n', ton);
    ton->addTransition('t', tont); tont->addTransition('a', tonta);

    // Para "torta"
    Automaton_State* tort = new Automaton_State(); Automaton_State* torta = new Automaton_State(true); // Estado final para "torta"
    to->addTransition('r', tort); tort->addTransition('t', tort); tort->addTransition('a', torta);

    // Para "vieja"
    Automaton_State* v = new Automaton_State(); Automaton_State* vi = new Automaton_State(); Automaton_State* vie = new Automaton_State();
    Automaton_State* viej = new Automaton_State(); Automaton_State* vieja = new Automaton_State(true); // Estado final para "vieja"
    start->addTransition('v', v); v->addTransition('i', vi); vi->addTransition('e', vie);
    vie->addTransition('j', viej); viej->addTransition('a', vieja);

    // Para "feminista"
    Automaton_State* feminis = new Automaton_State(); Automaton_State* feminist = new Automaton_State(); Automaton_State* feminista = new Automaton_State(true); // Estado final para "feminista"
    femin->addTransition('i', feminis); feminis->addTransition('s', feminist); feminist->addTransition('t', feminist);
    feminist->addTransition('a', feminista);

    nfa.addState(start);
    nfa.addState(f1); nfa.addState(fe1); nfa.addState(fer1); nfa.addState(ferm1); nfa.addState(fermi1); nfa.addState(fermin); nfa.addState(ferminal);
    nfa.addState(fe2); nfa.addState(fem2); nfa.addState(femi2); nfa.addState(femin); nfa.addState(femina); nfa.addState(feminaz); nfa.addState(feminazi);
    nfa.addState(t); nfa.addState(to); nfa.addState(ton); nfa.addState(tont); nfa.addState(torta); nfa.addState(torta);
    nfa.addState(v); nfa.addState(vi); nfa.addState(vie); nfa.addState(viej); nfa.addState(vieja);
    nfa.addState(feminis); nfa.addState(feminist); nfa.addState(feminista);
}

bool word_detected(const std::string& word) {
    static const std::vector<std::string> reservedWords = {"ferminal", "feminazi", "tonta", "torta", "vieja", "feminista"};
    for (const auto& rw : reservedWords) {
        if (word == rw) return true;
    }
    return false;
}


void run_program_3() {
    std::ifstream file("texto.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file texto.txt" << std::endl;
        return;
    }
    std::ofstream logFile("process_log.txt");
    Automaton automaton;
    buildAutomaton(automaton);

    std::string line;
    int lineNum = 0;
    std::map<std::string, std::vector<Position>> occurrences;

    while (getline(file, line)) {
        lineNum++;
        for (size_t i = 0; i < line.size(); ++i) {
            for (size_t len = 1; len <= line.size() - i; ++len) {
                std::string word = line.substr(i, len);
                if (automaton.accepts(word)) {
                    logFile << "Palabra encontrada: " << word << " en (" << lineNum << ", " << (i+1) << ")\n";
                    occurrences[word].push_back({lineNum, static_cast<int>(i+1)});
                }
            }
        }
    }

    // Registro de ocurrencias y posiciones
    std::ofstream countFile("output.txt");
    for (const auto& pair : occurrences) {
        countFile << "Palabra: " << pair.first << " - Ocurrencias: " << pair.second.size() << std::endl;
        for (const auto& pos : pair.second) {
            countFile << "  Encontrada en linea " << pos.line << ", columna " << pos.column << std::endl;
        }
    }

    file.close();
    logFile.close();
    countFile.close();

}