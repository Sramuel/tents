// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include "game_sdl_model.h"

#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

/* **************************************************************** */

#define BACKGROUND "res/background.png"
#define GRID "res/grid.png"
#define TENT_IMG "res/tent_printemps.png"
#define TREE_IMG "res/tree32_32.png"
#define BIGTREE_IMG "res/bigtree32_32.png"
#define GRASS_IMG "res/grass32_32.png"
#define FONT "res/arial.ttf"
#define FONTSIZE 36

/* **************************************************************** */

struct Env_t {
  SDL_Texture *background;
  SDL_Texture *grid;
  SDL_Texture *tent;
  SDL_Texture *tree;
  SDL_Texture *bigtree;
  SDL_Texture *grass;
  TTF_Font *font;
  game g;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));
  if (argc > 1) {  // initialiser le jeu soit avec un fichier, soit par défaut
    env->g = game_load(argv[1]);
  } else {  // si pas de fichier fourni on peut charger le jeu par défaut
    env->g = game_default();
  }

  env->background = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

  env->grid = IMG_LoadTexture(ren, GRID);
  if (!env->grid) ERROR("IMG_LoadTexture: %s\n", GRID);

  env->tree = IMG_LoadTexture(ren, TREE_IMG);
  if (!env->tree) ERROR("IMG_LoadTexture: %s\n", TREE_IMG);

  env->tent = IMG_LoadTexture(ren, TENT_IMG);
  if (!env->tent) ERROR("IMG_LoadTexture: %s\n", TENT_IMG);

  env->bigtree = IMG_LoadTexture(ren, BIGTREE_IMG);
  if (!env->bigtree) ERROR("IMG_LoadTexture: %s\n", BIGTREE_IMG);

  env->grass = IMG_LoadTexture(ren, GRASS_IMG);
  if (!env->grass) ERROR("IMG_LoadTexture: %s\n", GRASS_IMG);

  env->font = TTF_OpenFont(FONT, FONTSIZE);
  if (!env->font) ERROR("TTF_OpenFont: %s\n", FONT);

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  SDL_RenderCopy(ren, env->background, NULL, NULL);

  int largeur_fenetre;
  int hauteur_fenetre;
  SDL_GetWindowSize(win, &largeur_fenetre, &hauteur_fenetre);  // on prend la largeur et la hauteur réel de la fenêtre

  uint nb_lignes = game_nb_rows(env->g);
  uint nb_colonnes = game_nb_cols(env->g);

  float ratio_plateau = 0.7;
  int cote_max_plateau =
      (largeur_fenetre < hauteur_fenetre) ? largeur_fenetre : hauteur_fenetre;  // trouver le plus grand côté
  int taille_plateau_px = cote_max_plateau * ratio_plateau;

  SDL_Rect rect_plateau;  // position et taille du plateau de jeu "grid" (toujours centré et carré)
  rect_plateau.w = taille_plateau_px;
  rect_plateau.h = taille_plateau_px;
  rect_plateau.x = (largeur_fenetre - taille_plateau_px) / 2;
  rect_plateau.y = (hauteur_fenetre - taille_plateau_px) / 2;
  SDL_RenderCopy(ren, env->grid, NULL, &rect_plateau);  // on affiche le plateau du jeu

  int taille_case = taille_plateau_px / ((nb_lignes > nb_colonnes) ? nb_lignes : nb_colonnes);

  // pour centre la grille au milieu
  int decalage_x = (largeur_fenetre - (nb_colonnes * taille_case)) / 2;
  int decalage_y = (hauteur_fenetre - (nb_lignes * taille_case)) / 2;

  SDL_Rect rect;  // rappel : x et y : la position du rectangle par rapport au repère de votre fenêtre, w: la largeur du
                  // rectangle, h : la hauteur du rectangle
  rect.w = taille_case;  // le rectangle est un carré donc h et w taille case
  rect.h = taille_case;

  for (uint i = 0; i < nb_lignes; i++) {
    for (uint j = 0; j < nb_colonnes; j++) {
      // on commence au début du décalage et on ajoute la taille de j ou i nb de case
      rect.x = decalage_x + (j * taille_case);
      rect.y = decalage_y + (i * taille_case);

      SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);  // on initialise la couleur rouge au stylo
      SDL_RenderDrawRect(ren, &rect);               // on dessine un rectangle au x,y et h,w donné

      square s = game_get_square(env->g, i, j);  // si il contient quelque chose alors...

      // on prend la texture correspondante et on la met dans le rectangle qu'on a dessiné
      if (s == TREE && env->tree != NULL) {
        SDL_RenderCopy(ren, env->tree, NULL, &rect);
      } else if (s == TENT && env->tent != NULL) {
        SDL_RenderCopy(ren, env->tent, NULL, &rect);
      } else if (s == BIGTREE && env->bigtree != NULL) {
        SDL_RenderCopy(ren, env->bigtree, NULL, &rect);
      } else if (s == GRASS && env->grass != NULL) {
        SDL_RenderCopy(ren, env->grass, NULL, &rect);
      }
    }
  }

  SDL_Color couleur_texte = {0, 255, 255, 255};  // couleur cyan pour le texte
  char texte_a_afficher[10];                     // pour convertir le nombre en texte (ex: "3")

  // on affiche le nombre de tentes attendues pour chaque ligne
  for (uint i = 0; i < nb_lignes; i++) {
    uint nb_tents = game_get_expected_nb_tents_row(env->g, i);
    sprintf(texte_a_afficher, "%u", nb_tents);  // convertit l'entier en chaine

    // création de la texture du texte
    SDL_Surface *surf = TTF_RenderText_Blended(env->font, texte_a_afficher, couleur_texte);
    SDL_Texture *text = SDL_CreateTextureFromSurface(ren, surf);

    // récupération des dimensions du texte généré
    int w_texte, h_texte;
    SDL_QueryTexture(text, NULL, NULL, &w_texte, &h_texte);

    // positionnement à droite de la grille
    SDL_Rect rect_texte;
    rect_texte.x = decalage_x + (nb_colonnes * taille_case) + 10;  // +10 de marge après la dernière case
    rect_texte.y = decalage_y + (i * taille_case) + (taille_case / 2) -
                   (h_texte / 2);  // centré verticalement par rapport à la case
    rect_texte.w = w_texte;
    rect_texte.h = h_texte;

    SDL_RenderCopy(ren, text, NULL, &rect_texte);

    // on nettoie les surfaces/textures temporaires créées pour ce texte
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(text);
  }

  // on affiche le nombre de tentes attendues pour chaque colonne
  for (uint j = 0; j < nb_colonnes; j++) {
    uint nb_tents = game_get_expected_nb_tents_col(env->g, j);
    sprintf(texte_a_afficher, "%u", nb_tents);

    SDL_Surface *surf = TTF_RenderText_Blended(env->font, texte_a_afficher, couleur_texte);
    SDL_Texture *text = SDL_CreateTextureFromSurface(ren, surf);

    int w_texte, h_texte;
    SDL_QueryTexture(text, NULL, NULL, &w_texte, &h_texte);

    // positionnement en bas de la grille
    SDL_Rect rect_texte;
    rect_texte.x = decalage_x + (j * taille_case) + (taille_case / 2) -
                   (w_texte / 2);                                // centré horizontalement par rapport à la case
    rect_texte.y = decalage_y + (nb_lignes * taille_case) + 10;  // +10 de marge sous la dernière ligne
    rect_texte.w = w_texte;
    rect_texte.h = h_texte;

    SDL_RenderCopy(ren, text, NULL, &rect_texte);

    SDL_FreeSurface(surf);
    SDL_DestroyTexture(text);
  }
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  if (e->type == SDL_QUIT) {
    // SHAME
    return true;
  }

  if (e->type == SDL_KEYDOWN) {
    if (e->key.keysym.sym == SDLK_ESCAPE) {
      // SHAME
      return true;
    }
  }

  if (e->type == SDL_MOUSEBUTTONDOWN) {
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
    int largeur_fenetre;
    int hauteur_fenetre;
    SDL_GetWindowSize(win, &largeur_fenetre, &hauteur_fenetre);
    uint nb_lignes = game_nb_rows(env->g);
    uint nb_colonnes = game_nb_cols(env->g);
    int largeur_case = (largeur_fenetre * 0.7) / nb_colonnes;
    int hauteur_case = (hauteur_fenetre * 0.7) / nb_lignes;
    int taille_case;
    if (largeur_case < hauteur_case) {
      taille_case = largeur_case;
    } else {
      taille_case = hauteur_case;
    }
    int decalage_x = (largeur_fenetre - (nb_colonnes * taille_case)) / 2;
    int decalage_y = (hauteur_fenetre - (nb_lignes * taille_case)) / 2;
    int souris_x = mouse.x;
    int souris_y = mouse.y;
    if (souris_x >= decalage_x && souris_x < decalage_x + (nb_colonnes * taille_case) && souris_y >= decalage_y &&
        souris_y < decalage_y + (nb_lignes * taille_case)) {  // si la souris est dans la grille
      uint colonne = (souris_x - decalage_x) /
                     taille_case;  // on trouve la colonne et la ligne de la case sur laquelle on a cliqué
      uint ligne = (souris_y - decalage_y) / taille_case;
      square s = game_get_square(env->g, ligne, colonne);
      if (e->button.button == SDL_BUTTON_LEFT) {
        if (s == GRASS || s == EMPTY) {
          game_play_move(env->g, ligne, colonne, TENT);
        } else {
          game_play_move(env->g, ligne, colonne, GRASS);
        }
      }
      if (e->button.button == SDL_BUTTON_RIGHT) {
        game_play_move(env->g, ligne, colonne, EMPTY);
      }
    }
  }

  if (game_won(env->g)) {
    // CONGRATULATION
    return true;
  }

  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* PUT YOUR CODE HERE TO CLEAN MEMORY */
  if (env->background != NULL) {
    SDL_DestroyTexture(env->background);
  }
  if (env->tent != NULL) {
    SDL_DestroyTexture(env->tent);
  }
  if (env->tree != NULL) {
    SDL_DestroyTexture(env->tree);
  }
  if (env->bigtree != NULL) {
    SDL_DestroyTexture(env->bigtree);
  }
  if (env->grass != NULL) {
    SDL_DestroyTexture(env->grass);
  }

  if (env->g != NULL) {
    game_delete(env->g);
  }

  if (env->font != NULL) {
    TTF_CloseFont(env->font);
  }

  free(env);
}

/* **************************************************************** */
