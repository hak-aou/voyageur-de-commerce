#include "MLV/MLV_all.h"
#include "../inc/visite.h"
#include "../inc/permutation.h"


int main(int argc, char *argv[]){
    srand(time(NULL));
    int mode_option;
    int nb_ville;
    char *nom_file = (char *) malloc(sizeof(char) * 256);

    strcpy(nom_file, "file");

    if(argc < 4){
        printf("Vous devez choisir au moins 4 paramètre\n");
        return 0;
    }

    MLV_create_window("TP9", "carte", LARGEUR_X, LARGEUR_Y);

    if(strcmp(argv[4], "-c") == 0)
        mode_option = 0;
    else if(strcmp(argv[4], "-h") == 0){
        mode_option = 1;
        nb_ville = atoi(argv[5]);
    }
    else if(strcmp(argv[4], "-f") == 0){
        mode_option = 2;
        strcpy(nom_file, argv[5]);
    }
        
    permutation(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), nb_ville, mode_option, nom_file);
    
    MLV_free_window();
    return 0;
}