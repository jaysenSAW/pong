#ifndef TYPE_CST_H
#define TYPE_CST_H

#define WIDTH  700
#define HEIGHT 700
#define BANDEAU_WIDTH 140
#define BANDEAU_HEIGHT 700
#define RATIO_BORDURES 10
#define nbBords 4 
// NB_BORDS etc...
#define nbJoueurs 4
//Vitesse de deplacement
#define VitesseMin 0.13
#define VitesseMax 0.14
#define VitesseJoueur 0.5
#define VitesseIA 0.1
#define Frottement 0.99
#define Taille_bille 0.3
#define nb_bille_max 10
#define nb_Tex 28
//distance pour les joueurs et les bords
#define diametre 1.0
#define delay 15
#define delay_choc 0.05
#define posCJ1X 300 //position de la banière comment jouer en X
#define posCJ1Y 350 //position de la banière comment jouer en Y
#define posCP1X 300 //position de la banière commencer partie en X
#define posCP1Y 500 //position de la banière commencer partie en Y


typedef struct {
  int numero;
  int vie;
  double X1;
  double Y1;
  double directionX;
  double directionY;
} joueur;

typedef struct {
  double vectX;
  double vectY;
} vecteur;   

typedef struct {
  double posX;
  double posY;
  vecteur vitesse;
} bille;

typedef struct {
  double limX1;
  double limX2;
  double limY1;
  double limY2;
} bordure;

typedef struct {
  int temps_bille[nb_bille_max][nb_bille_max];
  int temps_joueur[nbJoueurs][nb_bille_max];
} data;

typedef struct {
  SDL_Texture *image;
} texture;

#endif
