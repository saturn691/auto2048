#pragma once
#include <sdl/SDL.h>
#include <sdl/SDL_image.h>
#include <board.h>
#include <string>
#include <vector>

class Game {
private:
    Board* board;
    std::vector<std::vector<int>> currentBoardPosition;
public:
    static int boardTopLeftX;
    static int boardTopLeftY;
    bool isAutoPlay;
    
    ~Game();

    void init();
    void render();
    void resize();
    void move(char move);
    void autoMove();
    void reset();
    void toggleAutoPlay();
};