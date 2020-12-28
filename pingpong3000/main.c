#include "SDL.h"
#include "SDL_mixer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h> //pour mettre des déplacements aléatoires
#include <math.h>
#include <string.h>
#include <inttypes.h>
#include "graphisme.h"


int refresh(texture Tex[nb_Tex], bordure *tab, joueur *tabJ,
	    bille *tabBilles, SDL_Renderer *renderer,
	    int nb_bille, data donnees, Mix_Chunk *choc,
	    double (*IA_pl)[nb_bille_max+1]){

  affichage_textures(Tex, tab, tabJ, tabBilles, renderer, nb_bille);
  return maj_baballe(tabBilles, tab, tabJ, nb_bille, donnees, choc, IA_pl);
}

void initialisation_jeu(joueur *tabJ, bordure *tab){
  //On définit les bordure
  tab[0] = (bordure) {0,1,0,1};
  tab[1] = (bordure) {9,10,0,1};
  tab[2] = (bordure) {0,1,9,10};
  tab[3] = (bordure) {9,10,9,10};
  
  //situé en bas
  tabJ[0] = (joueur){1,20,4.5,9,0,0};
  //situé à gauche
  tabJ[1] = (joueur) {2,20,0,4.5,0,0};
  //situé en haut
  tabJ[2] =(joueur) {3,20,4.5,0,0,0};
  //situé à droite
  tabJ[3] = (joueur){4,20,9,4.5,0,0};
}

Mix_Chunk *load_music(char *s){
  Mix_Chunk *choc = Mix_LoadWAV(s);
  if (!choc) {
    printf("echec choc\n");
    exit(1);
  }
  printf("choc ok\n");
  return choc;
}

enum status {QUIT, CONTINUE}; 
/* PLAY, etc., etat actuel de la boucle d'evenements, c.f. plus bas. */

int main(int argc, char *argv[]) {
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  enum status status = CONTINUE;
  char nom_Tex[nb_Tex][20];
  texture Tex[nb_Tex];
  bordure tab[nbBords];
  joueur tabJ[nbJoueurs];
  bille tabBilles[nb_bille_max];
  int nb_bille = 0;
  srand(time(NULL));
  time_t start = time(NULL);
  Uint32 tps = SDL_GetTicks();
  initialisation_SDL (&window, &renderer);

  data donnees;
  int menu = 0;
  int sourisX, sourisY;
  int i, j, pause = 1; //-1 pour pause
	int reprise = -1; // pour une nouvelle reprise
  double IA_pl[nbJoueurs-1][nb_bille_max+1] = {{0}};
  for(i = 0 ; i < nb_bille_max; i++){
    for(j = 0; j < nb_bille_max; j++){
      if(i < nbJoueurs)
	donnees.temps_joueur[i][j]=0;
      
      donnees.temps_bille[i][j] = 0;

    }
  }
  

  //partie son
  if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, 
		   MIX_DEFAULT_FORMAT, 
		   MIX_DEFAULT_CHANNELS, 
		   4096) == -1) //Initialisation de l'API Mixer
    {
      printf("%s", Mix_GetError());
    }
    
  Mix_Chunk *choc = load_music("bell.wav");

  Mix_AllocateChannels(16);
  Mix_Volume(0, MIX_MAX_VOLUME);

  Mix_Chunk *theme = load_music("Neo_Geo_-_Iso.wav");;
  Mix_PlayChannel(1, theme, 0);
    
  chargement_texture(Tex, nom_Tex, renderer);
  printf("textures chargees.\n");

  initialisation_jeu (tabJ, tab);
  printf("valeurs initialisees.\n");
  
  //Affichage de l'ensemble des textures
  affichage_textures(Tex, tab, tabJ, tabBilles, renderer, nb_bille);
  printf("textures affichees.");

  do {
    SDL_Event e;
      if(menu == 0 && SDL_PollEvent(&e)){
        affichage_menu(Tex, renderer);
        switch (e.type) {
	        case SDL_QUIT :
          printf("fermeture de la fenetre.\n"); 
		      status = QUIT;
          menu = 4;
		      break;
          case SDL_MOUSEBUTTONDOWN :
          sourisX = e.button.x;
          sourisY = e.button.y;
          if(verif_comment_jouer(sourisX, sourisY) == 1){
            menu = 2;
            SDL_RenderClear(renderer);
          }else if(verif_commencer_partie(sourisX, sourisY) == 1){
            menu = 3;
            SDL_RenderClear(renderer);
          }
        }
      }else if(menu == 2 && SDL_PollEvent(&e)){
        affichage_didact(Tex, renderer);
        switch (e.type) {
	        case SDL_QUIT :
          printf("fermeture de la fenetre.\n"); 
	        status = QUIT;
          menu = 4;
	        break;
          case SDL_MOUSEBUTTONDOWN :
          sourisX = e.button.x;
          sourisY = e.button.y;
          if(verif_commencer_partie(sourisX, sourisY) == 1){
            menu = 3;
            SDL_RenderClear(renderer);
          }
        }
      }
		//on met un timer de 3 sec pour la reprise du jeu
	if(menu == 3 && pause == 1 && reprise == -1){
		start = time(NULL) - start; //on bloque la venue de nouvelle bille, en sauvegardant le temps qu'il s'est passé avant apparition
		for(i = 3; i >= 1; i--){
			nb_bille = refresh(Tex, tab, tabJ, tabBilles, renderer, nb_bille, donnees, choc, IA_pl);
			timeur(renderer,Tex, i);
		}
		reprise = 1;
	   	start = time(NULL) + start; //le délais d'apparition est remis
	}

    if(menu == 3 && (nb_bille == 0 || (start + 10 < time(NULL) )) && pause == 1 && reprise == 1){
      nb_bille = initialisation_bille(tabBilles, nb_bille);
      nb_bille = refresh(Tex, tab, tabJ, tabBilles, renderer, nb_bille, donnees, choc, IA_pl);
      start = time(NULL);
    }
    if(menu == 3 && (tps + delay < SDL_GetTicks()) && pause == 1 && reprise == 1){
      SDL_RenderClear(renderer);
      //Affichage de l'ensemble des textures
      nb_bille = refresh(Tex, tab, tabJ, tabBilles, renderer, nb_bille, donnees, choc, IA_pl);	    
      tps = SDL_GetTicks();
    }
	
	if(victoire(Tex,tabJ, renderer) > -1){
		pause = -1;
	}

    if (menu == 3 && SDL_PollEvent(&e)) {
      /* examen du type de l'evenement */
      switch (e.type) {
	/* fermeture de la fenetre */
      case SDL_QUIT :
	printf("fermeture de la fenetre.\n"); 
	status = QUIT;
	break;
	/* deplacement de la souris, pointeur dans la fenetre */
      case SDL_MOUSEMOTION :
	/*printf("deplacement : %d %d\n", 
	       e.motion.x, 
	       e.motion.y);*/
	break;
	/* clic de souris, pointeur dans la fenetre */
      case SDL_MOUSEBUTTONDOWN :
	printf("clic : %d %d, bouton ", 
	       e.button.x, 
	       e.button.y);
	/* avec l'un des boutons : */
	switch (e.button.button) {
	case SDL_BUTTON_LEFT   : printf ("gauche.\n");  break;
	case SDL_BUTTON_MIDDLE : printf ("central.\n"); break;
	case SDL_BUTTON_RIGHT  : printf ("droit.\n"); break;
	default : printf("autre.\n"); break; 
	}
	break;
      case SDL_MOUSEBUTTONUP :
	/*printf("bouton relache en %d %d\n",
	       e.button.x, 
	       e.button.y);*/
	break;
      case SDL_KEYDOWN :
	/* touche presse, focus sur la fenetre */
	if(e.key.keysym.sym == 'p'){
	  pause = pause*-1;
		reprise = -1;
	  if(pause == -1)
	    start = time(NULL) - start; //on bloque la venue de nouvelle bille, en sauvegardant le temps qu'il s'est passé avant apparition
	  else
	    start = time(NULL) + start; //le délais d'apparition est remis
	}else if(e.key.keysym.sym == 'm'){
		menu = 0;
		initialisation_jeu(tabJ, tab);
		nb_bille = 0;
		reprise = -1;
	  /*pause = 1;
	  
	  if(pause == -1)
	    start = time(NULL) - start; //on bloque la venue de nouvelle bille, en sauvegardant le temps qu'il s'est passé avant apparition
	  else
	    start = time(NULL) + start; //le délais d'apparition est remis*/
	}else if(e.key.keysym.sym == 'r'){
	  initialisation_jeu(tabJ, tab);
		nb_bille = 0;
		pause = 1;
		reprise = -1;
	}else if (e.key.keysym.sym >= 32 && e.key.keysym.sym <= 127 && pause == 1){
	  //printf(" soit '%c'\n", (char) ks.sym);
		    
		    
	  deplacement_joueur((char) e.key.keysym.sym, tabJ);
		    
	}else printf("\n");
	break;
	/*case SDL_KEYUP :
	  break;*/	
      }
    }
  } while (status == CONTINUE);

  /* liberation des ressources allouees non encore liberees */
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  Mix_FreeChunk(choc);
  Mix_FreeChunk(theme);
  Mix_CloseAudio();

  return 0;
}
