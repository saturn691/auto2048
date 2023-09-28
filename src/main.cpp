#include <headers/window.h>
#include <ctime>

int main(int argc, char* argv[]) {
    const int FPS = 120;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart = SDL_GetTicks();
    int frameTime = 0;
    float deltaTime;

    // Initialize SDL, window and renderer
    Window* window = new Window;
    window->init("2048", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, false);

    // Main Loop
    while (window->running()) {
        deltaTime = SDL_GetTicks() - frameStart;
        frameStart = SDL_GetTicks();
        window->handleEvents();
        window->render();
        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    
    return 0;
}