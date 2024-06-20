#include "TuringMachine.h"
#include <fstream>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

TuringMachine::TuringMachine() {
    // States and transitions
    initialState = "q0";
    acceptStates = {"q4"};
    rejectStates = {"q_reject"};

    // Transitions for the Turing Machine to recognize 0^n1^n
    transitions["q0"]['0'] = {'X', 'R', "q1"};
    transitions["q0"]['Y'] = {'Y', 'R', "q3"};

    transitions["q1"]['0'] = {'0', 'R', "q1"};
    transitions["q1"]['1'] = {'Y', 'L', "q2"};
    transitions["q1"]['Y'] = {'Y', 'R', "q1"};

    transitions["q2"]['0'] = {'0', 'L', "q2"};
    transitions["q2"]['X'] = {'X', 'R', "q0"};
    transitions["q2"]['Y'] = {'Y', 'L', "q2"};

    transitions["q3"]['Y'] = {'Y', 'R', "q3"};
    transitions["q3"]['B'] = {'B', 'R', "q4"};
}

bool TuringMachine::process(const std::string& input) {
    std::string tape = input + "B";
    std::string currentState = initialState;
    int head = 0;

    while (true) {
        char currentChar = tape[head];
        if (transitions[currentState].find(currentChar) == transitions[currentState].end()) {
            break;
        }

        Transition t = transitions[currentState][currentChar];
        tape[head] = t.write;
        head += (t.move == 'R' ? 1 : -1);
        currentState = t.nextState;

        if (acceptStates.find(currentState) != acceptStates.end()) {
            return true;
        }
        if (rejectStates.find(currentState) != rejectStates.end()) {
            return false;
        }
    }
    return false;
}

void TuringMachine::printInstantaneousDescriptions(const std::string& input, const std::string& filename) {
    std::ofstream file(filename);
    std::string tape = input + "B";
    std::string currentState = initialState;
    int head = 0;

    while (true) {
        file << "State: " << currentState << ", Tape: " << tape << ", Head Position: " << head << "\n";
        char currentChar = tape[head];
        if (transitions[currentState].find(currentChar) == transitions[currentState].end()) {
            break;
        }

        Transition t = transitions[currentState][currentChar];
        tape[head] = t.write;
        head += (t.move == 'R' ? 1 : -1);
        currentState = t.nextState;

        if (acceptStates.find(currentState) != acceptStates.end() || rejectStates.find(currentState) != rejectStates.end()) {
            break;
        }
    }
    file.close();
}

void TuringMachine::visualize(const std::string& input) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Window* window = SDL_CreateWindow("Turing Machine Visualization",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          1200, 400, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return;
    }

    TTF_Font* font = TTF_OpenFont("LinLibertine_aBS.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return;
    }

    std::string tape = input + "B";
    std::string currentState = initialState;
    int head = 0;

    while (true) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw the tape
        int cellSize = 60;
        int tapeStartX = (1200 - static_cast<int>(tape.size()) * cellSize) / 2;
        int tapeStartY = 200;

        for (size_t i = 0; i < tape.size(); ++i) {
            SDL_Rect cellRect = {tapeStartX + static_cast<int>(i) * cellSize, tapeStartY, cellSize, cellSize};
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &cellRect);

            SDL_Rect innerRect = {cellRect.x + 5, cellRect.y + 5, cellSize - 10, cellSize - 10};
            SDL_RenderDrawRect(renderer, &innerRect);

            // Character within cell
            SDL_Color textColor = {0, 0, 0, 255};
            std::string cellText(1, tape[i]);
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, cellText.c_str(), textColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            int textWidth = textSurface->w;
            int textHeight = textSurface->h;
            SDL_FreeSurface(textSurface);
            SDL_Rect textRect = {cellRect.x + (cellSize - textWidth) / 2, cellRect.y + (cellSize - textHeight) / 2, textWidth, textHeight};
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);

            // Cell being scanned
            if (i == static_cast<size_t>(head)) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderDrawLine(renderer, cellRect.x + cellSize / 2, cellRect.y - 20, cellRect.x + cellSize / 2, cellRect.y - 5);
                SDL_RenderDrawLine(renderer, cellRect.x + cellSize / 2, cellRect.y - 20, cellRect.x + cellSize / 2 - 5, cellRect.y - 15);
                SDL_RenderDrawLine(renderer, cellRect.x + cellSize / 2, cellRect.y - 20, cellRect.x + cellSize / 2 + 5, cellRect.y - 15);
            }
        }

        // Current state
        SDL_Color textColor = {0, 0, 0, 255};
        std::string stateDisplay = "State: " + currentState + ", Head Position: " + std::to_string(head);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, stateDisplay.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect stateRect = {50, 50, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, nullptr, &stateRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        SDL_RenderPresent(renderer);
        SDL_Delay(5000);

        char currentChar = tape[head];
        if (transitions[currentState].find(currentChar) == transitions[currentState].end()) {
            break;
        }

        Transition t = transitions[currentState][currentChar];
        tape[head] = t.write;
        head += (t.move == 'R' ? 1 : -1);
        currentState = t.nextState;

        if (acceptStates.find(currentState) != acceptStates.end() || rejectStates.find(currentState) != rejectStates.end()) {
            break;
        }
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
