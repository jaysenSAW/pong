#ifndef OPERATION_H
#define OPERATION_H

#include "SDL.h"
#include "SDL_mixer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h> //pour mettre des déplacements aléatoires
#include <math.h>
#include <string.h>
#include <inttypes.h>

#include "type_cst.h"

vecteur plus_vect(vecteur u, vecteur v);
vecteur fois_vect(double d, vecteur u);
vecteur moins_vect(vecteur u, vecteur v);
double scal_vect(vecteur u, vecteur v);
double sq_norme_vect(vecteur u);
vecteur nouv_traj(bille b, vecteur BA);
vecteur centre_bille(bille b);
vecteur centre_joueur(joueur J);
vecteur mult_vect(vecteur v, double d);
int initialisation_bille(bille *tabBilles, int nb_bille);
void produit_scal_par_norme(bille *tabBille, int posi, int i);
void switch_value(bille *tabBille, int i, int nb_bille);
int supprime_vie(joueur *tabJ, int indice);
int suppression_bille(bille *tabBille, int i, int nb_bille, joueur *tabJ);
int rebond(bille *tabBille, bordure *tab, joueur *tabJ, int i, int nb_bille,data donnees);
void choc_bille_alt(bille *tabBille, bordure *tab, joueur *tabJ, int nb_bille, data donnees, Mix_Chunk *choc);
int joueur_concerne(int i, bille *tabBilles, int nb_bille, double (*IA_pl)[nb_bille_max+1]);
void dist_ia_bille(int i, bille *tabBilles,int nb_bille, double (*IA_pl)[nb_bille_max+1]);
void ia_joueur(joueur *tabJ,bille *tabBilles, double (*IA_pl)[nb_bille_max+1]);
int maj_baballe(bille *tabBille, bordure *tab, joueur *tabJ, int nb_bille, data donnees,Mix_Chunk *choc, double (*IA_pl)[nb_bille_max+1]);
void deplacement_joueur(char c, joueur *tabJ);
int verif_comment_jouer(int sourisX, int sourisY);
int verif_commencer_partie(int sourisX, int sourisY);

#endif
