#include "prise.h"
#include <stdio.h>
// Inclusion supposée pour la gestion mémoire, comme vu dans borne.c 
#include <memoire_borne.h> 

static entrees *io = NULL;

void prise_initialiser(entrees *p_io)
{
    // Si p_io est fourni (venant du main), on l'utilise.
    // Sinon, on pourrait appeler acces_memoire(&shmid) ici si nécessaire.
    io = p_io;
    
    // État initial : Prise éteinte et trappe déverrouillée
    if (io != NULL) {
        io->led_prise = OFF;
        io->led_trappe = OFF; 
    }
}

void prise_set_prise(led couleur)
{
    if (io != NULL) {
        io->led_prise = couleur;
    }
}

void prise_verouiller_trappe(void)
{
    if (io != NULL) {
        // Dans le simulateur, allumer la LED trappe indique souvent le verrouillage
        io->led_trappe = VERT; 
        printf("Trappe VERROUILLEE\n");
    }
}

void prise_devrouiller_trappe(void)
{
    if (io != NULL) {
        io->led_trappe = OFF;
        printf("Trappe DEVERROUILLEE\n");
    }
}