#include "voyants.h"
#include <donnees_borne.h>
#include <memoire_borne.h>

entrees *io_timer;
int shmid_timer;
int depart_timer;

void timer_initialiser()
{
    io_timer=acces_memoire(&shmid_timer);
    /* associe la zone de memoire partagee au pointeur */
    if (io_timer==NULL) printf("Erreur pas de mem sh\n");
    depart_timer=io_timer->timer_sec;
}

void timer_raz()
{
    depart_timer=io_timer->timer_sec; // Initialisation du timer par rapport au temps courant
}

int timer_valeurs()
{
    int valeur_timer;
    valeur_timer = io_timer->timer_sec - depart_timer;
    printf("Temps écoulé: %d secondes\n", valeur_timer);

    return valeur_timer;
}