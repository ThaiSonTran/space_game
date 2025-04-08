#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include <list>
#include "vector2d.h"
#include "textureatlas.h"
#include "bullet.h"
#include "player.h"
#include "background.h"
#include "enemy.h"

const int WINDOW_WIDTH = 1300;
const int WINDOW_HEIGHT = 700;

const int WIN_MID_WIDTH = WINDOW_WIDTH / 2;
const int WIN_MID_HEIGHT = WINDOW_HEIGHT / 2;

const int ENEMY_SPAWN_DELAY = 300;
const int MAX_ENEMIES = 15;
const int COLLISION_RADIUS = 33;

SDL_Window *gameWindow;
SDL_Renderer* gameRenderer;

Player gamePlayer;
TiledBackground background[4];
Vector2D camera;

std::list<Bullet> bulletList;
TextureAtlas bulletAtlas;

std::list<Enemy> enemyList;
TextureAtlas enemyAtlas;

EnemySpawner spawner;

Mix_Chunk* soundEff = NULL;

int frameCounter = ENEMY_SPAWN_DELAY;

enum GameState{
    PLAYING,
    GAME_OVER
};
GameState currentState;

bool initGame(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;


    int imgFlags = IMG_INIT_PNG;
    if((IMG_Init(imgFlags) & imgFlags) == 0) return false;

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return false;

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
    Mix_Quit();
}
bool loadMedia(){
    if(!gamePlayer.loadTexture(gameRenderer, "resources/green.png")) return false;

    if(!background[0].loadTexture(gameRenderer, "resources/background1.png")) return false;
    background[0].setParallaxStrength(0);

    if(!background[1].loadTexture(gameRenderer, "resources/background2.png")) return false;
    background[1].setParallaxStrength(0.2);

    if(!background[2].loadTexture(gameRenderer, "resources/background3.png")) return false;
    background[2].setParallaxStrength(0.4);

    if(!background[3].loadTexture(gameRenderer, "resources/background4.png")) return false;
    background[3].setParallaxStrength(0.6);

    if(!bulletAtlas.loadTexture(gameRenderer, "resources/projectiles.png")) return false;
    bulletAtlas.calculateCellSize(3, 2);

    if(!enemyAtlas.loadTexture(gameRenderer, "resources/enemyAtlas.png")) return false;
    enemyAtlas.calculateCellSize(5, 2);

    soundEff = Mix_LoadWAV("resources/shoot.flac");
	if(soundEff == NULL){
		printf("Failed to load shoot sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

    return true;
}
bool intersect(Vector2D bulletPosition, Vector2D targetPosition){
    return Vector2D(targetPosition.x - bulletPosition.x, targetPosition.y - bulletPosition.y).magnitude() < COLLISION_RADIUS;
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
        int bulletToAdd = 0;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                gameRunning = false;
            }
            else if(event.type == SDL_KEYDOWN && event.key.repeat == false){
                gamePlayer.accelerateInDirection(event.key.keysym.sym);
            }
            else if(event.type == SDL_KEYUP && event.key.repeat == false){
                gamePlayer.decelerateInDirection(event.key.keysym.sym);
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                ++bulletToAdd;
                Mix_PlayChannel(-1, soundEff, 0);
            }
        }
        SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255); //black
        SDL_RenderClear(gameRenderer);

        gamePlayer.movePlayer();
        camera = gamePlayer.getPosition() - Vector2D(WIN_MID_WIDTH, WIN_MID_HEIGHT);

        if(frameCounter <= 0){
            frameCounter = ENEMY_SPAWN_DELAY;
            if(enemyList.size() < MAX_ENEMIES) spawner.createInto(enemyList, gamePlayer.getPosition());
        }
        else --frameCounter;

        for(auto enemyIter = enemyList.begin(); enemyIter != enemyList.end(); ++enemyIter){
            Enemy &e = *enemyIter;
            e.moveEnemy(gamePlayer.getPosition(), bulletList);
        }

        for(int i = 0; i < 4; ++i)
            background[i].renderSurroundedTiles(gameRenderer, camera);

        Vector2D mousePos;
        SDL_GetMouseState(&mousePos.x, &mousePos.y);
        Vector2D mouseDir = mousePos - Vector2D(WIN_MID_WIDTH, WIN_MID_HEIGHT);

        if(mouseDir.x == 0 && mouseDir.y == 0) mouseDir.x = 1;
        double angle = std::atan2((double)mouseDir.y, (double)mouseDir.x) * 180 / M_PI + 90;

        for(; bulletToAdd; --bulletToAdd){
            bulletList.push_back(Bullet(
                gamePlayer.getXCoord(), gamePlayer.getYCoord(),
                mouseDir.x, mouseDir.y, angle, false
            ));
        }
        for(auto bulletIter = bulletList.begin(); bulletIter != bulletList.end();){

            Bullet &bullet = *bulletIter;
            bullet.renderBullet(gameRenderer, 1, 1, bulletAtlas, camera);
            bullet.moveBullet();
            if(bullet.isTooFar(gamePlayer.getXCoord(), gamePlayer.getYCoord())){
                bulletIter = bulletList.erase(bulletIter);
                continue;
            }
            if(bullet.isEnemyBullet){
                if(intersect(bullet.getPosition(), gamePlayer.getPosition())){
                    bulletIter = bulletList.erase(bulletIter);
                    continue;
                }
            }
            else{
                bool doEraseBullet = false;
                for(auto enemyIter = enemyList.begin(); enemyIter != enemyList.end();){
                    Enemy &e = *enemyIter;
                    if(intersect(bullet.getPosition(), e.getPosition())){
                        bulletIter = bulletList.erase(bulletIter);
                        doEraseBullet = true;

                        e.decreaseCurrentHealth(1);
                        if(e.isDead()) enemyList.erase(enemyIter);

                        break;
                    }
                    ++enemyIter;
                }
                if(doEraseBullet) continue;
            }
            ++bulletIter;
        }

        for(auto enemyIter = enemyList.begin(); enemyIter != enemyList.end(); ++enemyIter){
            Enemy &e = *enemyIter;
            e.renderEnemy(gameRenderer, enemyAtlas, camera);
        }
        gamePlayer.render(
            gameRenderer,
            WIN_MID_WIDTH - gamePlayer.getShipWidth() / 2,
            WIN_MID_HEIGHT - gamePlayer.getShipHeight() / 2,
            angle
        );
        //consider adding crosshair for this shit
        SDL_RenderPresent(gameRenderer);
    }
    closeGame();
    return 0;
}

