#include "grammar.h"
#include <fstream>
#include <random>
#include <iostream>
#include <unordered_map>

namespace Program2{
    Grammar::Grammar(int maxDerivations) : maxDerivations(maxDerivations) {}

    void Grammar::derive() {
        std::string current = "S";
        derivationSteps.push_back(current);
        appliedRules.push_back("Initial: S");
        int count = 0;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);

        while (current.find('S') != std::string::npos && count < maxDerivations) {
            std::string rule;
            current = deriveStep(current, rule);
            derivationSteps.push_back(current);
            appliedRules.push_back(rule);
            count++;
        }
    }

    void Grammar::writeDerivationsToFile(const std::string &filename) const {
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            return;
        }
        for (size_t i = 0; i < derivationSteps.size(); ++i) {
            outFile << derivationSteps[i] << " (" << appliedRules[i] << ")" << std::endl;
        }
        outFile.close();
    }

    void Grammar::writePseudoCodeToFile(const std::string &filename) const {
        if (derivationSteps.empty()) {
            std::cerr << "No derivations to convert to pseudo-code." << std::endl;
            return;
        }
        std::string pseudoCode = generatePseudoCode(derivationSteps.back());
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            return;
        }
        outFile << pseudoCode << std::endl;
        outFile.close();
    }

    std::string Grammar::deriveStep(const std::string &current, std::string &appliedRule) {
        size_t pos = current.find('S');
        if (pos == std::string::npos) {
            appliedRule = "No 'S' found";
            return current;
        }

        std::string newStr = current;
        std::vector<std::string> rules = {"iCtSA", "iCt(S);eS"};
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, rules.size() - 1);
        int ruleIndex = dis(gen);

        newStr.replace(pos, 1, rules[ruleIndex]);
        appliedRule = "S -> " + rules[ruleIndex];

        size_t posA = newStr.find('A');
        if (posA != std::string::npos) {
            std::uniform_int_distribution<> disA(0, 1);
            if (disA(gen) == 0) {
                newStr.replace(posA, 1, ";eS");
                appliedRule += ", A -> ;eS";
            } else {
                newStr.replace(posA, 1, "");
                appliedRule += ", A -> ε";
            }
        }

        return newStr;
    }

    std::string Grammar::generatePseudoCode(const std::string &derived) const {
        static const std::unordered_map<char, std::string> replacements = {
            {'i', "if"},
            {'C', "(condition)"},
            {'t', "then"},
            {';', "else"}
        };

        std::string pseudoCode;
        pseudoCode.reserve(derived.size() * 2);  

        int indentLevel = 0;
        bool newLine = false;

        for (char ch : derived) {
            auto it = replacements.find(ch);
            if (it != replacements.end()) {
                if (ch == 'i' || ch == ';') {
                    if (newLine) {
                        pseudoCode.append("\n");
                        // Identation
                        pseudoCode.append(indentLevel * 4, ' '); 
                    }
                    newLine = true;
                }
                pseudoCode.append(it->second);
            } else {
                pseudoCode.push_back(ch);
                if (ch == '(') {
                    indentLevel++;
                    newLine = true;
                } else if (ch == ')') {
                    indentLevel--;
                    if (newLine) {
                        pseudoCode.append("\n");
                        pseudoCode.append(indentLevel * 4, ' ');
                        newLine = false;
                    }
                }
            }

            if (newLine) {
                pseudoCode.append("\n");
                pseudoCode.append(indentLevel * 4, ' ');  // Añadir indentación
                newLine = false;
            }
        }

        return pseudoCode;
    }
}
