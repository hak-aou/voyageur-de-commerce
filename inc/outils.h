#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <MLV/MLV_all.h>

#ifndef __OUTIL__
#define __OUTIL__

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define SIZE_MOT 256
#define NB_VILLE 128
#define ABS(x) ( (x < 0) ? -(x) : x )
#define LARGEUR_X 500
#define LARGEUR_Y 500
#define RAND(a, b) ( (rand() % (b - a + 1)) + a )

typedef struct{
    char *nom;
    int x;
    int y;
}Ville;

typedef struct{
    Ville *villes;
    int nb_ville;
    double km;
}Visite, *Liste;

double distance(int x1, int y1, int x2, int y2);
void distance_ville(Visite* visite);
void echange(Ville* ville1, Ville* ville2, int sens_uniq);
Ville genere_ville();
int est_dans_tab(int *tab, int e, int taille);
void vide_tab(int *tab, int taille);
void echange_visite(Visite *V1, Visite *V2, int sens_uniq);
void trie_Liste_Visite(Liste V, int nb_visites);

int max_ville(char *nom_fichier_carte);
int charger_ville(char *nom_fichier_carte, Ville *tab_ville, int max);
void dessine_grille(int max);

#endif