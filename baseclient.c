#include <baseclient.h>

int baseclient_authentifier(int numcarte)
{
    // Implémentation fictive pour l'authentification
    if (numcarte == 4) {
        return 1; // Authentification réussie
    } else {
        return 0; // Authentification échouée
    }
}