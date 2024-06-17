#ifndef NET_H
#define NET_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <cmath>

struct Point {
    int x, y;
};

class Net {
    public:
        explicit Net(const std::string& path, int player);
        ~Net();

        void draw();

    private:
        SDL_Renderer* renderer;
        SDL_Window* window;
        TTF_Font* font;
        std::string path;
        std::vector<std::vector<int>> winnerMoves;
        std::unordered_map<int, std::unordered_set<int>> uniqueStates;
        std::unordered_map<int, Point> statePositions;

        void initialize();
        void loadWinnerMoves();
        void createUniqueStateSet();
        void determineLayout(std::unordered_map<int, Point>& statePositions);
        void drawCircle(const Point& center, int radius, const std::string& text, const SDL_Color& color);
        void drawArrow(const Point& start, const Point& end);

        int initialState;
        int finalState;
        int player;
};

#endif // NET_H
