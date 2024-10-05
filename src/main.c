// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
// #include <math.h>
// #ifdef _WIN32
// #include <windows.h>
// #include <tchar.h>
// #endif

#include "terminal_mode.h"
#include "graphic_mode.h"

int main(int argc, char *argv[])
{
    int choix = 0;
    char title[100] = "TP1_ALSDD : Systeme de Gestion des Polynomes";

#ifdef _WIN32
    SetConsoleTitle(title);
#else
    printf("\033]0;%s\007", title);
#endif

    clearScreen();
    printf("__________________________________________________________________________________________________\n");
    printf("|            ===============================( BIENVENUE )==============================          |\n");
    printf("|                             TP1_ALSDD   :   Systeme de Gestion des Polynomes                   |\n");
    printf("|                                                                                                |\n");
    printf("|                                 Nom     :   Mersel                                             |\n");
    printf("|                                 Prenom  :   Lyes                                               |\n");
    printf("|                                 Groupe  :   1CPI_G6                                            |\n");
    printf("|                                 Module  :   ALSDD                                              |\n");
    printf("|                                                                                                |\n");
    printf("|                             Sous la Supervision de Monsieur BALLA Amar                         |\n");
    printf("|                [ Ecole Nationale Superieure d'Informatique (ESI Ex INI) - Alger ]              |\n");
    printf("|            =========================================================================           |\n");
    printf("|________________________________________________________________________________________________|\n\n");
    printf("\n");
    printf("__________________________________________________________________________________________________\n");
    printf("|            ---------------------------( Choix du Mode )---------------------------             |\n");
    printf("|             Veuillez donner votre preference :                                                 |\n");
    printf("|                                          1   : Pour continuer sur cette Console                |\n");
    printf("|                                          2   : Pour passer au mode Graphique                   |\n");
    printf("|                                        Autre : Pour fermer le Programme                        |\n");
    printf("|             choix  =                                                                           |\n");
    printf("|            -----------------------------------------------------------------------             |\n");
    printf("|________________________________________________________________________________________________|\n\n");
    printf("--------------------------------------------------------------------------------------------------\n\n");
    Locate(25, 21);
    scanf("%d", &choix);
    Locate(0, 25);

    switch (choix)
    {
    case 1:
        Passage_Terminal();
        break;

    case 2:
        Passage_2D();
        break;

    default:
        break;
    }

    clearScreen();
    printf("\n\n");
    printf("====================================================================================================\n");
    printf("||------------------------------------------------------------------------------------------------||\n");
    printf("||       **********     [ FIN DU PROGRAMME   |   Merci Pour Votre Attention ]    **********       ||\n");
    printf("||------------------------------------------------------------------------------------------------||\n");
    printf("====================================================================================================\n\n\n");
    systemPause();
    return (0);
}
