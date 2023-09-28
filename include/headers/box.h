#pragma once
#include <unordered_map>
#include <SDL.h>
#include <SDL_image.h>

struct Box {
    static std::unordered_map<int, SDL_Color> valueToColorMap;
    int x;
    int y;
    int value;

    Box(int x, int y, int value) : x(x), y(y), value(value) {}
};