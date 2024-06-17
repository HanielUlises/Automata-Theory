#include "Board.h"

#include "Net.h"

const int screenWidth = 800;
const int screenHeight = 800;
const int tileSize = 200;
const int boardSize = 4;

Board::Board(){
}

Board::Board(NFA player_1, NFA player_2, int string_size) 
    : player_1(player_1), player_2(player_2), string_size(string_size), 
      white_king_texture(nullptr), black_king_texture(nullptr) {
}

Board::~Board() {
}

SDL_Texture* Board::loadTexture(std::string path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        std::cout << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
    } else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr) {
            std::cout << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void Board::draw_board() {
    srand(static_cast<unsigned int>(time(nullptr)));
    bool isWhiteTurn = rand() % 2 == 0;

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init(); 

    SDL_Window* window = SDL_CreateWindow("NFA Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("LinuxLibertineDisplay-23pl.ttf", 24);
    if (font == nullptr) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }

    white_king_texture = loadTexture("king_white.png", renderer);
    black_king_texture = loadTexture("king_black.png", renderer);

    std::vector<std::vector<State*>> first_winner_moves = player_1.get_winner_moves();
    std::vector<std::vector<State*>> second_winner_moves = player_2.get_winner_moves();

    int whiteMoveIndex = 0, blackMoveIndex = 0;
    int whiteReconfigCount = 0, blackReconfigCount = 0;

    SDL_Event e;
    bool quit = false;
    std::string winnerMessage;
    bool winnerFound = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw the chessboard
        for (int y = 0; y < boardSize; ++y) {
            for (int x = 0; x < boardSize; ++x) {
                SDL_Rect rect = {x * tileSize, y * tileSize, tileSize, tileSize};
                if ((x + y) % 2 == 0) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black squares
                } else {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red squares
                }
                SDL_RenderFillRect(renderer, &rect);
            }
        }

        // Process player moves based on turns and path reconfiguration logic
        if (isWhiteTurn && !winnerFound) {
            if (whiteMoveIndex < first_winner_moves[0].size()) {
                int pos1 = first_winner_moves[0][whiteMoveIndex]->id;
                 // Ensuring there's a previous move by Black to compare
                if (blackMoveIndex > 0) {
                    int pos2 = second_winner_moves[0][blackMoveIndex - 1]->id;
                    if (pos1 == pos2) { // Need for reconfiguration
                        // Implement the reconfiguration logic for White
                        if (whiteReconfigCount >= first_winner_moves.size()) {
                            whiteReconfigCount = 0;
                        } else {
                            if (whiteReconfigCount + 1 < first_winner_moves.size()) {
                                first_winner_moves[0] = first_winner_moves[whiteReconfigCount + 1];
                                whiteReconfigCount++;
                            } else {
                                std::cout << "Out of moves" << std::endl;
                            }
                            whiteMoveIndex--; // Revisit this move
                        }
                    }
                }
                whiteMoveIndex++;
            }
            isWhiteTurn = false;
        } else if (!isWhiteTurn && !winnerFound) {
            if (blackMoveIndex < second_winner_moves[0].size()) {
                int pos2 = second_winner_moves[0][blackMoveIndex]->id;
                // Ensuring there's a previous move by White to compare
                if (whiteMoveIndex > 0) { 
                    int pos1 = first_winner_moves[0][whiteMoveIndex - 1]->id;
                    if (pos2 == pos1) { // Need for reconfiguration
                        // Implement the reconfiguration logic for Black
                        if (blackReconfigCount >= second_winner_moves.size()) {
                            blackReconfigCount = 0;
                        } else {
                            if (blackReconfigCount + 1 < second_winner_moves.size()) {
                                second_winner_moves[0] = second_winner_moves[blackReconfigCount + 1];
                                blackReconfigCount++;
                            } else {
                                std::cout << "Out of moves" << std::endl;
                            }
                            blackMoveIndex--;
                        }
                    }
                }
                blackMoveIndex++;
            }
            isWhiteTurn = true;
        }

        for (int y = 0; y < boardSize; ++y) {
            for (int x = 0; x < boardSize; ++x) {
                SDL_Rect rect = {x * tileSize, y * tileSize, tileSize, tileSize};
                int pos = y * boardSize + x;
                // Draw White piece if it's White's turn and the position matches
                if (whiteMoveIndex > 0 && pos == first_winner_moves[0][whiteMoveIndex - 1]->id) {
                    SDL_RenderCopy(renderer, white_king_texture, nullptr, &rect);
                }
                // Draw Black piece if it's Black's turn and the position matches
                if (blackMoveIndex > 0 && pos == second_winner_moves[0][blackMoveIndex - 1]->id) {
                    SDL_RenderCopy(renderer, black_king_texture, nullptr, &rect);
                }
            }
        }

        // Winner determination logic
        if (whiteMoveIndex >= first_winner_moves[0].size()) {
            winnerMessage = "White wins!";
            winnerFound = true;
        } else if (blackMoveIndex >= second_winner_moves[0].size()) {
            winnerMessage = "Black wins!";
            winnerFound = true;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1000);

        if (winnerFound) {
            break;
        }
    }

    // Displaying the winner
    if (winnerFound) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Prepare the text
        SDL_Color textColor = {255, 255, 255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, winnerMessage.c_str(), textColor);
        if (textSurface != nullptr) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture != nullptr) {

                // Text in the center of the screen
                int textWidth = textSurface->w;
                int textHeight = textSurface->h;
                SDL_Rect renderQuad = { (screenWidth - textWidth) / 2, (screenHeight - textHeight) / 2, textWidth, textHeight };

                SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
                
                SDL_RenderPresent(renderer);
                
                SDL_Delay(5000); 

                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        } else {
            std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        }
    }
    
    // Displaying the accepted paths of the winner
    if (winnerFound) {
        std::string winnerMovesFile = winnerMessage == "White wins!" ? "winner_moves1.txt" : "winner_moves2.txt";
        int player_num = winnerMessage == "White wins!" ? 1 : 2;
        Net net(winnerMovesFile, player_num);
        net.draw();

        SDL_Delay(5000);
    }

    SDL_DestroyTexture(white_king_texture);
    SDL_DestroyTexture(black_king_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}