#include "logic_lib.h"

/********************************************************************************/
/*                Procédure d'implémentation de la bibliothèque                 */
/********************************************************************************/

// allouer un espace mémoire de taille monome et retourne son adresse
Monome *allouer_mo()
{
    return malloc(sizeof(Monome));
}

// retourne l'adresse du prochain monome
Monome *suivant(Monome *M)
{
    return M->adr;
}

// On range le coefficient (C) dans le champ (coef) de M
void aff_coef(Monome *M, float C)
{
    M->coef = C;
    return;
}

// On range la puissance (P) dans le champ (puiss) de M
void aff_puiss(Monome *M, int P)
{
    M->puiss = P;
    return;
}

// On range l'adresse (ADR) dans le champ (adr) de M
void aff_adr(Monome *M, Monome *ADR)
{
    M->adr = ADR;
    return;
}

// Allouer un espace mémoire de taille LLC_Poly et retourne son adresse
LLC_Poly *allouer_poly()
{
    return malloc(sizeof(LLC_Poly));
}

/********************************************************************************/
// Supprime un des monomes du polynome
// Retourne (0) si la suppréssion est éffectuée, et retourne (-1) si non.
int supp_monome(Monome **P_tete, Monome *Mo)
{
    Monome *maillon = *P_tete;

    if (Mo == *P_tete)
    {
        *P_tete = suivant(*P_tete);
    }
    else
    {
        while ((maillon != NULL) && (suivant(maillon) != Mo))
            maillon = suivant(maillon);
        if (maillon == NULL)
            return (-1);
        aff_adr(maillon, suivant(Mo));
    }
    return 0;
}

/********************************************************************************/
// permute la position de deux monomes d'un polynome
void permut_monome(Monome *MO1, Monome *MO2)
{
    int sauv_puis = 0;
    float sauv_cof = 0;

    if (MO1 != MO2)
    {
        // Permuter les puissances
        sauv_puis = MO1->puiss;
        MO1->puiss = MO2->puiss;
        MO2->puiss = sauv_puis;
        // Permuter les coefficients
        sauv_cof = MO1->coef;
        MO1->coef = MO2->coef;
        MO2->coef = sauv_cof;
    }
    return;
}

/********************************************************************************/
// retourne l'adresse du monome qui a la plus grande puissance dans un polynome
Monome *max_puiss(Monome *P_tete)
{
    Monome *Mo = P_tete, *Mo_max_puis = P_tete;
    while (Mo != NULL)
    {

        if (Mo->puiss > Mo_max_puis->puiss)
            Mo_max_puis = Mo;
        Mo = suivant(Mo);
    }
    return Mo_max_puis;
}

/********************************************************************************/
// Libére tous les monomes formant un polynome
void free_poly(Monome **P_tete)
{
    Monome *sauv = NULL;
    while (*P_tete != NULL)
    {
        sauv = *P_tete;
        free(*P_tete);
        *P_tete = suivant(sauv);
    }
    return;
}

/********************************************************************************/
// retourne le nombre des coefficients d'un polynome
int taille_poly(Monome *P_tete)
{
    Monome *N = P_tete;
    int cpt = 0;
    while (N != NULL)
    {
        cpt++;
        N = suivant(N);
    }
    return cpt;
}

/********************************************************************************/
// Ajoute un Polynome à un Ensemble de Polynomes
void ajout_poly_ensemble(LLC_Poly **Ensemble_tete, Monome *P_tete)
{
    LLC_Poly *maillon = *Ensemble_tete, *mail = NULL;

    if (maillon == NULL)
    {
        *Ensemble_tete = allouer_poly();
        (*Ensemble_tete)->adr1 = P_tete;
        (*Ensemble_tete)->adr2 = NULL;
    }
    else
    {
        while (maillon->adr2 != NULL)
            maillon = maillon->adr2;
        mail = allouer_poly();
        maillon->adr2 = mail;
        mail->adr1 = P_tete;
        mail->adr2 = NULL;
    }
    return;
}

/********************************************************************************/
// Supprime un polynome d'un ensemble
// Retourne (0) si la suppréssion est éffectuée, et retourne (-1) si non.
int supp_poly_ensemble(LLC_Poly **Ensemble_tete, LLC_Poly *Poly)
{
    LLC_Poly *maillon = *Ensemble_tete;

    if (Poly == *Ensemble_tete)
    {
        *Ensemble_tete = (*Ensemble_tete)->adr2;
        free(Poly);
    }
    else
    {
        while ((maillon != NULL) && (maillon->adr2 != Poly))
            maillon = maillon->adr2;
        if (maillon == NULL)
            return (-1);
        maillon->adr2 = Poly->adr2;
        free(Poly);
    }
    return 0;
}

/********************************************************************************/
// Retourne l'adresse du maillon qui pointe à la tête du (num)eme polynome d'un ensemble
// Et retourner (NULL) s'il n'xiste pas
LLC_Poly *adr_maillon_ens(LLC_Poly *Ensemble_tete, int num)
{
    LLC_Poly *Ensemble = Ensemble_tete;
    int cpt = 1;

    if (num < 1)
        return NULL;

    while ((Ensemble != NULL) && (cpt < num))
    {
        Ensemble = Ensemble->adr2;
        cpt++;
    }

    if (cpt == num)
        return Ensemble;
    else
        return NULL;
}

/********************************************************************************/
// Construire un polynome
void lect_poly(Monome **P_tete)
{
    Monome *M, *N = NULL;
    int nb_coef, puis = 0;
    float cof = 0;

    printf("     ________________________________________________________________________________________\n");
    printf("     |--------------------------------( Lecture du Polynome )-------------------------------|\n");
    printf("     |       Veuillez introduire le nombre des coefficients de vote polynome : ");
    scanf("%d", &nb_coef);

    if (nb_coef <= 0)
    {
        *P_tete = NULL;
        printf("     |                             Votre polynome est vide (nul)                            |\n");
        printf("     |______________________________________________________________________________________|\n");
        return;
    }

    printf("     |          Entrez le 1 er monome en respectant la forme suivante :                     |\n");
    printf("     |                  <COEF * X^PUISS> (Ex : 2 * X^5)  ----->  ");
    *P_tete = allouer_mo();
    scanf("%f * X^%d", &cof, &puis);
    aff_coef(*P_tete, cof);
    aff_puiss(*P_tete, puis);
    aff_adr(*P_tete, NULL);
    N = *P_tete;
    for (int i = 2; i <= nb_coef; i++)
    {
        printf("     |           Entrez le %d eme monome en respectant la forme suivante :                   |\n", i);
        printf("     |                  <COEF * X^PUISS> (Ex : 2 * X^5)  ----->  ");
        scanf("%f * X^%d", &cof, &puis);
        M = allouer_mo();
        aff_coef(M, cof);
        aff_puiss(M, puis);
        aff_adr(M, NULL);
        aff_adr(N, M);
        N = M;
    }
    printf("     |______________________________________________________________________________________|\n");
    return;
}

/********************************************************************************/
// Affiche un polynome sous sa forme mathématique
void ecrit_poly(Monome *P_tete)
{
    Monome *M = P_tete;

    printf("     ________________________________________________________________________________________\n");
    printf("     |-------------------------------( Ecriture du Polynome )-------------------------------|\n");
    if (M == NULL)
    {
        printf("     |                              Ce Polynome est vide (nul)                              |\n");
        printf("     |______________________________________________________________________________________|\n");
        return;
    }

    printf("     |     P = ");
    while (suivant(M) != NULL)
    {
        printf("(%.2f * X^%d) + ", M->coef, M->puiss);
        M = suivant(M);
    }
    printf("(%.2f * X^%d)\n", M->coef, M->puiss);
    printf("     |______________________________________________________________________________________|\n");
    return;
}

/********************************************************************************/
// Affiche un polynome primitif sous sa forme mathématique
void ecrit_poly_primitif(Monome *P_tete)
{
    Monome *M = P_tete;

    printf("     ________________________________________________________________________________________\n");
    printf("     |-------------------------------( Ecriture du Polynome )-------------------------------|\n");
    if (M == NULL)
    {
        printf("     |                                    P = Constante                                     |\n");
        printf("     |______________________________________________________________________________________|\n");
        return;
    }

    printf("     |     P = ");
    while (M != NULL)
    {
        printf("(%.2f * X^%d) + ", M->coef, M->puiss);
        M = suivant(M);
    }
    printf("Constante\n");
    printf("     |______________________________________________________________________________________|\n");
    return;
}

/********************************************************************************/
// Construire un Ensemble de Polynomes
void lect_ensemble_poly(LLC_Poly **Ensemble_tete)
{
    LLC_Poly *Poly1 = NULL, *Poly2 = NULL;
    Monome *P_tete = NULL;
    int nb_poly = 0;

    Monome *M, *N = NULL;
    int nb_coef, puis = 0;
    float cof = 0;

    printf("     ________________________________________________________________________________________\n");
    printf("     |==============================( Lecture des Polynomes )===============================|\n");
    printf("     |     Veuillez introduire le nombre des Polynomes de vote Ensemble : ");
    scanf("%d", &nb_poly);

    if (nb_poly <= 0)
    {
        *Ensemble_tete = NULL;
        printf("     |                         Cet Ensemble De Polynomes est vide.                          |\n");
        printf("     |______________________________________________________________________________________|\n");
        return;
    }

    for (int i = 0; i < nb_poly; i++)
    {
        printf("     |                                                                                      |\n");
        printf("     |======================================================================================|\n");
        printf("     |                                                                                      |\n");
        lect_poly(&P_tete);
        Poly1 = allouer_poly();
        Poly1->adr1 = P_tete;
        Poly1->adr2 = NULL;
        if (i == 0)
            *Ensemble_tete = Poly1;
        else
            Poly2->adr2 = Poly1;
        Poly2 = Poly1;
    }
    printf("     |______________________________________________________________________________________|\n");
    return;
}

/********************************************************************************/
// Affiche un Ensemblre de polynomes sous leur formes mathématiques
void ecrit_ensemble_poly(LLC_Poly *Ensemble_tete)
{
    LLC_Poly *Poly = Ensemble_tete;
    Monome *P_tete = NULL;
    int cpt = 0;

    printf("     ________________________________________________________________________________________\n");
    printf("     |------------------------------( Ecriture des Polynomes )------------------------------|\n");
    if (Poly == NULL)
    {
        printf("     |                            Votre Ensemble est vide (nul).                            |\n");
        printf("     |______________________________________________________________________________________|\n");
        return;
    }

    while (Poly != NULL)
    {
        P_tete = Poly->adr1;
        cpt++;
        printf("     |     P%d = ", cpt);
        if (P_tete == NULL)
            printf(" Ce polynome est vide (nul).                                               \n");
        else
        {
            while (suivant(P_tete) != NULL)
            {
                printf("(%.2f * X^%d) + ", P_tete->coef, P_tete->puiss);
                P_tete = suivant(P_tete);
            }
            printf("(%.2f * X^%d)\n", P_tete->coef, P_tete->puiss);
        }
        Poly = Poly->adr2;
    }

    printf("     |______________________________________________________________________________________|\n");
    return;
}

/********************************************************************************/
// Retourne l'évaluation d'un polyôme pour une valeur réelle
float eval_poly(Monome *P_tete, float Val)
{
    float result = 0;
    Monome *M = P_tete;

    while (M != NULL)
    {
        if (M->coef != 0)
            result += (M->coef * powf(Val, M->puiss));
        M = suivant(M);
    }
    return result;
}

/********************************************************************************/
// Simplifier un polynome
void simpl_poly(Monome **P_tete)
{
    Monome *M = *P_tete, *N = NULL;
    int deg = 0;

    // Additionner les coeffs qui ont les mêmes degrés
    while (M != NULL)
    {
        N = suivant(M);
        while (N != NULL)
        {
            if (M->puiss == N->puiss)
            {
                M->coef += N->coef;
                supp_monome(&M, N);
            }
            N = suivant(N);
        }
        M = suivant(M);
    }

    // Supprimer les monomes dont leurs coefficients sont nuls
    M = *P_tete;
    while (M != NULL)
    {
        if (M->coef == 0)
            supp_monome(P_tete, M);
        M = suivant(M);
    }

    // Repositionner les coeffs à base de leur degrés
    M = *P_tete;
    if (M != NULL)
        while (suivant(M) != NULL)
        {
            if (M != max_puiss(M))
                permut_monome(M, max_puiss(M));
            M = suivant(M);
        }
    return;
}

/********************************************************************************/
// Comparer les degrés de 02 polynomes (P1 et P2)
// Retourne (0) si (deg(P1) = deg(P2)), un reel positif si (deg(P1) > deg(P2)), et un reel négatif si (deg(P1) < deg(P2))
float compare_degre_poly(Monome *P1_tete, Monome *P2_tete)
{
    Monome *mo1 = copie_poly(P1_tete);
    Monome *mo2 = copie_poly(P2_tete);

    simpl_poly(&mo1);
    simpl_poly(&mo2);

    while ((mo1 != NULL) && (mo2 != NULL))
    {
        if (mo1->puiss != mo2->puiss)
            return (mo1->puiss - mo2->puiss);
        mo1 = suivant(mo1);
        mo2 = suivant(mo2);
    }

    if ((mo1 == NULL) && (mo2 == NULL))
        return (0);
    else
    {
        if (mo1 == NULL)
            return (-1);
        else
            return 1;
    }
}

/********************************************************************************/
// Retourne une copie d'un polynome
Monome *copie_poly(Monome *P_tete)
{
    Monome *copie = NULL, *mo = P_tete;
    Monome *P = NULL, *Q = NULL;

    while (mo != NULL)
    {
        P = allouer_mo();
        aff_coef(P, mo->coef);
        aff_puiss(P, mo->puiss);
        aff_adr(P, NULL);
        if (copie == NULL)
            copie = P;
        else
            aff_adr(Q, P);
        Q = P;
        mo = suivant(mo);
    }
    return copie;
}

/********************************************************************************/
// Retourne l'inverse d'un polynome en multipliants tous les le coefficients par (-1)
Monome *inverse_poly(Monome *P_tete)
{
    Monome *inverse = NULL, *mo = P_tete;
    Monome *P = NULL, *Q = NULL;

    while (mo != NULL)
    {
        P = allouer_mo();
        aff_coef(P, -(mo->coef));
        aff_puiss(P, mo->puiss);
        aff_adr(P, NULL);
        if (inverse == NULL)
            inverse = P;
        else
            aff_adr(Q, P);
        Q = P;
        mo = suivant(mo);
    }
    return inverse;
}

/********************************************************************************/
// Additionne 02 polynomes (P1 + P2) et retourne le résultat
Monome *add_poly(Monome *P1_tete, Monome *P2_tete)
{
    Monome *result = NULL, *P = NULL, *Q = NULL;

    // Copier P1 vers result
    result = copie_poly(P1_tete);

    // Copier P2 et le coller à la fin de result
    P = copie_poly(P2_tete);
    Q = result;
    if (Q == NULL)
        Q = P;
    else
    {
        while (suivant(Q) != NULL)
            Q = suivant(Q);
        if (Q != NULL)
            aff_adr(Q, P);
    }

    // Simplifier le polynome résultat
    simpl_poly(&result);
    return result;
}

/********************************************************************************/
// Retourne la multiplication de 02 polynomes (P1 * P2)et retourne le résultat
Monome *mult_poly(Monome *P1_tete, Monome *P2_tete)
{
    Monome *mo1 = P1_tete, *mo2 = P2_tete;
    Monome *result = NULL, *P = NULL, *Q = NULL;

    while (mo1 != NULL)
    {
        while (mo2 != NULL)
        {
            P = allouer_mo();
            aff_coef(P, mo1->coef * mo2->coef);
            aff_puiss(P, mo1->puiss + mo2->puiss);
            aff_adr(P, NULL);
            if (result == NULL)
                result = P;
            else
                aff_adr(Q, P);
            Q = P;
            mo2 = suivant(mo2);
        }
        mo1 = suivant(mo1);
        mo2 = P2_tete;
    }
    simpl_poly(&result);
    return result;
}

/********************************************************************************/
// Réalize la division euclédienne (P1 / P2), Range le quotient dans (*quotinet) et le reste dans (*reste)
// Retourne (-1) dans le cas où il y a une division sur zéro, (0) sinon.
int div_poly(Monome *P1_tete, Monome *P2_tete, Monome **quotient, Monome **reste)
{
    Monome *P = NULL, *Q = NULL;

    if ((P2_tete == NULL) || (P2_tete->coef == 0))
        return (-1); // Division sur 0

    *quotient = NULL;
    *reste = copie_poly(P1_tete);
    simpl_poly(reste);
    while (compare_degre_poly(*reste, P2_tete) >= 0)
    {
        P = allouer_mo();
        aff_coef(P, ((*reste)->coef / P2_tete->coef));
        aff_puiss(P, ((*reste)->puiss - P2_tete->puiss));
        aff_adr(P, NULL);
        *reste = add_poly(*reste, inverse_poly(mult_poly(P2_tete, P)));
        if (*quotient == NULL)
            *quotient = P;
        else
            aff_adr(Q, P);
        Q = P;
    }
    simpl_poly(quotient);
    simpl_poly(reste);
    return 0;
}

/********************************************************************************/
// Retourne la fonction dérivée d'un polynome
Monome *derivee_poly(Monome *P_tete)
{
    Monome *derivee = NULL, *mo = P_tete;
    Monome *P, *Q = NULL;

    while (mo != NULL)
    {
        if (mo->puiss > 0)
        {
            P = allouer_mo();
            aff_coef(P, mo->coef * mo->puiss);
            aff_puiss(P, mo->puiss - 1);
            aff_adr(P, NULL);
            if (derivee == NULL)
                derivee = P;
            else
                aff_adr(Q, P);
            Q = P;
        }
        mo = suivant(mo);
    }
    simpl_poly(&derivee);
    return derivee;
}

/********************************************************************************/
// Retourne la fonction primitive d'un polynome
Monome *primitive_poly(Monome *P_tete)
{
    Monome *primitive = NULL, *mo = P_tete;
    Monome *P, *Q = NULL;
    float nb = 0;

    while (mo != NULL)
    {
        P = allouer_mo();
        aff_coef(P, mo->coef / (mo->puiss + 1));
        aff_puiss(P, mo->puiss + 1);
        aff_adr(P, NULL);
        if (primitive == NULL)
            primitive = P;
        else
            aff_adr(Q, P);
        Q = P;
        mo = suivant(mo);
    }
    simpl_poly(&primitive);
    return primitive;
}

/********************************************************************************/
// Executer la commande clear de la console (windows + linux)
void clearScreen()
{
#ifdef _WIN32
    // Windows implementation using system("cls")
    system("CLS");
#else
    // Linux and other POSIX systems using ANSI escape codes
    printf("\033[H\033[2J");
    fflush(stdout);
#endif
}

/********************************************************************************/
// Pour les machines windows: Faire une pause
// Attendre la validation du user pour continuer
void systemPause()
{
#ifdef _WIN32
    // Windows implementation using system("cls")
    system("PAUSE");
#else
    // Linux and other POSIX systems using ANSI escape codes
    printf("Press Enter to continue... ");
    char c;
    getchar();
    scanf("%c", &c);
#endif
}

/********************************************************************************/
// Changer la postion du curseur dans un Terminal
void Locate(int x, int y)
{
#ifdef _WIN32
    // Windows implementation using system("cls")
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD C;
    C.X = (SHORT)x;
    C.Y = (SHORT)y;
    SetConsoleCursorPosition(H, C);
#else
    // Linux and other POSIX systems using ANSI escape codes
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
#endif

    return;
}
/********************************************************************************/
// Retourne le nombre de positions d'un entier
int nb_pos(long long nb)
{
    int p = 0;
    long long q = 1;
    while (nb / q != 0)
    {
        p++;
        q *= 10;
    }
    return p;
}

/********************************************************************************/
// Verifier si la representation d'un nombre dans une base >=2 est juste
// Renvoiet (0) si oui et (-1) si non.
int test_base(long long nombre, int base)
{
    int p = 0;

    if (base < 2)
        return (-1);
    for (int i = 0; i < nb_pos(nombre); i++)
    {
        p = powf(10, i);
        if ((nombre / p) % 10 >= base)
            return (-1);
    }
    return 0;
}

/********************************************************************************/
// Representer un entier sous une forme polynomiale
Monome *entier_vers_poly(long long nombre)
{
    Monome *Tete = NULL, *P = NULL, *Q = NULL;
    long long p10 = 0;

    for (int i = 0; i < nb_pos(nombre); i++)
    {
        P = allouer_mo();
        p10 = (long long)powf(10, i);
        aff_coef(P, (nombre / p10) % 10);
        aff_puiss(P, i);
        aff_adr(P, NULL);
        if (Tete == NULL)
            Tete = P;
        else
            aff_adr(Q, P);
        Q = P;
    }
    simpl_poly(&Tete); // Supprimer les monomes nuls

    return Tete;
}

/********************************************************************************/
// Representer polynome par un entier
long long poly_vers_entier(Monome *Tete)
{
    Monome *P = Tete;
    long long nombre = 0;
    int puis = 0, cof = 0;

    while (P != NULL)
    {
        cof = (int)P->coef;
        puis = (int)P->puiss;
        nombre += cof * powf(10, puis);
        P = suivant(P);
    }
    return nombre;
}

/********************************************************************************/
// En Binaire : Simplifier un polynome
void simpl_poly_binaire(Monome **P_tete)
{
    Monome *M = *P_tete, *N = NULL;
    int deg = 0, cof = 0;

    // Additionner les coeffs qui ont les mêmes degrés
    while (M != NULL)
    {
        N = suivant(M);
        while (N != NULL)
        {
            if (M->puiss == N->puiss)
            {
                M->coef += N->coef;
                supp_monome(&M, N);
            }
            N = suivant(N);
        }
        M = suivant(M);
    }

    // Remettre les coeff a la base 2
    M = *P_tete;
    while (M != NULL)
    {
        if ((M->coef != 0) && (M->coef != 1))
        {
            cof = (int)(M->coef);
            if (cof < 0)
                cof *= -1;
            aff_coef(M, cof % 2);
        }
        M = suivant(M);
    }

    // Supprimer les monomes dont leurs coefficients sont nuls
    M = *P_tete;
    while (M != NULL)
    {
        if (M->coef == 0)
            supp_monome(P_tete, M);
        M = suivant(M);
    }

    // Repositionner des coeffs à base de leur degrés
    M = *P_tete;
    if (M != NULL)
        while (suivant(M) != NULL)
        {
            if (M != max_puiss(M))
                permut_monome(M, max_puiss(M));
            M = suivant(M);
        }

    return;
}

/********************************************************************************/
// En Binaire :
// Additionne 02 polynomes (P1 + P2) et retourne le résultat
Monome *add_poly_binaire(Monome *P1_tete, Monome *P2_tete)
{
    Monome *result = NULL, *P = NULL, *Q = NULL;

    // Copier P1 vers result
    result = copie_poly(P1_tete);

    // Copier P2 et le coller à la fin de result
    P = copie_poly(P2_tete);
    Q = result;
    if (Q == NULL)
        Q = P;
    else
    {
        while (suivant(Q) != NULL)
            Q = suivant(Q);
        if (Q != NULL)
            aff_adr(Q, P);
    }

    // Simplifier le polynome résultat
    simpl_poly_binaire(&result);
    return result;
}

/********************************************************************************/
// En Binaire :
// Réalize la division euclédienne (P1 / P2), Range le reste dans (*reste)
// Retourne (-1) dans le cas où il y a une division sur zéro, (0) sinon.
int div_poly_binaire(Monome *P1_tete, Monome *P2_tete, Monome **reste)
{
    Monome *P = NULL, *Q = NULL;

    P = copie_poly(P2_tete);
    simpl_poly(&P);
    if (P == NULL) // Division sur 0
        return (-1);

    *reste = copie_poly(P1_tete);
    simpl_poly_binaire(reste);
    while (compare_degre_poly(*reste, P2_tete) >= 0)
    {
        P = allouer_mo();
        aff_coef(P, 1);
        aff_puiss(P, ((*reste)->puiss - P2_tete->puiss));
        aff_adr(P, NULL);
        *reste = add_poly_binaire(*reste, inverse_poly(mult_poly(P2_tete, P)));
    }
    simpl_poly_binaire(reste);
    return 0;
}

/********************************************************************************/
