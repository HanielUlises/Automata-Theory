#include "Automaton.h"

int main() {
    std::string input;
    std::cout << "Enter a string: ";
    std::cin >> input;

    if (processString(input)) {
        std::cout << "The string is accepted." << std::endl;
    } else {
        std::cout << "The string is not accepted." << std::endl;
    }

    return 0;
}
