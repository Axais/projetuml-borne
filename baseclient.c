/**
 * @file base_clients.c
 * @brief Implémentation réelle de la BDD clients 
 */
#include "baseclient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static enregistrement_client bdd_client[MAX_CLIENTS];

static int nombreClients = 0;

void baseclient_initialiser() {
    enregistrement_client data[3] = {
        {"Jean", "Moreau", "3141"},
        {"Isabelle", "Durant", "1121"},
        {"Fleur", "Deschamps", "9101"}
    };
    
    nombreClients = 3;

    for(int i = 0; i < nombreClients; i++) {
        bdd_client[i] = data[i]; // Copie des données initiales
        bdd_client[i].badge = 0; // Aucun badge actif au départ
    }
}

int baseclient_authentifier(int numero_carte) {
    for (int i = 0; i < nombreClients; i++) {
        if ((int)strtol(bdd_client[i].ID_client, NULL, 10) == numero_carte) {
            bdd_client[i].badge = numero_carte; // Marquer ce badge comme actif
            return 1;
        }
    }
    return -1;
}

int baseclient_reprise(int numero_carte) {
    // Vérifie si la carte présentée est celle qui a lancé la charge
    for (int i = 0; i < nombreClients; i++) {
        if (bdd_client[i].badge == numero_carte) {
            bdd_client[i].badge = 0; // Réinitialisation fin de session
            return 1;
        }
    }
    return -1;
}

void baseclient_nouveauClient(char *prenom, char *nom, char *ID_client) {
    if (nombreClients >= MAX_CLIENTS) {
        printf("\nErreur: Base de donnees pleine.\n");
        return;
    }
    
    // Copie sécurisée des chaînes
    snprintf(bdd_client[nombreClients].prenom, sizeof(bdd_client[nombreClients].prenom), "%s", prenom);
    snprintf(bdd_client[nombreClients].nom, sizeof(bdd_client[nombreClients].nom), "%s", nom);
    snprintf(bdd_client[nombreClients].ID_client, sizeof(bdd_client[nombreClients].ID_client), "%s", ID_client);
    bdd_client[nombreClients].badge = 0; // Aucun badge actif au départ

    nombreClients++;
    printf("Client ajoute. Nombre total de clients : %d\n", nombreClients);
}

void baseclient_supprimerClient(char *clientID) {
    int found = 0;
    for (int i = 0; i < nombreClients; i++) {
        if (strcmp(bdd_client[i].ID_client, clientID) == 0) {
            found = 1;
            for (int j = i; j < nombreClients - 1; j++) {
                bdd_client[j] = bdd_client[j+1];
            }
            nombreClients--;
            printf("Client %s supprime.\n", clientID);
            break;
        }
    }
    if (!found){
         printf("Client ID %s non trouve.\n", clientID);
        }
}