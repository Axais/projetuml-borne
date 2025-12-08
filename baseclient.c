#include <baseclient.h>
#include <stdio.h>
#include <stdlib.h>


int baseclient_authentifier(int numcarte){  

    static int num_client[30]; // Tableau réservé en mémoire pour les numéros de carte

    FILE *fichier;
    int cpt=0;

    fichier= fopen("baseclient.txt", "a+");
    if (fichier == NULL){
        printf("Fichier baseclient.txt non trouvé.\n");
        exit(1);
    } else {
        while (!feof(fichier)) {
            fscanf(fichier, "%d", &num_client[cpt]);
            cpt ++;
        }
    }
    
    int x=0, i;
    for (i= 0; i < cpt; i++) {
        if (numcarte == num_client[i]){
            x=1; // Authentification réussie
        }
    }

    if (x==0) {
            int y;
            printf("Numéro de carte invalide: %d\n", numcarte);
            printf("Voulez-vous créer un nouveau compte? (1: Oui, 0: Non): ");
            scanf("%d", &y);
            switch (y) {
                case '1':
                    num_client[i] = numcarte;
                    fprintf(fichier, "%d\n", numcarte);
                    fclose(fichier);
                    return 1; // Nouveau compte créé et authentification réussie
                case '0':
                    return 0; // Authentification échouée
                default:
                    printf("Entrée invalide. Veuillez réessayer.\n");
                    return 0; // Authentification échouée
            }
        }
    
    return 1; // Authentification réussie
}
