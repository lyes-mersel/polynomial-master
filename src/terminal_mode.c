#include "terminal_mode.h"

void Passage_Terminal()
{
    Monome *Tete_Poly1 = NULL, *Tete_Poly2 = NULL, *Tete_Poly3 = NULL, *Tete_Poly4 = NULL;
    LLC_Poly *Tete_Ensemble = NULL, *Maillon_Ensemble = NULL;
    int choix = 0, nombre = 0;

DEBUT:
    clearScreen();
    printf("__________________________________________________________________________________________________\n");
    printf("|                             TP1_ALSDD   :   Systeme De Gestion Des Polynomes                   |\n");
    printf("|           <MERSEL>          <Lyes>          <1CPI_G6>              <ESI-Alger>     <2019/2020> |\n");
    printf("|________________________________________________________________________________________________|\n\n");
    printf("==================================================================================================\n");
    printf("__________________________________________________________________________________________________\n");
    printf("|          Veuillez choisir quelle action realiser :                                             |\n");
    printf("|                                                                                                |\n");
    printf("|               1)  Ajouter un nouveaux polynome                                                 |\n");
    printf("|               2)  Afficher la liste des polynomes                                              |\n");
    printf("|               3)  Supprimer un polynome existant                                               |\n");
    printf("|               4)  Copier un polynome existant                                                  |\n");
    printf("|               5)  Simplifier un polynome existant                                              |\n");
    printf("|                                                                                                |\n");
    printf("|               6)  Evaluer un polynome pour une valeur donnee                                   |\n");
    printf("|               7)  Additionner 02 polynomes                                                     |\n");
    printf("|               8)  Soustraire un polynome d'un autre                                            |\n");
    printf("|               9)  Multiplier 02 polynomes                                                      |\n");
    printf("|               10) Diviser un polynomes sur un autre                                            |\n");
    printf("|                                                                                                |\n");
    printf("|               11) Deriver un polynome                                                          |\n");
    printf("|               12) Integrer un polynome                                                         |\n");
    printf("|               13) Tracer la courbe d'un polynome                                               |\n");
    printf("|               14) L'application des polynomes (CRC)                                            |\n");
    printf("|               15) Quitter                                                                      |\n");
    printf("|                                                                                                |\n");
    printf("|          Votre choix ----->                                                                    |\n");
    printf("|________________________________________________________________________________________________|\n");
    Locate(30, 27);
    scanf("%d", &choix);
    clearScreen();

    switch (choix)
    {
    case 1:
        // Ajouter un nouveaux polynome
        printf(" Ajouter un nouveaux polynome : \n");
        lect_poly(&Tete_Poly1);
        ajout_poly_ensemble(&Tete_Ensemble, Tete_Poly1);
        printf("\n Operation reussit \n\n");
        systemPause();
        break;

    /********************************************************************************/
    case 2:
        // Afficher la liste des polynomes
        printf(" Afficher la liste des polynomes : \n");
        ecrit_ensemble_poly(Tete_Ensemble);
        printf("\n Operation reussit \n\n");
        systemPause();
        break;

    /********************************************************************************/
    case 3:
        // Supprimer un polynome existant
        printf(" Supprimer un polynome existant : \n");
        ecrit_ensemble_poly(Tete_Ensemble);
        printf("\n Veuillez taper le numero du polynome a supprimer ---> ");
        scanf("%d", &nombre);

        Maillon_Ensemble = adr_maillon_ens(Tete_Ensemble, nombre);
        if (Maillon_Ensemble == NULL)
        {
            printf("\n Y a rien a supprimer !\n\n");
        }
        else
        {
            supp_poly_ensemble(&Tete_Ensemble, Maillon_Ensemble);
            printf("\n Suppression reussit \n\n");
        }
        systemPause();
        break;

    /********************************************************************************/
    case 4:
        // Copier un polynome existant
        printf(" Copier un polynome existant : \n");
        ecrit_ensemble_poly(Tete_Ensemble);
        printf("\n Veuillez taper le numero du polynome a copier ---> ");
        scanf("%d", &nombre);

        Maillon_Ensemble = adr_maillon_ens(Tete_Ensemble, nombre);
        if (Maillon_Ensemble == NULL)
        {
            printf("\n Y a rien a copier !\n\n");
        }
        else
        {
            Tete_Poly1 = copie_poly(Maillon_Ensemble->adr1);
            printf("\n Operation reussit, voici le resultat : \n\n");
            ecrit_poly(Tete_Poly1);
        COPIE:
            printf("\n Que voulez vous faire : \n");
            printf("             1 : Pour le rajouter a la fin de la liste \n");
            printf("             2 : Pour revenir au menu principale \n");
            printf(" Votre choix ---> ");
            scanf("%d", &choix);
            switch (choix)
            {
            case 1:
                ajout_poly_ensemble(&Tete_Ensemble, Tete_Poly1);
                printf("\n Operation reussit \n\n");
                break;

            case 2:
                printf("\n");
                break;

            default:
                goto COPIE;
            }
        }
        systemPause();
        break;

    /********************************************************************************/
    case 5:
        // Simplifier un polynome existant
        printf(" Simplifier un polynome existant : \n");
        ecrit_ensemble_poly(Tete_Ensemble);
        printf("\n Veuillez taper le numero du polynome a simplifier ---> ");
        scanf("%d", &nombre);

        Maillon_Ensemble = adr_maillon_ens(Tete_Ensemble, nombre);
        if (Maillon_Ensemble == NULL)
        {
            printf("\n Y a rien a simplifier !\n\n");
        }
        else
        {
            Tete_Poly1 = copie_poly(Maillon_Ensemble->adr1);
            simpl_poly(&Tete_Poly1);
            printf("\n Operation reussit, voici le resultat : \n");
            ecrit_poly(Tete_Poly1);
        SIMPLE:
            printf("\n Que voulez vous faire : \n");
            printf("             1 : Pour le rajouter a la fin de la liste \n");
            printf("             2 : Pour remplacer le polynome par sa valeur simplifiee \n");
            printf("             3 : Pour revenir au menu principale \n");
            printf(" Votre choix ---> ");
            scanf("%d", &choix);
            switch (choix)
            {
            case 1:
                ajout_poly_ensemble(&Tete_Ensemble, Tete_Poly1);
                printf("\n Operation reussit \n\n");
                break;

            case 2:
                Maillon_Ensemble->adr1 = Tete_Poly1;
                printf("\n Operation reussit \n\n");
                break;

            case 3:
                printf("\n");
                break;

            default:
                goto SIMPLE;
            }
        }
        systemPause();
        break;

    /********************************************************************************/
    case 6:
        // Evaluer un polynome pour une valeur donnee
        printf(" Evaluer un polynome pour une valeur donnee : \n");
        ecrit_ensemble_poly(Tete_Ensemble);
        printf("\n Veuillez taper le numero du polynome a evaluer ---> ");
        scanf("%d", &nombre);

        Maillon_Ensemble = adr_maillon_ens(Tete_Ensemble, nombre);
        if (Maillon_Ensemble == NULL)
        {
            printf("\n Y a rien a evaluer !\n\n");
        }
        else
        {
            float nb_reel = 0;
            printf(" La valeur  --->  ");
            scanf("%f", &nb_reel);
            printf("\n Evaluation reussit : \n");
            printf("  Voici le resultat  --->  %.3f \n\n", eval_poly(Maillon_Ensemble->adr1, nb_reel));
        }
        systemPause();
        break;

    /********************************************************************************/
    case 7:
        // Additionner 02 polynomes
        printf(" Additionner 02 polynomes : \n");
        ecrit_ensemble_poly(Tete_Ensemble);
        printf("\n Veuillez taper le numero du 1er polynome a additionner  ---> ");
        scanf("%d", &nombre);

        Maillon_Ensemble = adr_maillon_ens(Tete_Ensemble, nombre);
        if (Maillon_Ensemble == NULL)
            printf("\n Y a rien a additionner !\n\n");
        else
        {
            Tete_Poly1 = Maillon_Ensemble->adr1;

            printf("\n Veuillez taper le numero du 2eme polynome a additionner ---> ");
            scanf("%d", &nombre);
            Maillon_Ensemble = adr_maillon_ens(Tete_Ensemble, nombre);
            if (Maillon_Ensemble == NULL)
                printf("\n Y a rien a additionner !\n\n");
            else
            {
                Tete_Poly2 = Maillon_Ensemble->adr1;

                Tete_Poly3 = add_poly(Tete_Poly1, Tete_Poly2);
                printf("\n Operation reussit, voici le resultat : \n");
                ecrit_poly(Tete_Poly3);
            ADD:
                printf("\n Que voulez vous faire : \n");
                printf("             1 : Pour le rajouter a la fin de la liste \n");
                printf("             2 : Pour revenir au menu principale \n");
                printf(" Votre choix ---> ");
                scanf("%d", &choix);
                switch (choix)
                {
                case 1:
                    ajout_poly_ensemble(&Tete_Ensemble, Tete_Poly3);
                    printf("\n Operation reussit \n\n");
                    break;

                case 2:
                    printf("\n");
                    break;

                default:
                    goto ADD;
                }
            }
        }
        systemPause();
        break;

    /********************************************************************************/
    case 8:
        // Soustraire un polynome d'un autre
        printf(" Soustraire un polynome d'un autre : \n");
        ecrit_ensemble_poly(Tete_Ensemble);
        printf("\n Veuillez taper le numero du 1er polynome ---> ");
        scanf("%d", &nombre);

        Maillon_Ensemble = adr_maillon_ens(Tete_Ensemble, nombre);
        if (Maillon_Ensemble == NULL)
            printf("\n Y a rien a soustraire !\n\n");
        else
        {
            Tete_Poly1 = Maillon_Ensemble->adr1;

            printf("\n Veuillez taper le numero du 2eme polynome pour le soustraire du 1er ---> ");
            scanf("%d", &nombre);
            Maillon_Ensemble = adr_maillon_ens(Tete_Ensemble, nombre);
            if (Maillon_Ensemble == NULL)
                printf("\n Y a rien a additionner !\n\n");
            else
            {
                Tete_Poly2 = Maillon_Ensemble->adr1;

                Tete_Poly3 = add_poly(Tete_Poly1, inverse_poly(Tete_Poly2));
                printf("\n Operation reussit, voici le resultat : \n");
                ecrit_poly(Tete_Poly3);
            SOUSTR:
                printf("\n Que voulez vous faire : \n");
                printf("             1 : Pour le rajouter a la fin de la liste \n");
                printf("             2 : Pour revenir au menu principale \n");
                printf(" Votre choix ---> ");
                scanf("%d", &choix);
                switch (choix)
                {
                case 1:
                    ajout_poly_ensemble(&Tete_Ensemble, Tete_Poly3);
                    printf("\n Operation reussit \n\n");
                    break;

                case 2:
                    printf("\n");
                    break;

                default:
                    goto SOUSTR;
                }
            }
        }
        systemPause();
        break;

    /********************************************************************************/
    case 9:
        // Multiplier 02 polynomes
        printf(" Multiplier 02 polynomes : \n");
        ecrit_ensemble_poly(Tete_Ensemble);
        printf("\n Veuillez taper le numero du 1er polynome a multiplier  ---> ");
        scanf("%d", &nombre);

        Maillon_Ensemble = adr_maillon_ens(Tete_Ensemble, nombre);
        if (Maillon_Ensemble == NULL)
            printf("\n Y a rien a multiplier !\n\n");
        else
        {
            Tete_Poly1 = Maillon_Ensemble->adr1;
            printf("\n Veuillez taper le numero du 2eme polynome a multiplier ---> ");
            scanf("%d", &nombre);

            Maillon_Ensemble = adr_maillon_ens(Tete_Ensemble, nombre);
            if (Maillon_Ensemble == NULL)
                printf("\n Y a rien a multiplier !\n\n");
            else
            {
                Tete_Poly2 = Maillon_Ensemble->adr1;

                Tete_Poly3 = mult_poly(Tete_Poly1, Tete_Poly2);
                printf("\n Operation reussit, voici le resultat : \n");
                ecrit_poly(Tete_Poly3);
            MULT:
                printf("\n Que voulez vous faire : \n");
                printf("             1 : Pour le rajouter a la fin de la liste \n");
                printf("             2 : Pour revenir au menu principale \n");
                printf(" Votre choix ---> ");
                scanf("%d", &choix);
                switch (choix)
                {
                case 1:
                    ajout_poly_ensemble(&Tete_Ensemble, Tete_Poly3);
                    printf("\n Operation reussit \n\n");
                    break;

                case 2:
                    printf("\n");
                    break;

                default:
                    goto MULT;
                }
            }
        }
        systemPause();
        break;

    /********************************************************************************/
    case 10:
        // Diviser un polynomes sur un autre
        printf("Diviser un polynomes sur un autre : \n");
        ecrit_ensemble_poly(Tete_Ensemble);
        printf("\n Veuillez taper le numero du polynome a diviser ---> ");
        scanf("%d", &nombre);

        Maillon_Ensemble = adr_maillon_ens(Tete_Ensemble, nombre);
        if (Maillon_Ensemble == NULL)
            printf("\n Y a rien a diviser !\n\n");
        else
        {
            Tete_Poly1 = Maillon_Ensemble->adr1;
            printf("\n Veuillez taper le numero du polynome diviseur  ---> ");
            scanf("%d", &nombre);

            Maillon_Ensemble = adr_maillon_ens(Tete_Ensemble, nombre);
            if (Maillon_Ensemble == NULL)
                printf("\n Y a rien a diviser !\n\n");
            else
            {
                Tete_Poly2 = Maillon_Ensemble->adr1;
                if (div_poly(Tete_Poly1, Tete_Poly2, &Tete_Poly3, &Tete_Poly4) != 0)
                    printf("\n Operation echouee --> Vous avez demande une division sur 0\n\n");
                else
                {
                    printf("\n Operation reussit \n");
                    printf(" Voici le Quotient : \n");
                    ecrit_poly(Tete_Poly3);
                    printf(" Voici le Reste : \n");
                    ecrit_poly(Tete_Poly4);
                DIV:
                    printf("\n Que voulez vous faire : \n");
                    printf("             1 : Pour rajouter les 02 polynomes a la fin de la liste \n");
                    printf("             2 : Pour revenir au menu principale \n");
                    printf(" Votre choix ---> ");
                    scanf("%d", &choix);
                    switch (choix)
                    {
                    case 1:
                        ajout_poly_ensemble(&Tete_Ensemble, Tete_Poly3);
                        ajout_poly_ensemble(&Tete_Ensemble, Tete_Poly4);
                        printf("\n Operation reussit \n\n");
                        break;

                    case 2:
                        printf("\n");
                        break;

                    default:
                        goto DIV;
                    }
                }
            }
        }
        systemPause();
        break;

    /********************************************************************************/
    case 11:
        // Deriver un polynome
        printf("Deriver un polynome : \n");
        ecrit_ensemble_poly(Tete_Ensemble);
        printf("\n Veuillez taper le numero du polynome a diriver ---> ");
        scanf("%d", &nombre);

        Maillon_Ensemble = adr_maillon_ens(Tete_Ensemble, nombre);
        if (Maillon_Ensemble == NULL)
            printf("\n Y a rien a diriver !\n\n");
        else
        {
            Tete_Poly1 = derivee_poly(Maillon_Ensemble->adr1);
            printf("\n Operation reussit, voici le resultat : \n\n");
            ecrit_poly(Tete_Poly1);
        DERIV:
            printf("\n Que voulez vous faire : \n");
            printf("             1 : Pour le rajouter a la fin de la liste \n");
            printf("             2 : Pour revenir au menu principale \n");
            printf(" Votre choix ---> ");
            scanf("%d", &choix);
            switch (choix)
            {
            case 1:
                ajout_poly_ensemble(&Tete_Ensemble, Tete_Poly1);
                printf("\n Operation reussit \n\n");
                break;

            case 2:
                printf("\n");
                break;

            default:
                goto DERIV;
            }
        }
        systemPause();
        break;

    /********************************************************************************/
    case 12:
        // Integrer un polynome
        printf("Integrer un polynome : \n");
        ecrit_ensemble_poly(Tete_Ensemble);
        printf("\n Veuillez taper le numero du polynome a integrer ---> ");
        scanf("%d", &nombre);

        Maillon_Ensemble = adr_maillon_ens(Tete_Ensemble, nombre);
        if (Maillon_Ensemble == NULL)
            printf("\n Y a rien a integrer !\n\n");
        else
        {
            Tete_Poly1 = primitive_poly(Maillon_Ensemble->adr1);
            printf("\n Operation reussit, voici le resultat : \n\n");
            ecrit_poly_primitif(Tete_Poly1);
        PRIMITIVE:
            printf("\n Que voulez vous faire : \n");
            printf("             1 : Pour le rajouter a la fin de la liste \n");
            printf("             2 : Pour revenir au menu principale \n");
            printf(" Votre choix ---> ");
            scanf("%d", &choix);
            switch (choix)
            {
            case 1:
                ajout_poly_ensemble(&Tete_Ensemble, Tete_Poly1);
                printf("\n Operation reussit \n\n");
                break;

            case 2:
                printf("\n");
                break;

            default:
                goto PRIMITIVE;
            }
        }
        systemPause();
        break;

    /********************************************************************************/
    case 13:
        // Tracer la courbe d'un polynome
        ecrit_ensemble_poly(Tete_Ensemble);
        printf("\n Veuillez taper le numero du polynome pour dessiner sa courbe ---> ");
        scanf("%d", &nombre);

        Maillon_Ensemble = adr_maillon_ens(Tete_Ensemble, nombre);
        if (Maillon_Ensemble == NULL)
        {
            printf("\n Y a rien a dessiner !\n\n");
            systemPause();
        }
        else
            dessiner_courbe_1(Maillon_Ensemble->adr1);
        break;

    /********************************************************************************/
    case 14:
        // L'application des polynomes (CRC)
        printf("L'application des polynomes (CRC) : \n");
        printf("\n Que voulez vous faire : \n");
        printf("              1  : Pour calculer le code CRC d'un message avant de l'emettre \n");
        printf("              2  : Pour verifier l'authenticite d'un message recu \n");
        printf("           Autre : Pour revenir au menu principale \n");
        printf(" Votre choix  --->  ");
        scanf("%d", &choix);
        printf("-------------------------------------------------------------------------------\n\n");
        long long G = 0, MOT = 0, MOT1 = 0, CRC = 0, MESSAGE = 0, puis = 0;
        Monome *Poly_G = NULL, *Poly_MOT = NULL, *Poly_CRC = NULL, *Poly_MESSAGE = NULL;
        float puiss = 0;

        switch (choix)
        {
        case 1:
            printf("Calcul du code CRC : \n");
            printf(" Le polynome generateur represente par un mot binaire : \n");
            printf("         G  =  ");
            scanf("%lld", &G);
            printf(" Le mot a transmettre represente par un mot binaire : \n");
            printf("         MOT  =  ");
            scanf("%lld", &MOT);
            if ((test_base(G, 2) == -1) || (test_base(MOT, 2) == -1))
            {
                printf("\n Veuillez verifier vos donnees! Elles ne sont pas en binaire ! \n\n");
                break;
            }
            puiss = (float)(nb_pos(G) - 1);
            puis = (long long)powf(10, puiss);
            MOT1 = MOT * puis;
            Poly_G = entier_vers_poly(G);
            Poly_MOT = entier_vers_poly(MOT1);
            if (div_poly_binaire(Poly_MOT, Poly_G, &Poly_CRC) == -1)
            {
                printf("\n Division sur 0 ! \n");
            }
            else
            {
                CRC = poly_vers_entier(Poly_CRC);
                printf("\n Le code CRC est : %lld \n", CRC);
                printf(" D'ou le message a transmettre est : %lld%lld \n\n", MOT, CRC);
            }
            break;

        case 2:
            printf("Verification de l'authenticite du message : \n");
            printf(" Le polynome generateur represente par un mot binaire : \n");
            printf("         G  =  ");
            scanf("%lld", &G);
            printf(" Le message recu represente par un mot binaire : \n");
            printf("         MESSAGE  =  ");
            scanf("%lld", &MESSAGE);
            if ((test_base(G, 2) == -1) || (test_base(MESSAGE, 2) == -1))
            {
                printf("\n Veuillez verifier vos donnees! Elles ne sont pas en binaire ! \n\n");
                break;
            }
            Poly_G = entier_vers_poly(G);
            Poly_MESSAGE = entier_vers_poly(MESSAGE);
            if (div_poly_binaire(Poly_MESSAGE, Poly_G, &Poly_MOT) == -1)
            {
                printf("\n Division sur 0 ! \n");
            }
            else
            {
                MOT = poly_vers_entier(Poly_MOT);
                if (MOT == 0)
                    printf("\n Il n'y a pas d'erreurs ! \n\n");
                else
                    printf("\n Y a une erreur ! \n\n");
            }
            break;

        default:
            break;
        }
        systemPause();
        break;

    /********************************************************************************/
    case 15:
        // Quitter
        return;

    default:
        break;
    }

    goto DEBUT;

    return;
}