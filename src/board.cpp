#include <headers/board.h>
#include <headers/window.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <algorithm>

int Board::numBoxesX;
int Board::numBoxesY;
int Board::boxWidth;
int Board::boxHeight;

std::vector<std::vector<int>> Board::init() {
    numBoxesX = 4;
    numBoxesY = 4;
    boxWidth = Window::screenWidth / numBoxesX;
    boxHeight = Window::screenHeight / numBoxesY;
    paddingColor = {255,0,0,255};
    std::vector<std::vector<int>> boardPosition = std::vector(numBoxesY, std::vector<int>(numBoxesX, 0));
    generateTile(boardPosition);
    
    return boardPosition;
}

void Board::reset(std::vector<std::vector<int>>& boardPosition) {
    for (int i = 0; i < numBoxesX; i++) {
        for (int j = 0; j < numBoxesY; j++) {
            boardPosition[i][j] = 0;
        }
    }
    generateTile(boardPosition);
}

void Board::render(std::vector<std::vector<int>> boardPosition) {
    renderBoard(boardPosition);
}

void Board::renderBoard(std::vector<std::vector<int>> boardPosition) {
    for (int i = 0; i < numBoxesX; i++) {
        for (int j = 0; j < numBoxesY; j++) {
            SDL_Color currentColor = Box::valueToColorMap[boardPosition[i][j]];
            renderBox({i,j, boardPosition[i][j]});
        }
    }
}

void Board::renderBox(Box box) {
    SDL_Rect rect;
    rect.w = boxWidth;
    rect.h = boxHeight;
    SDL_Color drawColor = Box::valueToColorMap[box.value];
    SDL_SetRenderDrawColor(Window::renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);

    rect.x = box.x * boxWidth;
    rect.y = box.y * boxHeight;
    SDL_RenderFillRect(Window::renderer, &rect);
}

void Board::resize() {
	boxWidth = Window::screenWidth / numBoxesX;
	boxHeight = Window::screenHeight / numBoxesY;

	if (boxWidth > boxHeight) {
		boxWidth = boxHeight;
	}
	else {
		boxHeight = boxWidth;
	}
}

void Board::generateTile(std::vector<std::vector<int>>& boardPosition) {
    // Works out how many tiles remain in the board
    std::vector<std::pair<int,int>> remainingTiles;
    for (int i = 0; i < numBoxesX; i++) {
        for (int j = 0; j < numBoxesY; j++) {
            if (boardPosition[i][j] == 0) {
                remainingTiles.push_back(std::make_pair(i, j));   
            }
        }
    }

    // If there are no tiles left in the board, then it is either game over or invalid
    if (remainingTiles.size() == 0) {
        return;
    }
    
    // Generates a random number which will determine where the tile will be
    srand(time(NULL));
    int randomNumber = rand() % remainingTiles.size();

    // Generates a new tile. There is a 10% chance that the new tile will be a "4"
    int first = remainingTiles[randomNumber].first;
    int second = remainingTiles[randomNumber].second;
    srand(time(NULL));
    if (rand() % 10 == 0) {
        boardPosition[first][second] = 4;
    }
    else {
        boardPosition[first][second] = 2;
    }
}

void Board::updatePosition(std::queue<int>& queuedValues, int i, int j, std::vector<std::vector<int>>& boardPosition) {
    if (!queuedValues.empty()){
        int valueToMerge = queuedValues.front();
        queuedValues.pop();
        if (!queuedValues.empty()) {
            if (queuedValues.front() == valueToMerge) {
                queuedValues.pop();
                boardPosition[i][j] = 2 * valueToMerge;
            }
            else {
                boardPosition[i][j] = valueToMerge;
            }
        }
        else {
            boardPosition[i][j] = valueToMerge;
        }
    }
    else {
        boardPosition[i][j] = 0;
    }
}

std::vector<std::vector<int>> Board::makeMove(char move, std::vector<std::vector<int>> boardPosition) {
    std::queue<int> queuedValues;
    std::vector<std::vector<int>> previousBoardPosition = boardPosition;
    if (move == 'l') {
        for (int j = 0; j < numBoxesY; j++) {
            // First, we put the numbers on the row/column into a queue
            for (int i = 0; i < numBoxesX; i++) {
                if (boardPosition[i][j] != 0) {
                    queuedValues.push(boardPosition[i][j]);
                }

            }

            // Then we update the board position
            for (int i = 0; i < numBoxesX; i++) {
                updatePosition(queuedValues, i, j, boardPosition);
            }
        }
    }
    else if (move == 'r') {
        for (int j = 0; j < numBoxesY; j++) {
            for (int i = numBoxesX - 1; i >= 0; i--) {
                if (boardPosition[i][j] != 0) {
                    queuedValues.push(boardPosition[i][j]);
                }

            }

            for (int i = numBoxesX - 1; i >= 0; i--) {
                updatePosition(queuedValues, i, j, boardPosition);
            }
        }
    }
    else if (move == 'd') {
        for (int i = 0; i < numBoxesX; i++) {
            for (int j = numBoxesY - 1; j >= 0; j--) {
                if (boardPosition[i][j] != 0) {
                    queuedValues.push(boardPosition[i][j]);
                }

            }

            for (int j = numBoxesY - 1; j >= 0; j--) {
                updatePosition(queuedValues, i, j, boardPosition);
            }
        }
    }
    else if (move == 'u') {
        for (int i = 0; i < numBoxesX; i++) {
            for (int j = 0; j < numBoxesY; j++) {
                if (boardPosition[i][j] != 0) {
                    queuedValues.push(boardPosition[i][j]);
                }

            }

            for (int j = 0; j < numBoxesY; j++) {
                updatePosition(queuedValues, i, j, boardPosition);
            }
        }

    }
    else {
        std::cerr << "Invalid move: " << move << std::endl;
    }

    // If nothing has changed, then don't do anything else
    if (boardPosition == previousBoardPosition) {
        return boardPosition;
    }

    generateTile(boardPosition);
    return boardPosition;
}

// Should only be called when there are 16 tiles on the board as it's an expensive function
bool Board::checkForLoss(std::vector<std::vector<int>> boardPosition) {
    if (makeMove('l', boardPosition) == boardPosition) {
        return true;
    }
    if (makeMove('r', boardPosition) == boardPosition) {
        return true;
    }
    if (makeMove('u', boardPosition) == boardPosition) {
        return true;
    }
    if (makeMove('d', boardPosition) == boardPosition) {
        return true;
    }
    return false;
}

// Rules to determine the "better" move:
// 1. Does not lose the game
// 2. Reduces the amount of blocks in the game 
int Board::evaluatePosition(std::vector<std::vector<int>> boardPosition) {
    int numberOfBlocks = 0;
    for (int i = 0; i < numBoxesX; i++) {
        for (int j = 0; j < numBoxesY; j++) {
            if (boardPosition[i][j] != 0) {
                numberOfBlocks++;
            }
        }
    }

    if (numberOfBlocks == 16) {
        if (checkForLoss(boardPosition)) {
            return INT_MIN + 1;
        }
    }

    std::unordered_map<int, std::pair<int,int>> priorityToCoordinate = {
        {0, {3,3}}, {1, {3,2}}, {2, {3,1}}, {3, {3,0}},
        {4, {2,0}}, {5, {2,1}}, {6, {2,2}}, {7, {2,3}},
        {8, {1,3}}, {9, {1,2}}, {10, {1,1}}, {11, {1,0}},
        {12, {0,0}}, {13, {0,1}}, {14, {0,2}}, {15, {0,3}}
    };

    int chainLength = 0;
    for (int i = 0; i <= 15; i++) {
        std::pair<int,int> coord = priorityToCoordinate[i];
        std::pair<int,int> priorCoord = priorityToCoordinate[i-1];
        if (i == 0) {
            if (boardPosition[coord.first][coord.second] != 0) {
                chainLength++;
            }
            else {
                break;
            }
        }
        else {
            if (boardPosition[coord.first][coord.second] == 0) {
                break;
            }
            else if (boardPosition[coord.first][coord.second] > boardPosition[priorCoord.first][priorCoord.second]) {
                break;
            }
            else {
                chainLength++;
            }
        }
    }

    return 2 * (16-numberOfBlocks) + chainLength; 
}

int Board::recursiveGetScore(int depth, std::vector<std::vector<int>> boardPosition) {
    if (depth <= 1) {
        return evaluatePosition(boardPosition);
    }

    return std::max({
        recursiveGetScore(depth-1, makeMove('l', boardPosition)),
        recursiveGetScore(depth-1, makeMove('r', boardPosition)),
        recursiveGetScore(depth-1, makeMove('u', boardPosition)),
        recursiveGetScore(depth-1, makeMove('d', boardPosition))
    });
}

char Board::autoMove(std::vector<std::vector<int>> boardPosition) {
    // 5 depth testing of algorithm
    int depth = 5;

    std::vector<std::vector<int>> leftPosition = makeMove('l', boardPosition);
    std::vector<std::vector<int>> rightPosition = makeMove('r', boardPosition);
    std::vector<std::vector<int>> upPosition = makeMove('u', boardPosition);
    std::vector<std::vector<int>> downPosition = makeMove('d', boardPosition);

    int l_score = (leftPosition != boardPosition) ? recursiveGetScore(depth, leftPosition) - 10: INT_MIN;
    int r_score = (rightPosition != boardPosition) ? recursiveGetScore(depth, rightPosition) : INT_MIN;
    int u_score = (upPosition != boardPosition) ? recursiveGetScore(depth, upPosition): INT_MIN;
    int d_score = (downPosition != boardPosition) ? recursiveGetScore(depth, downPosition) : INT_MIN;

    // std::cout << l_score << " " << r_score << " " << u_score << " " << d_score << std::endl;

    int max_score = std::max({l_score, r_score, u_score, d_score});
    if (max_score == l_score) return 'l';
    else if (max_score == r_score) return 'r';
    else if (max_score == d_score) return 'd';
    else if (max_score == u_score) return 'u';
    else return 'r';
}
