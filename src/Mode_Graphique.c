#include "Biblio.h"
#include "Biblio_Graphique.h"

void Passage_2D()
{
    LLC_Poly *Tete_Ensemble = NULL;

    SDL_Window *window = NULL;
    SDL_Renderer *rendu = NULL;

    //Lancement SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        SDL_ExitWithError("Initialisation SDL échouéee");

    // Lancement SDL_ttf
    if (TTF_Init() != 0)
        SDL_ExitWithError("Initialisation SDL_ttf échouée");

    // Création Fentre
    window = SDL_CreateWindow("TP1_ALSDD : Systeme De Gestion Des Polynomes", SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH1, WINDOW_HEIGHT1, 0);
    if (window == NULL)
        SDL_ExitWithError("Création Fenêtre échouée");

    // Création des Rendus
    rendu = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (rendu == NULL)
        SDL_ExitWithError("Création du Rendu échouée");

    /*****************************************************************************/
    // Les surfaces
    SDL_Surface *page0 = NULL, *page1 = NULL, *page2 = NULL;
    SDL_Surface *page0_vert = NULL, *page0_orange = NULL, *page0_bleu = NULL;

    page0 = IMG_Load("images/00.jpg");
    if (page0 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    page0_bleu = IMG_Load("images/00.jpg");
    if (page0_bleu == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    page0_vert = IMG_Load("images/00.jpg");
    if (page0_vert == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    page0_orange = IMG_Load("images/00.jpg");
    if (page0_orange == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    page1 = IMG_Load("images/01.jpg");
    if (page1 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    page2 = IMG_Load("images/02.jpg");
    if (page2 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    /********************************************************************************/
    // Les polices
    SDL_Rect position_texte;
    SDL_Surface *texte_noir = NULL, *texte_bleu = NULL, *texte_vert = NULL, *texte_orange = NULL;
    TTF_Font *police1 = NULL, *police2 = NULL, *police3 = NULL;
    

    police1 = TTF_OpenFont("polices/angelina.ttf", 100);
    if (police1 == NULL)
        SDL_ExitWithError("Impossible de charger la police angelina");
    TTF_SetFontStyle(police1, TTF_STYLE_UNDERLINE);

    police2 = TTF_OpenFont("polices/times.ttf", 40);
    if (police2 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

    police3 = TTF_OpenFont("polices/times.ttf", 40);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    texte_noir = TTF_RenderText_Blended(police1, "BIENVENUE", NOIR);
    texte_bleu = TTF_RenderText_Blended(police1, "BIENVENUE", BLEU);
    texte_vert = TTF_RenderText_Blended(police1, "BIENVENUE", VERT);
    texte_orange = TTF_RenderText_Blended(police1, "BIENVENUE", ORANGE);
    position_texte.x = 430;
    position_texte.y = 20;
    SDL_BlitSurface(texte_noir, NULL, page0, &position_texte);
    SDL_BlitSurface(texte_bleu, NULL, page0_bleu, &position_texte);
    SDL_BlitSurface(texte_vert, NULL, page0_vert, &position_texte);
    SDL_BlitSurface(texte_orange, NULL, page0_orange, &position_texte);

    /*****************************************************************************/
    // Les textures
    SDL_Texture *texture_p0 = NULL, *texture_p1 = NULL, *texture_p2 = NULL;
    SDL_Texture *texture_p0_bleu = NULL, *texture_p0_vert = NULL, *texture_p0_orange = NULL;
    SDL_Rect position_textures;

    position_textures.x = 0;
    position_textures.y = 0;
    texture_p0 = SDL_CreateTextureFromSurface(rendu, page0);
    if (texture_p0 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p0, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");

    texture_p0_bleu = SDL_CreateTextureFromSurface(rendu, page0_bleu);
    if (texture_p0_bleu == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p0_bleu, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");

    texture_p0_vert = SDL_CreateTextureFromSurface(rendu, page0_vert);
    if (texture_p0_vert == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p0_vert, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");

    texture_p0_orange = SDL_CreateTextureFromSurface(rendu, page0_orange);
    if (texture_p0_orange == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p0_orange, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");

    texture_p1 = SDL_CreateTextureFromSurface(rendu, page1);
    if (texture_p1 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p1, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");

    texture_p2 = SDL_CreateTextureFromSurface(rendu, page2);
    if (texture_p2 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p2, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");

    if (SDL_RenderCopy(rendu, texture_p0, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
    if (SDL_SetRenderDrawColor(rendu, 255, 20, 20, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    SDL_RenderPresent(rendu);

    /********************************************************************************/
    // Evênements :
    SDL_bool program_launched = SDL_TRUE;
    short int rendu_modifie = -1, num_page = 0, ancienne_page = 0, cpt = 0;
    unsigned int temps_limite = 0;
    char chaine_char[20] = "";

    while (program_launched)
    {
        SDL_Event event;

        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        if (num_page == 0)
        {
            cpt = (cpt + 1) % 20;
            switch (cpt)
            {
            case 0:
                if (SDL_RenderCopy(rendu, texture_p0, NULL, &position_textures) != 0)
                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                SDL_RenderPresent(rendu);
                break;

            case 5:
                if (SDL_RenderCopy(rendu, texture_p0_bleu, NULL, &position_textures) != 0)
                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                SDL_RenderPresent(rendu);
                break;

            case 10:
                if (SDL_RenderCopy(rendu, texture_p0_vert, NULL, &position_textures) != 0)
                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                SDL_RenderPresent(rendu);
                break;

            case 15:
                if (SDL_RenderCopy(rendu, texture_p0_orange, NULL, &position_textures) != 0)
                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                SDL_RenderPresent(rendu);
                break;

            default:
                break;
            }
        }
        

        switch (rendu_modifie)
        {
        case 0:
            if (SDL_RenderCopy(rendu, texture_p0_bleu, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            rendu_modifie = -1;
            SDL_RenderPresent(rendu);
            break;
        
        case 1:
            if (SDL_RenderCopy(rendu, texture_p1, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            rendu_modifie = -1;
            SDL_RenderPresent(rendu);
            break;

        case 2:
            if (SDL_RenderCopy(rendu, texture_p2, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            rendu_modifie = -1;
            SDL_RenderPresent(rendu);
            break;

        default:
            break;
        }

        /********************************************************************************/
        /********************************************************************************/
        /********************************************************************************/

        switch (num_page)   // Gérer les positions de la souris
        {
        case 0:
            if ((event.motion.x < 890) && (event.motion.x > 390) && (event.motion.y < 710) && (event.motion.y > 610))
            {
                dessiner_rectangle_5(rendu, 388, 608, 504, 104);
                rendu_modifie = 0;
                SDL_RenderPresent(rendu);
            }
            break;

        /********************************************************************************/
        case 1:
            if ((event.motion.x < 849) && (event.motion.x > 438) && (event.motion.y < 691) && (event.motion.y > 636))
            {
                dessiner_rectangle_5(rendu, 435, 636, 415, 55);
                rendu_modifie = 1;
                SDL_RenderPresent(rendu);
            }
            else
            {
                if ((event.motion.x < 1180) && (event.motion.x > 1022) && (event.motion.y < 686) && (event.motion.y > 632))
                {
                    dessiner_rectangle_5(rendu, 1022, 632, 158, 54);
                    rendu_modifie = 1;
                    SDL_RenderPresent(rendu);
                }
                else
                {
                    if ((event.motion.x < 600) && (event.motion.x > 100) && (event.motion.y < 220) && (event.motion.y > 120))
                    {
                        dessiner_rectangle_5(rendu, 100, 120, 500, 100);
                        rendu_modifie = 1;
                        SDL_RenderPresent(rendu);
                    }
                    else
                    {
                        if ((event.motion.x < 600) && (event.motion.x > 100) && (event.motion.y < 370) && (event.motion.y > 270))
                        {
                            dessiner_rectangle_5(rendu, 100, 270, 500, 100);
                            rendu_modifie = 1;
                            SDL_RenderPresent(rendu);
                        }
                        else
                        {
                            if ((event.motion.x < 600) && (event.motion.x > 100) && (event.motion.y < 520) && (event.motion.y > 420))
                            {
                                dessiner_rectangle_5(rendu, 100, 420, 500, 100);
                                rendu_modifie = 1;
                                SDL_RenderPresent(rendu);
                            }
                            else
                            {
                                if ((event.motion.x < 1180) && (event.motion.x > 680) && (event.motion.y < 220) && (event.motion.y > 120))
                                {
                                    dessiner_rectangle_5(rendu, 680, 120, 500, 100);
                                    rendu_modifie = 1;
                                    SDL_RenderPresent(rendu);
                                }
                                else
                                {
                                    if ((event.motion.x < 1180) && (event.motion.x > 680) && (event.motion.y < 370) && (event.motion.y > 270))
                                    {
                                        dessiner_rectangle_5(rendu, 680, 270, 500, 100);
                                        rendu_modifie = 1;
                                        SDL_RenderPresent(rendu);
                                    }
                                    else
                                    {
                                        if ((event.motion.x < 1180) && (event.motion.x > 680) && (event.motion.y < 520) && (event.motion.y > 420))
                                        {
                                            dessiner_rectangle_5(rendu, 680, 420, 500, 100);
                                            rendu_modifie = 1;
                                            SDL_RenderPresent(rendu);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;

        /********************************************************************************/
        case 2:
            if ((event.motion.x < 850) && (event.motion.x > 437) && (event.motion.y < 688) && (event.motion.y > 638))
            {
                dessiner_rectangle_5(rendu, 432, 632, 420, 55);
                rendu_modifie = 2;
                SDL_RenderPresent(rendu);
            }
            else
            {
                if ((event.motion.x < 262) && (event.motion.x > 102) && (event.motion.y < 688) && (event.motion.y > 630))
                {
                    dessiner_rectangle_5(rendu, 102, 630, 160, 58);
                    rendu_modifie = 2;
                    SDL_RenderPresent(rendu);
                }
                else
                {
                    if ((event.motion.x < 590) && (event.motion.x > 90) && (event.motion.y < 180) && (event.motion.y > 80))
                    {
                        dessiner_rectangle_5(rendu, 90, 80, 500, 100);
                        rendu_modifie = 2;
                        SDL_RenderPresent(rendu);
                    }
                    else
                    {
                        if ((event.motion.x < 590) && (event.motion.x > 90) && (event.motion.y < 305) && (event.motion.y > 205))
                        {
                            dessiner_rectangle_5(rendu, 90, 205, 500, 100);
                            rendu_modifie = 2;
                            SDL_RenderPresent(rendu);
                        }
                        else
                        {
                            if ((event.motion.x < 590) && (event.motion.x > 90) && (event.motion.y < 420) && (event.motion.y > 320))
                            {
                                dessiner_rectangle_5(rendu, 90, 320, 500, 100);
                                rendu_modifie = 2;
                                SDL_RenderPresent(rendu);
                            }
                            else
                            {
                                if ((event.motion.x < 590) && (event.motion.x > 90) && (event.motion.y < 550) && (event.motion.y > 450))
                                {
                                    dessiner_rectangle_5(rendu, 90, 450, 500, 100);
                                    rendu_modifie = 2;
                                    SDL_RenderPresent(rendu);
                                }
                                else
                                {
                                    if ((event.motion.x < 1190) && (event.motion.x > 690) && (event.motion.y < 180) && (event.motion.y > 80))
                                    {
                                        dessiner_rectangle_5(rendu, 690, 80, 500, 100);
                                        rendu_modifie = 2;
                                        SDL_RenderPresent(rendu);
                                    }
                                    else
                                    {
                                        if ((event.motion.x < 1190) && (event.motion.x > 690) && (event.motion.y < 305) && (event.motion.y > 205))
                                        {
                                            dessiner_rectangle_5(rendu, 690, 205, 500, 100);
                                            rendu_modifie = 2;
                                            SDL_RenderPresent(rendu);
                                        }
                                        else
                                        {
                                            if ((event.motion.x < 1190) && (event.motion.x > 690) && (event.motion.y < 420) && (event.motion.y > 320))
                                            {
                                                dessiner_rectangle_5(rendu, 690, 320, 500, 100);
                                                rendu_modifie = 2;
                                                SDL_RenderPresent(rendu);
                                            }
                                            else
                                            {
                                                if ((event.motion.x < 1190) && (event.motion.x > 690) && (event.motion.y < 550) && (event.motion.y > 450))
                                                {
                                                    dessiner_rectangle_5(rendu, 690, 450, 500, 100);
                                                    rendu_modifie = 2;
                                                    SDL_RenderPresent(rendu);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;

        default:
            break;
        }

        /********************************************************************************/
        /********************************************************************************/
        /********************************************************************************/
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN: // Clicks de souris
                switch (num_page)
                {
                case 0:
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if ((event.button.x < 890) && (event.button.x > 390) && (event.button.y < 710) && (event.button.y > 610))
                        {
                            if (SDL_RenderCopy(rendu, texture_p1, NULL, &position_textures) != 0)
                                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                            SDL_RenderPresent(rendu);
                            num_page = 1;
                            rendu_modifie = 1;
                        }
                    }
                    break;

                /********************************************************************************/
                case 1:
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if ((event.button.x < 849) && (event.button.x > 438) && (event.button.y < 691) && (event.button.y > 636))
                        {
                            program_launched = SDL_FALSE;
                        }
                        else
                        {
                            if ((event.button.x < 1180) && (event.button.x > 1022) && (event.button.y < 686) && (event.button.y > 632))
                            {
                                if (SDL_RenderCopy(rendu, texture_p2, NULL, &position_textures) != 0)
                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                SDL_RenderPresent(rendu);
                                num_page = 2;
                                rendu_modifie = 2;
                            }
                            else
                            {
                                if ((event.button.x < 600) && (event.button.x > 100) && (event.button.y < 220) && (event.button.y > 120))
                                {
                                    ajout_new_polynome_2D(rendu, &Tete_Ensemble);
                                    if (SDL_RenderCopy(rendu, texture_p1, NULL, &position_textures) != 0)
                                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                    SDL_RenderPresent(rendu);
                                }
                                else
                                {
                                    if ((event.button.x < 600) && (event.button.x > 100) && (event.button.y < 370) && (event.button.y > 270))
                                    {                                 
                                        afficher_liste_polynomes_2D(rendu, Tete_Ensemble);
                                        if (SDL_RenderCopy(rendu, texture_p1, NULL, &position_textures) != 0)
                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                        SDL_RenderPresent(rendu);
                                    }
                                    else
                                    {
                                        if ((event.button.x < 600) && (event.button.x > 100) && (event.button.y < 520) && (event.button.y > 420))
                                        {
                                            supprimer_polynome_2D(rendu, &Tete_Ensemble);
                                            if (SDL_RenderCopy(rendu, texture_p1, NULL, &position_textures) != 0)
                                                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                            SDL_RenderPresent(rendu);
                                        }
                                        else
                                        {
                                            if ((event.button.x < 1180) && (event.button.x > 680) && (event.button.y < 220) && (event.button.y > 120))
                                            {
                                                copier_polynome_2D(rendu, Tete_Ensemble);
                                                if (SDL_RenderCopy(rendu, texture_p1, NULL, &position_textures) != 0)
                                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                                SDL_RenderPresent(rendu);
                                            }
                                            else
                                            {
                                                if ((event.button.x < 1180) && (event.button.x > 680) && (event.button.y < 370) && (event.button.y > 270))
                                                {
                                                    simplifier_polynome_2D(rendu, Tete_Ensemble);
                                                    if (SDL_RenderCopy(rendu, texture_p1, NULL, &position_textures) != 0)
                                                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                                    SDL_RenderPresent(rendu);
                                                }
                                                else
                                                {
                                                    if ((event.button.x < 1180) && (event.button.x > 680) && (event.button.y < 520) && (event.button.y > 420))
                                                    {
                                                        evaluer_polynome_2D(rendu, Tete_Ensemble);
                                                        if (SDL_RenderCopy(rendu, texture_p1, NULL, &position_textures) != 0)
                                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                                        SDL_RenderPresent(rendu);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;

                /********************************************************************************/
                case 2:
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if ((event.button.x < 850) && (event.button.x > 437) && (event.button.y < 688) && (event.button.y > 638))
                        {
                            program_launched = SDL_FALSE;
                        }
                        else
                        {
                            if ((event.button.x < 262) && (event.button.x > 102) && (event.button.y < 688) && (event.button.y > 630))
                            {
                                if (SDL_RenderCopy(rendu, texture_p1, NULL, &position_textures) != 0)
                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                SDL_RenderPresent(rendu);
                                num_page = 1;
                                rendu_modifie = 1;
                            }
                            else
                            {
                                if ((event.button.x < 590) && (event.button.x > 90) && (event.button.y < 180) && (event.button.y > 80))
                                {
                                    addition_polynomes_2D(rendu, Tete_Ensemble);
                                    if (SDL_RenderCopy(rendu, texture_p2, NULL, &position_textures) != 0)
                                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                    SDL_RenderPresent(rendu);
                                }
                                else
                                {
                                    if ((event.button.x < 590) && (event.button.x > 90) && (event.button.y < 305) && (event.button.y > 205))
                                    {
                                        soustraction_polynomes_2D(rendu, Tete_Ensemble);
                                        if (SDL_RenderCopy(rendu, texture_p2, NULL, &position_textures) != 0)
                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                        SDL_RenderPresent(rendu);
                                    }
                                    else
                                    {
                                        if ((event.button.x < 590) && (event.button.x > 90) && (event.button.y < 420) && (event.button.y > 320))
                                        {
                                            multiplication_polynomes_2D(rendu, Tete_Ensemble);
                                            if (SDL_RenderCopy(rendu, texture_p2, NULL, &position_textures) != 0)
                                                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                            SDL_RenderPresent(rendu);
                                        }
                                        else
                                        {
                                            if ((event.button.x < 590) && (event.button.x > 90) && (event.button.y < 550) && (event.button.y > 450))
                                            {
                                                division_polynomes_2D(rendu, Tete_Ensemble);
                                                if (SDL_RenderCopy(rendu, texture_p2, NULL, &position_textures) != 0)
                                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                                SDL_RenderPresent(rendu);
                                            }
                                            else
                                            {
                                                if ((event.button.x < 1190) && (event.button.x > 690) && (event.button.y < 180) && (event.button.y > 80))
                                                {
                                                    deriver_polynome_2D(rendu, Tete_Ensemble);
                                                    if (SDL_RenderCopy(rendu, texture_p2, NULL, &position_textures) != 0)
                                                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                                    SDL_RenderPresent(rendu);
                                                }
                                                else
                                                {
                                                    if ((event.button.x < 1190) && (event.button.x > 690) && (event.button.y < 305) && (event.button.y > 205))
                                                    {
                                                        integrer_polynome_2D(rendu, Tete_Ensemble);
                                                        if (SDL_RenderCopy(rendu, texture_p2, NULL, &position_textures) != 0)
                                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                                        SDL_RenderPresent(rendu);
                                                    }
                                                    else
                                                    {
                                                        if ((event.button.x < 1190) && (event.button.x > 690) && (event.button.y < 420) && (event.button.y > 320))
                                                        {
                                                            dessiner_courbe_2D(rendu, Tete_Ensemble);
                                                            if (SDL_RenderCopy(rendu, texture_p2, NULL, &position_textures) != 0)
                                                                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                                            SDL_RenderPresent(rendu);
                                                        }
                                                        else
                                                        {
                                                            if ((event.button.x < 1190) && (event.button.x > 690) && (event.button.y < 550) && (event.button.y > 450))
                                                            {
                                                                application_CRC_2D(rendu);
                                                                if (SDL_RenderCopy(rendu, texture_p2, NULL, &position_textures) != 0)
                                                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                                                SDL_RenderPresent(rendu);
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;

                default:
                    break;
                }
                continue;

            /********************************************************************************/
            /********************************************************************************/
            case SDL_QUIT:
                program_launched = SDL_FALSE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }

    /********************************************************************************/
    // Fermer Tous
    TTF_CloseFont(police1);
    TTF_CloseFont(police2);
    TTF_CloseFont(police3);
    TTF_Quit();
    IMG_Quit();

    
    SDL_FreeSurface(texte_noir);
    SDL_FreeSurface(texte_bleu);
    SDL_FreeSurface(texte_vert);
    SDL_FreeSurface(texte_orange);
    SDL_FreeSurface(page0);
    SDL_FreeSurface(page0_bleu);
    SDL_FreeSurface(page0_vert);
    SDL_FreeSurface(page0_orange);
    SDL_FreeSurface(page1);
    SDL_FreeSurface(page2);

    SDL_DestroyTexture(texture_p0);
    SDL_DestroyTexture(texture_p0_bleu);
    SDL_DestroyTexture(texture_p0_vert);
    SDL_DestroyTexture(texture_p0_orange);
    SDL_DestroyTexture(texture_p1);
    SDL_DestroyTexture(texture_p2);
    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return;
}