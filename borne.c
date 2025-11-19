#include <stdio.h>
#include <memoire_borne.h>

#include "lecteurcarte.h"

int main()
{
    lecteurcarte_initialiser();

    while (1)
    {
        lecteurcarte_lire_carte();
    }
}
