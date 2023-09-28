#include <headers/game.h>
#include <headers/window.h>
#include <iostream>

int Game::boardTopLeftX;
int Game::boardTopLeftY;

Game::~Game() {
    delete(board);
}

void Game::init() {
    isAutoPlay = false;
    boardTopLeftX = boardTopLeftY = 0;
    board = new Board;
    currentBoardPosition = board->init();
}

void Game::render() {
    board->render(currentBoardPosition);
}

void Game::resize() {
    board->resize();
}

void Game::move(char move) {
    currentBoardPosition = board->makeMove(move, currentBoardPosition);
    board->evaluatePosition(currentBoardPosition);
}

void Game::autoMove() {
    char CPUMove = board->autoMove(currentBoardPosition);
    move(CPUMove);
}

void Game::reset() {
    board->reset(currentBoardPosition);
}

void Game::toggleAutoPlay() {
    isAutoPlay = !isAutoPlay;
}