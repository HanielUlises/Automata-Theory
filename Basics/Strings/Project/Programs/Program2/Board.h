#ifndef BOARD_H
#define BOARD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "NFA.h"

class Board{
    public:
        Board();
        Board(NFA player_1, NFA player_2, int string_size);
        ~Board();
        std::vector<State*> reconfigure_route (NFA& player, const std::vector<std::vector<State*>>& all_winner_routes, size_t current_index);
        void draw_board();

    private:
        NFA player_1, player_2;
        int string_size;
        SDL_Texture* loadTexture(std::string path, SDL_Renderer* renderer);

        SDL_Texture* white_king_texture;
        SDL_Texture* black_king_texture;
};

#endif // BOARD_H
