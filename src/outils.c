#include "../inc/outils.h"

double distance(int x1, int y1, int x2, int y2){
    return ABS(sqrt(pow((double) x2-x1, (double) 2) + pow((double)y2-y1, (double) 2)));
}

void distance_ville(Visite* visite){

    int i;
    double total_distance;

    assert(visite != NULL);

    total_distance = 0;

    for(i = 1; i < visite->nb_ville ; i++){
        total_distance += distance(visite->villes[i].x, visite->villes[i].y,
        visite->villes[i - 1].x, visite->villes[i - 1].y);
    }
    visite->km = total_distance;
}

void echange(Ville* ville1, Ville* ville2, int sens_uniq){
    Ville tmp;
    tmp.nom = (char*) malloc(sizeof(char) * SIZE_MOT);
    
    strcpy(tmp.nom, (*ville1).nom);
    tmp.x = (*ville1).x;
    tmp.y = (*ville1).y;
    
    strcpy((*ville1).nom, (*ville2).nom);
    (*ville1).x = (*ville2).x;
    (*ville1).y = (*ville2).y;
    
    if(sens_uniq == 0){
        strcpy((*ville2).nom, tmp.nom);
        (*ville2).x = tmp.x;
        (*ville2).y = tmp.y;
    }

    free(tmp.nom);
}

Ville genere_ville(){
    Ville ville;
    ville.nom = (char*) malloc(sizeof(char) * SIZE_MOT);
    strcpy(ville.nom, "ville");
    ville.x = RAND(10, LARGEUR_X-10);
    ville.y = RAND(50, LARGEUR_Y-10);
    return ville;
}


int est_dans_tab(int *tab, int e, int taille){
    int j;
    for(j = 0 ; j < taille ; j++)
        if(tab[j] == e)
            return 1;
    return 0;
}

void vide_tab(int *tab, int taille){
    int j;
    for(j = 0 ; j < taille ; j++)
        tab[j] = -1;
}


/* Fonction echange_visite()                                              */
/*                                                                        */
/* Si l'entier sens_uniq vaut :                                           */
/* 0 : on echange les deux visites                                        */
/* 1 : la première visite prends les valeurs de la deuxième et c'est tout */
void echange_visite(Visite *V1, Visite *V2, int sens_uniq){
    
    int j, nb_ville;
    double tmp_km;

    nb_ville = V2->nb_ville;
    
    if(sens_uniq == 0){

        tmp_km = V1->km;

        for(j = 0 ; j < nb_ville ; ++j)
            echange(&V1->villes[j], &V2->villes[j], 0);

    }
    else
        for(j = 0 ; j < nb_ville ; ++j)
            echange(&V1->villes[j], &V2->villes[j], 1);
    
    V1->km = V2->km;

    V1->nb_ville = V2->nb_ville;

    if(sens_uniq == 0)
        V2->km = tmp_km;
}

void trie_Liste_Visite(Liste V, int nb_visites){ /* trie à bulles */
    int i, permu, n;
    n = 1;
    permu = 1;
    while(permu){
        permu = 0;
        for(i = 0 ; i < nb_visites - n ; ++i)
            if(V[i].km > V[i + 1].km){
                echange_visite(&V[i], &V[i+1], 0);
                permu = 1;
            }
        ++n;
    }       
}

/* OPTION QUI CHARGE VILLES DE FICHIERS */

int max_ville(char *nom_fichier_carte){
    FILE *file_carte = fopen(nom_fichier_carte, "r");
    int max;
    char *mot = (char*) malloc(sizeof(SIZE_MOT));
    char *x = (char*) malloc(sizeof(SIZE_MOT));
    char *y = (char*) malloc(sizeof(SIZE_MOT));
    max = 0;
    while(fscanf(file_carte, "%s %s %s", mot, x, y) != EOF){
        if(abs(atoi(x)) > max)
            max = abs(atoi(x));
        if(abs(atoi(y)) > max)
            max = abs(atoi(y));
    }
    fclose(file_carte);
    free(mot);
    free(x);
    free(y);
    return max + 1;
}

int charger_ville(char *nom_fichier_carte, Ville *tab_ville, int max){
    FILE *file_carte = fopen(nom_fichier_carte, "r");
    int indice;
    char *mot =(char*) malloc(sizeof(SIZE_MOT));

    assert(nom_fichier_carte != NULL);

    indice = 0;

    while(fscanf(file_carte, "%s", mot) != EOF){
        
        strcpy(tab_ville[indice].nom, mot);
        fscanf(file_carte, "%s", mot);
        tab_ville[indice].x = atoi(mot);
        fscanf(file_carte, "%s", mot);
        tab_ville[indice].y = atoi(mot);
        
        indice++;
        mot =(char*) malloc(sizeof(SIZE_MOT));
    }
    fclose(file_carte);
    return indice;
}

/* DESSINE LE REPERE ORTHONORME */

void dessine_grille(int max){
    int x, y;
    int x_val, y_val;
    
    MLV_draw_line(LARGEUR_X/2, 0, LARGEUR_X/2, LARGEUR_Y, MLV_COLOR_WHITE);
    MLV_draw_line(0, LARGEUR_Y/2, LARGEUR_X, LARGEUR_Y/2, MLV_COLOR_WHITE);
    
    for(y=LARGEUR_Y/2, y_val=0 ; y<LARGEUR_Y ; y+=LARGEUR_Y/max/2, --y_val){
        MLV_draw_line(0, y, LARGEUR_X, y, MLV_COLOR_WHITE);
        MLV_draw_text(LARGEUR_X/2 + 2, y + 2, "%d", MLV_COLOR_WHITE, y_val);
    }


    for(y=LARGEUR_Y/2, y_val=0 ; y>0 ; y-=LARGEUR_Y/max/2, ++y_val){
        MLV_draw_line(0, y, LARGEUR_X, y, MLV_COLOR_WHITE);
        MLV_draw_text(LARGEUR_X/2 + 2, y + 2, "%d", MLV_COLOR_WHITE, y_val);
    }

    for(x=LARGEUR_X/2, x_val=0; x<LARGEUR_X; x+=LARGEUR_X/max/2, x_val++){
        MLV_draw_line(x, 0, x, LARGEUR_Y, MLV_COLOR_WHITE);
        MLV_draw_text(x+2, LARGEUR_Y/2 + 2, "%d", MLV_COLOR_WHITE, x_val);
    }

    for(x=LARGEUR_X/2, x_val=0; x>0; x-=LARGEUR_X/max/2, x_val--){
        MLV_draw_line(x, 0, x, LARGEUR_Y, MLV_COLOR_WHITE);
        MLV_draw_text(x+2, LARGEUR_Y/2 + 2, "%d", MLV_COLOR_WHITE, x_val);
    }

}