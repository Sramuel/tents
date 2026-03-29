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

#define BACKGROUND_SPRING "res/background_spring.png"
#define BACKGROUND_AUTUMN "res/background_autumn.png"
#define BACKGROUND_WINTER "res/background_winter.png"

#define GRID_SPRING "res/grid_spring.png"
#define GRID_AUTUMN "res/grid_autumn.png"
#define GRID_WINTER "res/grid_winter.png"

#define TENT_SPRING "res/tent_spring.png"
#define TENT_AUTUMN "res/tent_autumn.png"
#define TENT_WINTER "res/tent_winter.png"

#define TREE_SPRING "res/tree_spring.png"
#define TREE_AUTUMN "res/tree_autumn.png"
#define TREE_WINTER "res/tree_winter.png"

#define BIGTREE_SPRING "res/bigtree_spring.png"
#define BIGTREE_AUTUMN "res/bigtree_autumn.png"
#define BIGTREE_WINTER "res/bigtree_winter.png"

#define GRASS_SPRING "res/grass_spring.png"
#define GRASS_AUTUMN "res/grass_autumn.png"
#define GRASS_WINTER "res/grass_winter.png"

#define FONT "res/arial.ttf"
#define FONTSIZE 36

// Coordonnées de tes zones de jeu (x, y, w, h)
const SDL_Rect ZONE_CARRE = {582, 162, 756, 756};
const SDL_Rect ZONE_PAYSAGE = {291, 205, 1338, 670};
const SDL_Rect ZONE_PORTRAIT = {670, 105, 580, 870};

/* **************************************************************** */

struct Env_t {
  Uint32 temps;
  bool quitting;
  bool show_help;

  SDL_Texture *background_spring;
  SDL_Texture *background_autumn;
  SDL_Texture *background_winter;

  SDL_Texture *grid_spring;
  SDL_Texture *grid_autumn;
  SDL_Texture *grid_winter;

  SDL_Texture *tent_spring;
  SDL_Texture *tent_autumn;
  SDL_Texture *tent_winter;

  SDL_Texture *tree_spring;
  SDL_Texture *tree_autumn;
  SDL_Texture *tree_winter;

  SDL_Texture *bigtree_spring;
  SDL_Texture *bigtree_autumn;
  SDL_Texture *bigtree_winter;

  SDL_Texture *grass_spring;
  SDL_Texture *grass_autumn;
  SDL_Texture *grass_winter;

  TTF_Font *font;
  game g;
  int season;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));
  env->temps = 0;
  env->quitting = false;
  env->show_help = false;
  if (argc > 1) {  // initialiser le jeu soit avec un fichier, soit par défaut
    env->g = game_load(argv[1]);
  } else {  // si pas de fichier fourni on peut charger le jeu par défaut
    env->g = game_default();
  }
  // les 3 fonds du jeu
  env->background_spring = IMG_LoadTexture(ren, BACKGROUND_SPRING);
  if (!env->background_spring) ERROR("IMG_LoadTexture: %s\n", BACKGROUND_SPRING);
  env->background_autumn = IMG_LoadTexture(ren, BACKGROUND_AUTUMN);
  if (!env->background_autumn) ERROR("IMG_LoadTexture: %s\n", BACKGROUND_AUTUMN);
  env->background_winter = IMG_LoadTexture(ren, BACKGROUND_WINTER);
  if (!env->background_winter) ERROR("IMG_LoadTexture: %s\n", BACKGROUND_WINTER);

  // les 3 plateaux
  env->grid_spring = IMG_LoadTexture(ren, GRID_SPRING);
  if (!env->grid_spring) ERROR("IMG_LoadTexture: %s\n", GRID_SPRING);
  env->grid_autumn = IMG_LoadTexture(ren, GRID_AUTUMN);
  if (!env->grid_autumn) ERROR("IMG_LoadTexture: %s\n", GRID_AUTUMN);
  env->grid_winter = IMG_LoadTexture(ren, GRID_WINTER);
  if (!env->grid_winter) ERROR("IMG_LoadTexture: %s\n", GRID_WINTER);

  // les 3 tentes
  env->tent_spring = IMG_LoadTexture(ren, TENT_SPRING);
  if (!env->tent_spring) ERROR("IMG_LoadTexture: %s\n", TENT_SPRING);
  env->tent_autumn = IMG_LoadTexture(ren, TENT_AUTUMN);
  if (!env->tent_autumn) ERROR("IMG_LoadTexture: %s\n", TENT_AUTUMN);
  env->tent_winter = IMG_LoadTexture(ren, TENT_WINTER);
  if (!env->tent_winter) ERROR("IMG_LoadTexture: %s\n", TENT_WINTER);

  // les 3 abres normaux
  env->tree_spring = IMG_LoadTexture(ren, TREE_SPRING);
  if (!env->tree_spring) ERROR("IMG_LoadTexture: %s\n", TREE_SPRING);
  env->tree_autumn = IMG_LoadTexture(ren, TREE_AUTUMN);
  if (!env->tree_autumn) ERROR("IMG_LoadTexture: %s\n", TREE_AUTUMN);
  env->tree_winter = IMG_LoadTexture(ren, TREE_WINTER);
  if (!env->tree_winter) ERROR("IMG_LoadTexture: %s\n", TREE_WINTER);

  // les 3 gros arbres
  env->bigtree_spring = IMG_LoadTexture(ren, BIGTREE_SPRING);
  if (!env->bigtree_spring) ERROR("IMG_LoadTexture: %s\n", BIGTREE_SPRING);
  env->bigtree_autumn = IMG_LoadTexture(ren, BIGTREE_AUTUMN);
  if (!env->bigtree_autumn) ERROR("IMG_LoadTexture: %s\n", BIGTREE_AUTUMN);
  env->bigtree_winter = IMG_LoadTexture(ren, BIGTREE_WINTER);
  if (!env->bigtree_winter) ERROR("IMG_LoadTexture: %s\n", BIGTREE_WINTER);

  // les 3 herbes
  env->grass_spring = IMG_LoadTexture(ren, GRASS_SPRING);
  if (!env->grass_spring) ERROR("IMG_LoadTexture: %s\n", GRASS_SPRING);
  env->grass_autumn = IMG_LoadTexture(ren, GRASS_AUTUMN);
  if (!env->grass_autumn) ERROR("IMG_LoadTexture: %s\n", GRASS_AUTUMN);
  env->grass_winter = IMG_LoadTexture(ren, GRASS_WINTER);
  if (!env->grass_winter) ERROR("IMG_LoadTexture: %s\n", GRASS_WINTER);

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

  /////////////////////////////////////// TEXTURES/ZONE A UTILISER ///////////////////////////////////////
  SDL_Rect zone_jeu_a_utiliser;
  SDL_Texture *fond_a_utiliser;
  SDL_Texture *grille_a_utiliser;
  SDL_Texture *tente_a_utiliser;
  SDL_Texture *arbre_a_utiliser;
  SDL_Texture *gros_arbre_a_utiliser;
  SDL_Texture *herbe_a_utiliser;

  if (nb_lignes == nb_colonnes) {
    zone_jeu_a_utiliser = ZONE_CARRE;
    fond_a_utiliser = env->background_spring;
    grille_a_utiliser = env->grid_spring;
    tente_a_utiliser = env->tent_spring;
    arbre_a_utiliser = env->tree_spring;
    gros_arbre_a_utiliser = env->bigtree_spring;
    herbe_a_utiliser = env->grass_spring;
    env->season = 0;
  } else if (nb_lignes < nb_colonnes) {
    zone_jeu_a_utiliser = ZONE_PAYSAGE;
    fond_a_utiliser = env->background_autumn;
    grille_a_utiliser = env->grid_autumn;
    tente_a_utiliser = env->tent_autumn;
    arbre_a_utiliser = env->tree_autumn;
    gros_arbre_a_utiliser = env->bigtree_autumn;
    herbe_a_utiliser = env->grass_autumn;
    env->season = 1;
  } else {
    zone_jeu_a_utiliser = ZONE_PORTRAIT;
    fond_a_utiliser = env->background_winter;
    grille_a_utiliser = env->grid_winter;
    tente_a_utiliser = env->tent_winter;
    arbre_a_utiliser = env->tree_winter;
    gros_arbre_a_utiliser = env->bigtree_winter;
    herbe_a_utiliser = env->grass_winter;
    env->season = 2;
  }
  SDL_RenderCopy(ren, fond_a_utiliser, NULL, NULL);

  /////////////////////////////////////// CALCULS ///////////////////////////////////////
  float echelle_x = (float)largeur_fenetre / 1920.0f;
  float echelle_y = (float)hauteur_fenetre / 1080.0f;

  // on adapte la zone de jeu à la taille de la fenêtre
  SDL_Rect zone_adaptee;
  zone_adaptee.x = (int)(zone_jeu_a_utiliser.x * echelle_x);
  zone_adaptee.y = (int)(zone_jeu_a_utiliser.y * echelle_y);
  zone_adaptee.w = (int)(zone_jeu_a_utiliser.w * echelle_x);
  zone_adaptee.h = (int)(zone_jeu_a_utiliser.h * echelle_y);

  // on calcule la taille des cases dans cette zone adaptée
  int largeur_case = zone_adaptee.w / nb_colonnes;
  int hauteur_case = zone_adaptee.h / nb_lignes;
  int taille_case = (largeur_case < hauteur_case) ? largeur_case : hauteur_case;

  // pour centre la grille au milieu
  int decalage_x = zone_adaptee.x + (zone_adaptee.w - (nb_colonnes * taille_case)) / 2;
  int decalage_y = zone_adaptee.y + (zone_adaptee.h - (nb_lignes * taille_case)) / 2;

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
      if (env->season == 1) {
        SDL_SetRenderDrawColor(ren, 41, 71, 204, 255);  // on initialise la couleur bleu au stylo
      } else if (env->season == 2) {
        SDL_SetRenderDrawColor(ren, 102, 51, 153, 255);  // on initialise la couleur violet au stylo
      } else {
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);  // on initialise la couleur rouge au stylo (printemps par défaut)
      }
      SDL_RenderDrawRect(ren, &rect);  // on dessine un rectangle au x,y et h,w donné

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
  SDL_Color couleur_texte;  // couleur du texte en fonciton du thème
  if (env->season == 1) {
    couleur_texte = (SDL_Color){41, 71, 204, 255};  // on initialise la couleur bleu au stylo
  } else if (env->season == 2) {
    couleur_texte = (SDL_Color){102, 51, 153, 255};  // on initialise la couleur violet au stylo
  } else {
    couleur_texte = (SDL_Color){255, 0, 0, 255};  // on initialise la couleur rouge au stylo (printemps par défaut)
  }
  char texte_a_afficher[10];  // pour convertir le nombre en texte (ex: "3")

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

  ///////////////////////////////////////// MESSAGE DE VICTOIRE ET DE SHAME ///////////////////////////////////////

  if (game_won(env->g)) {
    // on met le style de la police en italique et gras pour le message de victoire
    TTF_SetFontStyle(env->font, TTF_STYLE_ITALIC | TTF_STYLE_BOLD);

    SDL_Color couleur_or = {255, 215, 0, 255};  // on met la couleur or pour le texte de victoire

    // on crée le message CONGRATULATIONS et on lui applique la couleur or
    SDL_Surface *surf_victoire = TTF_RenderText_Blended(env->font, "CONGRATULATIONS !", couleur_or);
    SDL_Texture *text_victoire = SDL_CreateTextureFromSurface(ren, surf_victoire);

    // on crée les dimensions du texte de victoire pour pouvoir le positionner correctement par la suite
    int w_victoire, h_victoire;
    SDL_QueryTexture(text_victoire, NULL, NULL, &w_victoire, &h_victoire);

    // on agrandit le texte de victoire pour qu'il soit plus visible, on multiplie sa taille par 2
    w_victoire = w_victoire * 2;
    h_victoire = h_victoire * 2;

    // on positionne le texte au centre de la fenêtre
    SDL_Rect rect_victoire;
    rect_victoire.x = (largeur_fenetre / 2) - (w_victoire / 2);
    rect_victoire.y = (hauteur_fenetre / 2) - (h_victoire / 2);
    rect_victoire.w = w_victoire;
    rect_victoire.h = h_victoire;

    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);  // activer la transparence
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 150);             // noir avec 150 d'opacité

    // le fond noir prend toute la largeur, mais juste la hauteur du texte avec un peu de marge
    SDL_Rect rect_fond_noir = {0, rect_victoire.y - 10, largeur_fenetre, h_victoire + 20};
    SDL_RenderFillRect(ren, &rect_fond_noir);

    SDL_RenderCopy(ren, text_victoire, NULL, &rect_victoire);  // on dessine le texte par dessus le fond noir

    // on free toutes les surfaces/textures temporaires créées pour ce texte de victoire
    SDL_FreeSurface(surf_victoire);
    SDL_DestroyTexture(text_victoire);

    // On rénitialise le style de la police pour les prochains textes
    TTF_SetFontStyle(env->font, TTF_STYLE_NORMAL);
  }

  if (env->quitting) {
    TTF_SetFontStyle(env->font, TTF_STYLE_ITALIC | TTF_STYLE_BOLD);

    SDL_Color couleur_violet = {128, 0, 128, 255};

    SDL_Surface *surf_shame = TTF_RenderText_Blended(env->font, "SHAME !", couleur_violet);
    SDL_Texture *text_shame = SDL_CreateTextureFromSurface(ren, surf_shame);

    int w_shame, h_shame;
    SDL_QueryTexture(text_shame, NULL, NULL, &w_shame, &h_shame);

    w_shame = w_shame * 2;
    h_shame = h_shame * 2;

    SDL_Rect rect_shame;
    rect_shame.x = (largeur_fenetre / 2) - (w_shame / 2);
    rect_shame.y = (hauteur_fenetre / 2) - (h_shame / 2);
    rect_shame.w = w_shame;
    rect_shame.h = h_shame;

    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 150);

    SDL_Rect rect_fond_noir = {0, rect_shame.y - 10, largeur_fenetre, h_shame + 20};
    SDL_RenderFillRect(ren, &rect_fond_noir);

    SDL_RenderCopy(ren, text_shame, NULL, &rect_shame);

    SDL_FreeSurface(surf_shame);
    SDL_DestroyTexture(text_shame);

    TTF_SetFontStyle(env->font, TTF_STYLE_NORMAL);
  }

  ///////////////////////////////////////// BOUTON HELP ///////////////////////////////////////

  // on met le bouton HELP en haut à gauche de la fenêtre
  SDL_Rect rect_bouton_help = {10, 10, 100, 40};  // x=10, y=10, largeur=100, hauteur=40

  // on met le bouton en gris foncé
  SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
  SDL_RenderFillRect(ren, &rect_bouton_help);

  // on met le texte "HELP" en blanc sur le bouton
  SDL_Color couleur_blanche = {255, 255, 255, 255};
  SDL_Surface *surf_bouton = TTF_RenderText_Blended(env->font, "HELP", couleur_blanche);
  SDL_Texture *text_bouton = SDL_CreateTextureFromSurface(ren, surf_bouton);
  int w_btn, h_btn;
  SDL_QueryTexture(text_bouton, NULL, NULL, &w_btn, &h_btn);

  // centre le texte sur le bouton
  SDL_Rect rect_texte_btn = {rect_bouton_help.x + (rect_bouton_help.w - w_btn) / 2,
                             rect_bouton_help.y + (rect_bouton_help.h - h_btn) / 2, w_btn, h_btn};
  SDL_RenderCopy(ren, text_bouton, NULL, &rect_texte_btn);
  SDL_FreeSurface(surf_bouton);
  SDL_DestroyTexture(text_bouton);

  /////////////////////////////////////// PANNEAU D'AIDE ///////////////////////////////////////

  if (env->show_help) {
    // on met un fond noir transparent pour faire ressortir le panneau d'aide
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 200);  // 200 d'opacité
    SDL_Rect plein_ecran = {0, 0, largeur_fenetre, hauteur_fenetre};
    SDL_RenderFillRect(ren, &plein_ecran);

    // préparation du texte d'aide (ligne par ligne car SDL_ttf ne gère pas bien les \n)
    char *lignes_aide[] = {"Aide au jeu Tents",
                           "Clic Gauche : Poser une TENTE / HERBE",
                           "Clic Droit : Effacer (EMPTY)",
                           "Touche 'F' : Remplir la grille d'herbe",
                           "Touche 'R' : Recommencer le niveau",
                           "Touche 'Z' : Annuler (Undo)",
                           "Touche 'Y' : Refaire (Redo)",
                           "Touche 'Q' : Quitter (Shame)"};
    int nb_lignes_aide = 8;
    int hauteur_ligne = 40;                                                        // espace vertical entre chaque ligne
    int debut_y = (hauteur_fenetre / 2) - ((nb_lignes_aide * hauteur_ligne) / 2);  // centré verticalement

    // on dessine chaque ligne du texte d'aide au centre de la fenêtre
    for (int i = 0; i < nb_lignes_aide; i++) {
      SDL_Surface *surf_ligne = TTF_RenderText_Blended(env->font, lignes_aide[i], couleur_blanche);
      SDL_Texture *text_ligne = SDL_CreateTextureFromSurface(ren, surf_ligne);

      int w_ligne, h_ligne;
      SDL_QueryTexture(text_ligne, NULL, NULL, &w_ligne, &h_ligne);

      SDL_Rect rect_ligne = {(largeur_fenetre / 2) - (w_ligne / 2), debut_y + (i * hauteur_ligne), w_ligne, h_ligne};

      SDL_RenderCopy(ren, text_ligne, NULL, &rect_ligne);
      SDL_FreeSurface(surf_ligne);
      SDL_DestroyTexture(text_ligne);
    }
  }
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  if (game_won(env->g) || env->quitting) {
    // si on vient de gagner ou de quitter et que le chrono n'est pas encore démarré, on le démarre en enregistrant le
    // temps actuel
    if (env->temps == 0) {
      env->temps =
          SDL_GetTicks();  // on démarre le chrono en enregistrant le temps actuel en millisecondes depuis la victoire
    }
    // on vérifie si 2 secondes (2000 ms) se sont écoulées depuis la victoire ou le quit
    if (SDL_GetTicks() - env->temps >= 2000) {
      return true;  // on quitte le jeu après 2 secondes
    }
  } else {
    // si on annule une victoire en faisant un undo par exemple, on réinitialise le chrono et one le remet a sa valeur
    // sentinelle 0
    env->temps = 0;
  }

  int largeur_fenetre;
  int hauteur_fenetre;
  SDL_GetWindowSize(win, &largeur_fenetre, &hauteur_fenetre);  // on prend la largeur et la hauteur réel de la fenêtre
  uint nb_lignes = game_nb_rows(env->g);
  uint nb_colonnes = game_nb_cols(env->g);

  /////////////////////////////////////// TEXTURES/ZONE A UTILISER ///////////////////////////////////////
  SDL_Rect zone_jeu_a_utiliser;

  if (env->season == 0) {
    zone_jeu_a_utiliser = ZONE_CARRE;
  } else if (env->season == 1) {
    zone_jeu_a_utiliser = ZONE_PAYSAGE;
  } else {
    zone_jeu_a_utiliser = ZONE_PORTRAIT;
  }

  /////////////////////////////////////// CALCULS ///////////////////////////////////////
  float echelle_x = (float)largeur_fenetre / SCREEN_WIDTH;
  float echelle_y = (float)hauteur_fenetre / SCREEN_HEIGHT;

  // on adapte la zone de jeu à la taille de la fenêtre
  SDL_Rect zone_adaptee;
  zone_adaptee.x = (int)(zone_jeu_a_utiliser.x * echelle_x);
  zone_adaptee.y = (int)(zone_jeu_a_utiliser.y * echelle_y);
  zone_adaptee.w = (int)(zone_jeu_a_utiliser.w * echelle_x);
  zone_adaptee.h = (int)(zone_jeu_a_utiliser.h * echelle_y);

  // on calcule la taille des cases dans cette zone adaptée
  int largeur_case = zone_adaptee.w / nb_colonnes;
  int hauteur_case = zone_adaptee.h / nb_lignes;
  int taille_case = (largeur_case < hauteur_case) ? largeur_case : hauteur_case;

  // pour centre la grille au milieu
  int decalage_x = zone_adaptee.x + (zone_adaptee.w - (nb_colonnes * taille_case)) / 2;
  int decalage_y = zone_adaptee.y + (zone_adaptee.h - (nb_lignes * taille_case)) / 2;

  /////////////////////////////////////// REDIMENSIONNEMENT DE LA FENETRE ///////////////////////////////////////
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

  /////////////////////////////////////// EVENEMENTS CLAVIER ///////////////////////////////////////
  if (e->type == SDL_KEYDOWN) {
    if (e->key.keysym.sym == SDLK_q) {  // quand on presse la touche "q" le jeu est quitté
      // SHAME
      env->quitting = true;
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

  /////////////////////////////////////// EVENEMENTS SOURIS ///////////////////////////////////////
  if (e->type == SDL_MOUSEBUTTONDOWN) {
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
    int souris_x = mouse.x;
    int souris_y = mouse.y;

    if (souris_x >= 10 && souris_x <= 100 && souris_y >= 10 &&
        souris_y <= 40) {  // si on clique dans la zone du bouton HELP
      if (e->button.button == SDL_BUTTON_LEFT) {
        env->show_help = !(env->show_help);  // alterne entre vrai et faux à chaque clic sur le bouton
        return false;  // on ne fait rien d'autre que d'ouvrir ou fermer le panneau d'aide, on ne veut pas que le clic
                       // fasse aussi une action sur la grille en dessous (ce qui ne devrait pas arriver car le bouton
                       // est en dehors de la grille)
      }
    }

    if (env->show_help) {
      if (e->button.button == SDL_BUTTON_LEFT) {  // si on clique n'importe où et que le panneau d'aide est ouvert alors
                                                  // on ferme le panneau d'aide
        env->show_help = false;
      }
      return false;  // on laisse le panneau d'aide ouvert tant que l'utilisateur n'a pas cliqué pour le fermer
    }

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
  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  if (env == NULL) return;

  // nettoyage des fonds (Backgrounds)
  if (env->background_spring) SDL_DestroyTexture(env->background_spring);
  if (env->background_autumn) SDL_DestroyTexture(env->background_autumn);
  if (env->background_winter) SDL_DestroyTexture(env->background_winter);

  // nettoyage des plateaux (Grids)
  if (env->grid_spring) SDL_DestroyTexture(env->grid_spring);
  if (env->grid_autumn) SDL_DestroyTexture(env->grid_autumn);
  if (env->grid_winter) SDL_DestroyTexture(env->grid_winter);

  // nettoyage des tentes (Tents)
  if (env->tent_spring) SDL_DestroyTexture(env->tent_spring);
  if (env->tent_autumn) SDL_DestroyTexture(env->tent_autumn);
  if (env->tent_winter) SDL_DestroyTexture(env->tent_winter);

  // nettoyage des arbres normaux (Trees)
  if (env->tree_spring) SDL_DestroyTexture(env->tree_spring);
  if (env->tree_autumn) SDL_DestroyTexture(env->tree_autumn);
  if (env->tree_winter) SDL_DestroyTexture(env->tree_winter);

  // nettoyage des gros arbres (Big Trees)
  if (env->bigtree_spring) SDL_DestroyTexture(env->bigtree_spring);
  if (env->bigtree_autumn) SDL_DestroyTexture(env->bigtree_autumn);
  if (env->bigtree_winter) SDL_DestroyTexture(env->bigtree_winter);

  // nettoyage des herbes (Grass)
  if (env->grass_spring) SDL_DestroyTexture(env->grass_spring);
  if (env->grass_autumn) SDL_DestroyTexture(env->grass_autumn);
  if (env->grass_winter) SDL_DestroyTexture(env->grass_winter);

  // nettoyage du reste (Police, Jeu et Structure)
  if (env->font) TTF_CloseFont(env->font);
  if (env->g) game_delete(env->g);

  free(env);
}

/* **************************************************************** */
