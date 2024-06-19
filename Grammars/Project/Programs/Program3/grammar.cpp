#include "grammar.h"
#include <iostream>
#include <fstream>
#include <random>

namespace Program3 {

Grammar::Grammar(int length) : maxLength(length) {
    logContent.clear();
}

Grammar::~Grammar() {
}

// Logs the current state of the parsing process
void Grammar::logStep(const std::string& remainingInput, const std::string& nextSymbol, const std::string& derivation) {
    logContent += "Remaining Input: " + remainingInput + "\n";
    logContent += "Next symbol: " + nextSymbol + "\n";
    logContent += "Steps of leftmost derivation:\n";
    for (const auto& step : derivationSteps) {
        logContent += step + "\n";
    }
    logContent += derivation + "\n\n";
    derivationSteps.push_back(derivation);
}

// Parses according to B -> (RB | ε
bool Grammar::parseB(const std::string& input, size_t& pos) {
    if (pos >= input.length()) {
        logStep(input.substr(pos), "ε", "B -> ε");
        return true; // ε production
    }

    if (input[pos] == '(') {
        logStep(input.substr(pos), "(", "B -> (RB");
        pos++; // consume '('
        if (!parseR(input, pos)) {
            return false; // R production failed
        }
        if (pos >= input.length() || input[pos] != ')') {
            return false; // Ensure matching ')'
        }
        pos++; // consume ')'
        if (!parseB(input, pos)) {
            return false; // Ensure subsequent B production
        }
        return true;
    }

    return false; // no matching production
}

// Parses according to R -> ) | (RR
bool Grammar::parseR(const std::string& input, size_t& pos) {
    if (pos < input.length() && input[pos] == ')') {
        logStep(input.substr(pos), ")", "R -> )");
        pos++; // consume ')'
        return true;
    } else if (pos < input.length() && input[pos] == '(') {
        logStep(input.substr(pos), "(", "R -> (RR");
        pos++; // consume '('
        if (!parseR(input, pos)) {
            return false; // first R production failed
        }
        if (pos >= input.length() || input[pos] != ')') {
            return false; // Ensure matching ')'
        }
        pos++; // consume ')'
        if (!parseR(input, pos)) {
            return false; // second R production failed
        }
        return true;
    }

    return false; // no matching production
}

void Grammar::parse(const std::string& input) {
    size_t pos = 0;
    logContent += "Attempting to parse: " + input + "\n";
    derivationSteps.clear();  // Clear previous derivation steps

    if (parseB(input, pos) && pos == input.size()) {
        std::cout << "Successfully parsed: " << input << std::endl;
    } else {
        std::cout << "Parsing failed for input: " << input << std::endl;
    }
}

std::string Grammar::generateRandomBalancedParentheses(int maxDepth) {
    std::string str;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    int open = 0;

    for (int i = 0; i < 2 * maxDepth; ++i) {
        if (dis(gen) % 2 == 0 && open < maxDepth) {
            str += '(';
            open++;
        } else if (open > 0) {
            str += ')';
            open--;
        }
    }

    while (open > 0) {
        str += ')';
        open--;
    }

    return str;
}

void Grammar::writeDerivationsToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << logContent;
        file.close();
    }
}

} // namespace Program3
