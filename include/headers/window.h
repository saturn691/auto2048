#pragma once
#include <sdl/SDL.h>
#include <sdl/SDL_image.h>
#include <game.h>

#define MIN_WINDOW_WIDTH 400
#define MIN_WINDOW_HEIGHT 400

//defines for the default background color fo the window.
#define BACKGROUND_RED 122
#define BACKGROUND_GREEN 122
#define BACKGROUND_BLUE 122

class Window {
public:
    Window();
    ~Window();

    void handleEvents();
    void clean();
    void render();
    void handleKeyDown(SDL_KeyboardEvent& event);
    void handleKeyUp(SDL_KeyboardEvent& event);
    void resizeWindow(int width, int height);
    bool running();

    void calculateInitialWindowDimensions();
    void init(const char* title, int xpos, int ypos, bool fullscreen);

    static SDL_Renderer* renderer;
    static int screenWidth;
    static int screenHeight;
private:
    bool isRunning;
    bool frozen;
    SDL_Window* window;
    Game* game;
};