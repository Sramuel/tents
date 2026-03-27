#include "game_sdl_model.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

/* **************************************************************** */

#define BACKGROUND_SQUARE "res/background_square.png"
#define BACKGROUND_RECTANGLE "res/background_rectangle.png"

#define GRID_SPRING "res/grid_spring.png"
#define GRID_AUTUMN "res/grid_autumn.png"

#define TENT_SPRING "res/tent_spring.png"
#define TENT_AUTUMN "res/tent_autumn.png"

#define TREE_SPRING "res/tree_spring.png"
#define TREE_AUTUMN "res/tree_autumn.png"

#define BIGTREE_SPRING "res/bigtree_spring.png"
#define BIGTREE_AUTUMN "res/bigtree_autumn.png"

#define GRASS_SPRING "res/grass_spring.png"
#define GRASS_AUTUMN "res/grass_autumn.png"

#define FONT "res/arial.ttf"
#define FONTSIZE 36

/* **************************************************************** */

struct Env_t {
  SDL_Texture *background_square;
  SDL_Texture *background_rectangle;

  SDL_Texture *grid_spring;
  SDL_Texture *grid_autumn;

  SDL_Texture *tent_spring;
  SDL_Texture *tent_autumn;

  SDL_Texture *tree_spring;
  SDL_Texture *tree_autumn;

  SDL_Texture *bigtree_spring;
  SDL_Texture *bigtree_autumn;

  SDL_Texture *grass_spring;
  SDL_Texture *grass_autumn;

  TTF_Font *font;
  game g;
  bool spring;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));
  if (argc > 1) {  // initialiser le jeu soit avec un fichier, soit par défaut
    env->g = game_load(argv[1]);
  } else {  // si pas de fichier fourni on peut charger le jeu par défaut
    env->g = game_default();
  }
  // les 2 fonds du jeu
  env->background_square = IMG_LoadTexture(ren, BACKGROUND_SQUARE);
  if (!env->background_square) ERROR("IMG_LoadTexture: %s\n", BACKGROUND_SQUARE);
  env->background_rectangle = IMG_LoadTexture(ren, BACKGROUND_RECTANGLE);
  if (!env->background_rectangle) ERROR("IMG_LoadTexture: %s\n", BACKGROUND_RECTANGLE);

  // les 2 plateaux
  env->grid_spring = IMG_LoadTexture(ren, GRID_SPRING);
  if (!env->grid_spring) ERROR("IMG_LoadTexture: %s\n", GRID_SPRING);
  env->grid_autumn = IMG_LoadTexture(ren, GRID_AUTUMN);
  if (!env->grid_autumn) ERROR("IMG_LoadTexture: %s\n", GRID_AUTUMN);

  // les 2 tentes
  env->tent_spring = IMG_LoadTexture(ren, TENT_SPRING);
  if (!env->tent_spring) ERROR("IMG_LoadTexture: %s\n", TENT_SPRING);
  env->tent_autumn = IMG_LoadTexture(ren, TENT_AUTUMN);
  if (!env->tent_autumn) ERROR("IMG_LoadTexture: %s\n", TENT_AUTUMN);

  // les 2 abres normaux
  env->tree_spring = IMG_LoadTexture(ren, TREE_SPRING);
  if (!env->tree_spring) ERROR("IMG_LoadTexture: %s\n", TREE_SPRING);
  env->tree_autumn = IMG_LoadTexture(ren, TREE_AUTUMN);
  if (!env->tree_autumn) ERROR("IMG_LoadTexture: %s\n", TREE_AUTUMN);

  // les 2 gros arbres
  env->bigtree_spring = IMG_LoadTexture(ren, BIGTREE_SPRING);
  if (!env->bigtree_spring) ERROR("IMG_LoadTexture: %s\n", BIGTREE_SPRING);
  env->bigtree_autumn = IMG_LoadTexture(ren, BIGTREE_AUTUMN);
  if (!env->bigtree_autumn) ERROR("IMG_LoadTexture: %s\n", BIGTREE_AUTUMN);

  // les 2 herbes
  env->grass_spring = IMG_LoadTexture(ren, GRASS_SPRING);
  if (!env->grass_spring) ERROR("IMG_LoadTexture: %s\n", GRASS_SPRING);
  env->grass_autumn = IMG_LoadTexture(ren, GRASS_AUTUMN);
  if (!env->grass_autumn) ERROR("IMG_LoadTexture: %s\n", GRASS_AUTUMN);

  env->font = TTF_OpenFont(FONT, FONTSIZE);
  if (!env->font) ERROR("TTF_OpenFont: %s\n", FONT);

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  int largeur_fenetre;
  int hauteur_fenetre;
  SDL_GetWindowSize(win, &largeur_fenetre, &hauteur_fenetre);  // on prend la largeur et la hauteur réel de la fenêtre

  uint nb_lignes = game_nb_rows(env->g);
  uint nb_colonnes = game_nb_cols(env->g);
  int largeur_case =
      (largeur_fenetre * 0.7) / nb_colonnes;  // 0.7, donc 70% de l'écran pour pas que ca prenne toute la fenêtre car on
                                              // va mettre des chiffres pour le nb de tent_expected etc...
  int hauteur_case = (hauteur_fenetre * 0.7) / nb_lignes;
  int taille_case;
  if (largeur_case < hauteur_case) {
    taille_case = largeur_case;
  } else {
    taille_case = hauteur_case;
  }
  // pour centre la grille au milieu
  int decalage_x = (largeur_fenetre - (nb_colonnes * taille_case)) / 2;
  int decalage_y = (hauteur_fenetre - (nb_lignes * taille_case)) / 2;

  /////////////////////////////////////// TEXTURES A UTILISER ///////////////////////////////////////
  SDL_Texture *fond_a_utiliser;
  SDL_Texture *grille_a_utiliser;
  SDL_Texture *tente_a_utiliser;
  SDL_Texture *arbre_a_utiliser;
  SDL_Texture *gros_arbre_a_utiliser;
  SDL_Texture *herbe_a_utiliser;

  if (nb_lignes == nb_colonnes) {
    fond_a_utiliser = env->background_square;
    grille_a_utiliser = env->grid_spring;
    tente_a_utiliser = env->tent_spring;
    arbre_a_utiliser = env->tree_spring;
    gros_arbre_a_utiliser = env->bigtree_spring;
    herbe_a_utiliser = env->grass_spring;
    env->spring = true;
  } else {
    fond_a_utiliser = env->background_rectangle;
    grille_a_utiliser = env->grid_autumn;
    tente_a_utiliser = env->tent_autumn;
    arbre_a_utiliser = env->tree_autumn;
    gros_arbre_a_utiliser = env->bigtree_autumn;
    herbe_a_utiliser = env->grass_autumn;
    env->spring = false;
  }
  SDL_RenderCopy(ren, fond_a_utiliser, NULL, NULL);

  /////////////////////////////////////// GRILLE DE JEU ///////////////////////////////////////
  SDL_Rect rect;  // rappel : x et y : la position du rectangle par rapport au repère de votre fenêtre, w: la largeur du
                  // rectangle, h : la hauteur du rectangle
  rect.w = taille_case;  // le rectangle est un carré donc h et w taille case
  rect.h = taille_case;

  for (uint i = 0; i < nb_lignes; i++) {
    for (uint j = 0; j < nb_colonnes; j++) {
      // on commence au début du décalage et on ajoute la taille de j ou i nb de case
      rect.x = decalage_x + (j * taille_case);
      rect.y = decalage_y + (i * taille_case);

      if (grille_a_utiliser != NULL) {
        SDL_RenderCopy(ren, grille_a_utiliser, NULL, &rect);  // on met le fond "grid" sous chaque case du plateau
      }
      SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);  // on initialise la couleur rouge au stylo
      if (!env->spring) {
        SDL_SetRenderDrawColor(ren, 41, 71, 204, 255);  // on initialise la couleur bleue au stylo
      }
      SDL_RenderDrawRect(ren, &rect);               // on dessine un rectangle au x,y et h,w donné

      square s = game_get_square(env->g, i, j);  // si il contient quelque chose alors...

      // on prend la texture correspondante et on la met dans le rectangle qu'on a dessiné
      if (s == TENT && tente_a_utiliser != NULL) {
        SDL_RenderCopy(ren, tente_a_utiliser, NULL, &rect);
      } else if (s == TREE && arbre_a_utiliser != NULL) {
        SDL_RenderCopy(ren, arbre_a_utiliser, NULL, &rect);
      } else if (s == BIGTREE && gros_arbre_a_utiliser != NULL) {
        SDL_RenderCopy(ren, gros_arbre_a_utiliser, NULL, &rect);
      } else if (s == GRASS && herbe_a_utiliser != NULL) {
        SDL_RenderCopy(ren, herbe_a_utiliser, NULL, &rect);
      }
    }
  }

  /////////////////////////////////////// NUMEROS DES TENTES ATTENDUES ///////////////////////////////////////
  SDL_Color couleur_texte = {255, 0, 0, 255};                       // couleur rouge pour le texte
  if (!env->spring) couleur_texte = (SDL_Color){41, 71, 204, 255};  // couleur bleue pour le texte

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
  int largeur_fenetre;
  int hauteur_fenetre;
  SDL_GetWindowSize(win, &largeur_fenetre, &hauteur_fenetre);  // on prend la largeur et la hauteur réel de la fenêtre

  uint nb_lignes = game_nb_rows(env->g);
  uint nb_colonnes = game_nb_cols(env->g);
  int largeur_case =
      (largeur_fenetre * 0.7) / nb_colonnes;  // 0.7, donc 70% de l'écran pour pas que ca prenne toute la fenêtre car on
                                              // va mettre des chiffres pour le nb de tent_expected etc...
  int hauteur_case = (hauteur_fenetre * 0.7) / nb_lignes;
  int taille_case;
  if (largeur_case < hauteur_case) {
    taille_case = largeur_case;
  } else {
    taille_case = hauteur_case;
  }
  // pour centre la grille au milieu
  int decalage_x = (largeur_fenetre - (nb_colonnes * taille_case)) / 2;
  int decalage_y = (hauteur_fenetre - (nb_lignes * taille_case)) / 2;

  float aspect_ratio = 16.0f / 9.0f;
  if (e->type == SDL_WINDOWEVENT && e->window.event == SDL_WINDOWEVENT_RESIZED) {
    static bool resizing = false;  // empêche la boucle infinie
    if (!resizing) {
      resizing = true;
      int w = e->window.data1;
      int h = e->window.data2;
      int new_w = w;
      int new_h = (int)(w / aspect_ratio);
      // choisir si on ajuste largeur ou hauteur
      if (new_h > h) {
        new_h = h;
        new_w = (int)(h * aspect_ratio);
      }
      SDL_SetWindowSize(win, new_w, new_h);
      resizing = false;
    }
  }

  if (e->type == SDL_KEYDOWN) {
    if (e->key.keysym.sym == SDLK_q) {  // quand on presse la touche "q" le jeu est quitté
      // SHAME
      return true;
    } else if (e->key.keysym.sym == SDLK_f) {  // quand on presse la touche "f" le jeu est rempli d'herbe
      game_fill_grass(env->g);
    } else if (e->key.keysym.sym == SDLK_r) {  // quand on presse la touche "r" le jeu redémarre
      game_restart(env->g);
    } else if (e->key.keysym.sym == SDLK_z) {  // quand on presse la touche "z" le coup est annulé
      game_undo(env->g);
    } else if (e->key.keysym.sym == SDLK_y) {  // quand on presse la touche "y" le coup est rejoué
      game_redo(env->g);
    }
  }

  if (e->type == SDL_MOUSEBUTTONDOWN) {
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
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
  if (env->background_square != NULL) {
    SDL_DestroyTexture(env->background_square);
  }
  if (env->background_rectangle != NULL) {
    SDL_DestroyTexture(env->background_rectangle);
  }
  if (env->grid_spring != NULL) {
    SDL_DestroyTexture(env->grid_spring);
  }
  if (env->grid_autumn != NULL) {
    SDL_DestroyTexture(env->grid_autumn);
  }
  if (env->tree_spring != NULL) {
    SDL_DestroyTexture(env->tree_spring);
  }
  if (env->tree_autumn != NULL) {
    SDL_DestroyTexture(env->tree_autumn);
  }
  if (env->tent_spring != NULL) {
    SDL_DestroyTexture(env->tent_spring);
  }
  if (env->tent_autumn != NULL) {
    SDL_DestroyTexture(env->tent_autumn);
  }
  if (env->bigtree_spring != NULL) {
    SDL_DestroyTexture(env->bigtree_spring);
  }
  if (env->bigtree_autumn != NULL) {
    SDL_DestroyTexture(env->bigtree_autumn);
  }
  if (env->grass_spring != NULL) {
    SDL_DestroyTexture(env->grass_spring);
  }
  if (env->grass_autumn != NULL) {
    SDL_DestroyTexture(env->grass_autumn);
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
