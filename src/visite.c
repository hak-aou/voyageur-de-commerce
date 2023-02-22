#include "../inc/visite.h"

Visite initialiser_visite(char *nom_fichier_carte){
    FILE *file_carte = fopen(nom_fichier_carte, "r");
    Visite visite;
    int indice;
    char *mot =(char*) malloc(sizeof(SIZE_MOT));

    assert(nom_fichier_carte != NULL);

    visite.villes = (Ville*) malloc(sizeof(Ville) * NB_VILLE);

    indice = 0;
    while(fscanf(file_carte, "%s", mot) != EOF){
        
        visite.villes[indice].nom = (char *) malloc(sizeof(char) * SIZE_MOT);
        strcpy(visite.villes[indice].nom, mot);
        fscanf(file_carte, "%s", mot);
        visite.villes[indice].x = atoi(mot);
        fscanf(file_carte, "%s", mot);
        visite.villes[indice].y = atoi(mot);
        
        indice++;
    }
    visite.nb_ville = indice;
    distance_ville(&visite);
    return visite;
}

void affiche_visite(Visite visite){
    int i;
    for(i = 0 ; i < visite.nb_ville ; i++)
        printf("%s %d %d, ", visite.villes[i].nom, visite.villes[i].x, visite.villes[i].y);
    printf("km = %.2f\n", visite.km);
}