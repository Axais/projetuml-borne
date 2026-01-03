#ifndef PRISE_H
#define PRISE_H

#include "donnees_borne.h" // Nécessaire pour le type 'led' 

/**
 * @brief Initialise le module prise et l'accès à la mémoire partagée.
 * @param p_io Pointeur vers la structure partagée (optionnel selon votre architecture mémoire)
 */
void prise_initialiser(entrees *p_io);

/**
 * @brief Définit la couleur de la LED de la prise.
 * @param couleur : OFF, ROUGE ou VERT
 */
void prise_set_prise(led couleur);

/**
 * @brief Simule le verrouillage de la trappe (ex: allume LED trappe).
 */
void prise_verouiller_trappe(void);

/**
 * @brief Simule le déverrouillage de la trappe.
 */
void prise_devrouiller_trappe(void);

#endif // PRISE_H