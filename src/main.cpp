#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "player.h"
#include "globals.h"

bool initGame(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;


    int imgFlags = IMG_INIT_PNG;
    if((IMG_Init(imgFlags) & imgFlags) == 0) return false;


    if(SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) == 0)
        printf("Warning: Linear texture filtering not enabled!");


    gameWindow = SDL_CreateWindow(
        "My SDL2 Application",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if(gameWindow == NULL) return false;


    gameRenderer = SDL_CreateRenderer(
        gameWindow,
        -1, // Driver index (-1 chooses the first available)
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if(gameRenderer == NULL) return false;


    return true;
}
void closeGame(){
    SDL_DestroyRenderer(gameRenderer);
    gameRenderer = NULL;

    SDL_DestroyWindow(gameWindow);
    gameWindow = NULL;

    SDL_Quit();
    IMG_Quit();
}
bool loadMedia(){
    if(!gamePlayer.loadTexture(gameRenderer, "resources/green.png")) return false;

    return true;
}
void gameLogic(){
    SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gameRenderer);

    gamePlayer.render(gameRenderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    SDL_RenderPresent(gameRenderer);
}
int main(int argc, char* args[]){
    if(!initGame()){
        printf("Failed to initialize game\n");
        return 1;
    }
    if(!loadMedia()){
        printf("Failed to load media\n");
        return 1;
    }
    bool gameRunning = true;
    while(gameRunning){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT) gameRunning = false;
        }
        gameLogic();
    }
    closeGame();
    return 0;
}
