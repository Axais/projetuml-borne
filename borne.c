#include <stdio.h>
#include <memoire_borne.h>
#include <voyants.h>
#include <timer.h>
#include "lecteurcarte.h"

int main()
{
    lecteurcarte_initialiser();
    voyants_initialiser();
    timer_initialiser();

    while (1)
    {
        lecteurcarte_lire_carte();
    }
}
