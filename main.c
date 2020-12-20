//  main.c
//  Snake_final
//
//  Created by Ali ElGuindi







/*
 Le but du projet est de mettre en place une animation d'un serpent et faire un petit jeu de "Snake" où il ya des pommes que le serpent est censé de les manger. Vu que le projet décrit par le prof est une simulation; je n'ai pas fait une interaction par l'utilisateur. Le serpent se déplace aléatoirement et mange ces pommes et son score augmente au fur et à mesure qu'il les mange. Il faut regarder ExecutionCode.pdf afin d’exécuter le code car l'utilisation de la librairie SDL nécessite une marche à suivre et sans cette marche à suivre le code ne sera pas exécuté.
 
Si vous voulez regarder les fonctionalités de la biboliothèque SDL, rendez-vous sur leur sites. https://wiki.libsdl.org/

*/

 
 
 /*
 Inclusion de bibliothèques standards et une autre bibliothèque SDL pour une animation visiuelle
 */
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h> //Manipulation date et temps
#include <math.h>


//Définition des constantes qui nous seront utiles tout au long du code
#define HEIGHT 640 //Hauteur de la grille
#define WIDTH 640  //Largeur de la grille







// Structure pour la position (x,y)
typedef struct
{
    int x;
    int y;
} vec2d;

//Structure pour modéliser le serpent et son mouvement aléatoire.
typedef struct
{
    int x, y;
    int size;
    int score;
    vec2d pos[100];

} Snake;

//Structure pour modéliser la position des pommes, fixé à 15 pommes voir plus tard
typedef struct
{
    int x, y;
} Apple;



// Inspiration par des gens qui ont fait des jeux 2D sur internet


/*
Sites qui m'ont aidé : https://wiki.libsdl.org/SDL_GetKeyboardState
 https://stackoverflow.com/questions/18454588/sdl2-not-receiving-window-events
 https://vertostudio.com/pub/learnc/contro_particle.c
*/


//Fonction qui manipule les évenements en manipulant le clavier, Beaucoup de gens l'utilisent sur Internet.
//SDL wiki. Ref  :https://www.libsdl.org/. Pour les fonctions relatives à <SDL.h>
//Ref : https://www.libsdl.org/release/SDL-1.2.15/docs/html/guideinputkeyboard.html
//Ref :http://lazyfoo.net/SDL_tutorials/lesson10/index.php


int processEvents(SDL_Window *window, Snake *snake, int *hist)
{
    SDL_Event event;
    int done = 0;

  while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_WINDOWEVENT_CLOSE:
        {
            if (window)
            {
                SDL_DestroyWindow(window);
                window = NULL;
                done = 1;
            }
        }
        break;
        case SDL_KEYDOWN:
        {
            switch (event.key.keysym.sym)
            {
            //Si escape key appuyé done = 1 et sortir de la boucle et le programme s'arrête.
            case SDLK_ESCAPE:
                done = 1;
                break;
            }
        }
        break;
                //SDL_QUIT condition nécessaire, la boucle doit finir avec cette condition
        case SDL_QUIT:
            done = 1;
            break;
        }
    }
    //Randomiser le mouvement du serpent à l'aide de rand() allant de 0 à 3
    int random_dir = rand() % 4;
        
    
    
    
    //Aller à gauche
    if (random_dir == 0)
    {
        //stocker l'ancien emplacement du serpent en p
        vec2d p = {snake->x, snake->y};
        //mettre le tableau pos égale à p
        snake->pos[*hist] = p;
        //Augemneter hist pour écrire dans la case qui suit
        (*hist)++;
        //modulo pour toujours écrire dans la case correspondante, si longeur = 2 écrire dans les cases 0, 1
        *hist %= snake->score;

        snake->x -= 40;
        
        
        //Le serpent en dehors de la grille
        if (snake->x < 0)
        {
        //Lui faire apparaitre de l'autre côté
            snake->x = 16 * 40;
        }
        //Le serpent en dehors de la grille
        if (snake->x > WIDTH)
        {
        //Lui faire apparaitre de l'autre côté
            snake->x = 0;
        }
       
    }
    
    //Aller à droite
    else if (random_dir == 1)
    {
        vec2d p = {snake->x, snake->y};
        snake->pos[*hist] = p;
        (*hist)++;
        *hist %= snake->score;

        snake->x += 40;
        
        //Le serpent en dehors de la grille
        if (snake->x < 0)
        {
        //Lui faire apparaitre de l'autre côté
            snake->x = 16 * 40;
        }
        if (snake->x > WIDTH)
        {
            snake->x = 0;
        }
       
    }
    
    //Aller en haut
    else if (random_dir == 2)
    {
        vec2d p = {snake->x, snake->y};
        snake->pos[*hist] = p;
        (*hist)++;
        *hist %= snake->score;

        snake->y -= 40;
        if (snake->y < 0)
        {
            snake->y = 16 * 40;
        }
        if (snake->y > HEIGHT)
        {
            snake->y = 0;
        }
       
    }
    //Aller en bas
    else if (random_dir == 3)
    {
        vec2d p = {snake->x, snake->y};
        snake->pos[*hist] = p;
        (*hist)++;
        *hist %= snake->score;

        snake->y += 40;
        if (snake->y < 0)
        {
            snake->y = 16 * 40;
        }
        if (snake->y > HEIGHT)
        {
            snake->y = 0;
        }
        
    }

    return done;
}



/*
 La fonction qui va nous dessiner tout. La grille, le serpent et les pommes avec des fonctions dans la librairie SDL.
 Il faut faire attention au fait que l'origine (0,0) se trouve en haut à gauche.
 Faire aussi attention au fait que l'axe x est orienté positivement à droite et l'axe y est orienté positivement en bas.
 
 Ref:https://wiki.libsdl.org/
*/





void doRender(SDL_Renderer *renderer, Snake *snake, Apple *apple, size_t nb_apples)
{
    //Ref :https://www.rapidtables.com/web/color/RGB_Color.html
    //Faire la couleur du fond, couleur quasi rouge voir l'animation (RGB datavalue)
    SDL_SetRenderDrawColor(renderer, 175, 129, 167, 255);
    
    
    //Supprimer l'écran
    //Effacez tout l'écran avec la couleur sélectionnée.
    SDL_RenderClear(renderer);

    //Faire une couleur blanche(RGB = (255, 255, 255))
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    

    
    
    
    
    // Boucle pour dessiner les lignes horizentales de la grille, de haut en bas
    for (int i = 0; i <= HEIGHT; i += 40)
    {
        SDL_RenderDrawLine(renderer, 0, i, WIDTH, i);
    }
    
    //Boucle pour dessiner les lignes verticales de la grille, de gauche à droit
    for (int i = 0; i <= WIDTH; i += 40)
    {
        SDL_RenderDrawLine(renderer, i, 0, i, HEIGHT);
    }
   
    
    
    //Structure, qui contient la définition d'un rectangle, commence en haut à gauche
    SDL_Rect rect_snake = {snake->x, snake->y, snake->size, snake->size};
    SDL_RenderFillRect(renderer, &rect_snake);
    
    
    
    //Boucle pour augementer la taille du serpent, aussi en utilisant le SDL_Rect
    for (int i = 0; i < snake->score; i++)
    {
        SDL_Rect rect_snake_test = {snake->pos[i].x, snake->pos[i].y, snake->size, snake->size};
        SDL_RenderFillRect(renderer, &rect_snake_test);
    }
    
    
    
    //Couleur rouge
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    
    
    //Utilisation de size_t vu que i ici ne peut jamais être négatif
    for (size_t i = 0; i < nb_apples; i++)
    {
        SDL_Rect rect_apple = {apple[i].x, apple[i].y, 40, 40};
        SDL_RenderFillRect(renderer, &rect_apple);
    }
    
    
    
    /*
     Fonction qui affichera le nouveau contenu de la fenêtre, tout ce qui précédait n'était pas encore visible sur l'écran
     */
    SDL_RenderPresent(renderer);
}






//Fonction qui supprime les pommes si elles sont mangées par le serpent
void is_coliding(Snake *snake, Apple *apple)
{
    /*
    Boucle if pour voir si la position en (x,y) du serpent coïncide avec celle de la pomme
    */
    if (snake->x == apple->x && snake->y == apple->y)
    {
        //Supprimer la pomme de la grille en la dessinant à l'extérieur
        apple->x = 1000;
        apple->y = 1000;
        //Augmenter le score
        snake->score++;
        //Printer sur le terminal le score qui commence par 2
        printf("score = %d\n", snake->score);
    }
}






//La fonction main

int main(int argc, char *argv[])
{
    
    
    //Générateur de nombres aléatoires avec NULL pour avoir de séquences de nombres différentes
    srand((int)time(NULL));
    
    //Ref : https://wiki.libsdl.org/
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);
    
    
    //Initialisation d'une copie snake à partir de la structure Snake
    Snake snake;
    snake.x = 0;
    snake.y = 0;
    snake.size = 40;
    snake.score = 1;

    
    
    
    //Fixer le nombre de pommes à 15 dans la grille
    const int NB_APPLES = 15;
    
    //Mettre tout dans un tableau à 15 cases
    Apple apples[NB_APPLES];
    
    //Boucle pour générer toutes les pommes dans la grille fixé à 15
    for (int i = 0; i < NB_APPLES; i++)
    {
        Apple apple;
        
        //Randomiser la position de la pomme avec la fonction rand (de 0 à 639)
        apple.x = rand() % 16 * 40;
        apple.y = rand() % 16 * 40;
        //Chaque case de tableau correspond à une copie apple
        apples[i] = apple;
    }

    
    //Fonction pour créer une fenêtre avec les indicateurs spécifiés
    //Ref: https://wiki.libsdl.org/SDL_CreateWindow
    window = SDL_CreateWindow("Snake Sim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);

    //Fonction pour créer un contexte de rendu 2D pour une fenêtre
    //Ref:https://wiki.libsdl.org/SDL_CreateRenderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    
    
    
    //hist à 0, car quand appelé dans la fonction il écrit à la case 0 du tableau pos
    int hist = 0;
    int done = 0;
    
    
    //Si done prend la valeur 1, on sort de la boucle et le programme s'étiend
    while (!done)
    {
        
        //Gérer le mouvement du serpent. Appeler la fonction processEvents
        done = processEvents(window, &snake, &hist);
        
        
        //Dessiner des objets sur l'écran. Appeler la fonction doRender
        doRender(renderer, &snake, apples, NB_APPLES);

        
        
        
    
    //vérifier si le serpent a mangé une pomme grâce à la fonction is_coliding
        for (int i = 0; i < NB_APPLES; i++)
        {
            is_coliding(&snake, &apples[i]);
        }
        //Fonction pour attendre 200 millisecondes
        SDL_Delay(200);
    }

    
    
    //Détruire la fenêtre
    //Si done prend la valeur 1, on exécute ces commandes
    SDL_DestroyWindow(window);
    
    
    SDL_DestroyRenderer(renderer);
    //Nettoyer tous les sous-systèmes initialisés.
    SDL_Quit();
    //Fin du programme
    return 0;
}
