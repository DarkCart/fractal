#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

uint32_t palette[] = {0x0000FF, 0x00FF00, 0x00FFFF, 0xFF0000, 0xFF00FF, 0xFFFF00, 0xFFFFFF, 0x000000};

float scale(float old_value, float old_max, float new_min, float new_max) {
	return ((old_value) / (old_max)) * (new_max - new_min) + new_min;
}

void plot_pixel(SDL_Surface *surface, int x, int y, uint32_t value) {
	uint32_t * const target_pixel = (uint32_t *) ((uint8_t *) surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel);
	*target_pixel = value;
}

void render(SDL_Surface *screen) {
	for (int py = 0; py < screen->h; py++) {
		for (int px = 0; px < screen->w; px++) {
			float x0 = scale(px, screen->w, -2.0f, 0.47f);
			float y0 = scale(py, screen->h, -1.12f, 1.12f);
			float x = 0.0f, y = 0.0f;
			int iteration = 0, max_iteration = 256;
			
			do {
				float xtemp = x * x - y * y + x0;
				y = 2 * x * y + y0;
				x = xtemp;
				
				iteration = iteration + 1;
			} while ((x * x + y * y <= (2 * 2)) && iteration < max_iteration);
			
			uint32_t color = palette[iteration % 8];
			plot_pixel(screen, px, py, color);
		}
	}
}

int main() {
	SDL_Window *window = SDL_CreateWindow("Fractal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
	SDL_Surface *screen_surface = SDL_GetWindowSurface(window);
	
	bool should_close = false;
	
	render(screen_surface);
	SDL_UpdateWindowSurface(window);
	
	while (!should_close) {
		SDL_Event current_event;
		
		if (SDL_WaitEvent(&current_event)) {
			switch (current_event.type) {
				case SDL_QUIT:
					should_close = true;
					break;
				case SDL_KEYDOWN:
					if (current_event.key.keysym.sym == SDLK_q) {
						exit(0);
					}
					break;
			}
		}
	}
	
	SDL_FreeSurface(screen_surface);
	SDL_DestroyWindow(window);
	return 0;
}
