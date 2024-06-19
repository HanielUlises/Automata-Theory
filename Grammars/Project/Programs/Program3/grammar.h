#ifndef PROGRAM3_GRAMMAR_H
#define PROGRAM3_GRAMMAR_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

namespace Program3 {

class Grammar {
private:
    int maxLength;
    std::ofstream logFile;
    std::string logContent;  // For storing log data
    std::vector<std::string> derivationSteps;  // For storing derivation steps

    bool parseB(const std::string& input, size_t& pos);
    bool parseR(const std::string& input, size_t& pos);

    void logStep(const std::string& remainingInput, const std::string& nextSymbol, const std::string& derivation);

public:
    Grammar(int length);
    ~Grammar();
    void parse(const std::string& input);
    std::string generateRandomBalancedParentheses(int maxDepth);
    void writeDerivationsToFile(const std::string& filename);  // Function to write logs to a file
};

} // namespace Program3

#endif // PROGRAM3_GRAMMAR_H
