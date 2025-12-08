#include <unistd.h>
#include <stdio.h>
#include <lcarte.h>
#include <baseclient.h>
#include <voyants.h>
#include <timer.h>
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
    set_dispo();
    numero = lecture_numero_carte();
    printf("Numero de carte lue: %d\n", numero);
    
    if (carte_inseree() && dispo()){
        x = baseclient_authentifier(numero);
        if(x==1){
            printf("Authentification réussie\n");            
            timer_raz();
            do {
                blink_charge();
            } while(timer_valeurs() <= 8);  // Simule une charge de 8 secondes
        } else {
            printf("Authentification échouée\n");
            timer_raz();
            do {
                blink_defaut();
            } while(timer_valeurs() <= 8);  // Simule une charge de 8 secondes
        }
    }

    attente_retrait_carte();
    printf("Carte retirée\n");
    set_dispo();
}