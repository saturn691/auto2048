#pragma once
#include <sdl/SDL.h>
#include <sdl/SDL_image.h>
#include <box.h>
#include <vector>
#include <queue>

class Board {
private:
    SDL_Color paddingColor;
public:
    static int numBoxesX;
    static int numBoxesY;
    static int boxWidth;
    static int boxHeight;

    std::vector<std::vector<int>> init();
    void reset(std::vector<std::vector<int>>& boardPosition);
    void render(std::vector<std::vector<int>> boardPosition);
    void renderBoard(std::vector<std::vector<int>> boardPosition);
    void renderBox(Box box);
    void resize();
    void generateTile(std::vector<std::vector<int>>& boardPosition);
    void updatePosition(std::queue<int>& queuedValues, int i, int j, std::vector<std::vector<int>>& boardPosition);
    std::vector<std::vector<int>> makeMove(char move, std::vector<std::vector<int>> boardPosition);
    bool checkForLoss(std::vector<std::vector<int>> boardPosition);
    int evaluatePosition(std::vector<std::vector<int>> boardPosition);
    int recursiveGetScore(int depth, std::vector<std::vector<int>> boardPosition);
    char autoMove(std::vector<std::vector<int>> boardPosition);
};