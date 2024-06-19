#include "PDA.h"
#include <ctime>
#include <cstdlib>

PDA::PDA() {
    reset();
}

void PDA::reset() {
    while (!stack.empty()) stack.pop();
    stack.push('Z');
    state = "q0";
    id_sequence.clear();
}

bool PDA::transition(char input) {
    std::stack<char> current_stack = stack;
    if (state == "q0") {
        if (input == '0') {
            stack.push('0');
        } else if (input == '1' && stack.top() == '0') {
            stack.pop();
        } else {
            state = "reject";
            return false;
        }
        id_sequence.push_back({state, input, current_stack, stack});
        return true;
    }
    return false;
}

bool PDA::processString(const std::string& str) {
    reset();
    for (char ch : str) {
        if (!transition(ch)) {
            break;
        }
    }
    if (stack.size() == 1 && stack.top() == 'Z') {
        state = "accept";
    } else {
        state = "reject";
    }
    id_sequence.push_back({state, '\0', stack, stack});
    return state == "accept";
}

void PDA::logIdSequence(const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& id : id_sequence) {
        file << "State: " << id.state << ", Input: " << id.input 
             << ", Stack before: " << stackToString(id.stack_before) 
             << ", Stack after: " << stackToString(id.stack_after) << "\n";
    }
}

std::string PDA::stackToString(const std::stack<char>& stack) {
    std::stack<char> temp = stack;
    std::string result;
    while (!temp.empty()) {
        result += temp.top();
        temp.pop();
    }
    return result;
}

std::string generateRandomString(size_t max_length) {
    srand(time(0));
    size_t n = rand() % (max_length / 2) + 1;
    return std::string(n, '0') + std::string(n, '1');
}

void PDA::visualize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    SDL_Window *win = SDL_CreateWindow("PDA Visualization", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    if (win == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        SDL_DestroyWindow(win);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    TTF_Font* font = TTF_OpenFont("LinLibertine_aBS.ttf", 24);
    if (font == nullptr) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return;
    }

    for (const auto& id : id_sequence) {
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);

        drawStack(ren, id.stack_before, 350, 400, font);
        if (id.input != '\0') {
            drawInputSymbol(ren, id.input, 100, 400, font);
        }
        SDL_RenderPresent(ren);

        SDL_Delay(1500);

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);
        drawStack(ren, id.stack_after, 350, 400, font);
        SDL_RenderPresent(ren);

        SDL_Delay(1500);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
}

void PDA::drawStack(SDL_Renderer* renderer, const std::stack<char>& stack, int x, int y, TTF_Font* font) {
    std::stack<char> temp = stack;
    int i = 0;
    while (!temp.empty()) {
        SDL_Rect rect = { x, y - i * 40, 50, 40 };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &rect);
        renderText(renderer, std::string(1, temp.top()), x + 15, y - i * 40 + 5, font, {0, 0, 0, 255});
        temp.pop();
        i++;
    }
}

void PDA::drawInputSymbol(SDL_Renderer* renderer, char input, int x, int y, TTF_Font* font) {
    SDL_Rect rect = { x, y, 50, 50 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);

    renderText(renderer, std::string(1, input), x + 15, y + 10, font, {255, 255, 255, 255});
}

void PDA::renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, TTF_Font* font, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
