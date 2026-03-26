#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>

#include "game_sdl_model.h"

/* **************************************************************** */

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) ERROR("Error: SDL_Init VIDEO (%s)", SDL_GetError());
  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) ERROR("Error: IMG_Init PNG (%s)", SDL_GetError());
  if (TTF_Init() != 0) ERROR("Error: TTF_Init (%s)", SDL_GetError());

  // creation de la fenetre et de l'affichage
  SDL_Window* win = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                     SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (!win) ERROR("Error: SDL_CreateWindow (%s)", SDL_GetError());
  SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!ren) ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
  if (!ren) ERROR("Error: SDL_CreateRenderer (%s)", SDL_GetError());

  // initialise l'environnement
  Env* env = init(win, ren, argc, argv);
  SDL_Event e;
  bool quit = false;
  while (!quit) {
    // gestion des evenements
    while (SDL_PollEvent(&e)) {
      quit = process(win, ren, env, &e);
      if (quit) break;
    }
    // initialisation du render
    render(win, ren, env);
    SDL_RenderPresent(ren);
    SDL_Delay(DELAY);
  }
  // nettoyage du jeu
  clean(win, ren, env);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  IMG_Quit();
  TTF_Quit();
  SDL_Quit();

  return EXIT_SUCCESS;
}
