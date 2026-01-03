#ifndef BASE_CLIENTS_H
#define BASE_CLIENTS_H

#define MAX_CLIENTS 10 // Nombre maximum de clients dans la base de données

/**
 * @brief Structure représentant un client autorisé
 */
typedef struct {
    char prenom[20];
    char nom[20];
    char ID_client[10];
    int badge; // Badge actuellement utilisé pour une session
} enregistrement_client;

void baseclient_initialiser(void);
int baseclient_authentifier(int numero_carte); // Correction de "authetifier"
int baseclient_reprise(int numero_carte);
void baseclient_supprimerClient(char *ID_client);
void baseclient_nouveauClient(char *prenom, char *nom, char *ID_client);

#endif