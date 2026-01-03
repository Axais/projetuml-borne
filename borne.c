/**
 * @file borne.c
 * @brief Programme principal de la borne de recharge EV
 */

#include <stdio.h>
#include <stdlib.h>
#include <memoire_borne.h>
#include <donnees_borne.h>
#include "lecteurcarte.h"
#include "base_clients.h"
#include "voyants.h"

int shmid;
entrees* io;

void vider_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int choix = 0;
    char prenom[10], nom[10], ID_client[10];
    
    // Initialisation Mémoire Partagée
    io = acces_memoire(&shmid);
    if (io == NULL) {
        perror("Erreur memoire partagee");
        return -1;
    }

    initialisations_ports();
    baseclient_initialiser();
    lecteurcarte_initialiser(io);

    do {
        system("clear");
        printf("==================================\n");
        printf("==== BORNE DE RECHARGE EV v2.0 ===\n");
        printf("==================================\n");
        printf("=== 1) Gestion Base Clients     ==\n");
        printf("=== 2) Lancer Terminal Recharge ==\n");
        printf("=== 0) Quitter                  ==\n");
        printf("==================================\n");
        printf("Votre choix : ");
        
        if (scanf("%d", &choix) != 1) {
            vider_buffer(); // Protection contre saisie invalide
            continue;
        }

        switch(choix) {
            case 1: // Menu Base de Données
                system("clear");
                printf("--- GESTION CLIENTS ---\n");
                printf("1. Ajouter un client\n");
                printf("2. Supprimer un client\n");
                printf("Choix : ");
                if (scanf("%d", &choix) != 1) break;

                if (choix == 1) {
                    printf("Prenom : "); scanf("%9s", prenom);
                    printf("Nom : "); scanf("%9s", nom);
                    printf("ID (Badge) : "); scanf("%9s", ID_client);
                    baseclient_nouveauClient(prenom, nom, ID_client);
                } else if (choix == 2) {
                    printf("ID Client a supprimer : "); scanf("%9s", ID_client);
                    baseclient_supprimerClient(ID_client);
                }
                vider_buffer();
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;

            case 2: // Lancement du processus de charge
                lecteurcarte_lire_carte();
                break;
                
            case 0:
                printf("Fermeture du système...\n");
                break;

            default:
                printf("Option invalide.\n");
        }
    } while (choix != 0);

    liberation_ports();
    return 0;
}