#ifndef __GRAPHIC_LIB__
#define __GRAPHIC_LIB__

#include "logic_lib.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

/********************************************************************************/
/*                        Déclaration des structures                            */
/********************************************************************************/

#define WINDOW_WIDTH1 1280
#define WINDOW_HEIGHT1 720
#define FPS_LIMIT 50 // (50 milisecondes ==> 20 images par seconde ==> 20FPS)
/*  servira a limiter la vitesse de la diffusion des images
    c.à.dire : ne pas utiliser toute la puissance du processeur   */

extern SDL_Color NOIR, BLANC, BLEU, VERT, ORANGE;

typedef struct
{
    short int page;
    int x1, x2, y1, y2;
    SDL_bool mouseInside;
} Rectangle;

/********************************************************************************/
/*                        Déclaration des fonctions                            */
/********************************************************************************/

void SDL_ExitWithError(const char message[]);
void limiter_FPS(unsigned int limit);
SDL_bool isMouseInsideRect(int mouseX, int mouseY, Rectangle rect);
void dessiner_ligne(SDL_Renderer *rendu, float x1, float y1, float x2, float y2);
void dessiner_point(SDL_Renderer *rendu, float x1, float y1);
void dessiner_rectangle(SDL_Renderer *rendu, int x, int y, int largeur, int hauteur);
void dessiner_rectangle_5(SDL_Renderer *rendu, int x, int y, int largeur, int hauteur);
void dessiner_courbe_1(Monome *P_tete);

int SDL_lire_entier(int *nombre, SDL_Renderer *rendu, SDL_Surface **page, int X, int Y);
int SDL_lire_reel(float *nombre, SDL_Renderer *rendu, SDL_Surface **page, int X, int Y);

void ajout_new_polynome_2D(SDL_Renderer *rendu, LLC_Poly **Tete_Ensemble);
void afficher_liste_polynomes_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble);
void supprimer_polynome_2D(SDL_Renderer *rendu, LLC_Poly **Tete_Ensemble);
void copier_polynome_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble);
void simplifier_polynome_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble);
void evaluer_polynome_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble);
void addition_polynomes_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble);
void soustraction_polynomes_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble);
void multiplication_polynomes_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble);
void division_polynomes_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble);
void deriver_polynome_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble);
void integrer_polynome_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble);
void dessiner_courbe_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble);
void application_CRC_2D(SDL_Renderer *rendu);

#endif