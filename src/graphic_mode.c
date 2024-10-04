#include "graphic_mode.h"

void Passage_2D()
{
    LLC_Poly *Tete_Ensemble = NULL;

    SDL_Window *window = NULL;
    SDL_Renderer *rendu = NULL;

    Rectangle rectangles[] = {
        {0, 390, 890, 610, 710, SDL_FALSE},

        {1, 100, 600, 120, 220, SDL_FALSE},
        {1, 100, 600, 270, 370, SDL_FALSE},
        {1, 100, 600, 420, 520, SDL_FALSE},
        {1, 680, 1180, 120, 220, SDL_FALSE},
        {1, 680, 1180, 270, 370, SDL_FALSE},
        {1, 680, 1180, 420, 520, SDL_FALSE},
        {1, 435, 855, 635, 690, SDL_FALSE},
        {1, 1022, 1180, 635, 685, SDL_FALSE},

        {2, 90, 590, 80, 180, SDL_FALSE},
        {2, 90, 590, 205, 305, SDL_FALSE},
        {2, 90, 590, 320, 420, SDL_FALSE},
        {2, 90, 590, 450, 550, SDL_FALSE},
        {2, 690, 1190, 80, 180, SDL_FALSE},
        {2, 690, 1190, 205, 305, SDL_FALSE},
        {2, 690, 1190, 320, 420, SDL_FALSE},
        {2, 690, 1190, 450, 550, SDL_FALSE},
        {2, 105, 260, 630, 685, SDL_FALSE},
        {2, 435, 855, 635, 690, SDL_FALSE}};

    // Lancement SDL
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

    page0 = IMG_Load("assets/images/00.jpg");
    if (page0 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    page0_bleu = IMG_Load("assets/images/00.jpg");
    if (page0_bleu == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    page0_vert = IMG_Load("assets/images/00.jpg");
    if (page0_vert == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    page0_orange = IMG_Load("assets/images/00.jpg");
    if (page0_orange == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    page1 = IMG_Load("assets/images/01.jpg");
    if (page1 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    page2 = IMG_Load("assets/images/02.jpg");
    if (page2 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    /********************************************************************************/
    // Les polices
    SDL_Rect position_texte;
    SDL_Surface *texte_noir = NULL;
    TTF_Font *police1 = NULL, *police2 = NULL, *police3 = NULL;

    police1 = TTF_OpenFont("assets/polices/angelina.ttf", 100);
    if (police1 == NULL)
        SDL_ExitWithError("Impossible de charger la police angelina");
    TTF_SetFontStyle(police1, TTF_STYLE_UNDERLINE);

    police2 = TTF_OpenFont("assets/polices/times.ttf", 40);
    if (police2 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

    police3 = TTF_OpenFont("assets/polices/times.ttf", 40);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    texte_noir = TTF_RenderText_Blended(police1, "BIENVENUE", NOIR);
    position_texte.x = 430;
    position_texte.y = 20;
    SDL_BlitSurface(texte_noir, NULL, page0, &position_texte);

    /*****************************************************************************/
    // Les textures
    SDL_Texture *texture_p0 = NULL, *texture_p1 = NULL, *texture_p2 = NULL;
    SDL_Rect position_textures;

    position_textures.x = 0;
    position_textures.y = 0;
    texture_p0 = SDL_CreateTextureFromSurface(rendu, page0);
    if (texture_p0 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p0, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
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

    while (program_launched)
    {
        SDL_Event event;

        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        switch (rendu_modifie) // changer la page
        {
        case 0:
            if (SDL_RenderCopy(rendu, texture_p0, NULL, &position_textures) != 0)
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

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        switch (num_page) // Gérer les positions de la souris
        {
        case 0:
            stateChanged = SDL_FALSE;
            SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, rectangles[0]);
            if (currentlyInside != rectangles[0].mouseInside)
            {
                rectangles[0].mouseInside = currentlyInside;
                stateChanged = SDL_TRUE;
            }
            if (stateChanged)
            {
                if (SDL_RenderCopy(rendu, texture_p0, NULL, &position_textures) != 0)
                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");

                if (rectangles[0].mouseInside)
                {
                    dessiner_rectangle_5(rendu, rectangles[0].x1, rectangles[0].y1, rectangles[0].x2 - rectangles[0].x1, rectangles[0].y2 - rectangles[0].y1);
                }
                SDL_RenderPresent(rendu);
            }
            break;

        /********************************************************************************/
        case 1:
            stateChanged = SDL_FALSE;
            for (int i = 1; i < 9; ++i)
            {
                SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, rectangles[i]);
                if (currentlyInside != rectangles[i].mouseInside)
                {
                    rectangles[i].mouseInside = currentlyInside;
                    stateChanged = SDL_TRUE;
                }
            }
            if (stateChanged)
            {
                if (SDL_RenderCopy(rendu, texture_p1, NULL, &position_textures) != 0)
                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");

                for (int i = 1; i < 9; ++i)
                {
                    if (rectangles[i].mouseInside)
                    {
                        dessiner_rectangle_5(rendu, rectangles[i].x1, rectangles[i].y1, rectangles[i].x2 - rectangles[i].x1, rectangles[i].y2 - rectangles[i].y1);
                    }
                }
                SDL_RenderPresent(rendu);
            }
            break;

        /********************************************************************************/
        case 2:
            stateChanged = SDL_FALSE;
            for (int i = 9; i < 19; ++i)
            {
                SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, rectangles[i]);
                if (currentlyInside != rectangles[i].mouseInside)
                {
                    rectangles[i].mouseInside = currentlyInside;
                    stateChanged = SDL_TRUE;
                }
            }
            if (stateChanged)
            {
                if (SDL_RenderCopy(rendu, texture_p2, NULL, &position_textures) != 0)
                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");

                for (int i = 9; i < 19; ++i)
                {
                    if (rectangles[i].mouseInside)
                    {
                        dessiner_rectangle_5(rendu, rectangles[i].x1, rectangles[i].y1, rectangles[i].x2 - rectangles[i].x1, rectangles[i].y2 - rectangles[i].y1);
                    }
                }
                SDL_RenderPresent(rendu);
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
                        if ((event.button.x < rectangles[0].x2) && (event.button.x > rectangles[0].x1) && (event.button.y < rectangles[0].y2) && (event.button.y > rectangles[0].y1))
                        {
                            num_page = 1;
                            rendu_modifie = 1;
                        }
                    }
                    break;

                /********************************************************************************/
                case 1:
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if ((event.button.x < rectangles[1].x2) && (event.button.x > rectangles[1].x1) && (event.button.y < rectangles[1].y2) && (event.button.y > rectangles[1].y1))
                        {
                            ajout_new_polynome_2D(rendu, &Tete_Ensemble);
                            num_page = 1;
                            rendu_modifie = 1;
                            break;
                        }
                        if ((event.button.x < rectangles[2].x2) && (event.button.x > rectangles[2].x1) && (event.button.y < rectangles[2].y2) && (event.button.y > rectangles[2].y1))
                        {
                            afficher_liste_polynomes_2D(rendu, Tete_Ensemble);
                            num_page = 1;
                            rendu_modifie = 1;
                            break;
                        }
                        if ((event.button.x < rectangles[3].x2) && (event.button.x > rectangles[3].x1) && (event.button.y < rectangles[3].y2) && (event.button.y > rectangles[3].y1))
                        {
                            supprimer_polynome_2D(rendu, &Tete_Ensemble);
                            num_page = 1;
                            rendu_modifie = 1;
                            break;
                        }
                        if ((event.button.x < rectangles[4].x2) && (event.button.x > rectangles[4].x1) && (event.button.y < rectangles[4].y2) && (event.button.y > rectangles[4].y1))
                        {
                            copier_polynome_2D(rendu, Tete_Ensemble);
                            num_page = 1;
                            rendu_modifie = 1;
                            break;
                        }
                        if ((event.button.x < rectangles[5].x2) && (event.button.x > rectangles[5].x1) && (event.button.y < rectangles[5].y2) && (event.button.y > rectangles[5].y1))
                        {
                            simplifier_polynome_2D(rendu, Tete_Ensemble);
                            num_page = 1;
                            rendu_modifie = 1;
                            break;
                        }
                        if ((event.button.x < rectangles[6].x2) && (event.button.x > rectangles[6].x1) && (event.button.y < rectangles[6].y2) && (event.button.y > rectangles[6].y1))
                        {
                            evaluer_polynome_2D(rendu, Tete_Ensemble);
                            num_page = 1;
                            rendu_modifie = 1;
                            break;
                        }
                        if ((event.button.x < rectangles[7].x2) && (event.button.x > rectangles[7].x1) && (event.button.y < rectangles[7].y2) && (event.button.y > rectangles[7].y1))
                        {
                            program_launched = SDL_FALSE;
                            break;
                        }
                        if ((event.button.x < rectangles[8].x2) && (event.button.x > rectangles[8].x1) && (event.button.y < rectangles[8].y2) && (event.button.y > rectangles[8].y1))
                        {
                            num_page = 2;
                            rendu_modifie = 2;
                            break;
                        }
                    }
                    break;

                /********************************************************************************/
                case 2:
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if ((event.button.x < rectangles[9].x2) && (event.button.x > rectangles[9].x1) && (event.button.y < rectangles[9].y2) && (event.button.y > rectangles[9].y1))
                        {
                            addition_polynomes_2D(rendu, Tete_Ensemble);
                            num_page = 2;
                            rendu_modifie = 1;
                            break;
                        }
                        if ((event.button.x < rectangles[10].x2) && (event.button.x > rectangles[10].x1) && (event.button.y < rectangles[10].y2) && (event.button.y > rectangles[10].y1))
                        {
                            soustraction_polynomes_2D(rendu, Tete_Ensemble);
                            num_page = 2;
                            rendu_modifie = 1;
                            break;
                        }
                        if ((event.button.x < rectangles[11].x2) && (event.button.x > rectangles[11].x1) && (event.button.y < rectangles[11].y2) && (event.button.y > rectangles[11].y1))
                        {
                            multiplication_polynomes_2D(rendu, Tete_Ensemble);       
                            num_page = 2;
                            rendu_modifie = 1;
                            break;
                        }
                        if ((event.button.x < rectangles[12].x2) && (event.button.x > rectangles[12].x1) && (event.button.y < rectangles[12].y2) && (event.button.y > rectangles[12].y1))
                        {
                            division_polynomes_2D(rendu, Tete_Ensemble);
                            num_page = 2;
                            rendu_modifie = 1;
                            break;
                        }
                        if ((event.button.x < rectangles[13].x2) && (event.button.x > rectangles[13].x1) && (event.button.y < rectangles[13].y2) && (event.button.y > rectangles[13].y1))
                        {
                            deriver_polynome_2D(rendu, Tete_Ensemble);
                            num_page = 2;
                            rendu_modifie = 1;
                            break;
                        }
                        if ((event.button.x < rectangles[14].x2) && (event.button.x > rectangles[14].x1) && (event.button.y < rectangles[14].y2) && (event.button.y > rectangles[14].y1))
                        {
                            integrer_polynome_2D(rendu, Tete_Ensemble);
                            num_page = 2;
                            rendu_modifie = 1;
                            break;
                        }
                        if ((event.button.x < rectangles[15].x2) && (event.button.x > rectangles[15].x1) && (event.button.y < rectangles[15].y2) && (event.button.y > rectangles[15].y1))
                        {
                            dessiner_courbe_2D(rendu, Tete_Ensemble);
                            num_page = 2;
                            rendu_modifie = 1;
                            break;
                        }
                        if ((event.button.x < rectangles[16].x2) && (event.button.x > rectangles[16].x1) && (event.button.y < rectangles[16].y2) && (event.button.y > rectangles[16].y1))
                        {
                            application_CRC_2D(rendu);
                            num_page = 2;
                            rendu_modifie = 1;
                            break;
                        }
                        if ((event.button.x < rectangles[17].x2) && (event.button.x > rectangles[17].x1) && (event.button.y < rectangles[17].y2) && (event.button.y > rectangles[17].y1))
                        {
                            num_page = 1;
                            rendu_modifie = 1;
                            break;
                        }
                        if ((event.button.x < rectangles[18].x2) && (event.button.x > rectangles[18].x1) && (event.button.y < rectangles[18].y2) && (event.button.y > rectangles[18].y1))
                        {
                            program_launched = SDL_FALSE;
                            break;
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
    SDL_FreeSurface(page0);
    SDL_FreeSurface(page0_bleu);
    SDL_FreeSurface(page0_vert);
    SDL_FreeSurface(page0_orange);
    SDL_FreeSurface(page1);
    SDL_FreeSurface(page2);

    SDL_DestroyTexture(texture_p0);
    SDL_DestroyTexture(texture_p1);
    SDL_DestroyTexture(texture_p2);
    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return;
}