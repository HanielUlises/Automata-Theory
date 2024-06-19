#include "Programs/Program1/PDA.h"

#include <iostream>

int main() {
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
        return 1;
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

    return 0;
}
