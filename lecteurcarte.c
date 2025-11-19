#include <unistd.h>
#include <stdio.h>
#include <lcarte.h>
#include <baseclient.h>
#include "lecteurcarte.h"

// Attributs privés de lecteur carte
int numero;
int x;

void lecteurcarte_initialiser()
{
    initialisations_ports();
}

void lecteurcarte_lire_carte()
{
    attente_insertion_carte();
    numero = lecture_numero_carte();
    printf("Numero de carte lue: %d\n", numero);
    
    if (carte_inseree()/*&&voyants disponible*/){
        x = baseclient_authentifier(numero);
        if(x==1){
            printf("Authentification réussie\n");
        } else {
            printf("Authentification échouée\n");
        }
    }

    attente_retrait_carte();
    printf("Carte retirée\n");
}