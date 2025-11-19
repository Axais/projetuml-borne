#include "voyants.h"
#include <stdio.h>
#include <memoire_borne.h>
#include <lcarte.h>
//#include <mem_sh.h>
//#include <donnees.h>
#include <donnees_borne.h>
//#include <memoire_borne.h>
//#include <lecteurcarte.h>
#include <baseclient.h>
//#include <l_carte.h>


entrees *io;
int shmid;
int depart_timer;

void timer_initialiser()
{
    io=acces_memoire(&shmid);
    /* associe la zone de memoire partagee au pointeur */
    if (io==NULL) printf("Erreur pas de mem sh\n");
        depart_timer=io->timer_sec;
    
}

void blink_charge(){
    if (((io->timer_sec - depart_timer) % 2) == 0){
        if (baseclient_authentifier(lecture_numero_carte())==1){
            io->led_charge = VERT;
        } else {
            io->led_charge = ROUGE;
        }
    } else {
        io->led_charge = OFF;
    }
}

void set_charge()
{
    if(io->bouton_charge==1){
        io->led_charge = VERT;
    } else {
        io->led_charge = OFF;
    }

}

void set_dispo()
{
    if(carte_inseree()){
        io->led_charge = OFF;
    } else {
        io->led_charge = VERT;
    }
}

int dispo()
{
    return carte_inseree();
}