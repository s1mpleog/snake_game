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
#include <stdlib.h>

#define WIDTH 900
#define HEIGHT 600
#define CELL_SIZE 30
#define ROWS HEIGHT / CELL_SIZE
#define COLUMNS WIDTH / CELL_SIZE
#define COLOR_GRID 0x1f1f1f
#define COLOR_WHITE 0xffffff
#define COLOR_APPLE 0xff0000
#define COLOR_BLACK 0x000000
#define LINE_WIDTH 2
#define SNAKE(x, y) fill_cell(surface, x, y, COLOR_WHITE)
#define DRAW_GRID() draw_grid(surface)
#define APPLE(x, y) fill_cell(surface, x, y, COLOR_APPLE)

struct SnakeElement {
  int x, y;
  struct SnakeElement *next;
};

struct Direction {
  int x, y;
};

struct Apple {
  int x, y;
};

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

void draw_snake(SDL_Surface *surface, struct SnakeElement *psnake) {

  if (psnake != NULL) {
    SNAKE(psnake->x, psnake->y);

    draw_snake(surface, psnake->next);
  };
};

void move_snake(struct SnakeElement *psnake, struct Direction *pdirection) {
  psnake->x += pdirection->x;
  psnake->y += pdirection->y;
};

void reset_apple(struct SnakeElement *psnake, struct Apple *papple) {
  papple->x = COLUMNS * ((double)rand() / RAND_MAX);
  papple->y = ROWS * ((double)rand() / RAND_MAX);

  struct SnakeElement *pcurrent = psnake;

  do {
    if (pcurrent->x == papple->x && pcurrent->y == papple->y) {
      reset_apple(psnake, papple);
      break;
    };
    pcurrent = pcurrent->next;

  } while (pcurrent != NULL);
};

int main(int argc, const char *argv[]) {
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

  struct SnakeElement snake = {5, 5, NULL};
  struct SnakeElement *psnake = &snake;

  struct Direction direction = {0, 0};

  SDL_Rect override_rect = {0, 0, WIDTH, HEIGHT};

  struct Apple apple;
  struct Apple *papple = &apple;
  reset_apple(psnake, papple);

  while (game) {

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        game = 0;
      }
      if (event.type == SDL_KEYDOWN) {
        direction = (struct Direction){0, 0};
        if (event.key.keysym.sym == SDLK_RIGHT) {
          direction.x = 1;
        }
        if (event.key.keysym.sym == SDLK_LEFT) {
          direction.x = -1;
        }
        if (event.key.keysym.sym == SDLK_UP) {
          direction.y = -1;
        }
        if (event.key.keysym.sym == SDLK_DOWN) {
          direction.y = 1;
        }
      }
    }

    SDL_FillRect(surface, &override_rect, COLOR_BLACK);
    move_snake(psnake, &direction);

    if (psnake->x == papple->x && psnake->y == papple->y) {
      reset_apple(psnake, papple);
    }

    APPLE(papple->x, papple->y);
    draw_snake(surface, &snake);

    DRAW_GRID();

    SDL_UpdateWindowSurface(window);
    SDL_Delay(300);
  }
  return 0;
}
