#include <iostream>
#include <string>

#include "Programs/Program1/Universe.h"
#include "Programs/Program2/NFA.h"
#include "Programs/Program3/Automaton.h"

int main(int argc, char *argv[]) {
    int option;
    std::cout << "Seleccione un programa: " << std::endl;
    std::cout << std:: endl;
    std::cout << "1) Universo" << std::endl;
    std::cout << "2) NFA ajedrez" << std::endl;
    std::cout << "3) DFA palabras" << std::endl;
    std::cin >> option;

    switch (option){
    case 1:
        main_func();
        break;
    case 2:
        test_automatons();
        break;
    case 3:
        run_program_3();
        break;
    default:
        break;
    }
}