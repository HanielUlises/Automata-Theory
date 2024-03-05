#include "Automaton.h"

int main() {
    std::string input;
    std::cout << "Enter a binary string: ";
    std::cin >> input;

    Automaton automaton(input);
    if (automaton.checkParity()) {
        std::cout << "The string " << input << " has even parity.\n";
    } else {
        std::cout << "Not accepted " << input << " does not have even parity.\n";
    }

    return 0;
}
