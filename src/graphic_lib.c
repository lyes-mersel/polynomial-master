#include "graphic_lib.h"

SDL_Color NOIR = {0, 0, 0}, BLANC = {255, 255, 255};
SDL_Color BLEU = {50, 60, 200}, VERT = {30, 153, 67};
SDL_Color ORANGE = {255, 120, 20};

/********************************************************************************/
/*                Procédure d'implémentation de la bibliothèque                 */
/********************************************************************************/
void SDL_ExitWithError(const char message[])
{
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[%s] > %s", message, SDL_GetError());
    TTF_Quit();
    SDL_Quit();
    systemPause();
    exit(EXIT_FAILURE);
}

/********************************************************************************/
void limiter_FPS(unsigned int limit)
{
    unsigned int ticks = SDL_GetTicks();

    if (limit < ticks)
        return;
    else if (limit > ticks + FPS_LIMIT)
        SDL_Delay(FPS_LIMIT);
    else
        SDL_Delay(limit - ticks);
    return;
}

/********************************************************************************/
SDL_bool isMouseInsideRect(int mouseX, int mouseY, Rectangle rect)
{
    return (mouseX > rect.x1 && mouseX < rect.x2 &&
            mouseY > rect.y1 && mouseY < rect.y2);
}

/********************************************************************************/
// Dessiner une ligne triple (pour plus d'épaisseur)
void dessiner_ligne(SDL_Renderer *rendu, float x1, float y1, float x2, float y2)
{
    for (int i1 = -1; i1 < 2; i1++)
        for (int i2 = -1; i2 < 2; i2++)
            for (int i3 = -1; i3 < 2; i3++)
                for (int i4 = -1; i4 < 2; i4++)
                    SDL_RenderDrawLine(rendu, (x1 + i1), (y1 + i2), (x2 + i3), (y2 + i4));
    return;
}

/********************************************************************************/
// Dessiner un point (3 * 3 pixels) pour plus d'épaisseur
void dessiner_point(SDL_Renderer *rendu, float x1, float y1)
{
    for (int i1 = -1; i1 < 2; i1++)
        for (int i2 = -1; i2 < 2; i2++)
            SDL_RenderDrawPoint(rendu, (x1 + i1), (y1 + i2));
    return;
}

/********************************************************************************/
// Dessiner un rectangle triple (pour plus d'épaisseur)
void dessiner_rectangle(SDL_Renderer *rendu, int x, int y, int largeur, int hauteur)
{
    SDL_Rect rect;
    for (int i = -1; i < 2; i++)
    {
        rect.x = x + i;
        rect.y = y + i;
        rect.w = largeur;
        rect.h = hauteur;
        SDL_RenderDrawRect(rendu, &rect);
    }
    return;
}

/********************************************************************************/
// Dessiner un rectangle *5 (pour plus d'épaisseur)
void dessiner_rectangle_5(SDL_Renderer *rendu, int x, int y, int largeur, int hauteur)
{
    SDL_Rect rect;
    for (int i = -2; i < 3; i++)
    {
        rect.x = x + i;
        rect.y = y + i;
        rect.w = largeur;
        rect.h = hauteur;
        SDL_RenderDrawRect(rendu, &rect);
    }
    return;
}

/********************************************************************************/
// Dessiner la courbe d'un polynome (Utilisé durant le passage par Terminal)
void dessiner_courbe_1(Monome *P_tete)
{
    SDL_Window *window = NULL;
    SDL_Renderer *rendu = NULL;
    SDL_Surface *image = NULL;
    SDL_Texture *texture = NULL;

    // Initialisation du programme   (Lancement SDL)
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        SDL_ExitWithError("Initialisation SDL");

    // Création Fentre
    window = SDL_CreateWindow("Courbe du Polynome", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              800, 700, 0);
    if (window == NULL)
        SDL_ExitWithError("Création Fenêtre échouée");

    // Création Rendu
    rendu = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (rendu == NULL)
        SDL_ExitWithError("Création Rendu échouée");

    /*****************************************************************************/
    // Surface + Texture
    image = SDL_LoadBMP("assets/images/courbe1.bmp");
    if (image == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    texture = SDL_CreateTextureFromSurface(rendu, image);
    if (texture == NULL)
        SDL_ExitWithError("Impossible de créer la texture");

    SDL_Rect rectangle;
    if (SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");

    rectangle.x = 0;
    rectangle.y = 0;

    if (SDL_RenderCopy(rendu, texture, NULL, &rectangle) != 0)
        SDL_ExitWithError("Impossible d'afficher la texture");

    /*****************************************************************************/
    // Dessin
    if (SDL_SetRenderDrawColor(rendu, 20, 200, 20, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");

    dessiner_rectangle(rendu, 160, 140, 600, 540);
    dessiner_ligne(rendu, 160, 410, 760, 410);
    dessiner_ligne(rendu, 460, 140, 460, 680);
    dessiner_ligne(rendu, 25, 630, 25, 660);
    dessiner_ligne(rendu, 25, 660, 55, 660);

    if (SDL_SetRenderDrawColor(rendu, 255, 20, 20, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");

    float x, y = 0;      // Les coordonnées de chaque point en (cm)
    int Xp, Yp, cpt = 0; // Les coordonnées convertis en pixels

    for (x = -10; x <= 10; x += 0.001)
    {
        y = eval_poly(P_tete, x);
        Xp = 30 * x + 460;
        Yp = -30 * y + 410;
        if ((Yp < 680) && (Yp > 140))
            dessiner_point(rendu, Xp, Yp);
    }
    SDL_RenderPresent(rendu);

    /*****************************************************************************/
    // Evênements :
    SDL_Event event;
    SDL_bool program_launched = SDL_TRUE;
    unsigned int temps_limite = 0;

    while (program_launched)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                program_launched = SDL_FALSE;
                break;

            default:
                break;
            }
        }

        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }

    /*****************************************************************************/
    SDL_FreeSurface(image);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return;
}

/********************************************************************************/

/********************************************************************************/
// Ce module s'occuped de la lecture des long long > 0
// Une fois la lecture terminée il renvoie 0, et renvoie (-1) aux cas ou l'utilisateur veut quitter la page
int SDL_lire_long_long(long long *nombre, SDL_Renderer *rendu, SDL_Surface **page, int X, int Y)
{
    SDL_Event Evenement;
    SDL_Texture *texture_p3 = NULL;
    SDL_Surface *texte = NULL, *page3 = NULL;
    TTF_Font *police3 = NULL;
    SDL_Rect position_texte, position_textures;
    SDL_bool lecture = SDL_TRUE, rendu_modifie = SDL_TRUE, lecture_faite = SDL_FALSE;
    unsigned int temps_limite = 0, cpt = 0;
    char chiffres[50] = "";
    Rectangle boutton_retour = {3, 100, 260, 632, 687, SDL_FALSE};

    police3 = TTF_OpenFont("assets/polices/times.ttf", 30);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    page3 = SDL_ConvertSurface(*page, (*page)->format, SDL_SWSURFACE);
    if (page3 == NULL)
        SDL_ExitWithError("Impossible de copier page dans page3");

    position_texte.x = X;
    position_texte.y = Y;
    position_textures.x = 0;
    position_textures.y = 0;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
    if (texture_p3 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
    SDL_RenderPresent(rendu);

    *nombre = 0;
    while (lecture)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        if (rendu_modifie)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            rendu_modifie = SDL_FALSE;
        }

        // Dessiner une bordure rouge si le boutton de retour est survolé
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
        if (currentlyInside != boutton_retour.mouseInside)
        {
            boutton_retour.mouseInside = currentlyInside;
            stateChanged = SDL_TRUE;
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");

            if (boutton_retour.mouseInside)
            {
                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
            }
            SDL_RenderPresent(rendu);
        }

        while (SDL_PollEvent(&Evenement))
        {
            switch (Evenement.type)
            {
            case SDL_KEYDOWN:
                switch (Evenement.key.keysym.sym)
                {
                case SDLK_0:
                    *nombre *= 10;
                    chiffres[cpt] = '0';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "0", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_KP_0:
                    *nombre *= 10;
                    chiffres[cpt] = '0';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "0", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_1:
                    *nombre *= 10;
                    *nombre += 1;
                    chiffres[cpt] = '1';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "1", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_KP_1:
                    *nombre *= 10;
                    *nombre += 1;
                    chiffres[cpt] = '1';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "1", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_BACKSPACE:
                    *nombre = (*nombre / 10);
                    if (cpt != 0)
                    {
                        cpt--;
                        if (cpt == 0)
                            chiffres[0] = ' ';
                        else
                            chiffres[cpt] = '\0';
                        *page = SDL_ConvertSurface(page3, page3->format, SDL_SWSURFACE);
                        if (*page == NULL)
                            SDL_ExitWithError("Impossible de copier page3 dans page");
                        position_texte.x = X;
                        position_texte.y = Y;
                        texte = TTF_RenderText_Blended(police3, chiffres, NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        if (cpt == 0)
                            position_texte.x -= 8;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_RETURN:
                    lecture = SDL_FALSE;
                    lecture_faite = SDL_TRUE;
                    break;

                case SDLK_KP_ENTER:
                    lecture = SDL_FALSE;
                    lecture_faite = SDL_TRUE;
                    break;

                default:
                    break;
                }
                continue;

            case SDL_MOUSEBUTTONDOWN:
                if (Evenement.button.button == SDL_BUTTON_LEFT)
                    if ((Evenement.button.x < 260) && (Evenement.button.x > 100) && (Evenement.button.y < 687) && (Evenement.motion.y > 632))
                    {
                        lecture = SDL_FALSE;
                        lecture_faite = SDL_FALSE;
                    }
                break;

            case SDL_QUIT:
                lecture = SDL_FALSE;
                lecture_faite = SDL_FALSE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }

    if (cpt == 0)
    {
        texte = TTF_RenderText_Blended(police3, "0", NOIR);
        SDL_BlitSurface(texte, NULL, *page, &position_texte);
        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
        if (texture_p3 == NULL)
            SDL_ExitWithError("Impossible de créer la texture");
        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
        SDL_RenderPresent(rendu);
    }

    TTF_CloseFont(police3);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(page3);
    SDL_DestroyTexture(texture_p3);
    if (lecture_faite)
        return (0);
    else
        return (-1);
}

/********************************************************************************/
// Ce module s'occuped de la lecture des nombres entiers
// Une fois la lecture terminée il renvoie 0, et renvoie (-1) aux cas ou l'utilisateur veut quitter la page
int SDL_lire_entier(int *nombre, SDL_Renderer *rendu, SDL_Surface **page, int X, int Y)
{
    SDL_Event Evenement;
    SDL_Texture *texture_p3 = NULL;
    SDL_Surface *texte = NULL, *page3 = NULL;
    TTF_Font *police3 = NULL;
    SDL_Rect position_texte, position_textures;
    SDL_bool lecture = SDL_TRUE, rendu_modifie = SDL_TRUE, lecture_faite = SDL_FALSE;
    unsigned int temps_limite = 0, a = 1, cpt = 0;
    char chiffres[30] = "";
    Rectangle boutton_retour = {3, 100, 260, 632, 687, SDL_FALSE};

    police3 = TTF_OpenFont("assets/polices/times.ttf", 30);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    page3 = SDL_ConvertSurface(*page, (*page)->format, SDL_SWSURFACE);
    if (page3 == NULL)
        SDL_ExitWithError("Impossible de copier page dans page3");

    position_texte.x = X;
    position_texte.y = Y;
    position_textures.x = 0;
    position_textures.y = 0;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
    if (texture_p3 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
    SDL_RenderPresent(rendu);

    *nombre = 0;
    while (lecture)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        if (rendu_modifie)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            rendu_modifie = SDL_FALSE;
        }

        // Dessiner une bordure rouge si le boutton de retour est survolé
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
        if (currentlyInside != boutton_retour.mouseInside)
        {
            boutton_retour.mouseInside = currentlyInside;
            stateChanged = SDL_TRUE;
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");

            if (boutton_retour.mouseInside)
            {
                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
            }
            SDL_RenderPresent(rendu);
        }

        while (SDL_PollEvent(&Evenement))
        {
            switch (Evenement.type)
            {
            case SDL_KEYDOWN:
                if (position_texte.x == X) // La premiere chose à lire
                {
                    if ((Evenement.key.keysym.sym == SDLK_MINUS) || (Evenement.key.keysym.sym == SDLK_KP_MINUS))
                    {
                        a *= -1;
                        chiffres[0] = '-';
                        cpt++;
                        texte = TTF_RenderText_Blended(police3, "-", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                }
                switch (Evenement.key.keysym.sym)
                {
                case SDLK_0:
                    *nombre *= 10;
                    chiffres[cpt] = '0';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "0", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_KP_0:
                    *nombre *= 10;
                    chiffres[cpt] = '0';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "0", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_1:
                    *nombre *= 10;
                    *nombre += 1;
                    chiffres[cpt] = '1';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "1", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_KP_1:
                    *nombre *= 10;
                    *nombre += 1;
                    chiffres[cpt] = '1';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "1", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_2:
                    *nombre *= 10;
                    *nombre += 2;
                    chiffres[cpt] = '2';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "2", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_KP_2:
                    *nombre *= 10;
                    *nombre += 2;
                    chiffres[cpt] = '2';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "2", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_3:
                    *nombre *= 10;
                    *nombre += 3;
                    chiffres[cpt] = '3';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "3", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_KP_3:
                    *nombre *= 10;
                    *nombre += 3;
                    chiffres[cpt] = '3';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "3", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_4:
                    *nombre *= 10;
                    *nombre += 4;
                    chiffres[cpt] = '4';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "4", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_KP_4:
                    *nombre *= 10;
                    *nombre += 4;
                    chiffres[cpt] = '4';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "4", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_5:
                    *nombre *= 10;
                    *nombre += 5;
                    chiffres[cpt] = '5';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "5", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_KP_5:
                    *nombre *= 10;
                    *nombre += 5;
                    chiffres[cpt] = '5';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "5", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_6:
                    *nombre *= 10;
                    *nombre += 6;
                    chiffres[cpt] = '6';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "6", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_KP_6:
                    *nombre *= 10;
                    *nombre += 6;
                    chiffres[cpt] = '6';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "6", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_7:
                    *nombre *= 10;
                    *nombre += 7;
                    chiffres[cpt] = '7';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "7", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_KP_7:
                    *nombre *= 10;
                    *nombre += 7;
                    chiffres[cpt] = '7';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "7", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_8:
                    *nombre *= 10;
                    *nombre += 8;
                    chiffres[cpt] = '8';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "8", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_KP_8:
                    *nombre *= 10;
                    *nombre += 8;
                    chiffres[cpt] = '8';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "8", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_9:
                    *nombre *= 10;
                    *nombre += 9;
                    chiffres[cpt] = '9';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "9", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_KP_9:
                    *nombre *= 10;
                    *nombre += 9;
                    chiffres[cpt] = '9';
                    cpt++;
                    texte = TTF_RenderText_Blended(police3, "9", NOIR);
                    SDL_BlitSurface(texte, NULL, *page, &position_texte);
                    position_texte.x += texte->w;
                    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                    if (texture_p3 == NULL)
                        SDL_ExitWithError("Impossible de créer la texture");
                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                    SDL_RenderPresent(rendu);
                    break;

                case SDLK_BACKSPACE:
                    *nombre = (*nombre / 10);
                    if (cpt != 0)
                    {
                        if ((cpt == 1) && (a == -1))
                            a = 1;
                        cpt--;
                        if (cpt == 0)
                            chiffres[0] = ' ';
                        else
                            chiffres[cpt] = '\0';
                        *page = SDL_ConvertSurface(page3, page3->format, SDL_SWSURFACE);
                        if (*page == NULL)
                            SDL_ExitWithError("Impossible de copier page3 dans page");
                        position_texte.x = X;
                        position_texte.y = Y;
                        texte = TTF_RenderText_Blended(police3, chiffres, NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        if (cpt == 0)
                            position_texte.x -= 8;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_RETURN:
                    *nombre *= a;
                    lecture = SDL_FALSE;
                    lecture_faite = SDL_TRUE;
                    break;

                case SDLK_KP_ENTER:
                    *nombre *= a;
                    lecture = SDL_FALSE;
                    lecture_faite = SDL_TRUE;
                    break;

                default:
                    break;
                }
                continue;

            case SDL_MOUSEBUTTONDOWN:
                if (Evenement.button.button == SDL_BUTTON_LEFT)
                    if ((Evenement.button.x < 260) && (Evenement.button.x > 100) && (Evenement.button.y < 687) && (Evenement.motion.y > 632))
                    {
                        lecture = SDL_FALSE;
                        lecture_faite = SDL_FALSE;
                    }
                break;

            case SDL_QUIT:
                lecture = SDL_FALSE;
                lecture_faite = SDL_FALSE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }

    if ((cpt == 0) || ((cpt == 1) && (a == -1)))
    {
        texte = TTF_RenderText_Blended(police3, "0", NOIR);
        SDL_BlitSurface(texte, NULL, *page, &position_texte);
        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
        if (texture_p3 == NULL)
            SDL_ExitWithError("Impossible de créer la texture");
        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
        SDL_RenderPresent(rendu);
    }

    TTF_CloseFont(police3);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(page3);
    SDL_DestroyTexture(texture_p3);
    if (lecture_faite)
        return (0);
    else
        return (-1);
}

/********************************************************************************/
// Ce module s'occuped de la lecture de nombres reels
// Une fois la lecture terminée il renvoie 0, et renvoie (-1) aux cas ou l'utilisateur veut quitter la page
int SDL_lire_reel(float *nombre, SDL_Renderer *rendu, SDL_Surface **page, int X, int Y)
{
    SDL_Event Evenement;
    SDL_Texture *texture_p3 = NULL;
    SDL_Surface *texte = NULL, *page3 = NULL;
    TTF_Font *police3 = NULL;
    SDL_Rect position_texte, position_textures;
    SDL_bool lecture = SDL_TRUE, rendu_modifie = SDL_TRUE, lecture_faite = SDL_FALSE;
    unsigned int temps_limite = 0;
    int a = 1, cpt1 = 0, cpt2 = 0, entier = 0;
    char chiffres[50] = "";
    Rectangle boutton_retour = {3, 100, 260, 632, 687, SDL_FALSE};

    police3 = TTF_OpenFont("assets/polices/times.ttf", 30);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    page3 = SDL_ConvertSurface(*page, (*page)->format, SDL_SWSURFACE);
    if (page3 == NULL)
        SDL_ExitWithError("Impossible de copier page dans page3");

    position_texte.x = X;
    position_texte.y = Y;
    position_textures.x = 0;
    position_textures.y = 0;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
    if (texture_p3 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
    SDL_RenderPresent(rendu);

    *nombre = 0;
    while (lecture)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        if (rendu_modifie)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            rendu_modifie = SDL_FALSE;
        }

        // Dessiner une bordure rouge si le boutton de retour est survolé
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
        if (currentlyInside != boutton_retour.mouseInside)
        {
            boutton_retour.mouseInside = currentlyInside;
            stateChanged = SDL_TRUE;
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");

            if (boutton_retour.mouseInside)
            {
                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
            }
            SDL_RenderPresent(rendu);
        }

        while (SDL_PollEvent(&Evenement))
        {
            switch (Evenement.type)
            {
            case SDL_KEYDOWN:
                if (position_texte.x == X) // La premiere chose à lire
                {
                    if ((Evenement.key.keysym.sym == SDLK_MINUS) || (Evenement.key.keysym.sym == SDLK_KP_MINUS))
                    {
                        a *= -1;
                        chiffres[0] = '-';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "-", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                }
                switch (Evenement.key.keysym.sym)
                {
                case SDLK_0:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        chiffres[cpt1 + cpt2] = '0';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "0", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        chiffres[cpt1 + cpt2] = '0';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "0", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_KP_0:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        chiffres[cpt1 + cpt2] = '0';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "0", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        chiffres[cpt1 + cpt2] = '0';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "0", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_1:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 1;
                        chiffres[cpt1 + cpt2] = '1';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "1", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 1 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '1';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "1", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_KP_1:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 1;
                        chiffres[cpt1 + cpt2] = '1';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "1", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 1 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '1';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "1", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_2:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 2;
                        chiffres[cpt1 + cpt2] = '2';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "2", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 2 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '2';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "2", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_KP_2:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 2;
                        chiffres[cpt1 + cpt2] = '2';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "2", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 2 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '2';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "2", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_3:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 3;
                        chiffres[cpt1 + cpt2] = '3';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "3", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 3 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '3';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "3", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_KP_3:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 3;
                        chiffres[cpt1 + cpt2] = '3';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "3", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 3 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '3';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "3", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_4:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 4;
                        chiffres[cpt1 + cpt2] = '4';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "4", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 4 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '1';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "4", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_KP_4:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 4;
                        chiffres[cpt1 + cpt2] = '4';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "4", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 4 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '4';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "4", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_5:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 5;
                        chiffres[cpt1 + cpt2] = '5';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "5", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 5 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '5';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "5", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_KP_5:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 5;
                        chiffres[cpt1 + cpt2] = '5';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "5", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 5 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '5';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "5", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_6:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 6;
                        chiffres[cpt1 + cpt2] = '6';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "6", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 6 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '6';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "6", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_KP_6:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 6;
                        chiffres[cpt1 + cpt2] = '6';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "6", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 6 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '6';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "6", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_7:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 7;
                        chiffres[cpt1 + cpt2] = '7';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "7", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 7 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '7';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "7", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_KP_7:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 7;
                        chiffres[cpt1 + cpt2] = '7';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "7", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 7 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '7';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "7", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_8:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 8;
                        chiffres[cpt1 + cpt2] = '8';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "8", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 8 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '8';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "8", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_KP_8:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 8;
                        chiffres[cpt1 + cpt2] = '8';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "8", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 8 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '8';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "8", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_9:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 9;
                        chiffres[cpt1 + cpt2] = '9';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "9", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 9 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '9';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "9", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_KP_9:
                    if (cpt2 == 0) // La partie entière
                    {
                        *nombre *= 10;
                        *nombre += 9;
                        chiffres[cpt1 + cpt2] = '9';
                        cpt1++;
                        texte = TTF_RenderText_Blended(police3, "9", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    else // La partie decimale
                    {
                        *nombre += 9 * powf(10, -(float)cpt2);
                        chiffres[cpt1 + cpt2] = '9';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, "9", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_SEMICOLON:
                    if (cpt2 == 0)
                    {
                        chiffres[cpt1 + cpt2] = '.';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, ".", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_KP_PERIOD:
                    if (cpt2 == 0)
                    {
                        chiffres[cpt1 + cpt2] = '.';
                        cpt2++;
                        texte = TTF_RenderText_Blended(police3, ".", NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_BACKSPACE:
                    if (cpt2 == 0)
                    {
                        if (cpt1 != 0)
                        {
                            entier = ((int)*nombre / 10);
                            *nombre = (float)entier;
                            if ((cpt1 == 1) && (a == -1))
                                a = 1;
                            cpt1--;
                            if (cpt1 == 0)
                                chiffres[0] = ' ';
                            else
                                chiffres[cpt1] = '\0';
                            *page = SDL_ConvertSurface(page3, page3->format, SDL_SWSURFACE);
                            if (*page == NULL)
                                SDL_ExitWithError("Impossible de copier page3 dans page");
                            position_texte.x = X;
                            position_texte.y = Y;
                            texte = TTF_RenderText_Blended(police3, chiffres, NOIR);
                            SDL_BlitSurface(texte, NULL, *page, &position_texte);
                            position_texte.x += texte->w;
                            if (cpt1 == 0)
                                position_texte.x -= 8;
                            texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                            if (texture_p3 == NULL)
                                SDL_ExitWithError("Impossible de créer la texture");
                            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                            SDL_RenderPresent(rendu);
                        }
                    }
                    else // cpt2 > 0
                    {
                        cpt2--;
                        *nombre *= powf(10, (float)cpt2);
                        entier = (int)*nombre;
                        entier -= (entier % 10);
                        *nombre = ((float)entier * powf(10, -(float)cpt2));
                        if (cpt1 + cpt2 == 0)
                            chiffres[0] = ' ';
                        else
                            chiffres[cpt1 + cpt2] = '\0';

                        *page = SDL_ConvertSurface(page3, page3->format, SDL_SWSURFACE);
                        if (*page == NULL)
                            SDL_ExitWithError("Impossible de copier page3 dans page");
                        position_texte.x = X;
                        position_texte.y = Y;
                        texte = TTF_RenderText_Blended(police3, chiffres, NOIR);
                        SDL_BlitSurface(texte, NULL, *page, &position_texte);
                        position_texte.x += texte->w;
                        if (cpt1 + cpt2 == 0)
                            position_texte.x -= 8;
                        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
                        if (texture_p3 == NULL)
                            SDL_ExitWithError("Impossible de créer la texture");
                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                        SDL_RenderPresent(rendu);
                    }
                    break;

                case SDLK_RETURN:
                    *nombre *= a;
                    lecture = SDL_FALSE;
                    lecture_faite = SDL_TRUE;
                    break;

                case SDLK_KP_ENTER:
                    *nombre *= a;
                    lecture = SDL_FALSE;
                    lecture_faite = SDL_TRUE;
                    break;

                default:
                    break;
                }
                continue;

            case SDL_MOUSEBUTTONDOWN:
                if (Evenement.button.button == SDL_BUTTON_LEFT)
                    if ((Evenement.button.x < 260) && (Evenement.button.x > 100) && (Evenement.button.y < 687) && (Evenement.motion.y > 632))
                    {
                        lecture = SDL_FALSE;
                        lecture_faite = SDL_FALSE;
                    }
                break;

            case SDL_QUIT:
                lecture = SDL_FALSE;
                lecture_faite = SDL_FALSE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }

    if (((cpt1 == 0) || ((cpt1 == 1) && (a == -1))) && (cpt2 == 0))
    {
        texte = TTF_RenderText_Blended(police3, "0", NOIR);
        SDL_BlitSurface(texte, NULL, *page, &position_texte);
        texture_p3 = SDL_CreateTextureFromSurface(rendu, *page);
        if (texture_p3 == NULL)
            SDL_ExitWithError("Impossible de créer la texture");
        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
        SDL_RenderPresent(rendu);
    }

    TTF_CloseFont(police3);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(page3);
    SDL_DestroyTexture(texture_p3);
    if (lecture_faite)
        return (0);
    else
        return (-1);
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
void ajout_new_polynome_2D(SDL_Renderer *rendu, LLC_Poly **Tete_Ensemble)
{
    Monome *Tete_Poly1 = NULL, *M = NULL, *N = NULL;
    unsigned int temps_limite = 0;
    int nb_coef = 0, puis = 0;
    float cof = 0;
    char phrase[100] = "";
    SDL_Texture *texture_p3 = NULL;
    SDL_Surface *page3 = NULL;
    SDL_Surface *texte = NULL;
    TTF_Font *police2 = NULL, *police3 = NULL;
    SDL_Rect position_textures, position_texte;
    SDL_Event event;
    SDL_bool sortir_tache = SDL_FALSE;
    Rectangle boutton_retour = {3, 100, 260, 632, 687, SDL_FALSE};

    page3 = IMG_Load("assets/images/03.jpg");
    if (page3 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    police2 = TTF_OpenFont("assets/polices/times.ttf", 40);
    if (police2 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

    police3 = TTF_OpenFont("assets/polices/times.ttf", 30);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    //---------------------------------------------------------------------------------
    position_texte.x = 80;
    position_texte.y = 80;
    texte = TTF_RenderText_Blended(police2, "Ajouter un nouveau polynome :", BLANC);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);
    position_texte.x = 300;
    position_texte.y = 200;
    texte = TTF_RenderText_Blended(police3, "Veuillez entrer le nombre de coefficients du polynome ---> ", NOIR);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);
    position_texte.x += texte->w;
    if (SDL_lire_entier(&nb_coef, rendu, &page3, position_texte.x, position_texte.y) == -1)
        goto FIN_LECTURE;

    if (nb_coef <= 0)
    {
        Tete_Poly1 = NULL;
        position_texte.x = 500;
        position_texte.y += 100;
        texte = TTF_RenderText_Blended(police3, "Votre polynome est vide (nul).", NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
    }
    else
    {
        position_texte.x = 350;
        position_texte.y += 80;
        texte = TTF_RenderText_Blended(police3, " Lecture du 1er monome :", NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
        position_texte.x = 400;
        position_texte.y += 50;
        texte = TTF_RenderText_Blended(police3, " Son coefficient  --->  ", NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
        position_texte.x += texte->w;
        if (SDL_lire_reel(&cof, rendu, &page3, position_texte.x, position_texte.y) == -1)
            goto FIN_LECTURE;
        position_texte.x = 400;
        position_texte.y += 50;
        texte = TTF_RenderText_Blended(police3, " Sa  puissance    --->  ", NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
        position_texte.x += texte->w;
        if (SDL_lire_entier(&puis, rendu, &page3, position_texte.x, position_texte.y) == -1)
            goto FIN_LECTURE;

        Tete_Poly1 = allouer_mo();
        aff_coef(Tete_Poly1, cof);
        aff_puiss(Tete_Poly1, puis);
        aff_adr(Tete_Poly1, NULL);
        N = Tete_Poly1;

        for (int i = 2; i <= nb_coef; i++)
        {
            page3 = IMG_Load("assets/images/03.jpg");
            if (page3 == NULL)
                SDL_ExitWithError("Impossible de charger l'image");

            position_texte.x = 80;
            position_texte.y = 80;
            texte = TTF_RenderText_Blended(police2, "Ajouter un nouveau polynome :", BLANC);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);

            position_texte.x = 300;
            position_texte.y = 200;
            sprintf(phrase, "Veuillez entrer le nombre de coefficients du polynome ---> %d", nb_coef);
            texte = TTF_RenderText_Blended(police3, phrase, NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);

            position_texte.x = 350;
            position_texte.y += 80;
            sprintf(phrase, "Lecture du %deme monome :", i);
            texte = TTF_RenderText_Blended(police3, phrase, NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);

            position_texte.x = 400;
            position_texte.y += 50;
            texte = TTF_RenderText_Blended(police3, "  Son coefficient  --->  ", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            if (SDL_lire_reel(&cof, rendu, &page3, position_texte.x, position_texte.y) == -1)
                goto FIN_LECTURE;

            position_texte.x = 400;
            position_texte.y += 50;
            texte = TTF_RenderText_Blended(police3, "  Sa  puissance    --->  ", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            if (SDL_lire_entier(&puis, rendu, &page3, position_texte.x, position_texte.y) == -1)
                goto FIN_LECTURE;

            M = allouer_mo();
            aff_coef(M, cof);
            aff_puiss(M, puis);
            aff_adr(M, NULL);
            aff_adr(N, M);
            N = M;
        }
    }

    ajout_poly_ensemble(Tete_Ensemble, Tete_Poly1);
    position_texte.x = 930;
    position_texte.y = 670;
    texte = TTF_RenderText_Blended(police2, "Operation reussit", BLEU);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);

    position_textures.x = 0;
    position_textures.y = 0;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
    if (texture_p3 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
    SDL_RenderPresent(rendu);

    while (!sortir_tache)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        // Dessiner une bordure rouge si le boutton de retour est survolé
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
        if (currentlyInside != boutton_retour.mouseInside)
        {
            boutton_retour.mouseInside = currentlyInside;
            stateChanged = SDL_TRUE;
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");

            if (boutton_retour.mouseInside)
            {
                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
            }
            SDL_RenderPresent(rendu);
        }

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                        sortir_tache = SDL_TRUE;
                break;

            case SDL_QUIT:
                sortir_tache = SDL_TRUE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }
FIN_LECTURE:
    TTF_CloseFont(police2);
    TTF_CloseFont(police3);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(page3);
    SDL_DestroyTexture(texture_p3);
    return;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
void afficher_liste_polynomes_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble)
{
    LLC_Poly *Poly = Tete_Ensemble;
    Monome *Tete_Poly1 = NULL;
    char chaine_char[30] = "";
    SDL_Texture *texture_p3 = NULL;
    SDL_Surface *page3 = NULL, *texte = NULL;
    TTF_Font *police2 = NULL, *police3 = NULL, *police4 = NULL;
    SDL_Rect position_textures, position_texte;
    SDL_Event event;
    SDL_bool sortir_tache = SDL_FALSE;
    unsigned int temps_limite = 0, cpt = 0;
    Rectangle boutton_retour = {3, 100, 260, 632, 687, SDL_FALSE};

    page3 = IMG_Load("assets/images/03.jpg");
    if (page3 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    police2 = TTF_OpenFont("assets/polices/times.ttf", 40);
    if (police2 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

    police3 = TTF_OpenFont("assets/polices/times.ttf", 25);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    police4 = TTF_OpenFont("assets/polices/times.ttf", 15);
    if (police4 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    position_texte.x = 80;
    position_texte.y = 80;
    texte = TTF_RenderText_Blended(police2, "Affichage de la liste des polynomes :", BLANC);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);

    //"-----------------------------------------------------------------------------------------------------"
    position_texte.y = 100;
    if (Poly == NULL)
    {
        position_texte.x = 600;
        position_texte.y = 300;
        texte = TTF_RenderText_Blended(police3, "Cet ensemble est vide", NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
    }
    while (Poly != NULL)
    {
        Tete_Poly1 = Poly->adr1;
        cpt++;
        position_texte.x = 280;
        position_texte.y += 50;
        sprintf(chaine_char, "P%d = ", cpt);
        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
        position_texte.x += texte->w;
        if (Tete_Poly1 == NULL)
        {
            texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        else
        {
            while (suivant(Tete_Poly1) != NULL)
            {
                sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                if (position_texte.x + texte->w >= 1200)
                {
                    position_texte.x = 330;
                    position_texte.y += 30;
                }
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                Tete_Poly1 = suivant(Tete_Poly1);
            }
            sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
            texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
            if (position_texte.x + texte->w >= 1230)
            {
                position_texte.x = 330;
                position_texte.y += 30;
            }
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            sprintf(chaine_char, "%d", Tete_Poly1->puiss);
            texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            texte = TTF_RenderText_Blended(police3, ")", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        Poly = Poly->adr2;
    }

    //"-----------------------------------------------------------------------------------------------------"
    position_textures.x = 0;
    position_textures.y = 0;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
    if (texture_p3 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
    SDL_RenderPresent(rendu);

    position_texte.x = 930;
    position_texte.y = 670;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");

    while (!sortir_tache)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        // Dessiner une bordure rouge si le boutton de retour est survolé
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
        if (currentlyInside != boutton_retour.mouseInside)
        {
            boutton_retour.mouseInside = currentlyInside;
            stateChanged = SDL_TRUE;
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");

            if (boutton_retour.mouseInside)
            {
                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
            }
            SDL_RenderPresent(rendu);
        }

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                        sortir_tache = SDL_TRUE;
                break;

            case SDL_QUIT:
                sortir_tache = SDL_TRUE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }

    TTF_CloseFont(police2);
    TTF_CloseFont(police3);
    TTF_CloseFont(police4);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(page3);
    SDL_DestroyTexture(texture_p3);
    return;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
void supprimer_polynome_2D(SDL_Renderer *rendu, LLC_Poly **Tete_Ensemble)
{
    LLC_Poly *Poly = *Tete_Ensemble;
    Monome *Tete_Poly1 = NULL;
    char chaine_char[30] = "";
    SDL_Texture *texture_p3 = NULL;
    SDL_Surface *page3 = NULL, *texte = NULL;
    TTF_Font *police2 = NULL, *police3 = NULL, *police4 = NULL;
    SDL_Rect position_textures, position_texte, cases;
    SDL_Event event;
    SDL_bool sortir_tache = SDL_FALSE;
    unsigned int temps_limite = 0, cpt = 0;
    Rectangle boutton_retour = {3, 100, 260, 632, 687, SDL_FALSE};
    Rectangle *bouttons = (Rectangle *)malloc(20 * sizeof(Rectangle));

    page3 = IMG_Load("assets/images/03.jpg");
    if (page3 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    police2 = TTF_OpenFont("assets/polices/times.ttf", 40);
    if (police2 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

    police3 = TTF_OpenFont("assets/polices/times.ttf", 25);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    police4 = TTF_OpenFont("assets/polices/times.ttf", 15);
    if (police4 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    position_texte.x = 80;
    position_texte.y = 80;
    texte = TTF_RenderText_Blended(police2, "Veuillez choisir le polynome a supprimer :", BLANC);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);

    //"-----------------------------------------------------------------------------------------------------"
    position_texte.y = 100;
    if (Poly == NULL)
    {
        position_texte.x = 600;
        position_texte.y = 300;
        texte = TTF_RenderText_Blended(police3, "Cet ensemble est vide", NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
    }
    while (Poly != NULL)
    {
        Tete_Poly1 = Poly->adr1;
        position_texte.x = 280;
        position_texte.y += 50;
        bouttons[cpt].page = 3;
        bouttons[cpt].x1 = 230;
        bouttons[cpt].x2 = 260;
        bouttons[cpt].y1 = 150 + 50 * cpt;
        bouttons[cpt].y2 = 180 + 50 * cpt;
        bouttons[cpt].mouseInside = SDL_FALSE;
        cpt++;
        sprintf(chaine_char, "P%d = ", cpt);
        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
        position_texte.x += texte->w;
        if (Tete_Poly1 == NULL)
        {
            texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        else
        {
            while (suivant(Tete_Poly1) != NULL)
            {
                sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                if (position_texte.x + texte->w >= 1200)
                {
                    position_texte.x = 330;
                    position_texte.y += 30;
                }
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                Tete_Poly1 = suivant(Tete_Poly1);
            }
            sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
            texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
            if (position_texte.x + texte->w >= 1230)
            {
                position_texte.x = 330;
                position_texte.y += 30;
            }
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            sprintf(chaine_char, "%d", Tete_Poly1->puiss);
            texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            texte = TTF_RenderText_Blended(police3, ")", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        Poly = Poly->adr2;
    }

    //"-----------------------------------------------------------------------------------------------------"
    position_textures.x = 0;
    position_textures.y = 0;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
    if (texture_p3 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
    SDL_RenderPresent(rendu);

    position_texte.x = 930;
    position_texte.y = 670;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");

    if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    cases.h = 30;
    cases.w = 30;
    for (int i = 0; i < cpt; i++)
    {
        cases.x = bouttons[i].x1;
        cases.y = bouttons[i].y1;
        if (SDL_RenderFillRect(rendu, &cases) != 0)
            SDL_ExitWithError("Impossible de dessiner les cases à cocher");
    }
    if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    SDL_RenderPresent(rendu);

    while (sortir_tache == SDL_FALSE)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        // Dessiner une bordure rouge si le boutton de retour est survolé
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
        if (currentlyInside != boutton_retour.mouseInside)
        {
            boutton_retour.mouseInside = currentlyInside;
            stateChanged = SDL_TRUE;
        }
        if (stateChanged)
        {
            // Copier la texture du départ (page 3)
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            // Dessiner les cases à cocher
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            if (boutton_retour.mouseInside)
            {
                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
            }
            SDL_RenderPresent(rendu);
        }

        // Dessiner une bordure et une croix si les caches à cocher sont survolées
        stateChanged = SDL_FALSE;
        SDL_GetMouseState(&mouseX, &mouseY);
        for (int i = 0; i < cpt; ++i)
        {
            currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons[i]);
            if (currentlyInside != bouttons[i].mouseInside)
            {
                bouttons[i].mouseInside = currentlyInside;
                stateChanged = SDL_TRUE;
            }
        }
        if (stateChanged)
        {
            // Copier la texture du départ (page 3)
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            // Dessiner les cases à cocher
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            for (int i = 0; i < cpt; ++i)
            {
                if (bouttons[i].mouseInside)
                {
                    dessiner_rectangle(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2 - bouttons[i].x1, bouttons[i].y2 - bouttons[i].y1);
                    SDL_RenderDrawLine(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2, bouttons[i].y2);
                    SDL_RenderDrawLine(rendu, bouttons[i].x2, bouttons[i].y1, bouttons[i].x1, bouttons[i].y2);
                }
            }
            SDL_RenderPresent(rendu);
        }

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                        sortir_tache = SDL_TRUE;
                    else
                    {
                        for (int i = 0; i < cpt; i++)
                        {
                            if ((event.button.x < bouttons[i].x2) && (event.button.x > bouttons[i].x1) && (event.button.y < bouttons[i].y2) && (event.button.y > bouttons[i].y1))
                            {
                                if (supp_poly_ensemble(Tete_Ensemble, adr_maillon_ens(*Tete_Ensemble, i + 1)) == 0)
                                {
                                    if (SDL_RenderDrawLine(rendu, 300, bouttons[i].y1 + 15, 1200, bouttons[i].y1 + 15) != 0)
                                        SDL_ExitWithError("Impossible de dessiner la ligne rouge");
                                    SDL_RenderPresent(rendu);
                                    SDL_Delay(500);
                                    supprimer_polynome_2D(rendu, Tete_Ensemble);
                                    sortir_tache = SDL_TRUE;
                                }
                                break;
                            }
                        }
                    }
                }
                continue;

            case SDL_QUIT:
                sortir_tache = SDL_TRUE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }

    free(bouttons);
    TTF_CloseFont(police2);
    TTF_CloseFont(police3);
    TTF_CloseFont(police4);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(page3);
    SDL_DestroyTexture(texture_p3);
    return;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
void copier_polynome_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble)
{
    LLC_Poly *Poly = Tete_Ensemble;
    Monome *Tete_Poly1 = NULL, *Tete_Poly2 = NULL;
    char chaine_char[30] = "";
    unsigned int temps_limite = 0, cpt = 0;
    SDL_Texture *texture_p3 = NULL;
    SDL_Surface *page3 = NULL, *texte = NULL;
    TTF_Font *police2 = NULL, *police3 = NULL, *police4 = NULL;
    SDL_Rect position_textures, position_texte, cases;
    SDL_Event event;
    SDL_bool sortir_tache = SDL_FALSE;
    Rectangle boutton_retour = {3, 100, 260, 632, 687, SDL_FALSE};
    Rectangle *bouttons = (Rectangle *)malloc(20 * sizeof(Rectangle));

    page3 = IMG_Load("assets/images/03.jpg");
    if (page3 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    police2 = TTF_OpenFont("assets/polices/times.ttf", 40);
    if (police2 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

    police3 = TTF_OpenFont("assets/polices/times.ttf", 25);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    police4 = TTF_OpenFont("assets/polices/times.ttf", 15);
    if (police4 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    position_texte.x = 80;
    position_texte.y = 80;
    texte = TTF_RenderText_Blended(police2, "Veuillez choisir le polynome a copier :", BLANC);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);

    //-----------------------------------------------------------------------------------------------------
    position_texte.y = 100;
    if (Poly == NULL)
    {
        position_texte.x = 600;
        position_texte.y = 300;
        texte = TTF_RenderText_Blended(police3, "Cet ensemble est vide", NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
    }
    while (Poly != NULL)
    {
        Tete_Poly1 = Poly->adr1;
        position_texte.x = 280;
        position_texte.y += 50;
        bouttons[cpt].page = 3;
        bouttons[cpt].x1 = 230;
        bouttons[cpt].x2 = 260;
        bouttons[cpt].y1 = 150 + 50 * cpt;
        bouttons[cpt].y2 = 180 + 50 * cpt;
        bouttons[cpt].mouseInside = SDL_FALSE;
        cpt++;
        sprintf(chaine_char, "P%d = ", cpt);
        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
        position_texte.x += texte->w;
        if (Tete_Poly1 == NULL)
        {
            texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        else
        {
            while (suivant(Tete_Poly1) != NULL)
            {
                sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                if (position_texte.x + texte->w >= 1200)
                {
                    position_texte.x = 330;
                    position_texte.y += 30;
                }
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                Tete_Poly1 = suivant(Tete_Poly1);
            }
            sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
            texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
            if (position_texte.x + texte->w >= 1230)
            {
                position_texte.x = 330;
                position_texte.y += 30;
            }
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            sprintf(chaine_char, "%d", Tete_Poly1->puiss);
            texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            texte = TTF_RenderText_Blended(police3, ")", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        Poly = Poly->adr2;
    }

    //-----------------------------------------------------------------------------------------------------
    position_textures.x = 0;
    position_textures.y = 0;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
    if (texture_p3 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
    SDL_RenderPresent(rendu);

    if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    cases.h = 30;
    cases.w = 30;
    for (int i = 0; i < cpt; i++)
    {
        cases.x = bouttons[i].x1;
        cases.y = bouttons[i].y1;
        if (SDL_RenderFillRect(rendu, &cases) != 0)
            SDL_ExitWithError("Impossible de dessiner les cases à cocher");
    }
    if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    SDL_RenderPresent(rendu);

    while (!sortir_tache)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        // Dessiner une bordure rouge si le boutton de retour est survolé
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
        if (currentlyInside != boutton_retour.mouseInside)
        {
            boutton_retour.mouseInside = currentlyInside;
            stateChanged = SDL_TRUE;
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            if (boutton_retour.mouseInside)
            {
                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
            }
            SDL_RenderPresent(rendu);
        }

        // Dessiner une bordure et une croix si les caches à cocher sont survolées
        stateChanged = SDL_FALSE;
        SDL_GetMouseState(&mouseX, &mouseY);
        for (int i = 0; i < cpt; ++i)
        {
            currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons[i]);
            if (currentlyInside != bouttons[i].mouseInside)
            {
                bouttons[i].mouseInside = currentlyInside;
                stateChanged = SDL_TRUE;
            }
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            for (int i = 0; i < cpt; ++i)
            {
                if (bouttons[i].mouseInside)
                {
                    dessiner_rectangle(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2 - bouttons[i].x1, bouttons[i].y2 - bouttons[i].y1);
                    SDL_RenderDrawLine(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2, bouttons[i].y2);
                    SDL_RenderDrawLine(rendu, bouttons[i].x2, bouttons[i].y1, bouttons[i].x1, bouttons[i].y2);
                }
            }
            SDL_RenderPresent(rendu);
        }

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                        sortir_tache = SDL_TRUE;
                    else
                    {
                        for (int i = 0; i < cpt; i++)
                        {
                            if ((event.button.x < bouttons[i].x2) && (event.button.x > bouttons[i].x1) && (event.button.y < bouttons[i].y2) && (event.button.y > bouttons[i].y1))
                            {
                                //-----------------------------------------------Copie du polynome------------------------------------------------------
                                Tete_Poly1 = copie_poly(adr_maillon_ens(Tete_Ensemble, i + 1)->adr1);
                                Tete_Poly2 = Tete_Poly1;
                                page3 = IMG_Load("assets/images/03.jpg");
                                if (page3 == NULL)
                                    SDL_ExitWithError("Impossible de charger l'image");
                                position_texte.x = 80;
                                position_texte.y = 80;
                                texte = TTF_RenderText_Blended(police2, "Voici le resultat :", BLANC);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_texte.x = 280;
                                position_texte.y += 50;
                                texte = TTF_RenderText_Blended(police3, "COPIE = ", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                position_texte.x += texte->w;
                                if (Tete_Poly1 == NULL)
                                {
                                    texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                }
                                else
                                {
                                    while (suivant(Tete_Poly1) != NULL)
                                    {
                                        sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                        if (position_texte.x + texte->w >= 1200)
                                        {
                                            position_texte.x = 330;
                                            position_texte.y += 30;
                                        }
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                        texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        Tete_Poly1 = suivant(Tete_Poly1);
                                    }
                                    sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                    texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                    if (position_texte.x + texte->w >= 1230)
                                    {
                                        position_texte.x = 330;
                                        position_texte.y += 30;
                                    }
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    position_texte.x += texte->w;
                                    sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                    texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    position_texte.x += texte->w;
                                    texte = TTF_RenderText_Blended(police3, ")", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                }
                                position_texte.x = 500;
                                position_texte.y += 200;
                                texte = TTF_RenderText_Blended(police3, " Avant de quitter la page :", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_texte.y += 50;
                                texte = TTF_RenderText_Blended(police3, " Voulez vous ajouter cette copie a l'ensemble ?", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_texte.x = 650;
                                position_texte.y += 50;
                                texte = TTF_RenderText_Blended(police3, "OUI", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_texte.x = 850;
                                texte = TTF_RenderText_Blended(police3, "NON", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_textures.x = 0;
                                position_textures.y = 0;
                                texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
                                if (texture_p3 == NULL)
                                    SDL_ExitWithError("Impossible de créer la texture");
                                if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");

                                Rectangle bouttons_oui_non[] = {
                                    {3, 600, 630, position_texte.y, position_texte.y + 30, SDL_FALSE},
                                    {3, 800, 830, position_texte.y, position_texte.y + 30, SDL_FALSE}};
                                if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                    SDL_ExitWithError("Impossible de changer la couleuur du rendu");
                                cases.x = bouttons_oui_non[0].x1;
                                cases.y = bouttons_oui_non[0].y1;
                                cases.h = 30;
                                cases.w = 30;
                                if (SDL_RenderFillRect(rendu, &cases) != 0)
                                    SDL_ExitWithError("Impossible de dessiner les cases");
                                cases.x = bouttons_oui_non[1].x1;
                                cases.y = bouttons_oui_non[1].y1;
                                if (SDL_RenderFillRect(rendu, &cases) != 0)
                                    SDL_ExitWithError("Impossible de dessiner les cases");
                                if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                    SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                SDL_RenderPresent(rendu);

                                SDL_bool oui_non = SDL_TRUE;
                                while (oui_non)
                                {
                                    temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

                                    // Dessiner une bordure rouge si le boutton de retour est survolé
                                    int mouseX, mouseY;
                                    SDL_GetMouseState(&mouseX, &mouseY);
                                    SDL_bool stateChanged = SDL_FALSE;
                                    SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
                                    if (currentlyInside != boutton_retour.mouseInside)
                                    {
                                        boutton_retour.mouseInside = currentlyInside;
                                        stateChanged = SDL_TRUE;
                                    }
                                    if (stateChanged)
                                    {
                                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                        if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                        cases.h = 30;
                                        cases.w = 30;
                                        for (int i = 0; i < 2; i++)
                                        {
                                            cases.x = bouttons_oui_non[i].x1;
                                            cases.y = bouttons_oui_non[i].y1;
                                            if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                        }
                                        if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                        if (boutton_retour.mouseInside)
                                        {
                                            dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
                                        }
                                        SDL_RenderPresent(rendu);
                                    }

                                    // Dessiner une bordure rouge si un des bouttons (oui et non) est survolé
                                    stateChanged = SDL_FALSE;
                                    SDL_GetMouseState(&mouseX, &mouseY);
                                    for (int i = 0; i < 2; i++)
                                    {
                                        currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons_oui_non[i]);
                                        if (currentlyInside != bouttons_oui_non[i].mouseInside)
                                        {
                                            bouttons_oui_non[i].mouseInside = currentlyInside;
                                            stateChanged = SDL_TRUE;
                                        }
                                    }
                                    if (stateChanged)
                                    {
                                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                        if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                        cases.h = 30;
                                        cases.w = 30;
                                        for (int i = 0; i < 2; i++)
                                        {
                                            cases.x = bouttons_oui_non[i].x1;
                                            cases.y = bouttons_oui_non[i].y1;
                                            if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                        }
                                        if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");

                                        for (int i = 0; i < 2; ++i)
                                        {
                                            if (bouttons_oui_non[i].mouseInside)
                                            {
                                                dessiner_rectangle(rendu, bouttons_oui_non[i].x1, bouttons_oui_non[i].y1, 30, 30);
                                                SDL_RenderDrawLine(rendu, bouttons_oui_non[i].x2, bouttons_oui_non[i].y1, bouttons_oui_non[i].x1, bouttons_oui_non[i].y2);
                                                SDL_RenderDrawLine(rendu, bouttons_oui_non[i].x1, bouttons_oui_non[i].y1, bouttons_oui_non[i].x2, bouttons_oui_non[i].y2);
                                            }
                                        }
                                        SDL_RenderPresent(rendu);
                                    }

                                    while (SDL_PollEvent(&event))
                                    {
                                        switch (event.type)
                                        {
                                        case SDL_MOUSEBUTTONDOWN:
                                            if (event.button.button == SDL_BUTTON_LEFT)
                                            {
                                                if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                                                    oui_non = SDL_FALSE;
                                                else
                                                {
                                                    if ((event.button.x < bouttons_oui_non[0].x2) && (event.button.x > bouttons_oui_non[0].x1) && (event.button.y < bouttons_oui_non[0].y2) && (event.button.y > bouttons_oui_non[0].y1))
                                                    {
                                                        ajout_poly_ensemble(&Tete_Ensemble, Tete_Poly2);
                                                        oui_non = SDL_FALSE;
                                                    }
                                                    if ((event.button.x < bouttons_oui_non[1].x2) && (event.button.x > bouttons_oui_non[1].x1) && (event.button.y < bouttons_oui_non[1].y2) && (event.button.y > bouttons_oui_non[1].y1))
                                                    {
                                                        oui_non = SDL_FALSE;
                                                    }
                                                }
                                            }
                                            continue;

                                        case SDL_QUIT:
                                            oui_non = SDL_FALSE;
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
                                }
                                //------------------------------------------------Fin Copie du Polynome-------------------------------------------------
                                sortir_tache = SDL_TRUE;
                                break;
                            }
                        }
                    }
                }
                continue;

            case SDL_QUIT:
                sortir_tache = SDL_TRUE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }

    free(bouttons);
    TTF_CloseFont(police2);
    TTF_CloseFont(police3);
    TTF_CloseFont(police4);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(page3);
    SDL_DestroyTexture(texture_p3);
    return;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
void simplifier_polynome_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble)
{
    LLC_Poly *Poly = Tete_Ensemble;
    Monome *Tete_Poly1 = NULL, *Tete_Poly2 = NULL;
    char chaine_char[30] = "";
    SDL_Texture *texture_p3 = NULL;
    SDL_Surface *page3 = NULL, *texte = NULL;
    TTF_Font *police2 = NULL, *police3 = NULL, *police4 = NULL;
    SDL_Rect position_textures, position_texte, cases;
    SDL_Event event;
    SDL_bool sortir_tache = SDL_FALSE;
    unsigned int temps_limite = 0, cpt = 0;
    Rectangle boutton_retour = {3, 100, 260, 632, 687, SDL_FALSE};
    Rectangle *bouttons = (Rectangle *)malloc(20 * sizeof(Rectangle));

    page3 = IMG_Load("assets/images/03.jpg");
    if (page3 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    police2 = TTF_OpenFont("assets/polices/times.ttf", 40);
    if (police2 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

    police3 = TTF_OpenFont("assets/polices/times.ttf", 25);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    police4 = TTF_OpenFont("assets/polices/times.ttf", 15);
    if (police4 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    position_texte.x = 80;
    position_texte.y = 80;
    texte = TTF_RenderText_Blended(police2, "Veuillez choisir le polynome a simplifier :", BLANC);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);

    //-----------------------------------------------------------------------------------------------------
    position_texte.y = 100;
    if (Poly == NULL)
    {
        position_texte.x = 600;
        position_texte.y = 300;
        texte = TTF_RenderText_Blended(police3, "Cet ensemble est vide", NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
    }
    while (Poly != NULL)
    {
        Tete_Poly1 = Poly->adr1;
        position_texte.x = 280;
        position_texte.y += 50;
        bouttons[cpt].page = 3;
        bouttons[cpt].x1 = 230;
        bouttons[cpt].x2 = 260;
        bouttons[cpt].y1 = 150 + 50 * cpt;
        bouttons[cpt].y2 = 180 + 50 * cpt;
        bouttons[cpt].mouseInside = SDL_FALSE;
        cpt++;
        sprintf(chaine_char, "P%d = ", cpt);
        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
        position_texte.x += texte->w;
        if (Tete_Poly1 == NULL)
        {
            texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        else
        {
            while (suivant(Tete_Poly1) != NULL)
            {
                sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                if (position_texte.x + texte->w >= 1200)
                {
                    position_texte.x = 330;
                    position_texte.y += 30;
                }
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                Tete_Poly1 = suivant(Tete_Poly1);
            }
            sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
            texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
            if (position_texte.x + texte->w >= 1230)
            {
                position_texte.x = 330;
                position_texte.y += 30;
            }
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            sprintf(chaine_char, "%d", Tete_Poly1->puiss);
            texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            texte = TTF_RenderText_Blended(police3, ")", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        Poly = Poly->adr2;
    }

    //-----------------------------------------------------------------------------------------------------
    position_textures.x = 0;
    position_textures.y = 0;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
    if (texture_p3 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
    SDL_RenderPresent(rendu);

    if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    cases.h = 30;
    cases.w = 30;
    for (int i = 0; i < cpt; i++)
    {
        cases.x = bouttons[i].x1;
        cases.y = bouttons[i].y1;
        if (SDL_RenderFillRect(rendu, &cases) != 0)
            SDL_ExitWithError("Impossible de dessiner les cases à cocher");
    }
    if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    SDL_RenderPresent(rendu);

    while (!sortir_tache)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        // Dessiner une bordure rouge si le boutton de retour est survolé
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
        if (currentlyInside != boutton_retour.mouseInside)
        {
            boutton_retour.mouseInside = currentlyInside;
            stateChanged = SDL_TRUE;
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            if (boutton_retour.mouseInside)
            {
                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
            }
            SDL_RenderPresent(rendu);
        }

        // Dessiner une bordure et une croix si les cases à cocher sont survolées
        stateChanged = SDL_FALSE;
        SDL_GetMouseState(&mouseX, &mouseY);
        for (int i = 0; i < cpt; ++i)
        {
            currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons[i]);
            if (currentlyInside != bouttons[i].mouseInside)
            {
                bouttons[i].mouseInside = currentlyInside;
                stateChanged = SDL_TRUE;
            }
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            for (int i = 0; i < cpt; ++i)
            {
                if (bouttons[i].mouseInside)
                {
                    dessiner_rectangle(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2 - bouttons[i].x1, bouttons[i].y2 - bouttons[i].y1);
                    SDL_RenderDrawLine(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2, bouttons[i].y2);
                    SDL_RenderDrawLine(rendu, bouttons[i].x2, bouttons[i].y1, bouttons[i].x1, bouttons[i].y2);
                }
            }
            SDL_RenderPresent(rendu);
        }

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                        sortir_tache = SDL_TRUE;
                    else
                    {
                        for (int i = 0; i < cpt; i++)
                        {
                            if ((event.button.x < bouttons[i].x2) && (event.button.x > bouttons[i].x1) && (event.button.y < bouttons[i].y2) && (event.button.y > bouttons[i].y1))
                            {
                                //-----------------------------------------------Simplification du polynome------------------------------------------------------
                                Tete_Poly1 = copie_poly(adr_maillon_ens(Tete_Ensemble, i + 1)->adr1);
                                simpl_poly(&Tete_Poly1);
                                Tete_Poly2 = Tete_Poly1;
                                page3 = IMG_Load("assets/images/03.jpg");
                                if (page3 == NULL)
                                    SDL_ExitWithError("Impossible de charger l'image");
                                position_texte.x = 80;
                                position_texte.y = 80;
                                texte = TTF_RenderText_Blended(police2, "Voici le resultat :", BLANC);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_texte.x = 280;
                                position_texte.y += 50;
                                texte = TTF_RenderText_Blended(police3, "P = ", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                position_texte.x += texte->w;
                                if (Tete_Poly1 == NULL)
                                {
                                    texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                }
                                else
                                {
                                    while (suivant(Tete_Poly1) != NULL)
                                    {
                                        sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                        if (position_texte.x + texte->w >= 1200)
                                        {
                                            position_texte.x = 330;
                                            position_texte.y += 30;
                                        }
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                        texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        Tete_Poly1 = suivant(Tete_Poly1);
                                    }
                                    sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                    texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                    if (position_texte.x + texte->w >= 1230)
                                    {
                                        position_texte.x = 330;
                                        position_texte.y += 30;
                                    }
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    position_texte.x += texte->w;
                                    sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                    texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    position_texte.x += texte->w;
                                    texte = TTF_RenderText_Blended(police3, ")", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                }
                                position_texte.x = 500;
                                position_texte.y += 200;
                                texte = TTF_RenderText_Blended(police3, " Avant de quitter la page :", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_texte.y += 50;
                                texte = TTF_RenderText_Blended(police3, " Voulez vous ajouter ce nouveau polynome a l'ensemble ?", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_texte.x = 650;
                                position_texte.y += 50;
                                texte = TTF_RenderText_Blended(police3, "OUI", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_texte.x = 850;
                                texte = TTF_RenderText_Blended(police3, "NON", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_textures.x = 0;
                                position_textures.y = 0;
                                texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
                                if (texture_p3 == NULL)
                                    SDL_ExitWithError("Impossible de créer la texture");
                                if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");

                                Rectangle bouttons_oui_non[] = {
                                    {3, 600, 630, position_texte.y, position_texte.y + 30, SDL_FALSE},
                                    {3, 800, 830, position_texte.y, position_texte.y + 30, SDL_FALSE}};
                                SDL_bool oui_non = SDL_TRUE;
                                if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                    SDL_ExitWithError("Impossible de changer la couleuur du rendu");
                                cases.x = bouttons_oui_non[0].x1;
                                cases.y = bouttons_oui_non[0].y1;
                                cases.h = 30;
                                cases.w = 30;
                                if (SDL_RenderFillRect(rendu, &cases) != 0)
                                    SDL_ExitWithError("Impossible de dessiner les cases");
                                cases.x = bouttons_oui_non[1].x1;
                                cases.y = bouttons_oui_non[1].y1;
                                if (SDL_RenderFillRect(rendu, &cases) != 0)
                                    SDL_ExitWithError("Impossible de dessiner les cases");
                                if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                    SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                SDL_RenderPresent(rendu);

                                while (oui_non)
                                {
                                    temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

                                    // Dessiner une bordure rouge si le boutton de retour est survolé
                                    int mouseX, mouseY;
                                    SDL_GetMouseState(&mouseX, &mouseY);
                                    SDL_bool stateChanged = SDL_FALSE;
                                    SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
                                    if (currentlyInside != boutton_retour.mouseInside)
                                    {
                                        boutton_retour.mouseInside = currentlyInside;
                                        stateChanged = SDL_TRUE;
                                    }
                                    if (stateChanged)
                                    {
                                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                        if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                        cases.h = 30;
                                        cases.w = 30;
                                        for (int i = 0; i < 2; i++)
                                        {
                                            cases.x = bouttons_oui_non[i].x1;
                                            cases.y = bouttons_oui_non[i].y1;
                                            if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                        }
                                        if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                        if (boutton_retour.mouseInside)
                                        {
                                            dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
                                        }
                                        SDL_RenderPresent(rendu);
                                    }

                                    // Dessiner une bordure rouge si un des bouttons (oui et non) est survolé
                                    stateChanged = SDL_FALSE;
                                    SDL_GetMouseState(&mouseX, &mouseY);
                                    for (int i = 0; i < 2; i++)
                                    {
                                        currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons_oui_non[i]);
                                        if (currentlyInside != bouttons_oui_non[i].mouseInside)
                                        {
                                            bouttons_oui_non[i].mouseInside = currentlyInside;
                                            stateChanged = SDL_TRUE;
                                        }
                                    }
                                    if (stateChanged)
                                    {
                                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                        if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                        cases.h = 30;
                                        cases.w = 30;
                                        for (int i = 0; i < 2; i++)
                                        {
                                            cases.x = bouttons_oui_non[i].x1;
                                            cases.y = bouttons_oui_non[i].y1;
                                            if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                        }
                                        if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");

                                        for (int i = 0; i < 2; ++i)
                                        {
                                            if (bouttons_oui_non[i].mouseInside)
                                            {
                                                dessiner_rectangle(rendu, bouttons_oui_non[i].x1, bouttons_oui_non[i].y1, 30, 30);
                                                SDL_RenderDrawLine(rendu, bouttons_oui_non[i].x2, bouttons_oui_non[i].y1, bouttons_oui_non[i].x1, bouttons_oui_non[i].y2);
                                                SDL_RenderDrawLine(rendu, bouttons_oui_non[i].x1, bouttons_oui_non[i].y1, bouttons_oui_non[i].x2, bouttons_oui_non[i].y2);
                                            }
                                        }
                                        SDL_RenderPresent(rendu);
                                    }

                                    while (SDL_PollEvent(&event))
                                    {
                                        switch (event.type)
                                        {
                                        case SDL_MOUSEBUTTONDOWN:
                                            if (event.button.button == SDL_BUTTON_LEFT)
                                            {
                                                if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                                                    oui_non = SDL_FALSE;
                                                else
                                                {
                                                    if ((event.button.x < bouttons_oui_non[0].x2) && (event.button.x > bouttons_oui_non[0].x1) && (event.button.y < bouttons_oui_non[0].y2) && (event.button.y > bouttons_oui_non[0].y1))
                                                    {
                                                        ajout_poly_ensemble(&Tete_Ensemble, Tete_Poly2);
                                                        oui_non = SDL_FALSE;
                                                    }
                                                    if ((event.button.x < bouttons_oui_non[1].x2) && (event.button.x > bouttons_oui_non[1].x1) && (event.button.y < bouttons_oui_non[1].y2) && (event.button.y > bouttons_oui_non[1].y1))
                                                    {
                                                        oui_non = SDL_FALSE;
                                                    }
                                                }
                                            }
                                            continue;

                                        case SDL_QUIT:
                                            oui_non = SDL_FALSE;
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
                                }
                                //--------------------------------------------Fin de la simplification du Polynome---------------------------------------------
                                sortir_tache = SDL_TRUE;
                                break;
                            }
                        }
                    }
                }
                continue;

            case SDL_QUIT:
                sortir_tache = SDL_TRUE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }

    free(bouttons);
    TTF_CloseFont(police2);
    TTF_CloseFont(police3);
    TTF_CloseFont(police4);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(page3);
    SDL_DestroyTexture(texture_p3);
    return;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
void evaluer_polynome_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble)
{
    LLC_Poly *Poly = Tete_Ensemble;
    Monome *Tete_Poly1 = NULL, *Tete_Poly2 = NULL;
    char chaine_char[30] = "";
    int coord_y[20] = {0};
    int valeur1 = -1;
    float valeur = -1;
    unsigned int temps_limite = 0, cpt = 0;
    SDL_Texture *texture_p3 = NULL;
    SDL_Surface *page3 = NULL, *texte = NULL;
    TTF_Font *police2 = NULL, *police3 = NULL, *police4 = NULL;
    SDL_Rect position_textures, position_texte, cases;
    SDL_Event event;
    SDL_bool sortir_tache = SDL_FALSE;
    Rectangle boutton_retour = {3, 100, 260, 632, 687, SDL_FALSE};
    Rectangle *bouttons = (Rectangle *)malloc(20 * sizeof(Rectangle));

    page3 = IMG_Load("assets/images/03.jpg");
    if (page3 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    police2 = TTF_OpenFont("assets/polices/times.ttf", 40);
    if (police2 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

    police3 = TTF_OpenFont("assets/polices/times.ttf", 25);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    police4 = TTF_OpenFont("assets/polices/times.ttf", 15);
    if (police4 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    position_texte.x = 80;
    position_texte.y = 80;
    texte = TTF_RenderText_Blended(police2, "Veuillez choisir le polynome a evaluer :", BLANC);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);

    //-----------------------------------------------------------------------------------------------------
    position_texte.y = 100;
    if (Poly == NULL)
    {
        position_texte.x = 600;
        position_texte.y = 300;
        texte = TTF_RenderText_Blended(police3, "Cet ensemble est vide", NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
    }
    while (Poly != NULL)
    {
        Tete_Poly1 = Poly->adr1;
        position_texte.x = 280;
        position_texte.y += 50;
        bouttons[cpt].page = 3;
        bouttons[cpt].x1 = 230;
        bouttons[cpt].x2 = 260;
        bouttons[cpt].y1 = 150 + 50 * cpt;
        bouttons[cpt].y2 = 180 + 50 * cpt;
        bouttons[cpt].mouseInside = SDL_FALSE;
        cpt++;
        sprintf(chaine_char, "P%d = ", cpt);
        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
        position_texte.x += texte->w;
        if (Tete_Poly1 == NULL)
        {
            texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        else
        {
            while (suivant(Tete_Poly1) != NULL)
            {
                sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                if (position_texte.x + texte->w >= 1200)
                {
                    position_texte.x = 330;
                    position_texte.y += 30;
                }
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                Tete_Poly1 = suivant(Tete_Poly1);
            }
            sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
            texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
            if (position_texte.x + texte->w >= 1230)
            {
                position_texte.x = 330;
                position_texte.y += 30;
            }
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            sprintf(chaine_char, "%d", Tete_Poly1->puiss);
            texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            texte = TTF_RenderText_Blended(police3, ")", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        Poly = Poly->adr2;
    }

    //-----------------------------------------------------------------------------------------------------
    position_textures.x = 0;
    position_textures.y = 0;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
    if (texture_p3 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");

    if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    cases.h = 30;
    cases.w = 30;
    for (int i = 0; i < cpt; i++)
    {
        cases.x = bouttons[i].x1;
        cases.y = bouttons[i].y1;
        if (SDL_RenderFillRect(rendu, &cases) != 0)
            SDL_ExitWithError("Impossible de dessiner les cases à cocher");
    }
    if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    SDL_RenderPresent(rendu);

    while (!sortir_tache)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        // Dessiner une bordure rouge si le boutton de retour est survolé
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
        if (currentlyInside != boutton_retour.mouseInside)
        {
            boutton_retour.mouseInside = currentlyInside;
            stateChanged = SDL_TRUE;
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            if (boutton_retour.mouseInside)
            {
                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
            }
            SDL_RenderPresent(rendu);
        }

        // Dessiner une bordure et une croix si les cases à cocher sont survolées
        stateChanged = SDL_FALSE;
        SDL_GetMouseState(&mouseX, &mouseY);
        for (int i = 0; i < cpt; ++i)
        {
            currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons[i]);
            if (currentlyInside != bouttons[i].mouseInside)
            {
                bouttons[i].mouseInside = currentlyInside;
                stateChanged = SDL_TRUE;
            }
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            for (int i = 0; i < cpt; ++i)
            {
                if (bouttons[i].mouseInside)
                {
                    dessiner_rectangle(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2 - bouttons[i].x1, bouttons[i].y2 - bouttons[i].y1);
                    SDL_RenderDrawLine(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2, bouttons[i].y2);
                    SDL_RenderDrawLine(rendu, bouttons[i].x2, bouttons[i].y1, bouttons[i].x1, bouttons[i].y2);
                }
            }
            SDL_RenderPresent(rendu);
        }

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                        sortir_tache = SDL_TRUE;
                    else
                    {
                        for (int i = 0; i < cpt; i++)
                        {
                            if ((event.button.x < bouttons[i].x2) && (event.button.x > bouttons[i].x1) && (event.button.y < bouttons[i].y2) && (event.button.y > bouttons[i].y1))
                            {
                                //-----------------------------------------------Evaluation du polynome------------------------------------------------------
                                Tete_Poly1 = adr_maillon_ens(Tete_Ensemble, i + 1)->adr1;
                                Tete_Poly2 = Tete_Poly1;
                                page3 = IMG_Load("assets/images/03.jpg");
                                if (page3 == NULL)
                                    SDL_ExitWithError("Impossible de charger l'image");
                                position_texte.x = 80;
                                position_texte.y = 80;
                                texte = TTF_RenderText_Blended(police2, "Veuillez entrer une valeur afin d'evaluer P :", BLANC);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_texte.x = 300;
                                position_texte.y += 50;
                                texte = TTF_RenderText_Blended(police3, "P = ", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                position_texte.x += texte->w;
                                if (Tete_Poly1 == NULL)
                                {
                                    texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                }
                                else
                                {
                                    while (suivant(Tete_Poly1) != NULL)
                                    {
                                        sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                        if (position_texte.x + texte->w >= 1200)
                                        {
                                            position_texte.x = 330;
                                            position_texte.y += 30;
                                        }
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                        texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        Tete_Poly1 = suivant(Tete_Poly1);
                                    }
                                    sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                    texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                    if (position_texte.x + texte->w >= 1230)
                                    {
                                        position_texte.x = 330;
                                        position_texte.y += 30;
                                    }
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    position_texte.x += texte->w;
                                    sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                    texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    position_texte.x += texte->w;
                                    texte = TTF_RenderText_Blended(police3, ")", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                }
                                position_texte.x = 300;
                                position_texte.y += 50;
                                texte = TTF_RenderText_Blended(police3, "     La valeur  --->  ", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_textures.x = 0;
                                position_textures.y = 0;
                                texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
                                if (texture_p3 == NULL)
                                    SDL_ExitWithError("Impossible de créer la texture");
                                if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                SDL_RenderPresent(rendu);

                                position_texte.x += texte->w;
                                if (SDL_lire_reel(&valeur, rendu, &page3, position_texte.x, position_texte.y) == -1)
                                    goto FIN_EVALUATION;

                                position_texte.x = 300;
                                position_texte.y += 50;
                                sprintf(chaine_char, "     P (%.2f)  =  %.3f", valeur, eval_poly(Tete_Poly2, valeur));
                                texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
                                if (texture_p3 == NULL)
                                    SDL_ExitWithError("Impossible de créer la texture");
                                if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                SDL_RenderPresent(rendu);

                                SDL_bool sortir_tache2 = SDL_FALSE;
                                while (!sortir_tache2)
                                {
                                    temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

                                    // Dessiner une bordure rouge si le boutton de retour est survolé
                                    int mouseX, mouseY;
                                    SDL_GetMouseState(&mouseX, &mouseY);
                                    SDL_bool stateChanged = SDL_FALSE;
                                    SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
                                    if (currentlyInside != boutton_retour.mouseInside)
                                    {
                                        boutton_retour.mouseInside = currentlyInside;
                                        stateChanged = SDL_TRUE;
                                    }
                                    if (stateChanged)
                                    {
                                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");

                                        if (boutton_retour.mouseInside)
                                        {
                                            dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
                                        }
                                        SDL_RenderPresent(rendu);
                                    }

                                    while (SDL_PollEvent(&event))
                                    {
                                        switch (event.type)
                                        {
                                        case SDL_MOUSEBUTTONDOWN:
                                            if (event.button.button == SDL_BUTTON_LEFT)
                                            {
                                                if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                                                    sortir_tache2 = SDL_TRUE;
                                            }
                                            continue;
                                        case SDL_QUIT:
                                            sortir_tache2 = SDL_TRUE;
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
                                }
                                sortir_tache = SDL_TRUE;
                            }
                        }
                    }
                }
                continue;

            case SDL_QUIT:
                sortir_tache = SDL_TRUE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }
FIN_EVALUATION:
    free(bouttons);
    TTF_CloseFont(police2);
    TTF_CloseFont(police3);
    TTF_CloseFont(police4);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(page3);
    SDL_DestroyTexture(texture_p3);
    return;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
void addition_polynomes_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble)
{
    LLC_Poly *Poly = Tete_Ensemble;
    Monome *Tete_Poly1 = NULL, *Tete_Poly2 = NULL, *Tete_Poly3 = NULL;
    char chaine_char[30] = "";
    unsigned int temps_limite = 0, cpt = 0, nb_polys = 0;
    SDL_Texture *texture_p3 = NULL;
    SDL_Surface *page3 = NULL, *texte = NULL;
    TTF_Font *police2 = NULL, *police3 = NULL, *police4 = NULL;
    SDL_Rect position_textures, position_texte, cases;
    SDL_Event event;
    SDL_bool sortir_tache = SDL_FALSE;
    Rectangle btn_selected = {3, 0, 0, 0, 0, SDL_FALSE};
    Rectangle boutton_retour = {3, 100, 260, 632, 687, SDL_FALSE};
    Rectangle *bouttons = (Rectangle *)malloc(20 * sizeof(Rectangle));

    page3 = IMG_Load("assets/images/03.jpg");
    if (page3 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    police2 = TTF_OpenFont("assets/polices/times.ttf", 40);
    if (police2 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

    police3 = TTF_OpenFont("assets/polices/times.ttf", 25);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    police4 = TTF_OpenFont("assets/polices/times.ttf", 15);
    if (police4 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    position_texte.x = 80;
    position_texte.y = 80;
    texte = TTF_RenderText_Blended(police2, "Veuillez selectionner 02 polynome a additionner :", BLANC);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);

    //-----------------------------------------------------------------------------------------------------
    position_texte.y = 100;
    if (Poly == NULL)
    {
        position_texte.x = 600;
        position_texte.y = 300;
        texte = TTF_RenderText_Blended(police3, "Cet ensemble est vide", NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
    }
    while (Poly != NULL)
    {
        Tete_Poly1 = Poly->adr1;
        position_texte.x = 280;
        position_texte.y += 50;
        bouttons[cpt].page = 3;
        bouttons[cpt].x1 = 230;
        bouttons[cpt].x2 = 260;
        bouttons[cpt].y1 = 150 + 50 * cpt;
        bouttons[cpt].y2 = 180 + 50 * cpt;
        bouttons[cpt].mouseInside = SDL_FALSE;
        cpt++;
        sprintf(chaine_char, "P%d = ", cpt);
        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
        position_texte.x += texte->w;
        if (Tete_Poly1 == NULL)
        {
            texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        else
        {
            while (suivant(Tete_Poly1) != NULL)
            {
                sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                if (position_texte.x + texte->w >= 1200)
                {
                    position_texte.x = 330;
                    position_texte.y += 30;
                }
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                Tete_Poly1 = suivant(Tete_Poly1);
            }
            sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
            texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
            if (position_texte.x + texte->w >= 1230)
            {
                position_texte.x = 330;
                position_texte.y += 30;
            }
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            sprintf(chaine_char, "%d", Tete_Poly1->puiss);
            texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            texte = TTF_RenderText_Blended(police3, ")", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        Poly = Poly->adr2;
    }

    //-----------------------------------------------------------------------------------------------------
    position_textures.x = 0;
    position_textures.y = 0;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
    if (texture_p3 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");

    if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    cases.h = 30;
    cases.w = 30;
    for (int i = 0; i < cpt; i++)
    {
        cases.x = bouttons[i].x1;
        cases.y = bouttons[i].y1;
        if (SDL_RenderFillRect(rendu, &cases) != 0)
            SDL_ExitWithError("Impossible de dessiner les cases à cocher");
    }
    if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    SDL_RenderPresent(rendu);

    while (!sortir_tache)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        // Dessiner une bordure rouge si le boutton de retour est survolé
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
        if (currentlyInside != boutton_retour.mouseInside)
        {
            boutton_retour.mouseInside = currentlyInside;
            stateChanged = SDL_TRUE;
        }
        if (stateChanged)
        {
            // Copier la texture du départ (page 3)
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            // Dessiner les cases à cocher
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            if (nb_polys == 1)
            {
                dessiner_rectangle(rendu, btn_selected.x1, btn_selected.y1, 30, 30);
                SDL_RenderDrawLine(rendu, btn_selected.x1, btn_selected.y1, btn_selected.x2, btn_selected.y2);
                SDL_RenderDrawLine(rendu, btn_selected.x2, btn_selected.y1, btn_selected.x1, btn_selected.y2);
            }
            if (boutton_retour.mouseInside)
            {
                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
            }
            SDL_RenderPresent(rendu);
        }

        // Dessiner une bordure et une croix si les caches à cocher sont survolées
        stateChanged = SDL_FALSE;
        SDL_GetMouseState(&mouseX, &mouseY);
        for (int i = 0; i < cpt; ++i)
        {
            currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons[i]);
            if (currentlyInside != bouttons[i].mouseInside)
            {
                bouttons[i].mouseInside = currentlyInside;
                stateChanged = SDL_TRUE;
            }
        }
        if (stateChanged)
        {
            // Copier la texture du départ (page 3)
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            // Dessiner les cases à cocher
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            if (nb_polys == 1)
            {
                dessiner_rectangle(rendu, btn_selected.x1, btn_selected.y1, 30, 30);
                SDL_RenderDrawLine(rendu, btn_selected.x1, btn_selected.y1, btn_selected.x2, btn_selected.y2);
                SDL_RenderDrawLine(rendu, btn_selected.x2, btn_selected.y1, btn_selected.x1, btn_selected.y2);
            }
            for (int i = 0; i < cpt; ++i)
            {
                if (bouttons[i].mouseInside)
                {
                    dessiner_rectangle(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2 - bouttons[i].x1, bouttons[i].y2 - bouttons[i].y1);
                    SDL_RenderDrawLine(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2, bouttons[i].y2);
                    SDL_RenderDrawLine(rendu, bouttons[i].x2, bouttons[i].y1, bouttons[i].x1, bouttons[i].y2);
                }
            }
            SDL_RenderPresent(rendu);
        }

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                        sortir_tache = SDL_TRUE;
                    else
                    {
                        for (int i = 0; i < cpt; i++)
                        {
                            if (nb_polys == 0)
                            {
                                if ((event.button.x < bouttons[i].x2) && (event.button.x > bouttons[i].x1) && (event.button.y < bouttons[i].y2) && (event.button.y > bouttons[i].y1))
                                {
                                    Tete_Poly1 = adr_maillon_ens(Tete_Ensemble, i + 1)->adr1;
                                    btn_selected = bouttons[i];
                                    nb_polys = 1;
                                }
                            }
                            else // nb_polys == 1 (nombre de polyonomes selectionnés : 0 ou 1, si 2 faire l'addition)
                            {
                                if ((event.button.x < bouttons[i].x2) && (event.button.x > bouttons[i].x1) && (event.button.y < bouttons[i].y2) && (event.button.y > bouttons[i].y1))
                                {
                                    //-----------------------------------------------Addition des polynomes------------------------------------------------------
                                    Tete_Poly2 = adr_maillon_ens(Tete_Ensemble, i + 1)->adr1;
                                    Tete_Poly3 = add_poly(Tete_Poly1, Tete_Poly2);
                                    Tete_Poly1 = Tete_Poly3;
                                    page3 = IMG_Load("assets/images/03.jpg");
                                    if (page3 == NULL)
                                        SDL_ExitWithError("Impossible de charger l'image");
                                    position_texte.x = 80;
                                    position_texte.y = 80;
                                    texte = TTF_RenderText_Blended(police2, "Voici le resultat :", BLANC);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                    position_texte.x = 280;
                                    position_texte.y += 50;
                                    texte = TTF_RenderText_Blended(police3, "P = ", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    position_texte.x += texte->w;
                                    if (Tete_Poly1 == NULL)
                                    {
                                        texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    }
                                    else
                                    {
                                        while (suivant(Tete_Poly1) != NULL)
                                        {
                                            sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                            texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                            if (position_texte.x + texte->w >= 1200)
                                            {
                                                position_texte.x = 330;
                                                position_texte.y += 30;
                                            }
                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                            position_texte.x += texte->w;
                                            sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                            texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                            position_texte.x += texte->w;
                                            texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                            position_texte.x += texte->w;
                                            Tete_Poly1 = suivant(Tete_Poly1);
                                        }
                                        sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                        if (position_texte.x + texte->w >= 1230)
                                        {
                                            position_texte.x = 330;
                                            position_texte.y += 30;
                                        }
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                        texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        texte = TTF_RenderText_Blended(police3, ")", NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    }
                                    position_texte.x = 500;
                                    position_texte.y += 200;
                                    texte = TTF_RenderText_Blended(police3, " Avant de quitter la page :", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                    position_texte.y += 50;
                                    texte = TTF_RenderText_Blended(police3, " Voulez vous ajouter ce resultat a l'ensemble ?", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                    position_texte.x = 650;
                                    position_texte.y += 50;
                                    texte = TTF_RenderText_Blended(police3, "OUI", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                    position_texte.x = 850;
                                    texte = TTF_RenderText_Blended(police3, "NON", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                    position_textures.x = 0;
                                    position_textures.y = 0;
                                    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
                                    if (texture_p3 == NULL)
                                        SDL_ExitWithError("Impossible de créer la texture");
                                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");

                                    Rectangle bouttons_oui_non[] = {
                                        {3, 600, 630, position_texte.y, position_texte.y + 30, SDL_FALSE},
                                        {3, 800, 830, position_texte.y, position_texte.y + 30, SDL_FALSE}};
                                    if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                        SDL_ExitWithError("Impossible de changer la couleuur du rendu");
                                    cases.x = bouttons_oui_non[0].x1;
                                    cases.y = bouttons_oui_non[0].y1;
                                    cases.h = 30;
                                    cases.w = 30;
                                    if (SDL_RenderFillRect(rendu, &cases) != 0)
                                        SDL_ExitWithError("Impossible de dessiner les cases");
                                    cases.x = bouttons_oui_non[1].x1;
                                    cases.y = bouttons_oui_non[1].y1;
                                    if (SDL_RenderFillRect(rendu, &cases) != 0)
                                        SDL_ExitWithError("Impossible de dessiner les cases");
                                    if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                    SDL_RenderPresent(rendu);

                                    SDL_bool oui_non = SDL_TRUE;
                                    while (oui_non)
                                    {
                                        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

                                        // Dessiner une bordure rouge si le boutton de retour est survolé
                                        int mouseX, mouseY;
                                        SDL_GetMouseState(&mouseX, &mouseY);
                                        SDL_bool stateChanged = SDL_FALSE;
                                        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
                                        if (currentlyInside != boutton_retour.mouseInside)
                                        {
                                            boutton_retour.mouseInside = currentlyInside;
                                            stateChanged = SDL_TRUE;
                                        }
                                        if (stateChanged)
                                        {
                                            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                            if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                            cases.h = 30;
                                            cases.w = 30;
                                            for (int i = 0; i < 2; i++)
                                            {
                                                cases.x = bouttons_oui_non[i].x1;
                                                cases.y = bouttons_oui_non[i].y1;
                                                if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                            }
                                            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                            if (boutton_retour.mouseInside)
                                            {
                                                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
                                            }
                                            SDL_RenderPresent(rendu);
                                        }

                                        // Dessiner une bordure rouge si un des bouttons (oui et non) est survolé
                                        stateChanged = SDL_FALSE;
                                        SDL_GetMouseState(&mouseX, &mouseY);
                                        for (int i = 0; i < 2; i++)
                                        {
                                            currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons_oui_non[i]);
                                            if (currentlyInside != bouttons_oui_non[i].mouseInside)
                                            {
                                                bouttons_oui_non[i].mouseInside = currentlyInside;
                                                stateChanged = SDL_TRUE;
                                            }
                                        }
                                        if (stateChanged)
                                        {
                                            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                            if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                            cases.h = 30;
                                            cases.w = 30;
                                            for (int i = 0; i < 2; i++)
                                            {
                                                cases.x = bouttons_oui_non[i].x1;
                                                cases.y = bouttons_oui_non[i].y1;
                                                if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                            }
                                            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

                                            for (int i = 0; i < 2; ++i)
                                            {
                                                if (bouttons_oui_non[i].mouseInside)
                                                {
                                                    dessiner_rectangle(rendu, bouttons_oui_non[i].x1, bouttons_oui_non[i].y1, 30, 30);
                                                    SDL_RenderDrawLine(rendu, bouttons_oui_non[i].x2, bouttons_oui_non[i].y1, bouttons_oui_non[i].x1, bouttons_oui_non[i].y2);
                                                    SDL_RenderDrawLine(rendu, bouttons_oui_non[i].x1, bouttons_oui_non[i].y1, bouttons_oui_non[i].x2, bouttons_oui_non[i].y2);
                                                }
                                            }
                                            SDL_RenderPresent(rendu);
                                        }

                                        while (SDL_PollEvent(&event))
                                        {
                                            switch (event.type)
                                            {
                                            case SDL_MOUSEBUTTONDOWN:
                                                if (event.button.button == SDL_BUTTON_LEFT)
                                                {
                                                    if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                                                        oui_non = SDL_FALSE;
                                                    else
                                                    {
                                                        if ((event.button.x < bouttons_oui_non[0].x2) && (event.button.x > bouttons_oui_non[0].x1) && (event.button.y < bouttons_oui_non[0].y2) && (event.button.y > bouttons_oui_non[0].y1))
                                                        {
                                                            ajout_poly_ensemble(&Tete_Ensemble, Tete_Poly3);
                                                            oui_non = SDL_FALSE;
                                                        }
                                                        if ((event.button.x < bouttons_oui_non[1].x2) && (event.button.x > bouttons_oui_non[1].x1) && (event.button.y < bouttons_oui_non[1].y2) && (event.button.y > bouttons_oui_non[1].y1))
                                                        {
                                                            oui_non = SDL_FALSE;
                                                        }
                                                    }
                                                }
                                                continue;

                                            case SDL_QUIT:
                                                oui_non = SDL_FALSE;
                                                break;

                                            default:
                                                break;
                                            }
                                        }
                                        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
                                    }
                                    //------------------------------------------------Fin de l'addition des Polynomes-------------------------------------------------
                                    sortir_tache = SDL_TRUE;
                                    break;
                                }
                            }
                        }
                    }
                }
                continue;

            case SDL_QUIT:
                sortir_tache = SDL_TRUE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }

    free(bouttons);
    TTF_CloseFont(police2);
    TTF_CloseFont(police3);
    TTF_CloseFont(police4);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(page3);
    SDL_DestroyTexture(texture_p3);
    return;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
void soustraction_polynomes_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble)
{
    LLC_Poly *Poly = Tete_Ensemble, *Poly2 = Tete_Ensemble;
    Monome *Tete_Poly1 = NULL, *Tete_Poly2 = NULL, *Tete_Poly3 = NULL;
    char chaine_char[30] = "";
    unsigned int temps_limite = 0, cpt = 0;
    SDL_Texture *texture_p3 = NULL;
    SDL_Surface *page3 = NULL, *texte = NULL;
    TTF_Font *police2 = NULL, *police3 = NULL, *police4 = NULL;
    SDL_Rect position_textures, position_texte, cases;
    SDL_Event event;
    SDL_bool sortir_tache = SDL_FALSE, sortir_tache1 = SDL_FALSE;
    Rectangle btn_selected = {3, 0, 0, 0, 0, SDL_FALSE};
    Rectangle boutton_retour = {3, 100, 260, 632, 687, SDL_FALSE};
    Rectangle *bouttons = (Rectangle *)malloc(20 * sizeof(Rectangle));

    page3 = IMG_Load("assets/images/03.jpg");
    if (page3 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    police2 = TTF_OpenFont("assets/polices/times.ttf", 40);
    if (police2 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

    police3 = TTF_OpenFont("assets/polices/times.ttf", 25);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    police4 = TTF_OpenFont("assets/polices/times.ttf", 15);
    if (police4 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    position_texte.x = 80;
    position_texte.y = 80;
    texte = TTF_RenderText_Blended(police2, "La soustraction (P1-P2), veuillez selectionner P1 :", BLANC);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);

    //-----------------------------------------------------------------------------------------------------
    position_texte.y = 100;
    if (Poly == NULL)
    {
        position_texte.x = 600;
        position_texte.y = 300;
        texte = TTF_RenderText_Blended(police3, "Cet ensemble est vide", NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
    }
    while (Poly != NULL)
    {
        Tete_Poly1 = Poly->adr1;
        position_texte.x = 280;
        position_texte.y += 50;
        bouttons[cpt].page = 3;
        bouttons[cpt].x1 = 230;
        bouttons[cpt].x2 = 260;
        bouttons[cpt].y1 = 150 + 50 * cpt;
        bouttons[cpt].y2 = 180 + 50 * cpt;
        bouttons[cpt].mouseInside = SDL_FALSE;
        cpt++;
        sprintf(chaine_char, "P%d = ", cpt);
        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
        position_texte.x += texte->w;
        if (Tete_Poly1 == NULL)
        {
            texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        else
        {
            while (suivant(Tete_Poly1) != NULL)
            {
                sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                if (position_texte.x + texte->w >= 1200)
                {
                    position_texte.x = 330;
                    position_texte.y += 30;
                }
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                Tete_Poly1 = suivant(Tete_Poly1);
            }
            sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
            texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
            if (position_texte.x + texte->w >= 1230)
            {
                position_texte.x = 330;
                position_texte.y += 30;
            }
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            sprintf(chaine_char, "%d", Tete_Poly1->puiss);
            texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            texte = TTF_RenderText_Blended(police3, ")", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        Poly = Poly->adr2;
    }

    //-----------------------------------------------------------------------------------------------------
    position_textures.x = 0;
    position_textures.y = 0;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
    if (texture_p3 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");

    if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    cases.h = 30;
    cases.w = 30;
    for (int i = 0; i < cpt; i++)
    {
        cases.x = bouttons[i].x1;
        cases.y = bouttons[i].y1;
        if (SDL_RenderFillRect(rendu, &cases) != 0)
            SDL_ExitWithError("Impossible de dessiner les cases à cocher");
    }
    if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    SDL_RenderPresent(rendu);

    while (!sortir_tache)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        // Dessiner une bordure rouge si le boutton de retour est survolé
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
        if (currentlyInside != boutton_retour.mouseInside)
        {
            boutton_retour.mouseInside = currentlyInside;
            stateChanged = SDL_TRUE;
        }
        if (stateChanged)
        {
            // Copier la texture du départ (page 3)
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            // Dessiner les cases à cocher
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            if (boutton_retour.mouseInside)
            {
                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
            }
            SDL_RenderPresent(rendu);
        }

        // Dessiner une bordure et une croix si les caches à cocher sont survolées
        stateChanged = SDL_FALSE;
        SDL_GetMouseState(&mouseX, &mouseY);
        for (int i = 0; i < cpt; ++i)
        {
            currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons[i]);
            if (currentlyInside != bouttons[i].mouseInside)
            {
                bouttons[i].mouseInside = currentlyInside;
                stateChanged = SDL_TRUE;
            }
        }
        if (stateChanged)
        {
            // Copier la texture du départ (page 3)
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            // Dessiner les cases à cocher
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            for (int i = 0; i < cpt; ++i)
            {
                if (bouttons[i].mouseInside)
                {
                    dessiner_rectangle(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2 - bouttons[i].x1, bouttons[i].y2 - bouttons[i].y1);
                    SDL_RenderDrawLine(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2, bouttons[i].y2);
                    SDL_RenderDrawLine(rendu, bouttons[i].x2, bouttons[i].y1, bouttons[i].x1, bouttons[i].y2);
                }
            }
            SDL_RenderPresent(rendu);
        }

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                        sortir_tache = SDL_TRUE;
                    else
                    {
                        for (int i = 0; i < cpt; i++)
                        {
                            if ((event.button.x < bouttons[i].x2) && (event.button.x > bouttons[i].x1) && (event.button.y < bouttons[i].y2) && (event.button.y > bouttons[i].y1))
                            {
                                Tete_Poly2 = adr_maillon_ens(Tete_Ensemble, i + 1)->adr1;
                                btn_selected = bouttons[i];
                                //-----------------------------------------------Selection du 2eme polynome------------------------------------------------------
                                page3 = IMG_Load("assets/images/03.jpg");
                                if (page3 == NULL)
                                    SDL_ExitWithError("Impossible de charger l'image");

                                police2 = TTF_OpenFont("assets/polices/times.ttf", 40);
                                if (police2 == NULL)
                                    SDL_ExitWithError("Impossible de charger la police Times New Romain");
                                TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

                                police3 = TTF_OpenFont("assets/polices/times.ttf", 25);
                                if (police3 == NULL)
                                    SDL_ExitWithError("Impossible de charger la police Times New Romain");
                                TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

                                police4 = TTF_OpenFont("assets/polices/times.ttf", 15);
                                if (police4 == NULL)
                                    SDL_ExitWithError("Impossible de charger la police Times New Romain");
                                TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

                                position_texte.x = 80;
                                position_texte.y = 80;
                                texte = TTF_RenderText_Blended(police2, "La soustraction (P1-P2), veuillez selectionner P2 :", BLANC);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                //-----------------------------------------------------------------------------------------------------
                                position_texte.y = 100;
                                Poly = Poly2;
                                cpt = 0;
                                if (Poly == NULL)
                                {
                                    position_texte.x = 600;
                                    position_texte.y = 300;
                                    texte = TTF_RenderText_Blended(police3, "Cet ensemble est vide", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                }
                                while (Poly != NULL)
                                {
                                    Tete_Poly1 = Poly->adr1;
                                    position_texte.x = 280;
                                    position_texte.y += 50;
                                    bouttons[cpt].page = 3;
                                    bouttons[cpt].x1 = 230;
                                    bouttons[cpt].x2 = 260;
                                    bouttons[cpt].y1 = 150 + 50 * cpt;
                                    bouttons[cpt].y2 = 180 + 50 * cpt;
                                    bouttons[cpt].mouseInside = SDL_FALSE;
                                    cpt++;
                                    sprintf(chaine_char, "P%d = ", cpt);
                                    texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    position_texte.x += texte->w;
                                    if (Tete_Poly1 == NULL)
                                    {
                                        texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    }
                                    else
                                    {
                                        while (suivant(Tete_Poly1) != NULL)
                                        {
                                            sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                            texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                            if (position_texte.x + texte->w >= 1200)
                                            {
                                                position_texte.x = 330;
                                                position_texte.y += 30;
                                            }
                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                            position_texte.x += texte->w;
                                            sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                            texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                            position_texte.x += texte->w;
                                            texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                            position_texte.x += texte->w;
                                            Tete_Poly1 = suivant(Tete_Poly1);
                                        }
                                        sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                        if (position_texte.x + texte->w >= 1230)
                                        {
                                            position_texte.x = 330;
                                            position_texte.y += 30;
                                        }
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                        texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        texte = TTF_RenderText_Blended(police3, ")", NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    }
                                    Poly = Poly->adr2;
                                }

                                //-----------------------------------------------------------------------------------------------------
                                position_textures.x = 0;
                                position_textures.y = 0;
                                texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
                                if (texture_p3 == NULL)
                                    SDL_ExitWithError("Impossible de créer la texture");
                                if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
                                    SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
                                if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");

                                if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                                    SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                cases.h = 30;
                                cases.w = 30;
                                for (int i = 0; i < cpt; i++)
                                {
                                    cases.x = bouttons[i].x1;
                                    cases.y = bouttons[i].y1;
                                    if (SDL_RenderFillRect(rendu, &cases) != 0)
                                        SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                }
                                if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                    SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                SDL_RenderPresent(rendu);

                                while (!sortir_tache1)
                                {
                                    temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

                                    // Dessiner une bordure rouge si le boutton de retour est survolé
                                    int mouseX, mouseY;
                                    SDL_GetMouseState(&mouseX, &mouseY);
                                    SDL_bool stateChanged = SDL_FALSE;
                                    SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
                                    if (currentlyInside != boutton_retour.mouseInside)
                                    {
                                        boutton_retour.mouseInside = currentlyInside;
                                        stateChanged = SDL_TRUE;
                                    }
                                    if (stateChanged)
                                    {
                                        // Copier la texture du départ (page 3)
                                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                        // Dessiner les cases à cocher
                                        if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                        cases.h = 30;
                                        cases.w = 30;
                                        for (int i = 0; i < cpt; i++)
                                        {
                                            cases.x = bouttons[i].x1;
                                            cases.y = bouttons[i].y1;
                                            if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                        }
                                        if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");

                                        dessiner_rectangle(rendu, btn_selected.x1, btn_selected.y1, 30, 30);
                                        SDL_RenderDrawLine(rendu, btn_selected.x1, btn_selected.y1, btn_selected.x2, btn_selected.y2);
                                        SDL_RenderDrawLine(rendu, btn_selected.x2, btn_selected.y1, btn_selected.x1, btn_selected.y2);
                                        if (boutton_retour.mouseInside)
                                        {
                                            dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
                                        }
                                        SDL_RenderPresent(rendu);
                                    }

                                    // Dessiner une bordure et une croix si les caches à cocher sont survolées
                                    stateChanged = SDL_FALSE;
                                    SDL_GetMouseState(&mouseX, &mouseY);
                                    for (int i = 0; i < cpt; ++i)
                                    {
                                        currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons[i]);
                                        if (currentlyInside != bouttons[i].mouseInside)
                                        {
                                            bouttons[i].mouseInside = currentlyInside;
                                            stateChanged = SDL_TRUE;
                                        }
                                    }
                                    if (stateChanged)
                                    {
                                        // Copier la texture du départ (page 3)
                                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                        // Dessiner les cases à cocher
                                        if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                        cases.h = 30;
                                        cases.w = 30;
                                        for (int i = 0; i < cpt; i++)
                                        {
                                            cases.x = bouttons[i].x1;
                                            cases.y = bouttons[i].y1;
                                            if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                        }
                                        if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");

                                        dessiner_rectangle(rendu, btn_selected.x1, btn_selected.y1, 30, 30);
                                        SDL_RenderDrawLine(rendu, btn_selected.x1, btn_selected.y1, btn_selected.x2, btn_selected.y2);
                                        SDL_RenderDrawLine(rendu, btn_selected.x2, btn_selected.y1, btn_selected.x1, btn_selected.y2);
                                        for (int i = 0; i < cpt; ++i)
                                        {
                                            if (bouttons[i].mouseInside)
                                            {
                                                dessiner_rectangle(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2 - bouttons[i].x1, bouttons[i].y2 - bouttons[i].y1);
                                                SDL_RenderDrawLine(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2, bouttons[i].y2);
                                                SDL_RenderDrawLine(rendu, bouttons[i].x2, bouttons[i].y1, bouttons[i].x1, bouttons[i].y2);
                                            }
                                        }
                                        SDL_RenderPresent(rendu);
                                    }

                                    while (SDL_PollEvent(&event))
                                    {
                                        switch (event.type)
                                        {
                                        case SDL_MOUSEBUTTONDOWN:
                                            if (event.button.button == SDL_BUTTON_LEFT)
                                            {
                                                if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                                                    sortir_tache = SDL_TRUE;
                                                else
                                                {
                                                    for (int i = 0; i < cpt; i++)
                                                    {
                                                        if ((event.button.x < bouttons[i].x2) && (event.button.x > bouttons[i].x1) && (event.button.y < bouttons[i].y2) && (event.button.y > bouttons[i].y1))
                                                        {
                                                            //-----------------------------------------------SOUSTRACTION des polynomes------------------------------------------------------
                                                            Tete_Poly3 = adr_maillon_ens(Tete_Ensemble, i + 1)->adr1;
                                                            Tete_Poly1 = add_poly(Tete_Poly2, inverse_poly(Tete_Poly3));
                                                            Tete_Poly3 = Tete_Poly1;
                                                            page3 = IMG_Load("assets/images/03.jpg");
                                                            if (page3 == NULL)
                                                                SDL_ExitWithError("Impossible de charger l'image");
                                                            position_texte.x = 80;
                                                            position_texte.y = 80;
                                                            texte = TTF_RenderText_Blended(police2, "Voici le resultat :", BLANC);
                                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                                            position_texte.x = 280;
                                                            position_texte.y += 50;
                                                            texte = TTF_RenderText_Blended(police3, "P = ", NOIR);
                                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                            position_texte.x += texte->w;
                                                            if (Tete_Poly1 == NULL)
                                                            {
                                                                texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
                                                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                            }
                                                            else
                                                            {
                                                                while (suivant(Tete_Poly1) != NULL)
                                                                {
                                                                    sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                                                    texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                                                    if (position_texte.x + texte->w >= 1200)
                                                                    {
                                                                        position_texte.x = 330;
                                                                        position_texte.y += 30;
                                                                    }
                                                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                    position_texte.x += texte->w;
                                                                    sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                                                    texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                    position_texte.x += texte->w;
                                                                    texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                                                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                    position_texte.x += texte->w;
                                                                    Tete_Poly1 = suivant(Tete_Poly1);
                                                                }
                                                                sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                                                texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                                                if (position_texte.x + texte->w >= 1230)
                                                                {
                                                                    position_texte.x = 330;
                                                                    position_texte.y += 30;
                                                                }
                                                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                position_texte.x += texte->w;
                                                                sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                                                texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                position_texte.x += texte->w;
                                                                texte = TTF_RenderText_Blended(police3, ")", NOIR);
                                                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                            }
                                                            position_texte.x = 500;
                                                            position_texte.y += 200;
                                                            texte = TTF_RenderText_Blended(police3, " Avant de quitter la page :", NOIR);
                                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                                            position_texte.y += 50;
                                                            texte = TTF_RenderText_Blended(police3, " Voulez vous ajouter ce resultat a l'ensemble ?", NOIR);
                                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                                            position_texte.x = 650;
                                                            position_texte.y += 50;
                                                            texte = TTF_RenderText_Blended(police3, "OUI", NOIR);
                                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                                            position_texte.x = 850;
                                                            texte = TTF_RenderText_Blended(police3, "NON", NOIR);
                                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                                            position_textures.x = 0;
                                                            position_textures.y = 0;
                                                            texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
                                                            if (texture_p3 == NULL)
                                                                SDL_ExitWithError("Impossible de créer la texture");
                                                            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                                                SDL_ExitWithError("Impossible de copier la texture dans le rendu");

                                                            Rectangle bouttons_oui_non[] = {
                                                                {3, 600, 630, position_texte.y, position_texte.y + 30, SDL_FALSE},
                                                                {3, 800, 830, position_texte.y, position_texte.y + 30, SDL_FALSE}};
                                                            if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                                                SDL_ExitWithError("Impossible de changer la couleuur du rendu");
                                                            cases.x = bouttons_oui_non[0].x1;
                                                            cases.y = bouttons_oui_non[0].y1;
                                                            cases.h = 30;
                                                            cases.w = 30;
                                                            if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                                SDL_ExitWithError("Impossible de dessiner les cases");
                                                            cases.x = bouttons_oui_non[1].x1;
                                                            cases.y = bouttons_oui_non[1].y1;
                                                            if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                                SDL_ExitWithError("Impossible de dessiner les cases");
                                                            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                                                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                                            SDL_RenderPresent(rendu);

                                                            SDL_bool oui_non = SDL_TRUE;
                                                            while (oui_non)
                                                            {
                                                                temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

                                                                // Dessiner une bordure rouge si le boutton de retour est survolé
                                                                int mouseX, mouseY;
                                                                SDL_GetMouseState(&mouseX, &mouseY);
                                                                SDL_bool stateChanged = SDL_FALSE;
                                                                SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
                                                                if (currentlyInside != boutton_retour.mouseInside)
                                                                {
                                                                    boutton_retour.mouseInside = currentlyInside;
                                                                    stateChanged = SDL_TRUE;
                                                                }
                                                                if (stateChanged)
                                                                {
                                                                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                                                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                                                    if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                                                        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                                                    cases.h = 30;
                                                                    cases.w = 30;
                                                                    for (int i = 0; i < 2; i++)
                                                                    {
                                                                        cases.x = bouttons_oui_non[i].x1;
                                                                        cases.y = bouttons_oui_non[i].y1;
                                                                        if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                                            SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                                                    }
                                                                    if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                                                        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                                                    if (boutton_retour.mouseInside)
                                                                    {
                                                                        dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
                                                                    }
                                                                    SDL_RenderPresent(rendu);
                                                                }

                                                                // Dessiner une bordure rouge si un des bouttons (oui et non) est survolé
                                                                stateChanged = SDL_FALSE;
                                                                SDL_GetMouseState(&mouseX, &mouseY);
                                                                for (int i = 0; i < 2; i++)
                                                                {
                                                                    currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons_oui_non[i]);
                                                                    if (currentlyInside != bouttons_oui_non[i].mouseInside)
                                                                    {
                                                                        bouttons_oui_non[i].mouseInside = currentlyInside;
                                                                        stateChanged = SDL_TRUE;
                                                                    }
                                                                }
                                                                if (stateChanged)
                                                                {
                                                                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                                                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                                                    if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                                                        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                                                    cases.h = 30;
                                                                    cases.w = 30;
                                                                    for (int i = 0; i < 2; i++)
                                                                    {
                                                                        cases.x = bouttons_oui_non[i].x1;
                                                                        cases.y = bouttons_oui_non[i].y1;
                                                                        if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                                            SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                                                    }
                                                                    if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                                                        SDL_ExitWithError("Impossible de changer la couleur au Rendu");

                                                                    for (int i = 0; i < 2; ++i)
                                                                    {
                                                                        if (bouttons_oui_non[i].mouseInside)
                                                                        {
                                                                            dessiner_rectangle(rendu, bouttons_oui_non[i].x1, bouttons_oui_non[i].y1, 30, 30);
                                                                            SDL_RenderDrawLine(rendu, bouttons_oui_non[i].x2, bouttons_oui_non[i].y1, bouttons_oui_non[i].x1, bouttons_oui_non[i].y2);
                                                                            SDL_RenderDrawLine(rendu, bouttons_oui_non[i].x1, bouttons_oui_non[i].y1, bouttons_oui_non[i].x2, bouttons_oui_non[i].y2);
                                                                        }
                                                                    }
                                                                    SDL_RenderPresent(rendu);
                                                                }

                                                                while (SDL_PollEvent(&event))
                                                                {
                                                                    switch (event.type)
                                                                    {
                                                                    case SDL_MOUSEBUTTONDOWN:
                                                                        if (event.button.button == SDL_BUTTON_LEFT)
                                                                        {
                                                                            if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                                                                                oui_non = SDL_FALSE;
                                                                            else
                                                                            {
                                                                                if ((event.button.x < bouttons_oui_non[0].x2) && (event.button.x > bouttons_oui_non[0].x1) && (event.button.y < bouttons_oui_non[0].y2) && (event.button.y > bouttons_oui_non[0].y1))
                                                                                {
                                                                                    ajout_poly_ensemble(&Tete_Ensemble, Tete_Poly3);
                                                                                    oui_non = SDL_FALSE;
                                                                                }
                                                                                if ((event.button.x < bouttons_oui_non[1].x2) && (event.button.x > bouttons_oui_non[1].x1) && (event.button.y < bouttons_oui_non[1].y2) && (event.button.y > bouttons_oui_non[1].y1))
                                                                                {
                                                                                    oui_non = SDL_FALSE;
                                                                                }
                                                                            }
                                                                        }
                                                                        continue;

                                                                    case SDL_QUIT:
                                                                        oui_non = SDL_FALSE;
                                                                        break;

                                                                    default:
                                                                        break;
                                                                    }
                                                                }
                                                                limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
                                                            }
                                                            sortir_tache1 = SDL_TRUE;
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                            continue;

                                        case SDL_QUIT:
                                            sortir_tache = SDL_TRUE;
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
                                }
                                //--------------------------------------------Fin de la soustraction du Polynome---------------------------------------------
                                sortir_tache = SDL_TRUE;
                                break;
                            }
                        }
                    }
                }
                continue;

            case SDL_QUIT:
                sortir_tache = SDL_TRUE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }

    free(bouttons);
    TTF_CloseFont(police2);
    TTF_CloseFont(police3);
    TTF_CloseFont(police4);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(page3);
    SDL_DestroyTexture(texture_p3);
    return;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
void multiplication_polynomes_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble)
{
    LLC_Poly *Poly = Tete_Ensemble;
    Monome *Tete_Poly1 = NULL, *Tete_Poly2 = NULL, *Tete_Poly3 = NULL;
    char chaine_char[30] = "";
    unsigned int temps_limite = 0, cpt = 0, nb_polys = 0;
    SDL_Texture *texture_p3 = NULL;
    SDL_Surface *page3 = NULL, *texte = NULL;
    TTF_Font *police2 = NULL, *police3 = NULL, *police4 = NULL;
    SDL_Rect position_textures, position_texte, cases;
    SDL_Event event;
    SDL_bool sortir_tache = SDL_FALSE;
    Rectangle btn_selected = {3, 0, 0, 0, 0, SDL_FALSE};
    Rectangle boutton_retour = {3, 100, 260, 632, 687, SDL_FALSE};
    Rectangle *bouttons = (Rectangle *)malloc(20 * sizeof(Rectangle));

    page3 = IMG_Load("assets/images/03.jpg");
    if (page3 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    police2 = TTF_OpenFont("assets/polices/times.ttf", 40);
    if (police2 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

    police3 = TTF_OpenFont("assets/polices/times.ttf", 25);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    police4 = TTF_OpenFont("assets/polices/times.ttf", 15);
    if (police4 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    position_texte.x = 80;
    position_texte.y = 80;
    texte = TTF_RenderText_Blended(police2, "Veuillez selectionner 02 polynome a multiplier :", BLANC);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);

    //-----------------------------------------------------------------------------------------------------
    position_texte.y = 100;
    if (Poly == NULL)
    {
        position_texte.x = 600;
        position_texte.y = 300;
        texte = TTF_RenderText_Blended(police3, "Cet ensemble est vide", NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
    }
    while (Poly != NULL)
    {
        Tete_Poly1 = Poly->adr1;
        position_texte.x = 280;
        position_texte.y += 50;
        bouttons[cpt].page = 3;
        bouttons[cpt].x1 = 230;
        bouttons[cpt].x2 = 260;
        bouttons[cpt].y1 = 150 + 50 * cpt;
        bouttons[cpt].y2 = 180 + 50 * cpt;
        bouttons[cpt].mouseInside = SDL_FALSE;
        cpt++;
        sprintf(chaine_char, "P%d = ", cpt);
        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
        position_texte.x += texte->w;
        if (Tete_Poly1 == NULL)
        {
            texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        else
        {
            while (suivant(Tete_Poly1) != NULL)
            {
                sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                if (position_texte.x + texte->w >= 1200)
                {
                    position_texte.x = 330;
                    position_texte.y += 30;
                }
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                Tete_Poly1 = suivant(Tete_Poly1);
            }
            sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
            texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
            if (position_texte.x + texte->w >= 1230)
            {
                position_texte.x = 330;
                position_texte.y += 30;
            }
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            sprintf(chaine_char, "%d", Tete_Poly1->puiss);
            texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            texte = TTF_RenderText_Blended(police3, ")", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        Poly = Poly->adr2;
    }

    //-----------------------------------------------------------------------------------------------------
    position_textures.x = 0;
    position_textures.y = 0;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
    if (texture_p3 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
    SDL_RenderPresent(rendu);

    if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    cases.h = 30;
    cases.w = 30;
    for (int i = 0; i < cpt; i++)
    {
        cases.x = bouttons[i].x1;
        cases.y = bouttons[i].y1;
        if (SDL_RenderFillRect(rendu, &cases) != 0)
            SDL_ExitWithError("Impossible de dessiner les cases à cocher");
    }
    if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    SDL_RenderPresent(rendu);

    while (!sortir_tache)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        // Dessiner une bordure rouge si le boutton de retour est survolé
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
        if (currentlyInside != boutton_retour.mouseInside)
        {
            boutton_retour.mouseInside = currentlyInside;
            stateChanged = SDL_TRUE;
        }
        if (stateChanged)
        {
            // Copier la texture du départ (page 3)
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            // Dessiner les cases à cocher
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            if (nb_polys == 1)
            {
                dessiner_rectangle(rendu, btn_selected.x1, btn_selected.y1, 30, 30);
                SDL_RenderDrawLine(rendu, btn_selected.x1, btn_selected.y1, btn_selected.x2, btn_selected.y2);
                SDL_RenderDrawLine(rendu, btn_selected.x2, btn_selected.y1, btn_selected.x1, btn_selected.y2);
            }
            if (boutton_retour.mouseInside)
            {
                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
            }
            SDL_RenderPresent(rendu);
        }

        // Dessiner une bordure et une croix si les caches à cocher sont survolées
        stateChanged = SDL_FALSE;
        SDL_GetMouseState(&mouseX, &mouseY);
        for (int i = 0; i < cpt; ++i)
        {
            currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons[i]);
            if (currentlyInside != bouttons[i].mouseInside)
            {
                bouttons[i].mouseInside = currentlyInside;
                stateChanged = SDL_TRUE;
            }
        }
        if (stateChanged)
        {
            // Copier la texture du départ (page 3)
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            // Dessiner les cases à cocher
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            if (nb_polys == 1)
            {
                dessiner_rectangle(rendu, btn_selected.x1, btn_selected.y1, 30, 30);
                SDL_RenderDrawLine(rendu, btn_selected.x1, btn_selected.y1, btn_selected.x2, btn_selected.y2);
                SDL_RenderDrawLine(rendu, btn_selected.x2, btn_selected.y1, btn_selected.x1, btn_selected.y2);
            }
            for (int i = 0; i < cpt; ++i)
            {
                if (bouttons[i].mouseInside)
                {
                    dessiner_rectangle(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2 - bouttons[i].x1, bouttons[i].y2 - bouttons[i].y1);
                    SDL_RenderDrawLine(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2, bouttons[i].y2);
                    SDL_RenderDrawLine(rendu, bouttons[i].x2, bouttons[i].y1, bouttons[i].x1, bouttons[i].y2);
                }
            }
            SDL_RenderPresent(rendu);
        }

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                        sortir_tache = SDL_TRUE;
                    else
                    {
                        for (int i = 0; i < cpt; i++)
                        {
                            if (nb_polys == 0)
                            {
                                if ((event.button.x < bouttons[i].x2) && (event.button.x > bouttons[i].x1) && (event.button.y < bouttons[i].y2) && (event.button.y > bouttons[i].y1))
                                {
                                    Tete_Poly1 = adr_maillon_ens(Tete_Ensemble, i + 1)->adr1;
                                    btn_selected = bouttons[i];
                                    nb_polys = 1;
                                }
                            }
                            else // nb_polys == 1 (nombre de polyonomes selectionnés : 0 ou 1, si 2 faire l'addition)
                            {
                                if ((event.button.x < bouttons[i].x2) && (event.button.x > bouttons[i].x1) && (event.button.y < bouttons[i].y2) && (event.button.y > bouttons[i].y1))
                                {
                                    //-----------------------------------------------Addition des polynomes------------------------------------------------------
                                    Tete_Poly2 = adr_maillon_ens(Tete_Ensemble, i + 1)->adr1;
                                    Tete_Poly3 = mult_poly(Tete_Poly1, Tete_Poly2);
                                    Tete_Poly1 = Tete_Poly3;
                                    page3 = IMG_Load("assets/images/03.jpg");
                                    if (page3 == NULL)
                                        SDL_ExitWithError("Impossible de charger l'image");
                                    position_texte.x = 80;
                                    position_texte.y = 80;
                                    texte = TTF_RenderText_Blended(police2, "Voici le resultat :", BLANC);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                    position_texte.x = 280;
                                    position_texte.y += 50;
                                    texte = TTF_RenderText_Blended(police3, "P = ", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    position_texte.x += texte->w;
                                    if (Tete_Poly1 == NULL)
                                    {
                                        texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    }
                                    else
                                    {
                                        while (suivant(Tete_Poly1) != NULL)
                                        {
                                            sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                            texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                            if (position_texte.x + texte->w >= 1200)
                                            {
                                                position_texte.x = 330;
                                                position_texte.y += 30;
                                            }
                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                            position_texte.x += texte->w;
                                            sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                            texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                            position_texte.x += texte->w;
                                            texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                            position_texte.x += texte->w;
                                            Tete_Poly1 = suivant(Tete_Poly1);
                                        }
                                        sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                        if (position_texte.x + texte->w >= 1230)
                                        {
                                            position_texte.x = 330;
                                            position_texte.y += 30;
                                        }
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                        texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        texte = TTF_RenderText_Blended(police3, ")", NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    }
                                    position_texte.x = 500;
                                    position_texte.y += 200;
                                    texte = TTF_RenderText_Blended(police3, " Avant de quitter la page :", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                    position_texte.y += 50;
                                    texte = TTF_RenderText_Blended(police3, " Voulez vous ajouter ce resultat a l'ensemble ?", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                    position_texte.x = 650;
                                    position_texte.y += 50;
                                    texte = TTF_RenderText_Blended(police3, "OUI", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                    position_texte.x = 850;
                                    texte = TTF_RenderText_Blended(police3, "NON", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                    position_textures.x = 0;
                                    position_textures.y = 0;
                                    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
                                    if (texture_p3 == NULL)
                                        SDL_ExitWithError("Impossible de créer la texture");
                                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                    SDL_RenderPresent(rendu);

                                    Rectangle bouttons_oui_non[] = {
                                        {3, 600, 630, position_texte.y, position_texte.y + 30, SDL_FALSE},
                                        {3, 800, 830, position_texte.y, position_texte.y + 30, SDL_FALSE}};
                                    if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                        SDL_ExitWithError("Impossible de changer la couleuur du rendu");
                                    cases.x = bouttons_oui_non[0].x1;
                                    cases.y = bouttons_oui_non[0].y1;
                                    cases.h = 30;
                                    cases.w = 30;
                                    if (SDL_RenderFillRect(rendu, &cases) != 0)
                                        SDL_ExitWithError("Impossible de dessiner les cases");
                                    cases.x = bouttons_oui_non[1].x1;
                                    cases.y = bouttons_oui_non[1].y1;
                                    if (SDL_RenderFillRect(rendu, &cases) != 0)
                                        SDL_ExitWithError("Impossible de dessiner les cases");
                                    if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                    SDL_RenderPresent(rendu);

                                    SDL_bool oui_non = SDL_TRUE;
                                    while (oui_non)
                                    {
                                        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

                                        // Dessiner une bordure rouge si le boutton de retour est survolé
                                        int mouseX, mouseY;
                                        SDL_GetMouseState(&mouseX, &mouseY);
                                        SDL_bool stateChanged = SDL_FALSE;
                                        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
                                        if (currentlyInside != boutton_retour.mouseInside)
                                        {
                                            boutton_retour.mouseInside = currentlyInside;
                                            stateChanged = SDL_TRUE;
                                        }
                                        if (stateChanged)
                                        {
                                            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                            if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                            cases.h = 30;
                                            cases.w = 30;
                                            for (int i = 0; i < 2; i++)
                                            {
                                                cases.x = bouttons_oui_non[i].x1;
                                                cases.y = bouttons_oui_non[i].y1;
                                                if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                            }
                                            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                            if (boutton_retour.mouseInside)
                                            {
                                                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
                                            }
                                            SDL_RenderPresent(rendu);
                                        }

                                        // Dessiner une bordure rouge si un des bouttons (oui et non) est survolé
                                        stateChanged = SDL_FALSE;
                                        SDL_GetMouseState(&mouseX, &mouseY);
                                        for (int i = 0; i < 2; i++)
                                        {
                                            currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons_oui_non[i]);
                                            if (currentlyInside != bouttons_oui_non[i].mouseInside)
                                            {
                                                bouttons_oui_non[i].mouseInside = currentlyInside;
                                                stateChanged = SDL_TRUE;
                                            }
                                        }
                                        if (stateChanged)
                                        {
                                            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                            if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                            cases.h = 30;
                                            cases.w = 30;
                                            for (int i = 0; i < 2; i++)
                                            {
                                                cases.x = bouttons_oui_non[i].x1;
                                                cases.y = bouttons_oui_non[i].y1;
                                                if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                            }
                                            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

                                            for (int i = 0; i < 2; ++i)
                                            {
                                                if (bouttons_oui_non[i].mouseInside)
                                                {
                                                    dessiner_rectangle(rendu, bouttons_oui_non[i].x1, bouttons_oui_non[i].y1, 30, 30);
                                                    SDL_RenderDrawLine(rendu, bouttons_oui_non[i].x2, bouttons_oui_non[i].y1, bouttons_oui_non[i].x1, bouttons_oui_non[i].y2);
                                                    SDL_RenderDrawLine(rendu, bouttons_oui_non[i].x1, bouttons_oui_non[i].y1, bouttons_oui_non[i].x2, bouttons_oui_non[i].y2);
                                                }
                                            }
                                            SDL_RenderPresent(rendu);
                                        }

                                        while (SDL_PollEvent(&event))
                                        {
                                            switch (event.type)
                                            {
                                            case SDL_MOUSEBUTTONDOWN:
                                                if (event.button.button == SDL_BUTTON_LEFT)
                                                {
                                                    if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                                                        oui_non = SDL_FALSE;
                                                    else
                                                    {
                                                        if ((event.button.x < bouttons_oui_non[0].x2) && (event.button.x > bouttons_oui_non[0].x1) && (event.button.y < bouttons_oui_non[0].y2) && (event.button.y > bouttons_oui_non[0].y1))
                                                        {
                                                            ajout_poly_ensemble(&Tete_Ensemble, Tete_Poly3);
                                                            oui_non = SDL_FALSE;
                                                        }
                                                        if ((event.button.x < bouttons_oui_non[1].x2) && (event.button.x > bouttons_oui_non[1].x1) && (event.button.y < bouttons_oui_non[1].y2) && (event.button.y > bouttons_oui_non[1].y1))
                                                        {
                                                            oui_non = SDL_FALSE;
                                                        }
                                                    }
                                                }
                                                continue;

                                            case SDL_QUIT:
                                                oui_non = SDL_FALSE;
                                                break;

                                            default:
                                                break;
                                            }
                                        }
                                        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
                                    }
                                    //------------------------------------------------Fin de l'addition des Polynomes-------------------------------------------------
                                    sortir_tache = SDL_TRUE;
                                    break;
                                }
                            }
                        }
                    }
                }
                continue;

            case SDL_QUIT:
                sortir_tache = SDL_TRUE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }

    free(bouttons);
    TTF_CloseFont(police2);
    TTF_CloseFont(police3);
    TTF_CloseFont(police4);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(page3);
    SDL_DestroyTexture(texture_p3);
    return;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
void division_polynomes_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble)
{
    LLC_Poly *Poly = Tete_Ensemble, *Poly2 = Tete_Ensemble;
    Monome *Tete_Poly1 = NULL, *Tete_Poly2 = NULL, *Tete_quotient = NULL, *Tete_reste = NULL;
    char chaine_char[30] = "";
    unsigned int temps_limite = 0, cpt = 0;
    SDL_Texture *texture_p3 = NULL;
    SDL_Surface *page3 = NULL, *texte = NULL;
    TTF_Font *police2 = NULL, *police3 = NULL, *police4 = NULL;
    SDL_Rect position_textures, position_texte, cases;
    SDL_Event event;
    SDL_bool sortir_tache = SDL_FALSE, sortir_tache1 = SDL_FALSE;
    Rectangle btn_selected = {3, 0, 0, 0, 0, SDL_FALSE};
    Rectangle boutton_retour = {3, 100, 260, 632, 687, SDL_FALSE};
    Rectangle *bouttons = (Rectangle *)malloc(20 * sizeof(Rectangle));

    page3 = IMG_Load("assets/images/03.jpg");
    if (page3 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    police2 = TTF_OpenFont("assets/polices/times.ttf", 40);
    if (police2 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

    police3 = TTF_OpenFont("assets/polices/times.ttf", 25);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    police4 = TTF_OpenFont("assets/polices/times.ttf", 15);
    if (police4 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    position_texte.x = 80;
    position_texte.y = 80;
    texte = TTF_RenderText_Blended(police2, "La division (P1/P2), veuillez selectionner P1 :", BLANC);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);

    //-----------------------------------------------------------------------------------------------------
    position_texte.y = 100;
    if (Poly == NULL)
    {
        position_texte.x = 600;
        position_texte.y = 300;
        texte = TTF_RenderText_Blended(police3, "Cet ensemble est vide", NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
    }
    while (Poly != NULL)
    {
        Tete_Poly1 = Poly->adr1;
        position_texte.x = 280;
        position_texte.y += 50;
        bouttons[cpt].page = 3;
        bouttons[cpt].x1 = 230;
        bouttons[cpt].x2 = 260;
        bouttons[cpt].y1 = 150 + 50 * cpt;
        bouttons[cpt].y2 = 180 + 50 * cpt;
        bouttons[cpt].mouseInside = SDL_FALSE;
        cpt++;
        sprintf(chaine_char, "P%d = ", cpt);
        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
        position_texte.x += texte->w;
        if (Tete_Poly1 == NULL)
        {
            texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        else
        {
            while (suivant(Tete_Poly1) != NULL)
            {
                sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                if (position_texte.x + texte->w >= 1200)
                {
                    position_texte.x = 330;
                    position_texte.y += 30;
                }
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                Tete_Poly1 = suivant(Tete_Poly1);
            }
            sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
            texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
            if (position_texte.x + texte->w >= 1230)
            {
                position_texte.x = 330;
                position_texte.y += 30;
            }
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            sprintf(chaine_char, "%d", Tete_Poly1->puiss);
            texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            texte = TTF_RenderText_Blended(police3, ")", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        Poly = Poly->adr2;
    }

    //-----------------------------------------------------------------------------------------------------
    position_textures.x = 0;
    position_textures.y = 0;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
    if (texture_p3 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");

    if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    cases.h = 30;
    cases.w = 30;
    for (int i = 0; i < cpt; i++)
    {
        cases.x = bouttons[i].x1;
        cases.y = bouttons[i].y1;
        if (SDL_RenderFillRect(rendu, &cases) != 0)
            SDL_ExitWithError("Impossible de dessiner les cases à cocher");
    }
    if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    SDL_RenderPresent(rendu);

    SDL_bool oui_non = SDL_TRUE;
    while (!sortir_tache)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        // Dessiner une bordure rouge si le boutton de retour est survolé
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
        if (currentlyInside != boutton_retour.mouseInside)
        {
            boutton_retour.mouseInside = currentlyInside;
            stateChanged = SDL_TRUE;
        }
        if (stateChanged)
        {
            // Copier la texture du départ (page 3)
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            // Dessiner les cases à cocher
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            if (boutton_retour.mouseInside)
            {
                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
            }
            SDL_RenderPresent(rendu);
        }

        // Dessiner une bordure et une croix si les caches à cocher sont survolées
        stateChanged = SDL_FALSE;
        SDL_GetMouseState(&mouseX, &mouseY);
        for (int i = 0; i < cpt; ++i)
        {
            currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons[i]);
            if (currentlyInside != bouttons[i].mouseInside)
            {
                bouttons[i].mouseInside = currentlyInside;
                stateChanged = SDL_TRUE;
            }
        }
        if (stateChanged)
        {
            // Copier la texture du départ (page 3)
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            // Dessiner les cases à cocher
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            for (int i = 0; i < cpt; ++i)
            {
                if (bouttons[i].mouseInside)
                {
                    dessiner_rectangle(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2 - bouttons[i].x1, bouttons[i].y2 - bouttons[i].y1);
                    SDL_RenderDrawLine(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2, bouttons[i].y2);
                    SDL_RenderDrawLine(rendu, bouttons[i].x2, bouttons[i].y1, bouttons[i].x1, bouttons[i].y2);
                }
            }
            SDL_RenderPresent(rendu);
        }

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                        sortir_tache = SDL_TRUE;
                    else
                    {
                        for (int i = 0; i < cpt; i++)
                        {
                            if ((event.button.x < bouttons[i].x2) && (event.button.x > bouttons[i].x1) && (event.button.y < bouttons[i].y2) && (event.button.y > bouttons[i].y1))
                            {
                                Tete_Poly2 = adr_maillon_ens(Tete_Ensemble, i + 1)->adr1;
                                btn_selected = bouttons[i];
                                //-----------------------------------------------Selection du 2eme polynome------------------------------------------------------
                                page3 = IMG_Load("assets/images/03.jpg");
                                if (page3 == NULL)
                                    SDL_ExitWithError("Impossible de charger l'image");

                                police2 = TTF_OpenFont("assets/polices/times.ttf", 40);
                                if (police2 == NULL)
                                    SDL_ExitWithError("Impossible de charger la police Times New Romain");
                                TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

                                police3 = TTF_OpenFont("assets/polices/times.ttf", 25);
                                if (police3 == NULL)
                                    SDL_ExitWithError("Impossible de charger la police Times New Romain");
                                TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

                                police4 = TTF_OpenFont("assets/polices/times.ttf", 15);
                                if (police4 == NULL)
                                    SDL_ExitWithError("Impossible de charger la police Times New Romain");
                                TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

                                position_texte.x = 80;
                                position_texte.y = 80;
                                texte = TTF_RenderText_Blended(police2, "La division (P1/P2), veuillez selectionner P2 :", BLANC);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                //-----------------------------------------------------------------------------------------------------
                                position_texte.y = 100;
                                Poly = Poly2;
                                cpt = 0;
                                if (Poly == NULL)
                                {
                                    position_texte.x = 600;
                                    position_texte.y = 300;
                                    texte = TTF_RenderText_Blended(police3, "Cet ensemble est vide", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                }
                                while (Poly != NULL)
                                {
                                    Tete_Poly1 = Poly->adr1;
                                    position_texte.x = 280;
                                    position_texte.y += 50;
                                    bouttons[cpt].page = 3;
                                    bouttons[cpt].x1 = 230;
                                    bouttons[cpt].x2 = 260;
                                    bouttons[cpt].y1 = 150 + 50 * cpt;
                                    bouttons[cpt].y2 = 180 + 50 * cpt;
                                    bouttons[cpt].mouseInside = SDL_FALSE;
                                    cpt++;
                                    sprintf(chaine_char, "P%d = ", cpt);
                                    texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    position_texte.x += texte->w;
                                    if (Tete_Poly1 == NULL)
                                    {
                                        texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    }
                                    else
                                    {
                                        while (suivant(Tete_Poly1) != NULL)
                                        {
                                            sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                            texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                            if (position_texte.x + texte->w >= 1200)
                                            {
                                                position_texte.x = 330;
                                                position_texte.y += 30;
                                            }
                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                            position_texte.x += texte->w;
                                            sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                            texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                            position_texte.x += texte->w;
                                            texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                            position_texte.x += texte->w;
                                            Tete_Poly1 = suivant(Tete_Poly1);
                                        }
                                        sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                        if (position_texte.x + texte->w >= 1230)
                                        {
                                            position_texte.x = 330;
                                            position_texte.y += 30;
                                        }
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                        texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        texte = TTF_RenderText_Blended(police3, ")", NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    }
                                    Poly = Poly->adr2;
                                }

                                //-----------------------------------------------------------------------------------------------------
                                position_textures.x = 0;
                                position_textures.y = 0;
                                texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
                                if (texture_p3 == NULL)
                                    SDL_ExitWithError("Impossible de créer la texture");
                                if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
                                    SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
                                if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                SDL_RenderPresent(rendu);

                                if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                                    SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                cases.h = 30;
                                cases.w = 30;
                                for (int i = 0; i < cpt; i++)
                                {
                                    cases.x = bouttons[i].x1;
                                    cases.y = bouttons[i].y1;
                                    if (SDL_RenderFillRect(rendu, &cases) != 0)
                                        SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                }
                                if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                    SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                SDL_RenderPresent(rendu);

                                while (!sortir_tache1)
                                {
                                    temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

                                    // Dessiner une bordure rouge si le boutton de retour est survolé
                                    int mouseX, mouseY;
                                    SDL_GetMouseState(&mouseX, &mouseY);
                                    SDL_bool stateChanged = SDL_FALSE;
                                    SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
                                    if (currentlyInside != boutton_retour.mouseInside)
                                    {
                                        boutton_retour.mouseInside = currentlyInside;
                                        stateChanged = SDL_TRUE;
                                    }
                                    if (stateChanged)
                                    {
                                        // Copier la texture du départ (page 3)
                                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                        // Dessiner les cases à cocher
                                        if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                        cases.h = 30;
                                        cases.w = 30;
                                        for (int i = 0; i < cpt; i++)
                                        {
                                            cases.x = bouttons[i].x1;
                                            cases.y = bouttons[i].y1;
                                            if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                        }
                                        if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");

                                        dessiner_rectangle(rendu, btn_selected.x1, btn_selected.y1, 30, 30);
                                        SDL_RenderDrawLine(rendu, btn_selected.x1, btn_selected.y1, btn_selected.x2, btn_selected.y2);
                                        SDL_RenderDrawLine(rendu, btn_selected.x2, btn_selected.y1, btn_selected.x1, btn_selected.y2);
                                        if (boutton_retour.mouseInside)
                                        {
                                            dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
                                        }
                                        SDL_RenderPresent(rendu);
                                    }

                                    // Dessiner une bordure et une croix si les caches à cocher sont survolées
                                    stateChanged = SDL_FALSE;
                                    SDL_GetMouseState(&mouseX, &mouseY);
                                    for (int i = 0; i < cpt; ++i)
                                    {
                                        currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons[i]);
                                        if (currentlyInside != bouttons[i].mouseInside)
                                        {
                                            bouttons[i].mouseInside = currentlyInside;
                                            stateChanged = SDL_TRUE;
                                        }
                                    }
                                    if (stateChanged)
                                    {
                                        // Copier la texture du départ (page 3)
                                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                        // Dessiner les cases à cocher
                                        if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                        cases.h = 30;
                                        cases.w = 30;
                                        for (int i = 0; i < cpt; i++)
                                        {
                                            cases.x = bouttons[i].x1;
                                            cases.y = bouttons[i].y1;
                                            if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                        }
                                        if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");

                                        dessiner_rectangle(rendu, btn_selected.x1, btn_selected.y1, 30, 30);
                                        SDL_RenderDrawLine(rendu, btn_selected.x1, btn_selected.y1, btn_selected.x2, btn_selected.y2);
                                        SDL_RenderDrawLine(rendu, btn_selected.x2, btn_selected.y1, btn_selected.x1, btn_selected.y2);
                                        for (int i = 0; i < cpt; ++i)
                                        {
                                            if (bouttons[i].mouseInside)
                                            {
                                                dessiner_rectangle(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2 - bouttons[i].x1, bouttons[i].y2 - bouttons[i].y1);
                                                SDL_RenderDrawLine(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2, bouttons[i].y2);
                                                SDL_RenderDrawLine(rendu, bouttons[i].x2, bouttons[i].y1, bouttons[i].x1, bouttons[i].y2);
                                            }
                                        }
                                        SDL_RenderPresent(rendu);
                                    }

                                    while (SDL_PollEvent(&event))
                                    {
                                        switch (event.type)
                                        {
                                        case SDL_MOUSEBUTTONDOWN:
                                            if (event.button.button == SDL_BUTTON_LEFT)
                                            {
                                                if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                                                    sortir_tache = SDL_TRUE;
                                                else
                                                {
                                                    for (int i = 0; i < cpt; i++)
                                                    {
                                                        if ((event.button.x < bouttons[i].x2) && (event.button.x > bouttons[i].x1) && (event.button.y < bouttons[i].y2) && (event.button.y > bouttons[i].y1))
                                                        {
                                                            //-----------------------------------------------DIVISION des polynomes------------------------------------------------------
                                                            Tete_Poly1 = adr_maillon_ens(Tete_Ensemble, i + 1)->adr1;
                                                            if (div_poly(Tete_Poly2, Tete_Poly1, &Tete_quotient, &Tete_reste) == 0)
                                                            {
                                                                Tete_Poly1 = Tete_quotient;
                                                                page3 = IMG_Load("assets/images/03.jpg");
                                                                if (page3 == NULL)
                                                                    SDL_ExitWithError("Impossible de charger l'image");
                                                                position_texte.x = 80;
                                                                position_texte.y = 80;
                                                                texte = TTF_RenderText_Blended(police2, "Voici les resultats :", BLANC);
                                                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                                                position_texte.x = 280;
                                                                position_texte.y += 50;
                                                                texte = TTF_RenderText_Blended(police3, "Quotient = ", NOIR);
                                                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                position_texte.x += texte->w;
                                                                if (Tete_Poly1 == NULL)
                                                                {
                                                                    texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
                                                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                }
                                                                else
                                                                {
                                                                    while (suivant(Tete_Poly1) != NULL)
                                                                    {
                                                                        sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                                                        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                                                        if (position_texte.x + texte->w >= 1200)
                                                                        {
                                                                            position_texte.x = 330;
                                                                            position_texte.y += 30;
                                                                        }
                                                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                        position_texte.x += texte->w;
                                                                        sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                                                        texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                        position_texte.x += texte->w;
                                                                        texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                                                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                        position_texte.x += texte->w;
                                                                        Tete_Poly1 = suivant(Tete_Poly1);
                                                                    }
                                                                    sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                                                    texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                                                    if (position_texte.x + texte->w >= 1230)
                                                                    {
                                                                        position_texte.x = 330;
                                                                        position_texte.y += 30;
                                                                    }
                                                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                    position_texte.x += texte->w;
                                                                    sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                                                    texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                    position_texte.x += texte->w;
                                                                    texte = TTF_RenderText_Blended(police3, ")", NOIR);
                                                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                }
                                                                Tete_Poly1 = Tete_reste;
                                                                position_texte.x = 280;
                                                                position_texte.y += 50;
                                                                texte = TTF_RenderText_Blended(police3, "Reste = ", NOIR);
                                                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                position_texte.x += texte->w;
                                                                if (Tete_Poly1 == NULL)
                                                                {
                                                                    texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
                                                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                }
                                                                else
                                                                {
                                                                    while (suivant(Tete_Poly1) != NULL)
                                                                    {
                                                                        sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                                                        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                                                        if (position_texte.x + texte->w >= 1200)
                                                                        {
                                                                            position_texte.x = 330;
                                                                            position_texte.y += 30;
                                                                        }
                                                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                        position_texte.x += texte->w;
                                                                        sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                                                        texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                        position_texte.x += texte->w;
                                                                        texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                                                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                        position_texte.x += texte->w;
                                                                        Tete_Poly1 = suivant(Tete_Poly1);
                                                                    }
                                                                    sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                                                    texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                                                    if (position_texte.x + texte->w >= 1230)
                                                                    {
                                                                        position_texte.x = 330;
                                                                        position_texte.y += 30;
                                                                    }
                                                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                    position_texte.x += texte->w;
                                                                    sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                                                    texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                    position_texte.x += texte->w;
                                                                    texte = TTF_RenderText_Blended(police3, ")", NOIR);
                                                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                                                }

                                                                position_texte.x = 500;
                                                                position_texte.y += 200;
                                                                texte = TTF_RenderText_Blended(police3, " Avant de quitter la page :", NOIR);
                                                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                                                position_texte.y += 50;
                                                                texte = TTF_RenderText_Blended(police3, " Voulez vous ajouter ces resultats a l'ensemble ?", NOIR);
                                                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                                                position_texte.x = 650;
                                                                position_texte.y += 50;
                                                                texte = TTF_RenderText_Blended(police3, "OUI", NOIR);
                                                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                                                position_texte.x = 850;
                                                                texte = TTF_RenderText_Blended(police3, "NON", NOIR);
                                                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                                                position_textures.x = 0;
                                                                position_textures.y = 0;
                                                                texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
                                                                if (texture_p3 == NULL)
                                                                    SDL_ExitWithError("Impossible de créer la texture");
                                                                if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                                                SDL_RenderPresent(rendu);

                                                                Rectangle bouttons_oui_non[] = {
                                                                    {3, 600, 630, position_texte.y, position_texte.y + 30, SDL_FALSE},
                                                                    {3, 800, 830, position_texte.y, position_texte.y + 30, SDL_FALSE}};
                                                                if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                                                    SDL_ExitWithError("Impossible de changer la couleuur du rendu");
                                                                cases.x = bouttons_oui_non[0].x1;
                                                                cases.y = bouttons_oui_non[0].y1;
                                                                cases.h = 30;
                                                                cases.w = 30;
                                                                if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                                    SDL_ExitWithError("Impossible de dessiner les cases");
                                                                cases.x = bouttons_oui_non[1].x1;
                                                                cases.y = bouttons_oui_non[1].y1;
                                                                if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                                    SDL_ExitWithError("Impossible de dessiner les cases");
                                                                if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                                                    SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                                                SDL_RenderPresent(rendu);

                                                                while (oui_non)
                                                                {
                                                                    temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

                                                                    // Dessiner une bordure rouge si le boutton de retour est survolé
                                                                    int mouseX, mouseY;
                                                                    SDL_GetMouseState(&mouseX, &mouseY);
                                                                    SDL_bool stateChanged = SDL_FALSE;
                                                                    SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
                                                                    if (currentlyInside != boutton_retour.mouseInside)
                                                                    {
                                                                        boutton_retour.mouseInside = currentlyInside;
                                                                        stateChanged = SDL_TRUE;
                                                                    }
                                                                    if (stateChanged)
                                                                    {
                                                                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                                                        if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                                                        cases.h = 30;
                                                                        cases.w = 30;
                                                                        for (int i = 0; i < 2; i++)
                                                                        {
                                                                            cases.x = bouttons_oui_non[i].x1;
                                                                            cases.y = bouttons_oui_non[i].y1;
                                                                            if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                                                SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                                                        }
                                                                        if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                                                        if (boutton_retour.mouseInside)
                                                                        {
                                                                            dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
                                                                        }
                                                                        SDL_RenderPresent(rendu);
                                                                    }

                                                                    // Dessiner une bordure rouge si un des bouttons (oui et non) est survolé
                                                                    stateChanged = SDL_FALSE;
                                                                    SDL_GetMouseState(&mouseX, &mouseY);
                                                                    for (int i = 0; i < 2; i++)
                                                                    {
                                                                        currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons_oui_non[i]);
                                                                        if (currentlyInside != bouttons_oui_non[i].mouseInside)
                                                                        {
                                                                            bouttons_oui_non[i].mouseInside = currentlyInside;
                                                                            stateChanged = SDL_TRUE;
                                                                        }
                                                                    }
                                                                    if (stateChanged)
                                                                    {
                                                                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                                                        if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                                                        cases.h = 30;
                                                                        cases.w = 30;
                                                                        for (int i = 0; i < 2; i++)
                                                                        {
                                                                            cases.x = bouttons_oui_non[i].x1;
                                                                            cases.y = bouttons_oui_non[i].y1;
                                                                            if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                                                SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                                                        }
                                                                        if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");

                                                                        for (int i = 0; i < 2; ++i)
                                                                        {
                                                                            if (bouttons_oui_non[i].mouseInside)
                                                                            {
                                                                                dessiner_rectangle(rendu, bouttons_oui_non[i].x1, bouttons_oui_non[i].y1, 30, 30);
                                                                                SDL_RenderDrawLine(rendu, bouttons_oui_non[i].x2, bouttons_oui_non[i].y1, bouttons_oui_non[i].x1, bouttons_oui_non[i].y2);
                                                                                SDL_RenderDrawLine(rendu, bouttons_oui_non[i].x1, bouttons_oui_non[i].y1, bouttons_oui_non[i].x2, bouttons_oui_non[i].y2);
                                                                            }
                                                                        }
                                                                        SDL_RenderPresent(rendu);
                                                                    }

                                                                    while (SDL_PollEvent(&event))
                                                                    {
                                                                        switch (event.type)
                                                                        {
                                                                        case SDL_MOUSEBUTTONDOWN:
                                                                            if (event.button.button == SDL_BUTTON_LEFT)
                                                                            {
                                                                                if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                                                                                    oui_non = SDL_FALSE;
                                                                                else
                                                                                {
                                                                                    if ((event.button.x < bouttons_oui_non[0].x2) && (event.button.x > bouttons_oui_non[0].x1) && (event.button.y < bouttons_oui_non[0].y2) && (event.button.y > bouttons_oui_non[0].y1))
                                                                                    {
                                                                                        ajout_poly_ensemble(&Tete_Ensemble, Tete_quotient);
                                                                                        ajout_poly_ensemble(&Tete_Ensemble, Tete_reste);
                                                                                        oui_non = SDL_FALSE;
                                                                                    }
                                                                                    if ((event.button.x < bouttons_oui_non[1].x2) && (event.button.x > bouttons_oui_non[1].x1) && (event.button.y < bouttons_oui_non[1].y2) && (event.button.y > bouttons_oui_non[1].y1))
                                                                                    {
                                                                                        oui_non = SDL_FALSE;
                                                                                    }
                                                                                }
                                                                            }
                                                                            continue;

                                                                        case SDL_QUIT:
                                                                            oui_non = SDL_FALSE;
                                                                            break;

                                                                        default:
                                                                            break;
                                                                        }
                                                                    }
                                                                    limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
                                                                }
                                                                sortir_tache1 = SDL_TRUE;
                                                                break;
                                                            }
                                                            else // Division sur 0 !!!
                                                            {
                                                                page3 = IMG_Load("assets/images/03.jpg");
                                                                if (page3 == NULL)
                                                                    SDL_ExitWithError("Impossible de charger l'image");
                                                                position_texte.x = 500;
                                                                position_texte.y = 400;
                                                                texte = TTF_RenderText_Blended(police2, "ERREUR : Division sur 0 !", ORANGE);
                                                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                                                texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
                                                                if (texture_p3 == NULL)
                                                                    SDL_ExitWithError("Impossible de créer la texture");
                                                                if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                                                SDL_RenderPresent(rendu);

                                                                oui_non = SDL_TRUE;
                                                                while (oui_non)
                                                                {
                                                                    temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

                                                                    // Dessiner une bordure rouge si le boutton de retour est survolé
                                                                    int mouseX, mouseY;
                                                                    SDL_GetMouseState(&mouseX, &mouseY);
                                                                    SDL_bool stateChanged = SDL_FALSE;
                                                                    SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
                                                                    if (currentlyInside != boutton_retour.mouseInside)
                                                                    {
                                                                        boutton_retour.mouseInside = currentlyInside;
                                                                        stateChanged = SDL_TRUE;
                                                                    }
                                                                    if (stateChanged)
                                                                    {
                                                                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");

                                                                        if (boutton_retour.mouseInside)
                                                                        {
                                                                            dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
                                                                        }
                                                                        SDL_RenderPresent(rendu);
                                                                    }

                                                                    while (SDL_PollEvent(&event))
                                                                    {
                                                                        switch (event.type)
                                                                        {
                                                                        case SDL_MOUSEBUTTONDOWN:
                                                                            if (event.button.button == SDL_BUTTON_LEFT)
                                                                            {
                                                                                if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                                                                                    oui_non = SDL_FALSE;
                                                                            }
                                                                            continue;

                                                                        case SDL_QUIT:
                                                                            oui_non = SDL_FALSE;
                                                                            break;

                                                                        default:
                                                                            break;
                                                                        }
                                                                    }
                                                                    limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
                                                                }
                                                                sortir_tache1 = SDL_TRUE;
                                                                break;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            continue;

                                        case SDL_QUIT:
                                            sortir_tache = SDL_TRUE;
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
                                }
                                //--------------------------------------------Fin de la soustraction du Polynome---------------------------------------------
                                sortir_tache = SDL_TRUE;
                                break;
                            }
                        }
                    }
                }
                continue;

            case SDL_QUIT:
                sortir_tache = SDL_TRUE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }

    free(bouttons);
    TTF_CloseFont(police2);
    TTF_CloseFont(police3);
    TTF_CloseFont(police4);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(page3);
    SDL_DestroyTexture(texture_p3);
    return;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
void deriver_polynome_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble)
{
    LLC_Poly *Poly = Tete_Ensemble;
    Monome *Tete_Poly1 = NULL, *Tete_Poly2 = NULL;
    char chaine_char[30] = "";
    SDL_Texture *texture_p3 = NULL;
    SDL_Surface *page3 = NULL, *texte = NULL;
    TTF_Font *police2 = NULL, *police3 = NULL, *police4 = NULL;
    SDL_Rect position_textures, position_texte, cases;
    SDL_Event event;
    SDL_bool sortir_tache = SDL_FALSE;
    unsigned int temps_limite = 0, cpt = 0;
    Rectangle boutton_retour = {3, 100, 260, 632, 687, SDL_FALSE};
    Rectangle *bouttons = (Rectangle *)malloc(20 * sizeof(Rectangle));

    page3 = IMG_Load("assets/images/03.jpg");
    if (page3 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    police2 = TTF_OpenFont("assets/polices/times.ttf", 40);
    if (police2 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

    police3 = TTF_OpenFont("assets/polices/times.ttf", 25);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    police4 = TTF_OpenFont("assets/polices/times.ttf", 15);
    if (police4 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    position_texte.x = 80;
    position_texte.y = 80;
    texte = TTF_RenderText_Blended(police2, "Veuillez choisir le polynome a deriver :", BLANC);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);

    //-----------------------------------------------------------------------------------------------------
    position_texte.y = 100;
    if (Poly == NULL)
    {
        position_texte.x = 600;
        position_texte.y = 300;
        texte = TTF_RenderText_Blended(police3, "Cet ensemble est vide", NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
    }
    while (Poly != NULL)
    {
        Tete_Poly1 = Poly->adr1;
        position_texte.x = 280;
        position_texte.y += 50;
        bouttons[cpt].page = 3;
        bouttons[cpt].x1 = 230;
        bouttons[cpt].x2 = 260;
        bouttons[cpt].y1 = 150 + 50 * cpt;
        bouttons[cpt].y2 = 180 + 50 * cpt;
        bouttons[cpt].mouseInside = SDL_FALSE;
        cpt++;
        sprintf(chaine_char, "P%d = ", cpt);
        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
        position_texte.x += texte->w;
        if (Tete_Poly1 == NULL)
        {
            texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        else
        {
            while (suivant(Tete_Poly1) != NULL)
            {
                sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                if (position_texte.x + texte->w >= 1200)
                {
                    position_texte.x = 330;
                    position_texte.y += 30;
                }
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                Tete_Poly1 = suivant(Tete_Poly1);
            }
            sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
            texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
            if (position_texte.x + texte->w >= 1230)
            {
                position_texte.x = 330;
                position_texte.y += 30;
            }
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            sprintf(chaine_char, "%d", Tete_Poly1->puiss);
            texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            texte = TTF_RenderText_Blended(police3, ")", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        Poly = Poly->adr2;
    }

    //-----------------------------------------------------------------------------------------------------
    position_textures.x = 0;
    position_textures.y = 0;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
    if (texture_p3 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");

    if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    cases.h = 30;
    cases.w = 30;
    for (int i = 0; i < cpt; i++)
    {
        cases.x = bouttons[i].x1;
        cases.y = bouttons[i].y1;
        if (SDL_RenderFillRect(rendu, &cases) != 0)
            SDL_ExitWithError("Impossible de dessiner les cases à cocher");
    }
    if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    SDL_RenderPresent(rendu);

    while (!sortir_tache)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        // Dessiner une bordure rouge si le boutton de retour est survolé
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
        if (currentlyInside != boutton_retour.mouseInside)
        {
            boutton_retour.mouseInside = currentlyInside;
            stateChanged = SDL_TRUE;
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            if (boutton_retour.mouseInside)
            {
                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
            }
            SDL_RenderPresent(rendu);
        }

        // Dessiner une bordure et une croix si les caches à cocher sont survolées
        stateChanged = SDL_FALSE;
        SDL_GetMouseState(&mouseX, &mouseY);
        for (int i = 0; i < cpt; ++i)
        {
            currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons[i]);
            if (currentlyInside != bouttons[i].mouseInside)
            {
                bouttons[i].mouseInside = currentlyInside;
                stateChanged = SDL_TRUE;
            }
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            for (int i = 0; i < cpt; ++i)
            {
                if (bouttons[i].mouseInside)
                {
                    dessiner_rectangle(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2 - bouttons[i].x1, bouttons[i].y2 - bouttons[i].y1);
                    SDL_RenderDrawLine(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2, bouttons[i].y2);
                    SDL_RenderDrawLine(rendu, bouttons[i].x2, bouttons[i].y1, bouttons[i].x1, bouttons[i].y2);
                }
            }
            SDL_RenderPresent(rendu);
        }

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                        sortir_tache = SDL_TRUE;
                    else
                    {
                        for (int i = 0; i < cpt; i++)
                        {
                            if ((event.button.x < bouttons[i].x2) && (event.button.x > bouttons[i].x1) && (event.button.y < bouttons[i].y2) && (event.button.y > bouttons[i].y1))
                            {
                                //-----------------------------------------------Derivation du polynome------------------------------------------------------
                                Tete_Poly1 = derivee_poly(adr_maillon_ens(Tete_Ensemble, i + 1)->adr1);
                                Tete_Poly2 = Tete_Poly1;
                                page3 = IMG_Load("assets/images/03.jpg");
                                if (page3 == NULL)
                                    SDL_ExitWithError("Impossible de charger l'image");
                                position_texte.x = 80;
                                position_texte.y = 80;
                                texte = TTF_RenderText_Blended(police2, "Voici le resultat :", BLANC);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_texte.x = 280;
                                position_texte.y += 50;
                                texte = TTF_RenderText_Blended(police3, "P' = ", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                position_texte.x += texte->w;
                                if (Tete_Poly1 == NULL)
                                {
                                    texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                }
                                else
                                {
                                    while (suivant(Tete_Poly1) != NULL)
                                    {
                                        sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                        if (position_texte.x + texte->w >= 1200)
                                        {
                                            position_texte.x = 330;
                                            position_texte.y += 30;
                                        }
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                        texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        Tete_Poly1 = suivant(Tete_Poly1);
                                    }
                                    sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                    texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                    if (position_texte.x + texte->w >= 1230)
                                    {
                                        position_texte.x = 330;
                                        position_texte.y += 30;
                                    }
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    position_texte.x += texte->w;
                                    sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                    texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                    position_texte.x += texte->w;
                                    texte = TTF_RenderText_Blended(police3, ")", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                }
                                position_texte.x = 500;
                                position_texte.y += 200;
                                texte = TTF_RenderText_Blended(police3, " Avant de quitter la page :", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_texte.y += 50;
                                texte = TTF_RenderText_Blended(police3, " Voulez vous ajouter cette derivee a l'ensemble ?", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_texte.x = 650;
                                position_texte.y += 50;
                                texte = TTF_RenderText_Blended(police3, "OUI", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_texte.x = 850;
                                texte = TTF_RenderText_Blended(police3, "NON", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_textures.x = 0;
                                position_textures.y = 0;
                                texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
                                if (texture_p3 == NULL)
                                    SDL_ExitWithError("Impossible de créer la texture");
                                if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");

                                Rectangle bouttons_oui_non[] = {
                                    {3, 600, 630, position_texte.y, position_texte.y + 30, SDL_FALSE},
                                    {3, 800, 830, position_texte.y, position_texte.y + 30, SDL_FALSE}};
                                if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                    SDL_ExitWithError("Impossible de changer la couleuur du rendu");
                                cases.x = bouttons_oui_non[0].x1;
                                cases.y = bouttons_oui_non[0].y1;
                                cases.h = 30;
                                cases.w = 30;
                                if (SDL_RenderFillRect(rendu, &cases) != 0)
                                    SDL_ExitWithError("Impossible de dessiner les cases");
                                cases.x = bouttons_oui_non[1].x1;
                                cases.y = bouttons_oui_non[1].y1;
                                if (SDL_RenderFillRect(rendu, &cases) != 0)
                                    SDL_ExitWithError("Impossible de dessiner les cases");
                                if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                    SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                SDL_RenderPresent(rendu);

                                SDL_bool oui_non = SDL_TRUE;
                                while (oui_non)
                                {
                                    temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

                                    // Dessiner une bordure rouge si le boutton de retour est survolé
                                    int mouseX, mouseY;
                                    SDL_GetMouseState(&mouseX, &mouseY);
                                    SDL_bool stateChanged = SDL_FALSE;
                                    SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
                                    if (currentlyInside != boutton_retour.mouseInside)
                                    {
                                        boutton_retour.mouseInside = currentlyInside;
                                        stateChanged = SDL_TRUE;
                                    }
                                    if (stateChanged)
                                    {
                                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                        if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                        cases.h = 30;
                                        cases.w = 30;
                                        for (int i = 0; i < 2; i++)
                                        {
                                            cases.x = bouttons_oui_non[i].x1;
                                            cases.y = bouttons_oui_non[i].y1;
                                            if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                        }
                                        if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                        if (boutton_retour.mouseInside)
                                        {
                                            dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
                                        }
                                        SDL_RenderPresent(rendu);
                                    }

                                    // Dessiner une bordure rouge si un des bouttons (oui et non) est survolé
                                    stateChanged = SDL_FALSE;
                                    SDL_GetMouseState(&mouseX, &mouseY);
                                    for (int i = 0; i < 2; i++)
                                    {
                                        currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons_oui_non[i]);
                                        if (currentlyInside != bouttons_oui_non[i].mouseInside)
                                        {
                                            bouttons_oui_non[i].mouseInside = currentlyInside;
                                            stateChanged = SDL_TRUE;
                                        }
                                    }
                                    if (stateChanged)
                                    {
                                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                        if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                        cases.h = 30;
                                        cases.w = 30;
                                        for (int i = 0; i < 2; i++)
                                        {
                                            cases.x = bouttons_oui_non[i].x1;
                                            cases.y = bouttons_oui_non[i].y1;
                                            if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                        }
                                        if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");

                                        for (int i = 0; i < 2; ++i)
                                        {
                                            if (bouttons_oui_non[i].mouseInside)
                                            {
                                                dessiner_rectangle(rendu, bouttons_oui_non[i].x1, bouttons_oui_non[i].y1, 30, 30);
                                                SDL_RenderDrawLine(rendu, bouttons_oui_non[i].x2, bouttons_oui_non[i].y1, bouttons_oui_non[i].x1, bouttons_oui_non[i].y2);
                                                SDL_RenderDrawLine(rendu, bouttons_oui_non[i].x1, bouttons_oui_non[i].y1, bouttons_oui_non[i].x2, bouttons_oui_non[i].y2);
                                            }
                                        }
                                        SDL_RenderPresent(rendu);
                                    }

                                    while (SDL_PollEvent(&event))
                                    {
                                        switch (event.type)
                                        {
                                        case SDL_MOUSEBUTTONDOWN:
                                            if (event.button.button == SDL_BUTTON_LEFT)
                                            {
                                                if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                                                    oui_non = SDL_FALSE;
                                                else
                                                {
                                                    if ((event.button.x < bouttons_oui_non[0].x2) && (event.button.x > bouttons_oui_non[0].x1) && (event.button.y < bouttons_oui_non[0].y2) && (event.button.y > bouttons_oui_non[0].y1))
                                                    {
                                                        ajout_poly_ensemble(&Tete_Ensemble, Tete_Poly2);
                                                        oui_non = SDL_FALSE;
                                                    }
                                                    if ((event.button.x < bouttons_oui_non[1].x2) && (event.button.x > bouttons_oui_non[1].x1) && (event.button.y < bouttons_oui_non[1].y2) && (event.button.y > bouttons_oui_non[1].y1))
                                                    {
                                                        oui_non = SDL_FALSE;
                                                    }
                                                }
                                            }
                                            continue;

                                        case SDL_QUIT:
                                            oui_non = SDL_FALSE;
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
                                }
                                //--------------------------------------------Fin de la derivation du Polynome---------------------------------------------
                                sortir_tache = SDL_TRUE;
                                break;
                            }
                        }
                    }
                }
                continue;

            case SDL_QUIT:
                sortir_tache = SDL_TRUE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }

    free(bouttons);
    TTF_CloseFont(police2);
    TTF_CloseFont(police3);
    TTF_CloseFont(police4);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(page3);
    SDL_DestroyTexture(texture_p3);
    return;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
void integrer_polynome_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble)
{
    LLC_Poly *Poly = Tete_Ensemble;
    Monome *Tete_Poly1 = NULL, *Tete_Poly2 = NULL;
    char chaine_char[30] = "";
    SDL_Texture *texture_p3 = NULL;
    SDL_Surface *page3 = NULL, *texte = NULL;
    TTF_Font *police2 = NULL, *police3 = NULL, *police4 = NULL;
    SDL_Rect position_textures, position_texte, cases;
    SDL_Event event;
    SDL_bool sortir_tache = SDL_FALSE;
    unsigned int temps_limite = 0, cpt = 0;
    Rectangle boutton_retour = {3, 100, 260, 632, 687, SDL_FALSE};
    Rectangle *bouttons = (Rectangle *)malloc(20 * sizeof(Rectangle));

    page3 = IMG_Load("assets/images/03.jpg");
    if (page3 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    police2 = TTF_OpenFont("assets/polices/times.ttf", 40);
    if (police2 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

    police3 = TTF_OpenFont("assets/polices/times.ttf", 25);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    police4 = TTF_OpenFont("assets/polices/times.ttf", 15);
    if (police4 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    position_texte.x = 80;
    position_texte.y = 80;
    texte = TTF_RenderText_Blended(police2, "Veuillez choisir le polynome a integrer :", BLANC);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);

    //-----------------------------------------------------------------------------------------------------
    position_texte.y = 100;
    if (Poly == NULL)
    {
        position_texte.x = 600;
        position_texte.y = 300;
        texte = TTF_RenderText_Blended(police3, "Cet ensemble est vide", NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
    }
    while (Poly != NULL)
    {
        Tete_Poly1 = Poly->adr1;
        position_texte.x = 280;
        position_texte.y += 50;
        bouttons[cpt].page = 3;
        bouttons[cpt].x1 = 230;
        bouttons[cpt].x2 = 260;
        bouttons[cpt].y1 = 150 + 50 * cpt;
        bouttons[cpt].y2 = 180 + 50 * cpt;
        bouttons[cpt].mouseInside = SDL_FALSE;
        cpt++;
        sprintf(chaine_char, "P%d = ", cpt);
        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
        position_texte.x += texte->w;
        if (Tete_Poly1 == NULL)
        {
            texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        else
        {
            while (suivant(Tete_Poly1) != NULL)
            {
                sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                if (position_texte.x + texte->w >= 1200)
                {
                    position_texte.x = 330;
                    position_texte.y += 30;
                }
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                Tete_Poly1 = suivant(Tete_Poly1);
            }
            sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
            texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
            if (position_texte.x + texte->w >= 1230)
            {
                position_texte.x = 330;
                position_texte.y += 30;
            }
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            sprintf(chaine_char, "%d", Tete_Poly1->puiss);
            texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            texte = TTF_RenderText_Blended(police3, ")", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        Poly = Poly->adr2;
    }

    //-----------------------------------------------------------------------------------------------------
    position_textures.x = 0;
    position_textures.y = 0;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
    if (texture_p3 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");

    if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    cases.h = 30;
    cases.w = 30;
    for (int i = 0; i < cpt; i++)
    {
        cases.x = bouttons[i].x1;
        cases.y = bouttons[i].y1;
        if (SDL_RenderFillRect(rendu, &cases) != 0)
            SDL_ExitWithError("Impossible de dessiner les cases à cocher");
    }
    if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    SDL_RenderPresent(rendu);

    while (!sortir_tache)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        // Dessiner une bordure rouge si le boutton de retour est survolé
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
        if (currentlyInside != boutton_retour.mouseInside)
        {
            boutton_retour.mouseInside = currentlyInside;
            stateChanged = SDL_TRUE;
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            if (boutton_retour.mouseInside)
            {
                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
            }
            SDL_RenderPresent(rendu);
        }

        // Dessiner une bordure et une croix si les caches à cocher sont survolées
        stateChanged = SDL_FALSE;
        SDL_GetMouseState(&mouseX, &mouseY);
        for (int i = 0; i < cpt; ++i)
        {
            currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons[i]);
            if (currentlyInside != bouttons[i].mouseInside)
            {
                bouttons[i].mouseInside = currentlyInside;
                stateChanged = SDL_TRUE;
            }
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            for (int i = 0; i < cpt; ++i)
            {
                if (bouttons[i].mouseInside)
                {
                    dessiner_rectangle(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2 - bouttons[i].x1, bouttons[i].y2 - bouttons[i].y1);
                    SDL_RenderDrawLine(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2, bouttons[i].y2);
                    SDL_RenderDrawLine(rendu, bouttons[i].x2, bouttons[i].y1, bouttons[i].x1, bouttons[i].y2);
                }
            }
            SDL_RenderPresent(rendu);
        }

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                        sortir_tache = SDL_TRUE;
                    else
                    {
                        for (int i = 0; i < cpt; i++)
                        {
                            if ((event.button.x < bouttons[i].x2) && (event.button.x > bouttons[i].x1) && (event.button.y < bouttons[i].y2) && (event.button.y > bouttons[i].y1))
                            {
                                //-----------------------------------------------Intregration du polynome------------------------------------------------------
                                Tete_Poly1 = primitive_poly(adr_maillon_ens(Tete_Ensemble, i + 1)->adr1);
                                Tete_Poly2 = Tete_Poly1;
                                page3 = IMG_Load("assets/images/03.jpg");
                                if (page3 == NULL)
                                    SDL_ExitWithError("Impossible de charger l'image");
                                position_texte.x = 80;
                                position_texte.y = 80;
                                texte = TTF_RenderText_Blended(police2, "Voici le resultat :", BLANC);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_texte.x = 280;
                                position_texte.y += 50;
                                texte = TTF_RenderText_Blended(police3, "P  =  ", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                position_texte.x += texte->w;
                                if (Tete_Poly1 == NULL)
                                {
                                    texte = TTF_RenderText_Blended(police3, "C", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                }
                                else
                                {
                                    while (Tete_Poly1 != NULL)
                                    {
                                        sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                                        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                                        if (position_texte.x + texte->w >= 1200)
                                        {
                                            position_texte.x = 330;
                                            position_texte.y += 30;
                                        }
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                                        texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                                        SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                        position_texte.x += texte->w;
                                        Tete_Poly1 = suivant(Tete_Poly1);
                                    }
                                    texte = TTF_RenderText_Blended(police3, "C", NOIR);
                                    SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                }
                                position_texte.x = 500;
                                position_texte.y += 200;
                                texte = TTF_RenderText_Blended(police3, " Avant de quitter la page :", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_texte.y += 50;
                                texte = TTF_RenderText_Blended(police3, " Voulez vous ajouter ce primitif a l'ensemble ?", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_texte.x = 650;
                                position_texte.y += 50;
                                texte = TTF_RenderText_Blended(police3, "OUI", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_texte.x = 850;
                                texte = TTF_RenderText_Blended(police3, "NON", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_textures.x = 0;
                                position_textures.y = 0;
                                texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
                                if (texture_p3 == NULL)
                                    SDL_ExitWithError("Impossible de créer la texture");
                                if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");

                                Rectangle bouttons_oui_non[] = {
                                    {3, 600, 630, position_texte.y, position_texte.y + 30, SDL_FALSE},
                                    {3, 800, 830, position_texte.y, position_texte.y + 30, SDL_FALSE}};
                                if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                    SDL_ExitWithError("Impossible de changer la couleuur du rendu");
                                cases.x = bouttons_oui_non[0].x1;
                                cases.y = bouttons_oui_non[0].y1;
                                cases.h = 30;
                                cases.w = 30;
                                if (SDL_RenderFillRect(rendu, &cases) != 0)
                                    SDL_ExitWithError("Impossible de dessiner les cases");
                                cases.x = bouttons_oui_non[1].x1;
                                cases.y = bouttons_oui_non[1].y1;
                                if (SDL_RenderFillRect(rendu, &cases) != 0)
                                    SDL_ExitWithError("Impossible de dessiner les cases");
                                if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                    SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                SDL_RenderPresent(rendu);

                                SDL_bool oui_non = SDL_TRUE;
                                while (oui_non)
                                {
                                    temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

                                    // Dessiner une bordure rouge si le boutton de retour est survolé
                                    int mouseX, mouseY;
                                    SDL_GetMouseState(&mouseX, &mouseY);
                                    SDL_bool stateChanged = SDL_FALSE;
                                    SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
                                    if (currentlyInside != boutton_retour.mouseInside)
                                    {
                                        boutton_retour.mouseInside = currentlyInside;
                                        stateChanged = SDL_TRUE;
                                    }
                                    if (stateChanged)
                                    {
                                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                        if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                        cases.h = 30;
                                        cases.w = 30;
                                        for (int i = 0; i < 2; i++)
                                        {
                                            cases.x = bouttons_oui_non[i].x1;
                                            cases.y = bouttons_oui_non[i].y1;
                                            if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                        }
                                        if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                        if (boutton_retour.mouseInside)
                                        {
                                            dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
                                        }
                                        SDL_RenderPresent(rendu);
                                    }

                                    // Dessiner une bordure rouge si un des bouttons (oui et non) est survolé
                                    stateChanged = SDL_FALSE;
                                    SDL_GetMouseState(&mouseX, &mouseY);
                                    for (int i = 0; i < 2; i++)
                                    {
                                        currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons_oui_non[i]);
                                        if (currentlyInside != bouttons_oui_non[i].mouseInside)
                                        {
                                            bouttons_oui_non[i].mouseInside = currentlyInside;
                                            stateChanged = SDL_TRUE;
                                        }
                                    }
                                    if (stateChanged)
                                    {
                                        if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                        if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                        cases.h = 30;
                                        cases.w = 30;
                                        for (int i = 0; i < 2; i++)
                                        {
                                            cases.x = bouttons_oui_non[i].x1;
                                            cases.y = bouttons_oui_non[i].y1;
                                            if (SDL_RenderFillRect(rendu, &cases) != 0)
                                                SDL_ExitWithError("Impossible de dessiner les cases à cocher");
                                        }
                                        if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                            SDL_ExitWithError("Impossible de changer la couleur au Rendu");

                                        for (int i = 0; i < 2; ++i)
                                        {
                                            if (bouttons_oui_non[i].mouseInside)
                                            {
                                                dessiner_rectangle(rendu, bouttons_oui_non[i].x1, bouttons_oui_non[i].y1, 30, 30);
                                                SDL_RenderDrawLine(rendu, bouttons_oui_non[i].x2, bouttons_oui_non[i].y1, bouttons_oui_non[i].x1, bouttons_oui_non[i].y2);
                                                SDL_RenderDrawLine(rendu, bouttons_oui_non[i].x1, bouttons_oui_non[i].y1, bouttons_oui_non[i].x2, bouttons_oui_non[i].y2);
                                            }
                                        }
                                        SDL_RenderPresent(rendu);
                                    }

                                    while (SDL_PollEvent(&event))
                                    {
                                        switch (event.type)
                                        {
                                        case SDL_MOUSEBUTTONDOWN:
                                            if (event.button.button == SDL_BUTTON_LEFT)
                                            {
                                                if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                                                    oui_non = SDL_FALSE;
                                                else
                                                {
                                                    if ((event.button.x < bouttons_oui_non[0].x2) && (event.button.x > bouttons_oui_non[0].x1) && (event.button.y < bouttons_oui_non[0].y2) && (event.button.y > bouttons_oui_non[0].y1))
                                                    {
                                                        ajout_poly_ensemble(&Tete_Ensemble, Tete_Poly2);
                                                        oui_non = SDL_FALSE;
                                                    }
                                                    if ((event.button.x < bouttons_oui_non[1].x2) && (event.button.x > bouttons_oui_non[1].x1) && (event.button.y < bouttons_oui_non[1].y2) && (event.button.y > bouttons_oui_non[1].y1))
                                                    {
                                                        oui_non = SDL_FALSE;
                                                    }
                                                }
                                            }
                                            continue;

                                        case SDL_QUIT:
                                            oui_non = SDL_FALSE;
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
                                }
                                //--------------------------------------------Fin de l'integration du Polynome---------------------------------------------
                                sortir_tache = SDL_TRUE;
                                break;
                            }
                        }
                    }
                }
                continue;

            case SDL_QUIT:
                sortir_tache = SDL_TRUE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }

    free(bouttons);
    TTF_CloseFont(police2);
    TTF_CloseFont(police3);
    TTF_CloseFont(police4);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(page3);
    SDL_DestroyTexture(texture_p3);
    return;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
// Il servira a dessiner la courbe des polynomes durant le passage par mode graphique
void dessiner_courbe_2D(SDL_Renderer *rendu, LLC_Poly *Tete_Ensemble)
{
    LLC_Poly *Poly = Tete_Ensemble;
    Monome *Tete_Poly1 = NULL, *Tete_Poly2 = NULL;
    char chaine_char[30] = "";
    unsigned int temps_limite = 0, cpt = 0;
    SDL_Texture *texture_p3 = NULL;
    SDL_Surface *page3 = NULL, *texte = NULL;
    TTF_Font *police2 = NULL, *police3 = NULL, *police4 = NULL;
    SDL_Rect position_textures, position_texte, cases;
    SDL_Event event;
    SDL_bool sortir_tache = SDL_FALSE;
    Rectangle boutton_retour = {3, 100, 260, 632, 687, SDL_FALSE};
    Rectangle *bouttons = (Rectangle *)malloc(20 * sizeof(Rectangle));

    page3 = IMG_Load("assets/images/03.jpg");
    if (page3 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    police2 = TTF_OpenFont("assets/polices/times.ttf", 40);
    if (police2 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

    police3 = TTF_OpenFont("assets/polices/times.ttf", 25);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    police4 = TTF_OpenFont("assets/polices/times.ttf", 20);
    if (police4 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    position_texte.x = 80;
    position_texte.y = 80;
    texte = TTF_RenderText_Blended(police2, "Veuillez selectionner le polynome pour dessiner sa courbe :", BLANC);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);

    //-----------------------------------------------------------------------------------------------------
    position_texte.y = 100;
    if (Poly == NULL)
    {
        position_texte.x = 600;
        position_texte.y = 300;
        texte = TTF_RenderText_Blended(police3, "Cet ensemble est vide", NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
    }
    while (Poly != NULL)
    {
        Tete_Poly1 = Poly->adr1;
        position_texte.x = 280;
        position_texte.y += 50;
        bouttons[cpt].page = 3;
        bouttons[cpt].x1 = 230;
        bouttons[cpt].x2 = 260;
        bouttons[cpt].y1 = 150 + 50 * cpt;
        bouttons[cpt].y2 = 180 + 50 * cpt;
        bouttons[cpt].mouseInside = SDL_FALSE;
        cpt++;
        sprintf(chaine_char, "P%d = ", cpt);
        texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
        SDL_BlitSurface(texte, NULL, page3, &position_texte);
        position_texte.x += texte->w;
        if (Tete_Poly1 == NULL)
        {
            texte = TTF_RenderText_Blended(police3, "Ce polynome est vide (nul).", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        else
        {
            while (suivant(Tete_Poly1) != NULL)
            {
                sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
                texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
                if (position_texte.x + texte->w >= 1200)
                {
                    position_texte.x = 330;
                    position_texte.y += 30;
                }
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                sprintf(chaine_char, "%d", Tete_Poly1->puiss);
                texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                texte = TTF_RenderText_Blended(police3, ") + ", NOIR);
                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                position_texte.x += texte->w;
                Tete_Poly1 = suivant(Tete_Poly1);
            }
            sprintf(chaine_char, "(%.2f * X", Tete_Poly1->coef);
            texte = TTF_RenderText_Blended(police3, chaine_char, NOIR);
            if (position_texte.x + texte->w >= 1230)
            {
                position_texte.x = 330;
                position_texte.y += 30;
            }
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            sprintf(chaine_char, "%d", Tete_Poly1->puiss);
            texte = TTF_RenderText_Blended(police4, chaine_char, NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
            position_texte.x += texte->w;
            texte = TTF_RenderText_Blended(police3, ")", NOIR);
            SDL_BlitSurface(texte, NULL, page3, &position_texte);
        }
        Poly = Poly->adr2;
    }

    //-----------------------------------------------------------------------------------------------------
    position_textures.x = 0;
    position_textures.y = 0;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
    if (texture_p3 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");
    if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    cases.h = 30;
    cases.w = 30;
    for (int i = 0; i < cpt; i++)
    {
        cases.x = bouttons[i].x1;
        cases.y = bouttons[i].y1;
        if (SDL_RenderFillRect(rendu, &cases) != 0)
            SDL_ExitWithError("Impossible de dessiner les cases à cocher");
    }
    if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    SDL_RenderPresent(rendu);

    while (!sortir_tache)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        // Dessiner une bordure rouge si le boutton de retour est survolé
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
        if (currentlyInside != boutton_retour.mouseInside)
        {
            boutton_retour.mouseInside = currentlyInside;
            stateChanged = SDL_TRUE;
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            if (boutton_retour.mouseInside)
            {
                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
            }
            SDL_RenderPresent(rendu);
        }

        // Dessiner une bordure et une croix si les caches à cocher sont survolées
        stateChanged = SDL_FALSE;
        SDL_GetMouseState(&mouseX, &mouseY);
        for (int i = 0; i < cpt; ++i)
        {
            currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons[i]);
            if (currentlyInside != bouttons[i].mouseInside)
            {
                bouttons[i].mouseInside = currentlyInside;
                stateChanged = SDL_TRUE;
            }
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            if (SDL_SetRenderDrawColor(rendu, 239, 228, 176, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < cpt; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            for (int i = 0; i < cpt; ++i)
            {
                if (bouttons[i].mouseInside)
                {
                    dessiner_rectangle(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2 - bouttons[i].x1, bouttons[i].y2 - bouttons[i].y1);
                    SDL_RenderDrawLine(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2, bouttons[i].y2);
                    SDL_RenderDrawLine(rendu, bouttons[i].x2, bouttons[i].y1, bouttons[i].x1, bouttons[i].y2);
                }
            }
            SDL_RenderPresent(rendu);
        }

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                        sortir_tache = SDL_TRUE;
                    else
                    {
                        for (int i = 0; i < cpt; i++)
                        {
                            if ((event.button.x < bouttons[i].x2) && (event.button.x > bouttons[i].x1) && (event.button.y < bouttons[i].y2) && (event.button.y > bouttons[i].y1))
                            {
                                //------------------------------------------------Traçage de la Courbe--------------------------------------------------
                                Tete_Poly1 = adr_maillon_ens(Tete_Ensemble, i + 1)->adr1;

                                page3 = IMG_Load("assets/images/courbe2.jpg");
                                if (page3 == NULL)
                                    SDL_ExitWithError("Impossible de charge la page3");

                                position_texte.x = 770;
                                position_texte.y = 79;
                                texte = TTF_RenderText_Blended(police4, "10", VERT);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                position_texte.x = 765;
                                position_texte.y = 700;
                                texte = TTF_RenderText_Blended(police4, "-10", VERT);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                position_texte.x = 1233;
                                position_texte.y = 390;
                                texte = TTF_RenderText_Blended(police4, "15", VERT);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                position_texte.x = 295;
                                position_texte.y = 390;
                                texte = TTF_RenderText_Blended(police4, "-15", VERT);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);

                                position_textures.x = 0;
                                position_textures.y = 0;
                                texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
                                if (texture_p3 == NULL)
                                    SDL_ExitWithError("Impossible de créer la texture");
                                if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");

                                SDL_Texture *render_target_texture = SDL_CreateTexture(
                                    rendu,
                                    SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET,
                                    WINDOW_WIDTH1,
                                    WINDOW_HEIGHT1);
                                if (render_target_texture == NULL)
                                    SDL_ExitWithError("Impossible de créer la texture cible");

                                if (SDL_SetRenderTarget(rendu, render_target_texture) != 0)
                                    printf("Failed to set render target: %s\n", SDL_GetError());
                                if (SDL_RenderCopy(rendu, texture_p3, NULL, NULL) != 0)
                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");

                                if (SDL_SetRenderDrawColor(rendu, 30, 153, 67, SDL_ALPHA_OPAQUE) != 0)
                                    SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                                dessiner_rectangle(rendu, 330, 100, 900, 600);
                                dessiner_ligne(rendu, 780, 100, 780, 700);
                                dessiner_ligne(rendu, 330, 400, 1230, 400);
                                SDL_RenderDrawLine(rendu, 480, 100, 480, 700);
                                SDL_RenderDrawLine(rendu, 630, 100, 630, 700);
                                SDL_RenderDrawLine(rendu, 930, 100, 930, 700);
                                SDL_RenderDrawLine(rendu, 1080, 100, 1080, 700);
                                SDL_RenderDrawLine(rendu, 330, 250, 1230, 250);
                                SDL_RenderDrawLine(rendu, 330, 550, 1230, 550);
                                if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                    SDL_ExitWithError("Impossible de changer la couleur au Rendu");

                                float x, y = 0;      // Les coordonnées de chaque point en (cm)
                                int Xp, Yp, cpt = 0; // Les coordonnées convertis en pixels
                                for (x = -15; x <= 15; x += 0.001)
                                {
                                    y = eval_poly(Tete_Poly1, x);
                                    Xp = 30 * x + 780;
                                    Yp = -30 * y + 400;
                                    if ((Yp < 700) && (Yp > 100))
                                        dessiner_point(rendu, Xp, Yp);
                                }

                                SDL_SetRenderTarget(rendu, NULL);
                                if (SDL_RenderCopy(rendu, render_target_texture, NULL, &position_textures) != 0)
                                    SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                SDL_RenderPresent(rendu);

                                SDL_bool sortir_tache2 = SDL_FALSE;
                                while (!sortir_tache2)
                                {
                                    temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

                                    // Dessiner une bordure rouge si le boutton de retour est survolé
                                    int mouseX, mouseY;
                                    SDL_GetMouseState(&mouseX, &mouseY);
                                    SDL_bool stateChanged = SDL_FALSE;
                                    SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
                                    if (currentlyInside != boutton_retour.mouseInside)
                                    {
                                        boutton_retour.mouseInside = currentlyInside;
                                        stateChanged = SDL_TRUE;
                                    }
                                    if (stateChanged)
                                    {
                                        if (SDL_RenderCopy(rendu, render_target_texture, NULL, &position_textures) != 0)
                                            SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                                        if (boutton_retour.mouseInside)
                                        {
                                            dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
                                        }
                                        SDL_RenderPresent(rendu);
                                    }

                                    while (SDL_PollEvent(&event))
                                    {
                                        switch (event.type)
                                        {
                                        case SDL_MOUSEBUTTONDOWN:
                                            if (event.button.button == SDL_BUTTON_LEFT)
                                                if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                                                    sortir_tache2 = SDL_TRUE;
                                            break;

                                        case SDL_QUIT:
                                            sortir_tache2 = SDL_TRUE;
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
                                }
                                //-------------------------------------------FIN du Traçage de la Courbe---------------------------------------------
                                sortir_tache = SDL_TRUE;
                                break;
                            }
                        }
                    }
                }
                continue;

            case SDL_QUIT:
                sortir_tache = SDL_TRUE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }
    free(bouttons);
    TTF_CloseFont(police2);
    TTF_CloseFont(police3);
    TTF_CloseFont(police4);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(page3);
    SDL_DestroyTexture(texture_p3);
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
// Une petite application pour les polynomes (CRC)
void application_CRC_2D(SDL_Renderer *rendu)
{
    long long G = 0, MOT = 0, MOT1 = 0, CRC = 0, MESSAGE = 0, puis = 0;
    Monome *Poly_G = NULL, *Poly_MOT = NULL, *Poly_CRC = NULL, *Poly_MESSAGE = NULL;
    float puiss = 0;
    char phrase[100] = "";
    unsigned int temps_limite = 0;
    SDL_Texture *texture_p3 = NULL;
    SDL_Surface *page3 = NULL, *texte = NULL;
    TTF_Font *police2 = NULL, *police3 = NULL, *police4 = NULL;
    SDL_Rect position_textures, position_texte, cases;
    SDL_Event event;
    SDL_bool sortir_tache = SDL_FALSE;
    Rectangle boutton_retour = {3, 100, 260, 632, 687, SDL_FALSE};

    page3 = IMG_Load("assets/images/03.jpg");
    if (page3 == NULL)
        SDL_ExitWithError("Impossible de charger l'image");

    police2 = TTF_OpenFont("assets/polices/times.ttf", 40);
    if (police2 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police2, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);

    police3 = TTF_OpenFont("assets/polices/times.ttf", 25);
    if (police3 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    police4 = TTF_OpenFont("assets/polices/times.ttf", 15);
    if (police4 == NULL)
        SDL_ExitWithError("Impossible de charger la police Times New Romain");
    TTF_SetFontStyle(police3, TTF_STYLE_NORMAL);

    position_texte.x = 80;
    position_texte.y = 80;
    texte = TTF_RenderText_Blended(police2, "Veuillez faire votre choix :", BLANC);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);

    position_texte.x = 400;
    position_texte.y = 250;
    texte = TTF_RenderText_Blended(police3, "Calculer le code CRC d'un message avant de l'emettre", NOIR);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);

    position_texte.y += 150;
    texte = TTF_RenderText_Blended(police3, "Verifier l'authenticite d'un message recu", NOIR);
    SDL_BlitSurface(texte, NULL, page3, &position_texte);

    //-----------------------------------------------------------------------------------------------------
    position_textures.x = 0;
    position_textures.y = 0;
    texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
    if (texture_p3 == NULL)
        SDL_ExitWithError("Impossible de créer la texture");
    if (SDL_QueryTexture(texture_p3, NULL, NULL, &position_textures.w, &position_textures.h) != 0)
        SDL_ExitWithError("Impossible de charger la texture (dans la mémoire)");
    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
        SDL_ExitWithError("Impossible de copier la texture dans le rendu");

    Rectangle bouttons[] = {
        {3, 350, 380, 250, 280, SDL_FALSE},
        {3, 350, 380, 400, 430, SDL_FALSE}};
    if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    cases.x = bouttons[0].x1;
    cases.y = bouttons[0].y1;
    cases.h = 30;
    cases.w = 30;
    if (SDL_RenderFillRect(rendu, &cases) != 0)
        SDL_ExitWithError("Impossible de dessiner les cases à cocher");
    cases.y = bouttons[1].y1;
    if (SDL_RenderFillRect(rendu, &cases) != 0)
        SDL_ExitWithError("Impossible de dessiner les cases à cocher");
    SDL_RenderPresent(rendu);
    if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur au Rendu");
    SDL_RenderPresent(rendu);

    while (!sortir_tache)
    {
        temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

        // Dessiner une bordure rouge si le boutton de retour est survolé
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_bool stateChanged = SDL_FALSE;
        SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
        if (currentlyInside != boutton_retour.mouseInside)
        {
            boutton_retour.mouseInside = currentlyInside;
            stateChanged = SDL_TRUE;
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < 2; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            if (boutton_retour.mouseInside)
            {
                dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
            }
            SDL_RenderPresent(rendu);
        }

        // Dessiner une bordure rouge si un des bouttons (oui et non) est survolé
        stateChanged = SDL_FALSE;
        SDL_GetMouseState(&mouseX, &mouseY);
        for (int i = 0; i < 2; i++)
        {
            currentlyInside = isMouseInsideRect(mouseX, mouseY, bouttons[i]);
            if (currentlyInside != bouttons[i].mouseInside)
            {
                bouttons[i].mouseInside = currentlyInside;
                stateChanged = SDL_TRUE;
            }
        }
        if (stateChanged)
        {
            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
            if (SDL_SetRenderDrawColor(rendu, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
            cases.h = 30;
            cases.w = 30;
            for (int i = 0; i < 2; i++)
            {
                cases.x = bouttons[i].x1;
                cases.y = bouttons[i].y1;
                if (SDL_RenderFillRect(rendu, &cases) != 0)
                    SDL_ExitWithError("Impossible de dessiner les cases à cocher");
            }
            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                SDL_ExitWithError("Impossible de changer la couleur au Rendu");

            for (int i = 0; i < 2; ++i)
            {
                if (bouttons[i].mouseInside)
                {
                    dessiner_rectangle(rendu, bouttons[i].x1, bouttons[i].y1, 30, 30);
                    SDL_RenderDrawLine(rendu, bouttons[i].x2, bouttons[i].y1, bouttons[i].x1, bouttons[i].y2);
                    SDL_RenderDrawLine(rendu, bouttons[i].x1, bouttons[i].y1, bouttons[i].x2, bouttons[i].y2);
                }
            }
            SDL_RenderPresent(rendu);
        }

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                        sortir_tache = SDL_TRUE;
                    else
                    {
                        if ((event.button.x < bouttons[0].x2) && (event.button.x > bouttons[0].x1) && (event.button.y < bouttons[0].y2) && (event.button.y > bouttons[0].y1))
                        {
                            //-----------------------------------------------Code CRC------------------------------------------------------
                            page3 = IMG_Load("assets/images/03.jpg");
                            if (page3 == NULL)
                                SDL_ExitWithError("Impossible de charger l'image");

                            position_texte.x = 80;
                            position_texte.y = 80;
                            texte = TTF_RenderText_Blended(police2, "Calcul du Code CRC :", BLANC);
                            SDL_BlitSurface(texte, NULL, page3, &position_texte);

                            position_texte.x = 300;
                            position_texte.y = 150;
                            texte = TTF_RenderText_Blended(police3, "Le polynome generateur represente par un mot binaire :", NOIR);
                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                            position_texte.x += 100;
                            position_texte.y += 50;
                            texte = TTF_RenderText_Blended(police3, "G  =  ", NOIR);
                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                            position_texte.x += texte->w;
                            if (SDL_lire_long_long(&G, rendu, &page3, position_texte.x, position_texte.y) == -1)
                                goto FIN_CRC;

                            position_texte.x = 300;
                            position_texte.y = 300;
                            texte = TTF_RenderText_Blended(police3, "Le mot a transmettre represente par un mot binaire :", NOIR);
                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                            position_texte.x += 100;
                            position_texte.y += 50;
                            texte = TTF_RenderText_Blended(police3, "MOT  =  ", NOIR);
                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                            position_texte.x += texte->w;
                            if (SDL_lire_long_long(&MOT, rendu, &page3, position_texte.x, position_texte.y) == -1)
                                goto FIN_CRC;

                            position_texte.x = 280;
                            position_texte.y += 50;
                            texte = TTF_RenderText_Blended(police3, "===========================================================", NOIR);
                            SDL_BlitSurface(texte, NULL, page3, &position_texte);

                            puiss = (float)(nb_pos(G) - 1);
                            puis = (long long)powf(10, puiss);
                            MOT1 = MOT * puis;
                            Poly_G = entier_vers_poly(G);
                            Poly_MOT = entier_vers_poly(MOT1);
                            if (div_poly_binaire(Poly_MOT, Poly_G, &Poly_CRC) == -1)
                            {
                                position_texte.x = 400;
                                position_texte.y = 500;
                                texte = TTF_RenderText_Blended(police2, "Division sur 0 !", ORANGE);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                            }
                            else
                            {
                                CRC = poly_vers_entier(Poly_CRC);
                                position_texte.x = 300;
                                position_texte.y = 500;
                                texte = TTF_RenderText_Blended(police3, "Le code CRC est : ", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                position_texte.x += texte->w;
                                sprintf(phrase, "%lld", CRC);
                                texte = TTF_RenderText_Blended(police3, phrase, BLEU);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                position_texte.x = 300;
                                position_texte.y += 50;
                                texte = TTF_RenderText_Blended(police3, "D'ou le message a transmettre est :", NOIR);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                                position_texte.x += 100;
                                position_texte.y += 50;
                                sprintf(phrase, " %lld%lld", MOT, CRC);
                                texte = TTF_RenderText_Blended(police3, phrase, BLEU);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                            }

                            texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
                            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                            SDL_RenderPresent(rendu);

                            SDL_bool oui_non = SDL_TRUE;
                            while (oui_non)
                            {
                                temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

                                // Dessiner une bordure rouge si le boutton de retour est survolé
                                int mouseX, mouseY;
                                SDL_GetMouseState(&mouseX, &mouseY);
                                SDL_bool stateChanged = SDL_FALSE;
                                SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
                                if (currentlyInside != boutton_retour.mouseInside)
                                {
                                    boutton_retour.mouseInside = currentlyInside;
                                    stateChanged = SDL_TRUE;
                                }
                                if (stateChanged)
                                {
                                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");

                                    if (boutton_retour.mouseInside)
                                    {
                                        dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
                                    }
                                    SDL_RenderPresent(rendu);
                                }

                                while (SDL_PollEvent(&event))
                                {
                                    switch (event.type)
                                    {
                                    case SDL_MOUSEBUTTONDOWN:
                                        if (event.button.button == SDL_BUTTON_LEFT)
                                        {
                                            if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                                                oui_non = SDL_FALSE;
                                        }
                                        continue;

                                    case SDL_QUIT:
                                        oui_non = SDL_FALSE;
                                        break;

                                    default:
                                        break;
                                    }
                                }
                                limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
                            }
                            sortir_tache = SDL_TRUE;
                            break;
                        }
                        if ((event.button.x < bouttons[1].x2) && (event.button.x > bouttons[1].x1) && (event.button.y < bouttons[1].y2) && (event.button.y > bouttons[1].y1))
                        {
                            //-----------------------------------------------Verification du message------------------------------------------------------
                            page3 = IMG_Load("assets/images/03.jpg");
                            if (page3 == NULL)
                                SDL_ExitWithError("Impossible de charger l'image");

                            position_texte.x = 80;
                            position_texte.y = 80;
                            texte = TTF_RenderText_Blended(police2, "Verification de l'authenticite du message :", BLANC);
                            SDL_BlitSurface(texte, NULL, page3, &position_texte);

                            position_texte.x = 300;
                            position_texte.y = 150;
                            texte = TTF_RenderText_Blended(police3, "Le polynome generateur represente par un mot binaire :", NOIR);
                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                            position_texte.x += 100;
                            position_texte.y += 50;
                            texte = TTF_RenderText_Blended(police3, "G  =  ", NOIR);
                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                            position_texte.x += texte->w;
                            if (SDL_lire_long_long(&G, rendu, &page3, position_texte.x, position_texte.y) == -1)
                                goto FIN_CRC;

                            position_texte.x = 300;
                            position_texte.y = 300;
                            texte = TTF_RenderText_Blended(police3, "Le message recu represente par un mot binaire :", NOIR);
                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                            position_texte.x += 100;
                            position_texte.y += 50;
                            texte = TTF_RenderText_Blended(police3, "MESSAGE  =  ", NOIR);
                            SDL_BlitSurface(texte, NULL, page3, &position_texte);
                            position_texte.x += texte->w;
                            if (SDL_lire_long_long(&MESSAGE, rendu, &page3, position_texte.x, position_texte.y) == -1)
                                goto FIN_CRC;

                            position_texte.x = 280;
                            position_texte.y += 50;
                            texte = TTF_RenderText_Blended(police3, "===========================================================", NOIR);
                            SDL_BlitSurface(texte, NULL, page3, &position_texte);

                            Poly_G = entier_vers_poly(G);
                            Poly_MESSAGE = entier_vers_poly(MESSAGE);
                            if (div_poly_binaire(Poly_MESSAGE, Poly_G, &Poly_MOT) == -1)
                            {
                                position_texte.x = 400;
                                position_texte.y = 500;
                                texte = TTF_RenderText_Blended(police2, "Division sur 0 !", ORANGE);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                            }
                            else
                            {
                                MOT = poly_vers_entier(Poly_MOT);
                                position_texte.x = 400;
                                position_texte.y = 500;
                                if (MOT == 0)
                                    texte = TTF_RenderText_Blended(police2, "Il n'y a pas d'erreurs !", BLEU);
                                else
                                    texte = TTF_RenderText_Blended(police2, "Y a une erreur !", BLEU);
                                SDL_BlitSurface(texte, NULL, page3, &position_texte);
                            }

                            texture_p3 = SDL_CreateTextureFromSurface(rendu, page3);
                            if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                SDL_ExitWithError("Impossible de copier la texture dans le rendu");
                            if (SDL_SetRenderDrawColor(rendu, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0)
                                SDL_ExitWithError("Impossible de changer la couleur au Rendu");
                            SDL_RenderPresent(rendu);

                            SDL_bool oui_non = SDL_TRUE;
                            while (oui_non)
                            {
                                temps_limite = SDL_GetTicks() + FPS_LIMIT; // -----> Limiter la vitesse du programme

                                // Dessiner une bordure rouge si le boutton de retour est survolé
                                int mouseX, mouseY;
                                SDL_GetMouseState(&mouseX, &mouseY);
                                SDL_bool stateChanged = SDL_FALSE;
                                SDL_bool currentlyInside = isMouseInsideRect(mouseX, mouseY, boutton_retour);
                                if (currentlyInside != boutton_retour.mouseInside)
                                {
                                    boutton_retour.mouseInside = currentlyInside;
                                    stateChanged = SDL_TRUE;
                                }
                                if (stateChanged)
                                {
                                    if (SDL_RenderCopy(rendu, texture_p3, NULL, &position_textures) != 0)
                                        SDL_ExitWithError("Impossible de copier la texture dans le rendu");

                                    if (boutton_retour.mouseInside)
                                    {
                                        dessiner_rectangle_5(rendu, boutton_retour.x1, boutton_retour.y1, boutton_retour.x2 - boutton_retour.x1, boutton_retour.y2 - boutton_retour.y1);
                                    }
                                    SDL_RenderPresent(rendu);
                                }

                                while (SDL_PollEvent(&event))
                                {
                                    switch (event.type)
                                    {
                                    case SDL_MOUSEBUTTONDOWN:
                                        if (event.button.button == SDL_BUTTON_LEFT)
                                        {
                                            if ((event.button.x < boutton_retour.x2) && (event.button.x > boutton_retour.x1) && (event.button.y < boutton_retour.y2) && (event.motion.y > boutton_retour.y1))
                                                oui_non = SDL_FALSE;
                                        }
                                        continue;

                                    case SDL_QUIT:
                                        oui_non = SDL_FALSE;
                                        break;

                                    default:
                                        break;
                                    }
                                }
                                limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
                            }
                            sortir_tache = SDL_TRUE;
                            break;
                        }
                    }
                }
                continue;

            case SDL_QUIT:
                sortir_tache = SDL_TRUE;
                break;

            default:
                break;
            }
        }
        limiter_FPS(temps_limite); // -----> Limiter la vitesse du programme
    }
FIN_CRC:
    TTF_CloseFont(police2);
    TTF_CloseFont(police3);
    TTF_CloseFont(police4);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(page3);
    SDL_DestroyTexture(texture_p3);
    return;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/