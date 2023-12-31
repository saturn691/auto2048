#include <box.h>

std::unordered_map<int, SDL_Color> Box::valueToColorMap = {
    {0, {0,0,0,0}},
    {2, {238,228,218,255}},
    {4, {236,224,200,255}},
    {8, {242,177,121,255}},
    {16, {245,149,99,255}},
    {32, {245,124,95,255}},
    {64, {239,97,59,255}},
    {128, {234,208,115,255}},
    {256, {237,204,97,255}},
    {512, {236,200,80,255}},
    {1024, {237,197,63,255}},
    {2048, {0,0,0,255}},
    {4096, {0,0,0,255}}
};