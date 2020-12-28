#include <stdio.h>
#include <stdlib.h>
#include <time.h> //pour mettre des déplacements aléatoires
#include <math.h>
#include <string.h>
#include <inttypes.h>

#include "operation.h"

vecteur plus_vect(vecteur u, vecteur v) {
  return (vecteur) { u.vectX + v.vectX, u.vectY + v.vectY };
}

vecteur fois_vect(double d, vecteur u) {
  return (vecteur) { d * u.vectX, d * u.vectY };
}

vecteur moins_vect(vecteur u, vecteur v) {
  return plus_vect(u, fois_vect(-1, v));
}

double scal_vect(vecteur u, vecteur v) {
  return (u.vectX * v.vectX + u.vectY * v.vectY);
}

double sq_norme_vect(vecteur u) {
  return scal_vect(u, u);
}

vecteur nouv_traj(bille b, vecteur BA) {
  double distBA2 = BA.vectX * BA.vectX + BA.vectY * BA.vectY;
  double k = (BA.vectX * b.vitesse.vectX + BA.vectY * b.vitesse.vectY) / distBA2;
  return (vecteur) {
    b.vitesse.vectX - 2 * k * BA.vectX,
      b.vitesse.vectY - 2 * k * BA.vectY
      };
}

vecteur centre_bille(bille b) {
  return (vecteur) {
    b.posX + 0.5 * Taille_bille, 
      b.posY + 0.5 * Taille_bille 
      };
}

vecteur centre_joueur(joueur J) {
  return (vecteur) {
    J.X1 + diametre/2,
      J.Y1 + diametre
      };
}

vecteur mult_vect(vecteur v, double d) {
  return (vecteur) {
    v.vectX*d,
      v.vectY*d
      };
}

int initialisation_bille(bille *tabBilles, int nb_bille){
  int coeff;
  int a = rand();
  if(nb_bille == nb_bille_max){
    return nb_bille_max;
  }
  //On va faire démarer la bille au milieu
  tabBilles[nb_bille].posX = 4.5;
  tabBilles[nb_bille].posY = 4.5;
  do{
    a = rand();
    if(a % 2 == 0)
      coeff = 1;
    else
      coeff = -1;
  }while(a == 0);
  tabBilles[nb_bille].vitesse.vectX = coeff*(1+rand()%10)*VitesseMin/10.0;
  do{
    a = rand();
    if(a % 2 == 0)
      coeff = 1;
    else
      coeff = -1;
  }while(a == 0);
  tabBilles[nb_bille].vitesse.vectY = coeff*(1+rand()%10)*VitesseMin/10.0;
  return nb_bille + 1;
}




void produit_scal_par_norme(bille *tabBille, int posi, int i) {
  vecteur *diff_V1_V2 = malloc(sizeof(vecteur));
  vecteur *res = malloc(sizeof(vecteur));
  if(diff_V1_V2 == NULL || res == NULL){
    printf("Erreur dans la fct produit scal par norme\n");
  }else{
    vecteur v1 = (vecteur) { tabBille[posi].vitesse.vectX, tabBille[posi].vitesse.vectY };
    vecteur v2 = (vecteur) { tabBille[i].vitesse.vectX, tabBille[i].vitesse.vectY };
    vecteur c1 = centre_bille (tabBille[posi]);
    vecteur c2 = centre_bille (tabBille[i]);
    vecteur cc = moins_vect(c2, c1);
    vecteur vv = moins_vect(v1, v2);
    double d = scal_vect(cc, vv)/sq_norme_vect(cc);
    // w2 -> i
    // w1 -> posi
    vecteur w2 = plus_vect(v2, mult_vect(cc, d));
    tabBille[i].vitesse.vectX = w2.vectX;
    tabBille[i].vitesse.vectY = w2.vectY;
    vecteur w1 = plus_vect(moins_vect(v1, w2), v2);
    tabBille[posi].vitesse.vectX = w1.vectX;
    tabBille[posi].vitesse.vectY = w1.vectY;

  }
}

void switch_value(bille *tabBille, int i, int nb_bille){
  tabBille[i].posX = tabBille[nb_bille].posX;
  tabBille[i].posY = tabBille[nb_bille].posY;
  tabBille[i].vitesse.vectX = tabBille[nb_bille].vitesse.vectX;
  tabBille[i].vitesse.vectY = tabBille[nb_bille].vitesse.vectY;
}

int supprime_vie(joueur *tabJ, int indice){
  //on retire une vie au joueur s'il existe toujours
  if(tabJ[indice].vie >= 0){
    tabJ[indice].vie -= 1;
    return 1;
  }
  return 0;
}

int suppression_bille(bille *tabBille, int i, int nb_bille, joueur *tabJ){
  //on détermine sur quel coté la bille quitte le plateau.
  if(tabBille[i].posX + tabBille[i].vitesse.vectX >= 10 - Taille_bille){
    //la bille quitte le plateau au niveau du coté droit
    //on supprime la bille courante et on la remplace par la dernière bille
    if(i < nb_bille && supprime_vie(tabJ, 3) == 1){
      switch_value(tabBille, i, nb_bille-1);
      return nb_bille-1; //l'avant dernière bille est maintenant la dernière  
    }else{
      //s'il n'y a plus de joueur 4, la bille rebondit
      tabBille[i].vitesse.vectX = -(tabBille[i].vitesse.vectX);
      return nb_bille;
    }
  }else if(tabBille[i].posX + tabBille[i].vitesse.vectX < Taille_bille){
    //la bille quitte le plateau par le coté gauche, joueur 2
    if(i < nb_bille && supprime_vie(tabJ, 1) == 1){
      switch_value(tabBille, i, nb_bille-1);
      return nb_bille-1; //la dernière bille est maintenant l'avant dernière  
    }else{
      //s'il n'y a plus de joueur 2, la bille rebondit
      tabBille[i].vitesse.vectX = -(tabBille[i].vitesse.vectX);
      return nb_bille;
    }
  }else if(tabBille[i].posY + tabBille[i].vitesse.vectY >= 10 - Taille_bille){
    //la bille quitte le plateau au niveau par le bas, joueur 1
    if(i < nb_bille && supprime_vie(tabJ, 0) == 1){
      switch_value(tabBille, i, nb_bille-1);
      return nb_bille-1; 
    }else{
      tabBille[i].vitesse.vectY = -(tabBille[i].vitesse.vectY);
      return nb_bille;
    }
  }else if(tabBille[i].posY + tabBille[i].vitesse.vectY < Taille_bille){
    //la bille quitte le plateau au niveau par le bas, joueur 3
    if(i < nb_bille && supprime_vie(tabJ, 2) == 1){
      switch_value(tabBille, i, nb_bille-1);
      return nb_bille-1;
    }else{
      tabBille[i].vitesse.vectY = -(tabBille[i].vitesse.vectY);
      return nb_bille;
    }
  }
  return nb_bille;
}
 

int rebond(bille *tabBille, bordure *tab, joueur *tabJ, int i, int nb_bille,data donnees){
  int j;
  double diffX = 0, diffY = 0;
  for(j = 0; j < nbBords; j++){
    diffX = pow( (tabBille[i].posX + Taille_bille/2) - (tab[j].limX1 + (diametre/2)) , 2);
    diffY = pow( (tabBille[i].posY + Taille_bille/2) - (tab[j].limY1 + (diametre/2)) , 2);
    //on rajoute +0.1 car les float ont une imprecision
    if(sqrt(diffX+diffY) <= ((Taille_bille+diametre)/2) + 0.1){
      vecteur c1 = centre_bille(tabBille[i]);
	  vecteur c2 = {
	    tab[j].limX1 + diametre/2,
	    tab[j].limY1 + diametre/2
	  };
	  vecteur BA = {
	    c1.vectX - c2.vectX,
	    c1.vectY - c2.vectY
	  };
	  vecteur traj_bille = nouv_traj(tabBille[i], BA);
	  tabBille[i].vitesse.vectX = traj_bille.vectX;
	  tabBille[i].vitesse.vectY = traj_bille.vectY;
    }
  }

  for(j=0;j<nbJoueurs;j++){
    if (donnees.temps_joueur[j][i]) {
      donnees.temps_joueur[j][i]--;
      continue;
    }
    //on vérifie que la bille ne touche pas un joueur
    if(j == 0 || j == 2){
      if(j == 0 && tabJ[j].vie >= 0){
	diffY = pow(( (tabBille[i].posY + Taille_bille/2) - (tabJ[j].Y1 + diametre) ),2);
      }else if(j == 2 && tabJ[j].vie >= 0){
	diffY = pow( (tabBille[i].posY + Taille_bille/2) - (tabJ[j].Y1),2);
      }
      diffX = pow( (tabBille[i].posX + Taille_bille/2) - (tabJ[j].X1 + diametre/2),2);
      if(sqrt(diffX+diffY) <= ((Taille_bille+diametre)*0.5)+0.0001 && tabJ[j].vie >= 0){
        //On calcule ici la nouvelle direction de la bille suite
        //au choc avec le joueur.
	if(j==0 && tabJ[j].vie >= 0) {
	  vecteur c1 = centre_bille(tabBille[i]);
	  vecteur c2 = {
	    tabJ[j].X1 + diametre/2,
	    tabJ[j].Y1 + diametre
	  };
	  vecteur BA = {
	    c1.vectX - c2.vectX,
	    c1.vectY - c2.vectY
	  };
	  vecteur traj_bille = nouv_traj(tabBille[i], BA);
	  tabBille[i].vitesse.vectX = traj_bille.vectX;
	  tabBille[i].vitesse.vectY = traj_bille.vectY;
	  donnees.temps_joueur[j][i] = 5;
	}else if(j==2 && tabJ[j].vie >= 0) {
	  vecteur c1 = centre_bille(tabBille[i]);
	  vecteur c2 = {
	    tabJ[j].X1 + diametre/2,
	    tabJ[j].Y1
	  };
	  vecteur BA = {
	    c1.vectX - c2.vectX,
	    c1.vectY - c2.vectY
	  };
	  vecteur traj_bille = nouv_traj(tabBille[i], BA);
	  tabBille[i].vitesse.vectX = traj_bille.vectX;
	  tabBille[i].vitesse.vectY = traj_bille.vectY;
	  donnees.temps_joueur[j][i] = 5;
	}
      }
    }else if(j==1 || j==3){
      //joueur se trouvant à gauche et à droite
      if(j == 1 && tabJ[j].vie >= 0){
	diffX = pow(( (tabBille[i].posX + Taille_bille/2 + tabBille[i].vitesse.vectX) - tabJ[j].X1),2);
      }else if(j == 3 && tabJ[j].vie >= 0){
	diffX = pow( (tabBille[i].posX + Taille_bille/2 + tabBille[i].vitesse.vectX) - (tabJ[j].X1+diametre),2);
      }
      diffY = pow(tabBille[i].posY + Taille_bille/2 + tabBille[i].vitesse.vectY - (tabJ[j].Y1 + diametre/2),2);
      if(sqrt(diffX+diffY) <= ((Taille_bille+diametre)/2)+0.1 && tabJ[j].vie >= 0){
        //On calcule ici la nouvelle direction de la bille suite
        //au choc avec le joueur.
	if(j==1 && tabJ[j].vie >= 0) {
	  vecteur c1 = centre_bille(tabBille[i]);
	  vecteur c2 = {
	    tabJ[j].X1,
	    tabJ[j].Y1 + diametre/2
	  };
	  vecteur BA = {
	    c1.vectX - c2.vectX,
	    c1.vectY - c2.vectY
	  };
	  vecteur traj_bille = nouv_traj(tabBille[i], BA);
	  tabBille[i].vitesse.vectX = traj_bille.vectX;
	  tabBille[i].vitesse.vectY = traj_bille.vectY;
	  donnees.temps_joueur[j][i] = 5;
	}else if(j==3 && tabJ[j].vie >= 0) {
	  vecteur c1 = centre_bille(tabBille[i]);
	  vecteur c2 = {
	    tabJ[j].X1 + diametre,
	    tabJ[j].Y1 + diametre/2
	  };
	  vecteur BA = {
	    c1.vectX - c2.vectX,
	    c1.vectY - c2.vectY
	  };
	  vecteur traj_bille = nouv_traj(tabBille[i], BA);
	  tabBille[i].vitesse.vectX = traj_bille.vectX;
	  tabBille[i].vitesse.vectY = traj_bille.vectY;
	  donnees.temps_joueur[j][i] = 5;
	}
      }
    }
    nb_bille = suppression_bille(tabBille, i, nb_bille, tabJ);
  }
  return nb_bille;
}


void choc_bille_alt(bille *tabBille, bordure *tab, joueur *tabJ, int nb_bille, data donnees, Mix_Chunk *choc){
  double distance = 0, diffX = 0, diffY = 0;
  int i = 0, posi;
    
  for (posi = 0; posi < nb_bille; posi++) {
    for(i=posi+1; i<nb_bille; i++){
      if (donnees.temps_bille[posi][i]) {
	donnees.temps_bille[posi][i]--;
	continue;
      }
      diffX = ((tabBille[posi].posX + Taille_bille/2 + tabBille[posi].vitesse.vectX) - (tabBille[i].posX + Taille_bille/2 + tabBille[i].vitesse.vectX));
      diffY = ((tabBille[posi].posY + Taille_bille/2 + tabBille[posi].vitesse.vectY) - (tabBille[i].posY + Taille_bille/2 + tabBille[i].vitesse.vectY));
      distance = sqrt(pow(diffX,2) + pow(diffY,2));
      if(distance <= Taille_bille){
	produit_scal_par_norme(tabBille, posi, i);
	donnees.temps_bille[posi][i] = 5;
	Mix_Volume(0, MIX_MAX_VOLUME);
	Mix_PlayChannel(-1, choc, 0);
	
      }
    }
  }	
}

int joueur_concerne(int i, bille *tabBilles, int nb_bille,
		   double (*IA_pl)[nb_bille_max+1]){
  int j=2;
  double X = tabBilles[i].vitesse.vectX + tabBilles[i].posX;
  double Y = tabBilles[i].vitesse.vectY + tabBilles[i].posY;
  while(X > 0 && X < 10 && Y > 0 && Y < 10){
    X = tabBilles[i].vitesse.vectX + j*tabBilles[i].posX;
    Y = tabBilles[i].vitesse.vectY + j*tabBilles[i].posY;
    j+=2;
  }
  if(X<0){
    IA_pl[0][i] = tabBilles[i].posX;
    return 0;
  }else if(X>10){
    IA_pl[2][i] = tabBilles[i].posX;
    return 2;
  }else if(Y<0){
    IA_pl[1][i] = tabBilles[i].posY;
    return 1;
  }
  else
    return -1;
}

void dist_ia_bille(int i, bille *tabBilles,int nb_bille,
		   double (*IA_pl)[nb_bille_max+1]){
  int ind =0;
  int indice_IA = joueur_concerne(i, tabBilles, nb_bille,IA_pl);

  //par défaut la bille la plus proche des joueurs est la bille 1
  if(i == 0){
    //le joueur 2
    IA_pl[0][nb_bille_max] = 0;
    //le joueur 3
    IA_pl[1][nb_bille_max] = 0;
    //le joueur 4
    IA_pl[2][nb_bille_max] = 0;

  }else if(i > 0 && indice_IA >= 0){
    //On compare la nouvelle distance avec celle considérée comme la plus petite
    ind = (int)IA_pl[indice_IA][nb_bille_max];
    //une bille se trouve plus proche du bord
    if(indice_IA == 0  && IA_pl[indice_IA][i] < IA_pl[indice_IA][ind]){
      //assignation de l'indice qui correspond à la bille la plus proche
      IA_pl[indice_IA][nb_bille_max] = i;
    }else if(indice_IA == 1  && IA_pl[indice_IA][i] < IA_pl[indice_IA][ind]){
      //assignation de l'indice qui correspond à la bille la plus proche
      IA_pl[indice_IA][nb_bille_max] = i;
    }else if(i == 2 && IA_pl[indice_IA][i] > IA_pl[indice_IA][ind]){
      IA_pl[indice_IA][nb_bille_max] = i;
    }
  }
}

void ia_joueur(joueur *tabJ,bille *tabBilles,
	       double (*IA_pl)[nb_bille_max+1]){
  int i = 0, ind = 0;
  for(i = 0; i < nbJoueurs-1; i++){
    
    ind = (int)IA_pl[i][nb_bille_max];
     
    if(i == 0 || i == 2){
      
      if(tabJ[i+1].Y1 < tabBilles[ind].posY){

	if(tabJ[i+1].Y1 + VitesseIA < 8)
	  tabJ[i+1].Y1 = tabJ[i+1].Y1 + VitesseIA;
	else
	  tabJ[i+1].Y1 = 8;
	  
      }else if(tabJ[i+1].Y1 + diametre > tabBilles[ind].posY){

	if(tabJ[i+1].Y1 - VitesseIA > 2)
	  tabJ[i+1].Y1 = tabJ[i+1].Y1 - VitesseIA;
	else
	  tabJ[i+1].Y1 = 2;
      }
    }else if(i == 1){
      if(tabJ[i+1].X1 < tabBilles[ind].posX){

	if(tabJ[i+1].X1 + VitesseIA < 8)
	  tabJ[i+1].X1 = tabJ[i+1].X1 + VitesseIA;
	else
	  tabJ[i+1].X1 = 8;
	
      }else if(tabJ[i+1].X1 + diametre > tabBilles[ind].posX){

	if(tabJ[i+1].X1 - VitesseIA > 2)
	  tabJ[i+1].X1 = tabJ[i+1].X1 - VitesseIA;
	else
	  tabJ[i+1].X1 = 2;
      }
    }
  }
}


int maj_baballe(bille *tabBille, bordure *tab, joueur *tabJ, int nb_bille, data donnees,Mix_Chunk *choc, double (*IA_pl)[nb_bille_max+1]){
  int i;
  for(i=0;i<nb_bille;i++){
    choc_bille_alt(tabBille, tab, tabJ, nb_bille, donnees, choc);
    if( (tabBille[i].posX + tabBille[i].vitesse.vectX < 10 - Taille_bille) && (tabBille[i].posX + tabBille[i].vitesse.vectX > Taille_bille) ){
      if(tabBille[i].vitesse.vectX * Frottement > VitesseMin)
	tabBille[i].vitesse.vectX = tabBille[i].vitesse.vectX * Frottement;
      tabBille[i].posX = tabBille[i].posX + tabBille[i].vitesse.vectX;
    }
    if(tabBille[i].posY + tabBille[i].vitesse.vectY  < 10 - Taille_bille && tabBille[i].posY + tabBille[i].vitesse.vectY > 0){
      if(tabBille[i].vitesse.vectY * Frottement > VitesseMin)
	tabBille[i].vitesse.vectY = tabBille[i].vitesse.vectY * Frottement;
      tabBille[i].posY = tabBille[i].posY + tabBille[i].vitesse.vectY;
    }
    //appel à rebond
    nb_bille = rebond(tabBille, tab, tabJ, i, nb_bille, donnees);
    dist_ia_bille(i,tabBille, nb_bille, IA_pl);
  }
  ia_joueur(tabJ, tabBille,IA_pl);
  return nb_bille;
}

/*La fonction doit permettre de déplacer tous les joueur et pas uniquement le premier
  Pour le moment je me suis cncentré que sur le premier joueur*/

void deplacement_joueur(char c, joueur *tabJ){
  
  if(c == 'd' && tabJ[0].X1 < 8){ 
    tabJ[0].X1 = tabJ[0].X1 + VitesseJoueur;
  }else if(c == 'q' && tabJ[0].X1 > 1){
    tabJ[0].X1 = tabJ[0].X1 - VitesseJoueur;
  }
}

int verif_comment_jouer(int sourisX, int sourisY){
    if(sourisX <= posCJ1X + 300 && sourisX >= posCJ1X){
        if(sourisY <= posCJ1Y + 67 && sourisY >= posCJ1Y){
            return 1;
        }
    }
    return 0;
}

int verif_commencer_partie(int sourisX, int sourisY){
    if(sourisX <= posCP1X + 300 && sourisX >= posCP1X){
        if(sourisY <= posCP1Y + 67 && sourisY >= posCP1Y){
            return 1;
        }
    }
    return 0;
}
