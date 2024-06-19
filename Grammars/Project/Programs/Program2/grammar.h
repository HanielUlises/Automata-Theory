#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <vector>

namespace Program2{
    class Grammar {
    public:
        Grammar(int maxDerivations);
        void derive();
        void writeDerivationsToFile(const std::string &filename) const;
        void writePseudoCodeToFile(const std::string &filename) const;

    private:
        std::string deriveStep(const std::string &current, std::string &appliedRule);
        std::string generatePseudoCode(const std::string &derived) const;

        std::vector<std::string> derivationSteps;
        std::vector<std::string> appliedRules;
        int maxDerivations;
    };
}


#endif // GRAMMAR_H
