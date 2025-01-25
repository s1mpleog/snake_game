#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

#define WIDTH 900
#define HEIGHT 600
#define CELL_SIZE 20
#define ROWS HEIGHT / CELL_SIZE
#define COLUMNS WIDTH / CELL_SIZE
#define COLOR_GRID 0x1f1f1f
#define COLOR_WHITE 0xffffff
#define COLOR_APPLE 0xff0000
#define LINE_WIDTH 2
#define SNAKE(x, y) fill_cell(surface, x, y, COLOR_WHITE)
#define DRAW_GRID() draw_grid(surface)
#define APPLE(x, y) fill_cell(surface, x, y, COLOR_APPLE)

void draw_grid(SDL_Surface *surface) {
  SDL_Rect row_line = {0, 0, WIDTH, LINE_WIDTH};

  for (row_line.y = 0; row_line.y < HEIGHT; row_line.y += CELL_SIZE) {
    SDL_FillRect(surface, &row_line, COLOR_GRID);
  }

  SDL_Rect col_line = {0, 0, LINE_WIDTH, HEIGHT};

  for (col_line.x = 0; col_line.x < WIDTH; col_line.x += CELL_SIZE) {
    SDL_FillRect(surface, &col_line, COLOR_GRID);
  }
}

void fill_cell(SDL_Surface *surface, int x, int y, Uint32 color) {
  SDL_Rect rect = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};

  SDL_FillRect(surface, &rect, color);
}

int main(int argc, const char *argv[]) {
  printf("hello snake game\n");

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("failed to initialize sdl%s\n", SDL_GetError());
    return -1;
  }

  SDL_Window *window =
      SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

  if (window == NULL) {
    printf("failed to create window%s\n", SDL_GetError());
    return -1;
  }

  SDL_Surface *surface = SDL_GetWindowSurface(window);
  int game = 1;
  SDL_Event event;

  int snake_x = 5;
  int snake_y = 5;
  int apple_x = 12;
  int apple_y = 18;

  while (game) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        game = 0;
      }

      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_RIGHT) {
          snake_x++;
        }
        if (event.key.keysym.sym == SDLK_LEFT) {
          snake_x--;
        }
        if (event.key.keysym.sym == SDLK_UP) {
          snake_y--;
        }
        if (event.key.keysym.sym == SDLK_DOWN) {
          snake_y++;
        }
      }
    }

    SNAKE(snake_x, snake_y);
    APPLE(apple_x, apple_y);

    DRAW_GRID();

    SDL_UpdateWindowSurface(window);
    SDL_Delay(20);
  }

  return 0;
}
