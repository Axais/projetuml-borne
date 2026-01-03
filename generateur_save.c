/**
 * @file generateur_save.c
 * @brief Gestion du générateur de puissance.
 * Version adaptée aux spécifications "voyants" (Lot 3).
 */

#include "generateur_save.h"
#include "base_clients.h"
#include "lecteurcarte.h"
#include "lcarte.h"
#include "prise.h"
#include "voyants.h" // Indispensable pour timer_initialiser, blink_charge, etc.
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

/* Raccourcis pour la lisibilité des tensions */
#define V_12_OUVERT  12.0
#define V_9_ATTENTE   9.0
#define V_6_CHARGE    6.0

/* Pointeur local vers la mémoire partagée */
static entrees* shm_io = NULL;

void generateursave_initialiser(entrees* p_io) {
    /* On récupère le pointeur vers la mémoire partagée */
    shm_io = p_io;
    
    /* Initialisation de la prise (verrouillage, etc.) */
    prise_initialiser(p_io);
    
    /* * Initialisation du module voyants. 
     * Selon le fichier voyants.c fourni, cette fonction s'occupe 
     * elle-même de récupérer l'accès mémoire.
     */
    timer_initialiser();
}

void generateursave_generer_PWM(char* type) {
    /* Pilotage direct du PWM selon la chaîne reçue */
    if (strcmp(type, "DC") == 0) {
        shm_io->gene_pwm = DC;
    } else if (strcmp(type, "AC") == 0) {
        shm_io->gene_pwm = AC_1K;
    } else if (strcmp(type, "AC_VAR") == 0) {
        shm_io->gene_pwm = AC_CL;
    }
}

float generateursave_tension() {
    /* Retourne la tension lue dans la mémoire partagée */
    return (shm_io != NULL) ? shm_io->gene_u : 0.0;
}

void generateursave_ouvrir_contacteur_AC() {
    if (shm_io) shm_io->contacteur_AC = 0;
}

void generateursave_fermer_contacteur_AC() {
    if (shm_io) shm_io->contacteur_AC = 1;
}

void generateursave_deconnecter() {
    /* Arrêt du signal PWM */
    if (shm_io) shm_io->gene_pwm = STOP;
}

void generateursave_mef() {
    float tension = generateursave_tension();

    /* CAS 1 : Pas de voiture (12V) */
    if (tension == V_12_OUVERT) {
        /* On met la borne en dispo via votre fonction voyants.c */
        set_dispo(); 
        prise_devrouiller_trappe();
        generateursave_generer_PWM("DC");
        printf("En attente de vehicule...\r");
    }
    
    /* CAS 2 : Voiture branchée (9V) - PWM DC actif */
    else if (tension == V_9_ATTENTE && shm_io->gene_pwm == DC) {
        printf("\n[Detection] Vehicule branche.\n");
        prise_set_prise(VERT);
        prise_verouiller_trappe();
        /* On envoie le signal 1kHz pour dire qu'on est prêt */
        generateursave_generer_PWM("AC");
    }
    
    /* CAS 3 : Voiture demande la charge (6V) */
    else if (tension == V_6_CHARGE) {
        /* Si on était en attente (AC_1K), on lance la charge */
        if (shm_io->gene_pwm == AC_1K) {
            printf("\n[Charge] Demarrage de la charge...\n");
            generateursave_fermer_contacteur_AC();
            generateursave_generer_PWM("AC_VAR");
        } 
        /* Si on est déjà en charge (AC_CL), on maintient */
        else if (shm_io->gene_pwm == AC_CL) {
            /* Mise à jour du voyant selon l'état du bouton charge (via voyants.c) */
            set_charge();
            printf("Charge en cours... (%.1f V)\r", tension);
        }
    }
}

void generateursave_charger() {
    int execution = 1; /* Remplace les flags complexes */
    int demande_arret = 0;

    /* --- PHASE 1 : BOUCLE DE SUPERVISION --- */
    while (execution) {
        
        /* Priorité au bouton STOP */
        if (shm_io->bouton_stop == 1) {
            printf("\n[ALERTE] Bouton STOP appuye.\n");
            demande_arret = 1;
            break; /* Sortie de boucle */
        }

        /* Détection fin de charge par le véhicule (retour à 9V pendant la charge) */
        if (generateursave_tension() == V_9_ATTENTE && shm_io->gene_pwm == AC_CL) {
            printf("\n[INFO] Fin de charge detectee (Vehicule).\n");
            demande_arret = 0;
            break; /* Sortie de boucle */
        }

        /* Exécution de la machine à états */
        generateursave_mef();
    }

    /* --- PHASE 2 : SECURISATION --- */
    printf("\nArret du processus...\n");
    if (shm_io->contacteur_AC == 1) {
        generateursave_ouvrir_contacteur_AC();
    }
    generateursave_generer_PWM("DC");
    
    /* On allume la LED verte pour dire "Viens badger" */
    shm_io->led_charge = VERT; 

    if (demande_arret) {
        printf("Veuillez presenter votre badge pour valider l'arret.\n");
    } else {
        printf("Veuillez presenter votre badge pour reprendre le vehicule.\n");
    }

    /* --- PHASE 3 : AUTHENTIFICATION DE SORTIE --- */
    while (1) {
        /* Si une carte est présente */
        if (carte_inseree()) {
            int numero = lecture_numero_carte();
            
            /* Vérification : est-ce le bon client ? */
            if (baseclient_reprise(numero) == 1) {
                printf("\n[OK] Authentification reussie.\n");
                
                /* Reset des boutons et LEDs */
                shm_io->bouton_charge = 0;
                shm_io->bouton_stop = 0;
                shm_io->led_charge = OFF;
                
                prise_devrouiller_trappe();
                attente_retrait_carte();
                break; /* On sort de la boucle d'auth */
            } else {
                printf("Mauvais badge.\n");
                attente_retrait_carte();
            }
        } else {
            /* * Utilisation de votre fonction blink_charge() du lot 3.
             * Elle gère le clignotement rouge/vert en fonction du timer
             */
            blink_charge();
        }
    }

    /* --- PHASE 4 : DECONNEXION PHYSIQUE --- */
    printf("Veuillez debrancher le cable.\n");
    
    while (1) {
        /* On attend que la tension remonte à 12V */
        if (generateursave_tension() == V_12_OUVERT) {
            generateursave_deconnecter(); /* PWM = STOP */
            prise_verouiller_trappe();
            prise_set_prise(OFF);
            shm_io->bouton_stop = 0;
            
            /* Remise à l'état disponible */
            set_dispo(); 
            
            printf("\n[FIN] Session terminee. Au revoir.\n");
            break;
        } else {
            /* Petit message d'attente */
            printf("Attente deconnexion... \r");
        }
    }
}