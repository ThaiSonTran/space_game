#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include <list>
#include <vector2d.h>
#include <bullet.h>
#include <player.h>
#include <background.h>

const int WINDOW_WIDTH = 1300;
const int WINDOW_HEIGHT = 700;

const int WIN_MID_WIDTH = WINDOW_WIDTH / 2;
const int WIN_MID_HEIGHT = WINDOW_HEIGHT / 2;

const int PI = 3.14159265359;

SDL_Window *gameWindow;
SDL_Renderer* gameRenderer;

Player gamePlayer;
TiledBackground background[4];
Vector2D camera;
std::list<Bullet> bulletList;

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
//texture atlas
//textureWidth textureHeight
//atlasCellSize
//numCellWide numCellTall
//clip = {cellIndexX, cellIndexY, atlasCellSize, atlasCellSize}
//bulletAtlas
//WinCoordX = bulletX - camera.x
//WinCoordY = bulletY - camera.y
//renderQuad = {WinCoordX - atlasCellSize / 2, WinCoordY - atlasCellSize / 2}
//SDL_RenderCopy(gameRenderer, bulletAtlas, clip, something about bullet pos square that take bullet pos as center)
//
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
            if(event.type == SDL_QUIT){
                gameRunning = false;
            }
            else if(event.type == SDL_KEYDOWN && event.key.repeat == false){
                gamePlayer.increaseVelocity(event.key.keysym.sym);
            }
            else if(event.type == SDL_KEYUP && event.key.repeat == false){
                gamePlayer.decreaseVelocity(event.key.keysym.sym);
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                //add bullet according to mouse dir
                //insert(bullet(player.x, player.y, mouseDirX, mouseDirY, angle)

            }

        }
        SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255); //black
        SDL_RenderClear(gameRenderer);

        gamePlayer.movePlayer();
        camera = gamePlayer.getPosition() - Vector2D(WIN_MID_WIDTH, WIN_MID_HEIGHT);
        for(int i = 0; i < 4; ++i)
            background[i].renderSurroundedTiles(gameRenderer, camera);

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        int mouseDirX = mouseX - WIN_MID_WIDTH;
        int mouseDirY = mouseY - WIN_MID_HEIGHT;
        if(mouseDirX == 0 && mouseDirY == 0) mouseDirX = 1;
        double angle = 90 + 180 * std::atan2(mouseDirY, mouseDirX) / PI;
        gamePlayer.render(
            gameRenderer,
            WIN_MID_WIDTH - gamePlayer.getTextureWidth() / 2,
            WIN_MID_HEIGHT - gamePlayer.getTextureHeight() / 2,
            angle
        );
        //render bulletlist
        SDL_RenderPresent(gameRenderer);
    }
    closeGame();
    return 0;
}
