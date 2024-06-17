#include "Net.h"
#include <fstream>
#include <sstream>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <algorithm>
#include <map>


Net::Net(const std::string& path, int player)
    : renderer(nullptr), window(nullptr), font(nullptr), path(path), player(player) {
    initialize();
    loadWinnerMoves();
    createUniqueStateSet();
    determineLayout(statePositions);
}

Net::~Net() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    if (font) {
        TTF_CloseFont(font);
    }
    TTF_Quit();
    SDL_Quit();
}

// Initialization of SDL and TTF, creating the window and renderer
void Net::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }

    window = SDL_CreateWindow("Winner paths", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    font = TTF_OpenFont("LinuxLibertineDisplay-23pl.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }
}

// Reads winner moves from the winner_moves_x.txt where x is the number of the player
void Net::loadWinnerMoves() {
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> path;
        std::string state;
        while (std::getline(iss, state, '>')) {
            if (!state.empty() && state[0] == 'S') {
                path.push_back(std::stoi(state.substr(1)));
            }
        }
        winnerMoves.push_back(path);
    }
}

// Creates a unique state set with their connections
void Net::createUniqueStateSet() {
    for (const auto& path : winnerMoves) {
        for (size_t i = 0; i < path.size(); ++i) {
            if (i < path.size() - 1) {
                uniqueStates[path[i]].insert(path[i + 1]);
            }
        }
    }

    // Initial and final states determined by player number
    initialState = (player == 1) ? 0 : 3;
    finalState = (player == 1) ? 15 : 12;
}

void Net::determineLayout(std::unordered_map<int, Point>& statePositions) {
    const int HORIZONTAL_SPACING = 80;
    const int VERTICAL_INCREMENT = 70;

    std::unordered_map<int, int> stateDepths; // Store depth for each state
    std::queue<int> queue; // For BFS

    // Initial states for each player
    // S_0 is the initial state for player 1
    // S_3 is the initial state for player 2
    initialState = (player == 1) ? 0 : 3;
    queue.push(initialState);
    stateDepths[initialState] = 0;

    // Important!!
    // BFS was used to determine the depth of each state
    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        for (int adjacent : uniqueStates[current]) {
            if (stateDepths.count(adjacent) == 0) {
                queue.push(adjacent);
                stateDepths[adjacent] = stateDepths[current] + 1;
            }
        }
    }

    // Determine X positions based on depths
    std::map<int, std::vector<int>> levels; // Store states by level
    for (const auto& stateDepthPair : stateDepths) {
        levels[stateDepthPair.second].push_back(stateDepthPair.first);
    }

    // Determine positions starting from the top-left, moving down and to the right
    for (const auto& level : levels) {
        int y = 50 + level.first * VERTICAL_INCREMENT; // Vertical position based on level
        int numStates = level.second.size();
        int x = (800 - (numStates - 1) * HORIZONTAL_SPACING) / 2; // Center the states on the level

        for (int state : level.second) {
            statePositions[state] = {x, y};
            x += HORIZONTAL_SPACING; // Move to the next horizontal position
        }
    }

    // Final state is the one with the highest depth
    finalState = (--levels.end())->second.front(); 
}

// Draw the NFA visualization
void Net::draw() {
    // Clear the window
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    determineLayout(statePositions);

    // Circles for states
    for (const auto& [state, position] : statePositions) {
        SDL_Color color = (state == initialState || state == finalState) ? SDL_Color{255, 0, 0, 255} : SDL_Color{0, 0, 255, 255};
        drawCircle(position, 20, "S" + std::to_string(state), color);
    }

    // Arrows for transitions
    for (const auto& [state, nextStates] : uniqueStates) {
        for (int nextState : nextStates) {
            drawArrow(statePositions[state], statePositions[nextState]);
        }
    }

    determineLayout(statePositions);

    SDL_RenderPresent(renderer);

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }
}

// Draw a circle with text at the center
void Net::drawCircle(const Point& center, int radius, const std::string& text, const SDL_Color& color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

    // Draw the circle
    for (int w = -radius; w < radius; w++) {
        for (int h = -radius; h < radius; h++) {
            if (w * w + h * h <= radius * radius) {
                SDL_RenderDrawPoint(renderer, center.x + w, center.y + h);
            }
        }
    }

    // Draw the text 
    // This text is just the state of the node
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), {255, 255, 255});
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = {
        center.x - surface->w / 2,
        center.y - surface->h / 2,
        surface->w,
        surface->h
    };
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Draw an arrow between two points
void Net::drawArrow(const Point& start, const Point& end) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);

}
