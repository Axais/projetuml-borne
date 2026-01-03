#ifndef GENERATEUR_SAVE_H
#define GENERATEUR_SAVE_H

#include "donnees_borne.h"

/**
 * @brief Initialise le générateur et lie la mémoire partagée.
 * @param p_io Pointeur vers la structure d'entrées/sorties.
 */
void generateursave_initialiser(entrees* p_io);

/**
 * @brief Fonction principale qui gère le cycle complet de charge.
 * Contient la boucle de supervision, l'arrêt et la reprise véhicule.
 */
void generateursave_charger(void);

/**
 * @brief Machine à États Finis (MEF) du processus de charge.
 * Gère les transitions 12V -> 9V -> 6V.
 */
void generateursave_mef(void);

/**
 * @brief Configure le signal PWM (Pilote).
 * @param type "DC", "AC", "AC_VAR" ou "STOP".
 */
void generateursave_generer_PWM(char* type);

/**
 * @brief Lit la tension simulée du fil pilote.
 * @return La tension en Volts (12.0, 9.0, 6.0, etc.)
 */
float generateursave_tension(void);

/**
 * @brief Ouvre le contacteur de puissance (Coupe le courant).
 */
void generateursave_ouvrir_contacteur_AC(void);

/**
 * @brief Ferme le contacteur de puissance (Envoie le courant).
 */
void generateursave_fermer_contacteur_AC(void);

/**
 * @brief Arrête la génération PWM (Signal STOP).
 */
void generateursave_deconnecter(void);

#endif // GENERATEUR_SAVE_H