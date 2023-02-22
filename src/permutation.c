#include "../inc/permutation.h"
#include <sys/time.h>

void mut(Visite *visite){
    int n, l, I, J, i, j;
    n = visite->nb_ville;

    l = rand() % (n / 2);
    I = rand() % n;
    J = rand() % n;
    while(abs(I - J) < l || I > (n-l) || J > (n - l)){ /* la distance entre I et J doit être plus grande que l */
        I = rand() % n;                               /* et I et J ne doivent pas déborder */
        J = rand() % n;
    }

    for(i = I , j = J ; i < I + l ; i++, j++){   /* Permutation */
        echange(&(visite->villes[i]), &(visite->villes[j]), 0);
    }
    distance_ville(visite);
}

void creer_visite(Liste V, int nb_ville, Ville *tab_ville, int i){
    int tab[nb_ville], indice, j;

    vide_tab(tab, nb_ville);

    for(j = 0 ; j < nb_ville ; j++){

        indice = rand() % nb_ville;
        
        while(est_dans_tab(tab, indice, nb_ville)) /* On vérifie que la ville à insérer n'est pas dèjà présente */
            indice = rand() % nb_ville;

        tab[j] = indice;

        V[i].villes[j].x = tab_ville[indice].x;
        V[i].villes[j].y = tab_ville[indice].y;

        V[i].villes[j].nom = (char *) malloc(sizeof(char) * 256);
        strcpy(V[i].villes[j].nom, tab_ville[indice].nom);

    }
    V[i].nb_ville = nb_ville;
    distance_ville(&V[i]);
}


int creer_toute_ville_clic(Ville* tab_ville){
    int x_pos, y_pos;
    int nb_ville;

    nb_ville = 0;

    MLV_draw_filled_rectangle(LARGEUR_X-60, 10, 50, 50, MLV_COLOR_RED);
    MLV_draw_text(LARGEUR_X-50, 25, "RUN", MLV_COLOR_WHITE);
    MLV_actualise_window();

    while(1){
        MLV_wait_mouse(&x_pos, &y_pos);
        if(LARGEUR_X-60 < x_pos && x_pos < LARGEUR_X -10)
            if(10 < y_pos && y_pos < 60)
                return nb_ville;

        MLV_draw_filled_rectangle(0, 45, 300, 20, MLV_COLOR_BLACK);
        MLV_draw_text(5, 45, "Nombre ville(s) : %d", MLV_COLOR_WHITE, nb_ville);

        tab_ville[nb_ville] = genere_ville();
        char str_new[256];
        sprintf(str_new, "%d", nb_ville); 
        strcpy(tab_ville[nb_ville].nom, str_new);
        tab_ville[nb_ville].x = x_pos;
        tab_ville[nb_ville].y = y_pos;


        nb_ville++;

        MLV_draw_filled_circle(x_pos, y_pos, 5, MLV_COLOR_BLUE);
        MLV_actualise_window();

    }

    return nb_ville;
}


void permutation(int alpha, int beta, int gamma, int nb_ville, int mode_option, char *nom_file){
    struct timeval temps_debut;
    struct timeval temps_fin;
    int temps_actuel;
    int precedent;
    Liste V, Vprime;
    Ville* tab_ville;

    int i, j, k, max;

    max = 1;

    tab_ville = (Ville*) malloc(sizeof(Ville) * MAX_NB_VILLE);

    for(i = 0 ; i < 256 ; ++i)
        tab_ville[i].nom = (char *) malloc(sizeof(char) * SIZE_MOT);

    k = alpha + beta + gamma;
    
    MLV_draw_text(5, 0, "Alpha (Précédents meilleurs mutés): %d", MLV_COLOR_WHITE, alpha);
    MLV_draw_text(5, 15, "Beta (Précédents meilleurs): %d", MLV_COLOR_WHITE, beta);
    MLV_draw_text(5, 30, "Gamma (Nouveau aléatoire): %d", MLV_COLOR_WHITE, gamma);



    if(mode_option == 0){
        nb_ville = creer_toute_ville_clic(tab_ville);
    }

    else if(mode_option == 1){
        for(i = 0 ; i < nb_ville ; i++){ /* On génère aléatoirement des villes */
            tab_ville[i] = genere_ville();
            char str_new[256];
            sprintf(str_new, "%d", i); 
            strcpy(tab_ville[i].nom, str_new);
        }
    }

    else if(mode_option == 2){
        max = max_ville(nom_file);
        nb_ville = charger_ville(nom_file, tab_ville, max);
    }

    gettimeofday(&temps_debut, NULL);

    V = (Visite*) malloc(sizeof(Visite) * k);
    
    /* Génération aléatoire de visites */

    for(i = 0 ; i < k ; ++i)
        V[i].villes = (Ville*) malloc(sizeof(Ville) * nb_ville);
    
    for(i = 0 ; i < k ; i++){
        creer_visite(V, nb_ville, tab_ville, i);
    }
    
    /* tri de la liste des visites par km */

    trie_Liste_Visite(V, k);

    precedent = 1000;
    /* Permutations */

    

    while(1){
        gettimeofday(&temps_fin, NULL);
        temps_actuel = (temps_fin.tv_sec - temps_debut.tv_sec)*1000 + (temps_fin.tv_usec - temps_debut.tv_usec) / 1000;

        MLV_draw_text(5, 5, "nb_sec : %.2f", MLV_COLOR_WHITE,(float) temps_actuel/1000);
        MLV_draw_text(5, 20, "longueur : %.2f km", MLV_COLOR_WHITE, V->km);

        Vprime = (Visite*) malloc(sizeof(Visite) * k);

        for(i = 0 ; i < k ; ++i){
            Vprime[i].villes = (Ville*) malloc(sizeof(Ville) * nb_ville);
            for(j = 0 ; j < nb_ville ; ++j)
                Vprime[i].villes[j].nom = (char*) malloc(sizeof(char) * SIZE_MOT);
        }

        for(i = 0 ; i < beta ; ++i){  /* On insère les beta meilleurs */
            echange_visite(&Vprime[i], &V[i], 1);
        }

        for(i = 0 ; i < alpha ; ++i){ /* Puis on ajoute les alpha meilleurs mais muté */
            mut(&(V[i]));
            echange_visite(&Vprime[beta + i], &V[i], 1);
        }

        for(i = 0 ; i < gamma ; ++i){  /* On termine par mettre gamma nouvelles visites */
            creer_visite(Vprime, nb_ville, tab_ville, alpha + beta + i);
        }

        trie_Liste_Visite(Vprime, k);

        V = Vprime;
                    
        /* dessine les chemins */

        int x, y, x1, y1, val;

        ++val;

        if(val == 1)
            val = 2;

        if(mode_option == 2)
            val =  LARGEUR_X / max;

        for(i = 0 ; i < nb_ville - 1 ; ++i){
            if(mode_option == 2){

                x = (V[0].villes[i].x * val) / 2 + LARGEUR_X / 2;
                y = LARGEUR_X - (V[0].villes[i].y * val) / 2 - LARGEUR_X / 2;

                x1 = (V[0].villes[i + 1].x * val) / 2 + LARGEUR_X / 2;
                y1 = LARGEUR_X - (V[0].villes[i + 1].y * val) / 2 - LARGEUR_X / 2;

                MLV_draw_line(x, y, x1, y1, MLV_COLOR_GREEN);
            }
            else
                MLV_draw_line(V[0].villes[i].x, V[0].villes[i].y, V[0].villes[i + 1].x, V[0].villes[i + 1].y, MLV_COLOR_GREEN);
        }

        /* dessine les villes */

        for(i = 0 ; i < nb_ville ; ++i){   
            if(mode_option == 2){

                if(V[0].km < precedent)
                    printf("%s ", V[0].villes[i].nom);

                x = (tab_ville[i].x * val) / 2 + LARGEUR_X / 2;
                y = LARGEUR_X - (tab_ville[i].y * val) / 2 - LARGEUR_X / 2;

                MLV_draw_filled_circle(x , y, 5, MLV_COLOR_BLUE);
            }
            else{
                if(V[0].km < precedent)
                    printf("ville-%s ", V[0].villes[i].nom);
                MLV_draw_filled_circle(tab_ville[i].x , tab_ville[i].y, 5, MLV_COLOR_BLUE);
            }
        }
        
        if(V[0].km < precedent){
            printf("%2f\n", V[0].km);
            precedent = V[0].km;
        }


        if(mode_option == 2)
            dessine_grille(max);
        
            
        MLV_actualise_window();
        sleep(0.25);
        MLV_clear_window(MLV_COLOR_BLACK);
    }
}