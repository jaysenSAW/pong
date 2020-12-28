#include "SDL.h"
#include "SDL_mixer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h> //pour mettre des déplacements aléatoires
#include <math.h>
#include <string.h>
#include <inttypes.h>

#include "graphisme.h"


void chargement_texture(texture Tex[nb_Tex], char nom_Tex[nb_Tex][20], SDL_Renderer *renderer){
  int i;
  int k = 0;
  char tex_fond[25];

  FILE* fichier = NULL;
  fichier = fopen("textures.txt", "r");

  while(fgets(tex_fond, 25, fichier) != NULL){
      int m;
      for(m=0;tex_fond[m]!='\n';m++){
          nom_Tex[k][m] = tex_fond[m];
      }
      nom_Tex[k][m] = '\0';
	printf("Chargement de %s\n", tex_fond);
      k++;   
  }

  fclose(fichier);

  SDL_Surface *bitmapSurface = NULL;
  SDL_Surface *nouv_img = NULL;
  for(i=0;i<nb_Tex;i++){
    bitmapSurface = SDL_LoadBMP(nom_Tex[i]);
    if (bitmapSurface == NULL) {
      fprintf(stderr,
	      "/nImpossible de charger l'image:%s %s\n",nom_Tex[i], 
	      SDL_GetError()
	      );
      exit(1);
    }
    if(i == 0 || i == 7 || i == 8 || i == 11){
      Tex[i].image = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
      SDL_FreeSurface(bitmapSurface);
    }else{
      nouv_img = SDL_ConvertSurfaceFormat(bitmapSurface, SDL_PIXELFORMAT_RGB332, 0);
      Uint32 colorkey = SDL_MapRGB(nouv_img->format, 0, 0, 255);
      SDL_SetColorKey(nouv_img, SDL_TRUE, colorkey);
      
      Tex[i].image = SDL_CreateTextureFromSurface(renderer, nouv_img);
      SDL_FreeSurface(bitmapSurface);
      SDL_FreeSurface(nouv_img);
    }
  }

}

void affichage_textures (texture Tex[nb_Tex], bordure *tab, joueur *tabJ, bille *tabBilles, SDL_Renderer *renderer, int nb_bille){
  int i, dizaine, unite;
  SDL_Rect dest_rect;
  //1 : affichage fond
  dest_rect = (SDL_Rect) { 0, 0, WIDTH, HEIGHT };
  SDL_RenderCopy(renderer, Tex[0].image, NULL, &dest_rect);
  //2 : affichage bordures
  for(i = 0; i < nbBords; i++){
    dest_rect = (SDL_Rect) 
      {  tab[i].limX1 * (WIDTH  / RATIO_BORDURES), 
	 tab[i].limY1 * (HEIGHT / RATIO_BORDURES),
	 (tab[i].limX2-tab[i].limX1) *  (WIDTH  / RATIO_BORDURES), 
	 (tab[i].limY2-tab[i].limY1) *  (HEIGHT / RATIO_BORDURES) 
      };
    SDL_RenderCopy(renderer, Tex[1].image, NULL, &dest_rect);
  }
  //4 : Affichage billes
  for(i=0;i<nb_bille;i++){
    dest_rect = 
      (SDL_Rect) { (tabBilles[i]).posX * (WIDTH  / 10),
		   (tabBilles[i]).posY * (HEIGHT / 10), 
		   Taille_bille * (WIDTH  / 10), 
		   Taille_bille * (HEIGHT / 10)};
    SDL_RenderCopy(renderer, Tex[2].image, NULL, &dest_rect);
  }   
  //affichage joueur
  for(i=0; i < nbJoueurs; i++){
    if(tabJ[i].vie >= 0){
      dest_rect = (SDL_Rect) {tabJ[i].X1 * WIDTH/10, tabJ[i].Y1*HEIGHT/10, WIDTH/10, HEIGHT/10};
      SDL_RenderCopy(renderer, Tex[3+i].image, NULL, &dest_rect);
    }
  }
  //affichage du bandeau
  dest_rect = (SDL_Rect) { WIDTH, 0, BANDEAU_WIDTH, HEIGHT };
  SDL_RenderCopy(renderer, Tex[7].image, NULL, &dest_rect);
  
  //on charge les skins des joueurs sur le bandeau
  for(i = 0; i < nbJoueurs; i++){
    dizaine = 0;
	unite = 0;
    dest_rect = (SDL_Rect) { WIDTH, i*50, 50, 50};
    SDL_RenderCopy(renderer, Tex[12+i].image, NULL, &dest_rect);

    //on indique le nombre de vie
	if(tabJ[i].vie < 0){
		dest_rect = (SDL_Rect) { WIDTH+45, i*50, 100, 50};
		SDL_RenderCopy(renderer, Tex[26].image, NULL, &dest_rect);
	}else{
		dizaine = tabJ[i].vie/10;
		unite = tabJ[i].vie - dizaine*10;
		dest_rect = (SDL_Rect) { WIDTH+45, i*50, 50, 50};
		SDL_RenderCopy(renderer, Tex[16+dizaine].image, NULL, &dest_rect);
		dest_rect = (SDL_Rect) { WIDTH+90, i*50, 50, 50};
		SDL_RenderCopy(renderer, Tex[16+unite].image, NULL, &dest_rect);
	}
  }
  SDL_RenderPresent(renderer);
}

void affichage_menu(texture Tex[nb_Tex], SDL_Renderer *renderer){
  SDL_Rect dest_rect;
  int i;
    for(i=8;i<nb_Tex;i++){
      switch (i){
        case 8:
        dest_rect = (SDL_Rect) { 0, 0, WIDTH + BANDEAU_WIDTH, HEIGHT };
	      SDL_RenderCopy(renderer, Tex[i].image, NULL, &dest_rect);
        break;
        case 9:
        dest_rect = (SDL_Rect) { posCJ1X, posCJ1Y, 300, 67 };   
	      SDL_RenderCopy(renderer, Tex[i].image, NULL, &dest_rect);
        break;
        case 10:
        dest_rect = (SDL_Rect) { posCP1X, posCP1Y, 300, 67 };   
	      SDL_RenderCopy(renderer, Tex[i].image, NULL, &dest_rect);
        break;
      }
    }
  SDL_RenderPresent(renderer);
}

void affichage_didact(texture Tex[nb_Tex], SDL_Renderer *renderer){
  SDL_Rect dest_rect;
        
  dest_rect = (SDL_Rect) {0, 0, WIDTH + BANDEAU_WIDTH, HEIGHT };
  SDL_RenderCopy(renderer, Tex[11].image, NULL, &dest_rect);

  dest_rect = (SDL_Rect) {posCP1X, posCP1Y, 300, 67 };
  SDL_RenderCopy(renderer, Tex[10].image, NULL, &dest_rect);

  SDL_RenderPresent(renderer);
}


void initialisation_SDL (SDL_Window **hdl_window, SDL_Renderer **hdl_renderer) {
  /* Initialisation de SDL */
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    /* gestion de l'echec de l'initialisation */
    fprintf(stderr,
	    "\nImpossible d'initialiser SDL :  %s\n",
	    SDL_GetError()
	    );
    //return EXIT_FAILURE;
  }
  atexit(SDL_Quit);
  
  *hdl_window = SDL_CreateWindow
    ("A window",                 /* titre */
     SDL_WINDOWPOS_CENTERED,     /* placement en x */
     SDL_WINDOWPOS_CENTERED,     /* placement en y */
     WIDTH + BANDEAU_WIDTH,                         /* largeur */
     HEIGHT,                         /* hauteur */
     0                            /* drapeaux, cf, la doc.  */
     );
  if (*hdl_window == NULL) {
    fprintf(stderr,
	    "\nCreation de la fenetre impossible : %s\n", 
	    SDL_GetError()
	    );
  }
  *hdl_renderer = SDL_CreateRenderer(*hdl_window, -1, 0);
  if (*hdl_renderer == NULL) {
    fprintf(stderr,
	    "\nCreation du renderer impossible : %s\n", 
	    SDL_GetError()
	    );
  }
  SDL_RenderClear(*hdl_renderer); 
}

void timeur( SDL_Renderer *renderer, texture Tex[nb_Tex], int i){
	SDL_Rect dest_rect;
	dest_rect = (SDL_Rect) { 350, 350, 50, 50};
	SDL_RenderCopy(renderer, Tex[16+i].image, NULL, &dest_rect);
	SDL_RenderPresent(renderer);
	SDL_Delay(1000);
}

int victoire(texture Tex[nb_Tex], joueur *tabJ, SDL_Renderer *renderer){
	int cpt = 0, i =0, ind_joueur = 0;
	SDL_Rect dest_rect;
	
  	for(i=0; i < nbJoueurs; i++){
    	if(tabJ[i].vie < 0){
			cpt++;
		}else{
			ind_joueur = i;
		}
	}
	if(cpt < 3){
		return -1;
	}else{
		dest_rect = (SDL_Rect) { 250, 350, 200, 175};
		SDL_RenderCopy(renderer, Tex[27].image, NULL, &dest_rect);
		dest_rect = (SDL_Rect) { 325, 410, 50, 50};
		SDL_RenderCopy(renderer, Tex[12+ind_joueur].image, NULL, &dest_rect);
		SDL_RenderPresent(renderer);	
		return ind_joueur;
	}
}

