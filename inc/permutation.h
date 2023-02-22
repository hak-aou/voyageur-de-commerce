#include "outils.h"
#include "visite.h"

#ifndef __PERMUTATION__
#define __PERMUTATION__

#define MAX_NB_VILLE 256

void mut(Visite *visite);
void permutation(int alpha, int beta, int gamma, int nb_ville, int mode_option, char *nom_file);
int creer_toute_ville(Ville* tab_ville);

#endif