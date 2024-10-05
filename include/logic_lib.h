#ifndef __LOGIC_LIB__
#define __LOGIC_LIB__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/********************************************************************************/
/*                        Déclaration des structures                            */
/********************************************************************************/

typedef struct Monome
{
    // Le coefficient du Monome
    float coef;

    // Le degré du Monome
    int puiss;

    // Adresse vers le prochain Monome
    struct Monome *adr;
} Monome;

typedef struct LLC_Poly
{
    // Adresse vers la tête du polynome
    struct Monome *adr1;

    // Adresse vers le prochain élément de la liste
    struct LLC_Poly *adr2;
} LLC_Poly;

/********************************************************************************/
/*                        Déclaration des fonctions                            */
/********************************************************************************/

Monome *allouer_mo();
Monome *suivant(Monome *M);
void aff_coef(Monome *M, float C);
void aff_puiss(Monome *M, int P);
void aff_adr(Monome *M, Monome *ADR);
LLC_Poly *allouer_poly();

int supp_monome(Monome **P_tete, Monome *Mo);
void permut_monome(Monome *MO1, Monome *MO2);
Monome *max_puiss(Monome *P_tete);
void free_poly(Monome **P_tete);
int taille_poly(Monome *M);
void ajout_poly_ensemble(LLC_Poly **Ensemble_tete, Monome *P_tete);
int supp_poly_ensemble(LLC_Poly **Ensemble_tete, LLC_Poly *Poly);
LLC_Poly *adr_maillon_ens(LLC_Poly *Ensemble_tete, int num);
void lect_poly(Monome **P_tete);
void lect_ensemble_poly(LLC_Poly **Ensemble_tete);
void ecrit_poly(Monome *P_tete);
void ecrit_poly_primitif(Monome *P_tete);
void ecrit_ensemble_poly(LLC_Poly *Ensemble_tete);
float eval_poly(Monome *P_tete, float Val);
void simpl_poly(Monome **P_tete);
float compare_degre_poly(Monome *P1_tete, Monome *P2_tete);
Monome *copie_poly(Monome *P_tete);
Monome *inverse_poly(Monome *P_tete);
Monome *add_poly(Monome *P1_tete, Monome *P2_tete);
int div_poly_binaire(Monome *P1_tete, Monome *P2_tete, Monome **reste);
Monome *mult_poly(Monome *P1_tete, Monome *P2_tete);
int div_poly(Monome *P1_tete, Monome *P2_tete, Monome **quotient, Monome **reste);
Monome *derivee_poly(Monome *P_tete);
Monome *primitive_poly(Monome *P_tete);

void clearScreen();
void systemPause();
void Locate(int x, int y);
int nb_pos(long long nb);
int test_base(long long nombre, int base);
Monome *entier_vers_poly(long long nombre);
long long poly_vers_entier(Monome *Tete);
void simpl_poly_binaire(Monome **P_tete);


#endif
