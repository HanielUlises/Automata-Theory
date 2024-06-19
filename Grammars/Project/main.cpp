#include "Programs/Program1/PDA.h"
#include "Programs/Program2/grammar.h"
#include "Programs/Program3/grammar.h"
#include <iostream>
#include <random>
#include <stack>

using namespace Program2;
using namespace Program3;

std::string generateRandomBalancedParentheses(int maxDepth) {
    std::string str;
    std::stack<char> s;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (int i = 0; i < maxDepth; ++i) {
        if (s.empty() || dis(gen) == 0) {
            str.push_back('(');
            s.push('(');
        } else {
            str.push_back(')');
            s.pop();
        }
    }

    while (!s.empty()) {
        str.push_back(')');
        s.pop();
    }

    return str;
}

void runProgram1() {
    PDA pda;
    std::string input_string;

    std::cout << "Enter 'manual' to input a string or 'random' for a random string: ";
    std::string choice;
    std::cin >> choice;

    if (choice == "manual") {
        std::cout << "Enter a string of form 0^n 1^n: ";
        std::cin >> input_string;
    } else if (choice == "random") {
        input_string = generateRandomString();
        std::cout << "Generated random string: " << input_string.substr(0, 50) 
                  << "..." << input_string.substr(input_string.size() - 50) 
                  << " (length: " << input_string.size() << ")\n";
    } else {
        std::cerr << "Invalid choice" << std::endl;
        return;
    }

    bool result = pda.processString(input_string);
    std::cout << "The input string is " << (result ? "accepted" : "rejected") << " by the PDA." << std::endl;
    
    std::string log_filename = "pda_log.txt";
    pda.logIdSequence(log_filename);
    std::cout << "Detailed evaluation log has been written to " << log_filename << std::endl;

    std::cout << "Do you want to visualize the PDA operation? (yes/no): ";
    std::string visualize_choice;
    std::cin >> visualize_choice;

    if (visualize_choice == "yes") {
        pda.visualize();
    }
}

void runProgram2() {
    int maxDerivations;
    std::cout << "Ingrese el número máximo de derivaciones (límite de 1000): ";
    std::cin >> maxDerivations;
    if (maxDerivations > 1000) {
        maxDerivations = 1000;
    }

    Program2::Grammar grammar(maxDerivations);
    grammar.derive();
    grammar.writeDerivationsToFile("derivations.txt");
    grammar.writePseudoCodeToFile("pseudocode.txt");

    std::cout << "Derivaciones y pseudo-código generados y guardados en archivos." << std::endl;
}

void runProgram3() {
    int maxDepth;
    std::string inputString;

    std::cout << "Enter the maximum depth for derivations (limit of 1000): ";
    std::cin >> maxDepth;
    if (maxDepth > 1000) {
        maxDepth = 1000;
    }

    std::cout << "Enter 'manual' to input a string or 'random' for a random string: ";
    std::string choice;
    std::cin >> choice;

    Program3::Grammar grammar(maxDepth);

    if (choice == "manual") {
        std::cout << "Enter a balanced parentheses string: ";
        std::cin >> inputString;
    } else if (choice == "random") {
        inputString = generateRandomBalancedParentheses(maxDepth);
        std::cout << "Generated string: " << inputString << "\n";
    } else {
        std::cerr << "Invalid choice." << std::endl;
        return;
    }

    grammar.parse(inputString);
    grammar.writeDerivationsToFile("derivations.txt");

    std::cout << "Parsing complete. Derivations saved to 'derivations.txt'." << std::endl;
}

int main() {
    std::cout << "Select the program you wish to run:\n";
    std::cout << "1. PDA Program\n";
    std::cout << "2. Backus-Naur Form Grammar for IF\n";
    std::cout << "3. Balanced Parentheses Grammar\n";
    std::cout << "Enter the program number: ";

    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            runProgram1();
            break;
        case 2:
            runProgram2();
            break;
        case 3:
            runProgram3();
            break;
        default:
            std::cerr << "Invalid option. Please choose 1, 2, or 3." << std::endl;
            return 1;
    }

    return 0;
}
