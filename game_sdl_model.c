// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include "game_sdl_model.h"

#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* **************************************************************** */

#define FONT "res/arial.ttf"
#define FONTSIZE 36
#define BACKGROUND "res/background.png"
#define MARIO "res/mario.png"
#define BOMB "res/bomb.png"

/* **************************************************************** */

struct Env_t {
  SDL_Texture *background;
  SDL_Texture *board;
  SDL_Texture *tent;
  SDL_Texture *tree;
  SDL_Texture *bigtree;
  SDL_Texture *grass;
  SDL_Texture *empty;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));

  /* PUT YOUR CODE HERE TO INIT TEXTURES, ... */

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) { /* PUT YOUR CODE HERE TO RENDER TEXTURES, ... */ }

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  if (e->type == SDL_QUIT) {
    return true;
  }

  /* PUT YOUR CODE HERE TO PROCESS EVENTS */

  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* PUT YOUR CODE HERE TO CLEAN MEMORY */

  free(env);
}

/* **************************************************************** */
