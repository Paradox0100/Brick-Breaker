#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "window.h"
#include "draw.h"
#include <iostream>

bool windowOpen = true;

SDL_Window* g_window = NULL;
SDL_Renderer* renderer = NULL;

int window() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    	std::cout << "SDL library failed to initialize\n";
    	std::cout << "Error: " << SDL_GetError() << "\n";
    	return -1;
	}

	if (TTF_Init() < 0) {
        std::cout << "SDL2_ttf library failed to initialize\n";
        std::cout << "Error: " << TTF_GetError() << "\n";
        SDL_Quit();
        return -1;
    }

	g_window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480, 0);

	if (!g_window) {
		std::cout << "window failed\n";
		std::cout << "Error: " << SDL_GetError() << "\n";
		return -1;
	}

	renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);

	if (!renderer) {
		std::cout << "failed to make a renderer for the window\n";
		std::cout << "Error: " << SDL_GetError() << "\n";
		return -1;
	}

	setup();

	SDL_Event e;
	while (windowOpen) {
		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_RIGHT] && x+10+w <= 680){
			x += 10;
		}
		if (state[SDL_SCANCODE_LEFT] && x-10 >= 0) {
			x -= 10;
		}
		/*if (state[SDL_SCANCODE_DOWN]) {
			y += 10;
		}
		if (state[SDL_SCANCODE_UP]) {
			y -= 10;
		}*/
		if (!game && !won) {
			draw();
		} else if (game || won) {
			gameOver();
		}
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				windowOpen = false;
			}

			SDL_UpdateWindowSurface(g_window);
		}
		SDL_Delay(20);
	}


	
	return 0;
}
