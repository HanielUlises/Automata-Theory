#ifndef PDA_H
#define PDA_H

#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class PDA {
public:
    PDA();
    void reset();
    bool transition(char input);
    bool processString(const std::string& str);
    void logIdSequence(const std::string& filename);
    void visualize();

private:
    struct ID {
        std::string state;
        char input;
        std::stack<char> stack_before;
        std::stack<char> stack_after;
    };

    std::stack<char> stack;
    std::string state;
    std::vector<ID> id_sequence;

    std::string stackToString(const std::stack<char>& stack);
    void drawStack(SDL_Renderer* renderer, const std::stack<char>& stack, int x, int y, TTF_Font* font);
    void drawInputSymbol(SDL_Renderer* renderer, char input, int x, int y, TTF_Font* font);
    void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, TTF_Font* font, SDL_Color color);
};

std::string generateRandomString(size_t max_length = 100000);

#endif // PDA_H
