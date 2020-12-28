#ifndef GRAPHISME_H
#define GRAPHISME_H

#include "SDL.h"
#include "SDL_mixer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h> //pour mettre des déplacements aléatoires
#include <math.h>
#include <string.h>
#include <inttypes.h>

#include "type_cst.h"
#include "operation.h"

void chargement_texture(texture Tex[nb_Tex], char nom_Tex[nb_Tex][20], SDL_Renderer *renderer);
void affichage_textures(texture Tex[nb_Tex], bordure *tab, joueur *tabJ, bille *tabBilles, SDL_Renderer *renderer, int nb_bille);
void affichage_menu(texture Tex[nb_Tex], SDL_Renderer *renderer);
void affichage_didact(texture Tex[nb_Tex], SDL_Renderer *renderer);
void initialisation_SDL (SDL_Window **hdl_window, SDL_Renderer **hdl_renderer);
void timeur( SDL_Renderer *renderer, texture Tex[nb_Tex], int i);
int victoire(texture Tex[nb_Tex], joueur *tabJ, SDL_Renderer *renderer);

#endif
