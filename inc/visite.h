#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "outils.h"

#ifndef __VISITE__
#define __VISITE__

Visite initialiser_visite(char *nom_fichier_carte);
void affiche_visite(Visite visite);

#endif