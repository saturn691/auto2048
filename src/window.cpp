#include <window.h>
#include <iostream>
#include <string>
#include <iostream>

int Window::screenHeight = 0;
int Window::screenWidth = 0;
SDL_Renderer* Window::renderer = nullptr;

Window::Window() {
    frozen = true;
}

Window::~Window() {}

void Window::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            handleKeyDown(event.key);
            break;
        case SDL_KEYUP:
            handleKeyUp(event.key);
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                resizeWindow(event.window.data1, event.window.data2);
            }
            break;
    }

    if (game->isAutoPlay) {
        game->autoMove();
    }
}

void Window::resizeWindow(int width, int height) {
    Window::screenWidth = width;
    Window::screenHeight = height;
}

void Window::clean() {
    delete(game);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "Successfully cleaned!" << std::endl;
}

bool Window::running() {
    return isRunning;
}

void Window::render() {
    SDL_SetRenderDrawColor(renderer, BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE, 255);
    SDL_RenderClear(renderer);
    
    game->render();

    SDL_RenderPresent(renderer);
}

void Window::handleKeyDown(SDL_KeyboardEvent& event) {
    switch (event.keysym.scancode) {
        case SDL_SCANCODE_DOWN:
            game->move('d');
            break;
        case SDL_SCANCODE_UP:
            game->move('u');
            break;
        case SDL_SCANCODE_LEFT:
            game->move('l');
            break;
        case SDL_SCANCODE_RIGHT:
            game->move('r');
            break;

        case SDL_SCANCODE_S:
            game->move('d');
            break;
        case SDL_SCANCODE_W:
            game->move('u');
            break;
        case SDL_SCANCODE_A:
            game->move('l');
            break;
        case SDL_SCANCODE_D:
            game->move('r');
            break;

        case SDL_SCANCODE_F:
            game->toggleAutoPlay();
            break;
        case SDL_SCANCODE_G:
            game->autoMove();
            break;
        case SDL_SCANCODE_R:
            game->reset();
            break;

        default:
            std::cout << "Scancode is: " << event.keysym.scancode << std::endl;
            break;
    }
}

void Window::handleKeyUp(SDL_KeyboardEvent& event) {
    switch (event.keysym.scancode) {
        default:
            break;
    }
}

void Window::init(const char* title, int xpos, int ypos, bool fullscreen) {
    int flags = 0;
    flags = SDL_WINDOW_RESIZABLE;

    if (fullscreen) {
        flags = flags | SDL_WINDOW_FULLSCREEN;
    }
    
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Subsystems Initialized!\n";
        calculateInitialWindowDimensions();
        window = SDL_CreateWindow(title, xpos, ypos, Window::screenWidth, Window::screenHeight, flags);
        
        if (window) {
			std::cout << "Window Created!\n";
			SDL_SetWindowMinimumSize(window, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
		}
		renderer = SDL_CreateRenderer(window, -1, 0);

		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 121, 121, 121, 255);
			std::cout << "Renderer created!\n";
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		}
		
        int imgFlags = IMG_INIT_PNG;
		
        if (!(IMG_Init(imgFlags) & imgFlags)) {
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			isRunning = false;
		}
		else {
			isRunning = true;

			game = new Game();
			game->init();

		}
    }
}

void Window::calculateInitialWindowDimensions() {
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);

    auto width = DM.w;
    auto height = DM.h;

    int squareWidth;
    if (width > height) {
        squareWidth = .8 * height;
    }
    else {
        squareWidth = .8 * width;
    }

    Window::screenHeight = Window::screenWidth = squareWidth;
}