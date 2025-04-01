#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <player.h>
#include <viewport.h>
#include <background.h>

const int WINDOW_WIDTH = 1300;
const int WINDOW_HEIGHT = 700;

SDL_Window *gameWindow;
SDL_Renderer* gameRenderer;

Player gamePlayer;
TiledBackground background[4];
ViewPort camera;

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

    if(!background[0].loadTexture(gameRenderer, "resources/background1.png")) return false;
    background[0].setParalaxStrength(0);

    if(!background[1].loadTexture(gameRenderer, "resources/background2.png")) return false;
    background[1].setParalaxStrength(0.2);

    if(!background[2].loadTexture(gameRenderer, "resources/background3.png")) return false;
    background[2].setParalaxStrength(0.4);

    if(!background[3].loadTexture(gameRenderer, "resources/background4.png")) return false;
    background[3].setParalaxStrength(0.6);

    return true;
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
            if(event.type == SDL_QUIT)
                gameRunning = false;
            else if(event.type == SDL_KEYDOWN && event.key.repeat == false){
                gamePlayer.increaseVelocity(event.key.keysym.sym);
            }
            else if(event.type == SDL_KEYUP && event.key.repeat == false){
                gamePlayer.decreaseVelocity(event.key.keysym.sym);
            }
        }
        SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255); //black
        SDL_RenderClear(gameRenderer);

        gamePlayer.movePlayer();
        camera = {gamePlayer.getX() - WINDOW_WIDTH / 2, gamePlayer.getY() - WINDOW_HEIGHT / 2};

        for(int i = 0; i < 4; ++i)
            background[i].renderSurroundedTiles(gameRenderer, camera);

        gamePlayer.render(
            gameRenderer,
            WINDOW_WIDTH / 2 - gamePlayer.getTextureWidth() / 2,
            WINDOW_HEIGHT / 2 - gamePlayer.getTextureHeight() / 2
        );

        SDL_RenderPresent(gameRenderer);
    }
    closeGame();
    return 0;
}
