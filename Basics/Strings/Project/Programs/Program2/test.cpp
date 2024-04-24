#include <iostream>
#include <memory>

#include "NFA.h"
#include "Board.h"

void test_automatons (){
    // States
    auto s0 = std::make_unique<State>(0);
    auto s1 = std::make_unique<State>(1);
    auto s2 = std::make_unique<State>(2);
    auto s3 = std::make_unique<State>(3);
    auto s4 = std::make_unique<State>(4);
    auto s5 = std::make_unique<State>(5);
    auto s6 = std::make_unique<State>(6);
    auto s7 = std::make_unique<State>(7);
    auto s8 = std::make_unique<State>(8);
    auto s9 = std::make_unique<State>(9);
    auto s10 = std::make_unique<State>(10);
    auto s11 = std::make_unique<State>(11);
    auto s12 = std::make_unique<State>(12, true);
    auto s13 = std::make_unique<State>(13);
    auto s14 = std::make_unique<State>(14);
    auto s15 = std::make_unique<State>(15, true);

    NFA nfa_1(s0.get(), s15.get());
    bool first_wins;
    NFA nfa_2(s3.get(), s12.get());
    bool second_wins;

    // S0 transitions
    s0->add_transition('r', s1.get());
    s0->add_transition('r', s4.get());
    s0->add_transition('b', s5.get());

    // S1 transitions
    s1->add_transition('b', s0.get());
    s1->add_transition('b', s2.get());
    s1->add_transition('r', s4.get());
    s1->add_transition('b', s5.get());
    s1->add_transition('r', s6.get());

    // S2 transitions
    s2->add_transition('r', s1.get());
    s2->add_transition('r', s3.get());
    s2->add_transition('b', s5.get());
    s2->add_transition('r', s6.get());
    s2->add_transition('b', s7.get());

    // S3 transitions
    s3->add_transition('b', s2.get());
    s3->add_transition('r', s6.get());
    s3->add_transition('b', s7.get());

    // S4 transitions
    s4->add_transition('b', s0.get());
    s4->add_transition('r', s1.get());
    s4->add_transition('b', s5.get());
    s4->add_transition('b', s8.get());
    s4->add_transition('r', s9.get());

    // S5 transitions
    s5->add_transition('b', s0.get());
    s5->add_transition('r', s1.get());
    s5->add_transition('b', s2.get());
    s5->add_transition('r', s4.get());
    s5->add_transition('r', s6.get());
    s5->add_transition('b', s8.get());
    s5->add_transition('r', s9.get());
    s5->add_transition('b', s10.get());

    // S6 transitions
    s6->add_transition('r', s1.get());
    s6->add_transition('b', s2.get());
    s6->add_transition('r', s3.get());
    s6->add_transition('b', s5.get());
    s6->add_transition('b', s7.get());
    s6->add_transition('r', s9.get());
    s6->add_transition('b', s10.get());
    s6->add_transition('r', s11.get());

    // S7 transitions
    s7->add_transition('b', s2.get());
    s7->add_transition('r', s3.get());
    s7->add_transition('r', s6.get());
    s7->add_transition('b', s10.get());
    s7->add_transition('r', s11.get());

    // S8 transitions
    s8->add_transition('r', s4.get());
    s8->add_transition('b', s5.get());
    s8->add_transition('r', s9.get());
    s8->add_transition('r', s12.get());
    s8->add_transition('b', s13.get());

    // S9 transitions
    s9->add_transition('r', s4.get());
    s9->add_transition('b', s5.get());
    s9->add_transition('r', s6.get());
    s9->add_transition('b', s8.get());
    s9->add_transition('b', s10.get());
    s9->add_transition('r', s12.get());
    s9->add_transition('b', s13.get());
    s9->add_transition('r', s14.get());

    // S10 transitions
    s10->add_transition('b', s5.get());
    s10->add_transition('r', s6.get());
    s10->add_transition('b', s7.get());
    s10->add_transition('r', s9.get());
    s10->add_transition('r', s11.get());
    s10->add_transition('r', s13.get());
    s10->add_transition('r', s14.get());
    s10->add_transition('b', s15.get());

    // S11 transitions
    s11->add_transition('r', s6.get());
    s11->add_transition('b', s7.get());
    s11->add_transition('b', s10.get());
    s11->add_transition('r', s14.get());
    s11->add_transition('b', s15.get());

    // S12 transitions

    s12->add_transition('b', s8.get());
    s12->add_transition('b', s13.get());
    s12->add_transition('r', s9.get());

    // S13 transitions

    s13->add_transition('b', s8.get());
    s13->add_transition('r', s9.get());
    s13->add_transition('b', s10.get());
    s13->add_transition('r', s12.get());
    s13->add_transition('r', s14.get());

    // S14 transitions

    s14->add_transition('r', s9.get());
    s14->add_transition('b', s10.get());
    s14->add_transition('r', s11.get());
    s14->add_transition('b', s13.get());
    s14->add_transition('b', s15.get());

    // S15 transitions
    s15->add_transition('b', s10.get());
    s15->add_transition('r', s11.get());
    s15->add_transition('r', s14.get());

    // Parameters for both players
    int string_size;
    std::string first, second;

    while(true){
        std::cout << "Size of strings: ";
        std::cin >> string_size;
        // Boundaries
        if(string_size >= 4 && string_size <= 100){
            while(true){
                std::cout << "String for first player's moves" << std::endl;
                std::cin >> first;
                if(!nfa_1.is_accepted(first) || first.size() != string_size){
                    std::cout << "Not valid string for player 1" << std::endl;
                }else{
                    break;
                }
            }

            while(true){
                std::cout << "String for second player's moves" << std::endl;
                std::cin >> second;
                if(!nfa_2.is_accepted(second) || second.size() != string_size){
                    std::cout << "Not valid string for player 2" << std::endl;
                }else{
                    break;
                }
            }

            if(first.size() == second.size()){
                break;
            }else{
                std::cout << "Strings do not have the same size" << std::endl;
            }
        }else{
            std::cout << "Not within the boundaries" << std::endl;
        }
    }
    std::cout << "Transitions for the first player" << std::endl;
    first_wins = nfa_1.print_paths(first, "1");
    std::cout << "Transitions for the second player" << std::endl;
    second_wins = nfa_2.print_paths(second, "2");

    if(first_wins && second_wins){
        Board newBoard (nfa_1, nfa_2, string_size);
        newBoard.draw_board();
    }else{
        std::cerr << "Non of players are winners" << std::endl;
    }
}
