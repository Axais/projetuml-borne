#include "voyants.h"
#include <stdio.h>
#include <memoire_borne.h>
#include <lcarte.h>
#include <donnees_borne.h>
#include <baseclient.h>
#include <unistd.h>


entrees *io_voyants;
int shmid_voyants;
int depart_timer_voyants;

void voyants_initialiser()
{
    io_voyants=acces_memoire(&shmid_voyants);
    /* associe la zone de memoire partagee au pointeur */
    if (io_voyants==NULL) printf("Erreur pas de mem sh voyants\n");
    depart_timer_voyants=io_voyants->timer_sec;
}

void blink_charge(){
    if (((io_voyants->timer_sec - depart_timer_voyants) % 2) == 0){ // Toutes les 2 secondes
            io_voyants->led_charge = VERT; 
        } else {
            io_voyants->led_charge = OFF;
        }
    
    usleep(100000); // Pause de 0.1 seconde pour éviter un clignotement trop rapide
}

void blink_defaut(){
    if (((io_voyants->timer_sec - depart_timer_voyants) % 2) == 0){ // Toutes les 2 secondes
            io_voyants->led_defaut = ROUGE;
        } else {
            io_voyants->led_defaut = OFF;
        }
    
    usleep(100000); // Pause de 0.1 seconde pour éviter un clignotement trop rapide
}
void set_charge()
{
    if(io_voyants->bouton_charge==1){
        io_voyants->led_charge = VERT;
    } else {
        io_voyants->led_charge = OFF;
    }

}


void set_dispo()
{
    if(carte_inseree()){
        io_voyants->led_dispo = OFF;
    } else {
        io_voyants->led_dispo = VERT;
    }
}


int dispo()
{
    return carte_inseree();
}