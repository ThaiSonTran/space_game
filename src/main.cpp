#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
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
#include "healthbar.h"

const int WINDOW_WIDTH = 1300;
const int WINDOW_HEIGHT = 700;

const int WIN_MID_WIDTH = WINDOW_WIDTH / 2;
const int WIN_MID_HEIGHT = WINDOW_HEIGHT / 2;

const int ENEMY_SPAWN_DELAY = 300;
const int MAX_ENEMIES = 15;
const int COLLISION_RADIUS = 33;

const int SCORE_X = WINDOW_WIDTH - 200;
const int SCORE_Y = WINDOW_HEIGHT - 50;

SDL_Window *gameWindow;
SDL_Renderer* gameRenderer;

Player gamePlayer;
TiledBackground background[4];
Vector2D camera;
TTF_Font* gameFont = NULL;

std::list<Bullet> bulletList;
TextureAtlas bulletAtlas;

std::list<Enemy> enemyList;
TextureAtlas enemyAtlas;

EnemySpawner spawner;

HealthBar lifebar;

Mix_Chunk* soundEff = NULL;

int frameCounter = ENEMY_SPAWN_DELAY;
int playerScore = 0;

enum GameState{
    PLAYING,
    GAME_OVER
};
GameState currentState;

SDL_Texture* createTextTexture(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color){
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    if(textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if(textTexture == NULL) {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(textSurface);
    return textTexture;
}
void renderText(SDL_Renderer* renderer, SDL_Texture* textTexture, int x, int y){
    int width, height;
    SDL_QueryTexture(textTexture, NULL, NULL, &width, &height);
    SDL_Rect renderRect = { x, y, width, height };
    SDL_RenderCopy(renderer, textTexture, NULL, &renderRect);
}

bool initGame(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;


    int imgFlags = IMG_INIT_PNG;
    if((IMG_Init(imgFlags) & imgFlags) == 0) return false;

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return false;

    if(TTF_Init() == -1) return false;

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

    if(!lifebar.loadTexture(gameRenderer, "resources/fill.png", lifebar.fillTexture)) return false;
    if(!lifebar.loadTexture(gameRenderer, "resources/border.png", lifebar.borderTexture)) return false;
    lifebar.healthBarTexture = SDL_CreateTexture(
        gameRenderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        76, 20
    );

    SDL_SetTextureBlendMode(lifebar.healthBarTexture, SDL_BLENDMODE_BLEND);

    soundEff = Mix_LoadWAV("resources/shoot.flac");
	if(soundEff == NULL){
		printf("Failed to load shoot sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	gameFont = TTF_OpenFont("resources/CommodorePixeled.ttf", 24);
    if(gameFont == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    return true;
}
bool intersect(Vector2D bulletPosition, Vector2D targetPosition){
    return Vector2D(targetPosition.x - bulletPosition.x, targetPosition.y - bulletPosition.y).magnitude() < COLLISION_RADIUS;
}
inline void normal_play(bool &gameRunning){
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
        bullet.moveBullet();
        if(bullet.isTooFar(gamePlayer.getXCoord(), gamePlayer.getYCoord())){
            bulletIter = bulletList.erase(bulletIter);
            continue;
        }
        if(bullet.isEnemyBullet){
            bullet.renderBullet(gameRenderer, 1, 1, bulletAtlas, camera);
            if(intersect(bullet.getPosition(), gamePlayer.getPosition())){
                bulletIter = bulletList.erase(bulletIter);

                gamePlayer.decreaseCurrentHealth(1);
                if(gamePlayer.isDead()) currentState = GAME_OVER;
                continue;
            }
        }
        else{
            bullet.renderBullet(gameRenderer, 0, 1, bulletAtlas, camera);
            bool doEraseBullet = false;
            for(auto enemyIter = enemyList.begin(); enemyIter != enemyList.end();){
                Enemy &e = *enemyIter;
                if(intersect(bullet.getPosition(), e.getPosition())){
                    bulletIter = bulletList.erase(bulletIter);
                    doEraseBullet = true;

                    e.decreaseCurrentHealth(1);
                    playerScore += 5;

                    if(e.isDead()){
                        enemyList.erase(enemyIter);
                        playerScore += 10;
                    }

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
    lifebar.updateHealthBarTexture(gameRenderer, gamePlayer.calculateHealthRatio());
    lifebar.render(gameRenderer, 5, 5);

    char scoreText[32];
    sprintf(scoreText, "Score: %d", playerScore);
    SDL_Color textColor = { 255, 255, 255, 255 }; // White text
    SDL_Texture* scoreTexture = createTextTexture(gameRenderer, gameFont, scoreText, textColor);
    if(scoreTexture != NULL){
        int width, height;
        SDL_QueryTexture(scoreTexture, NULL, NULL, &width, &height);
        renderText(gameRenderer, scoreTexture, WINDOW_WIDTH - width - 20, WINDOW_HEIGHT - height - 20);
        SDL_DestroyTexture(scoreTexture);
    }
    //consider adding crosshair for this shit
    SDL_RenderPresent(gameRenderer);
}
inline void lose_state(bool &gameRunning){
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
            gameRunning = false;
        }
        else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
            // Press Enter to restart game
            currentState = PLAYING;
            playerScore = 0;
            gamePlayer.resetHealth();
            enemyList.clear();
            bulletList.clear();
        }
    }
    SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gameRenderer);
    SDL_Color textColor = { 255, 0, 0, 255 }; // Red text
    SDL_Texture* gameOverTexture = createTextTexture(gameRenderer, gameFont, "GAME OVER", textColor);

    if(gameOverTexture != NULL) {
        int width, height;
        SDL_QueryTexture(gameOverTexture, NULL, NULL, &width, &height);
        renderText(gameRenderer, gameOverTexture, (WINDOW_WIDTH - width) / 2, (WINDOW_HEIGHT - height) / 2 - 50);
        SDL_DestroyTexture(gameOverTexture);
    }
    char finalScoreText[64];
    sprintf(finalScoreText, "Final Score: %d", playerScore);
    SDL_Color scoreColor = { 255, 255, 255, 255 }; // White text
    SDL_Texture* finalScoreTexture = createTextTexture(gameRenderer, gameFont, finalScoreText, scoreColor);

    if(finalScoreTexture != NULL) {
        int width, height;
        SDL_QueryTexture(finalScoreTexture, NULL, NULL, &width, &height);
        renderText(gameRenderer, finalScoreTexture, (WINDOW_WIDTH - width) / 2, (WINDOW_HEIGHT - height) / 2 + 50);
        SDL_DestroyTexture(finalScoreTexture);
    }
    SDL_Texture* restartTexture = createTextTexture(gameRenderer, gameFont, "Press ENTER to restart", scoreColor);

    if(restartTexture != NULL) {
        int width, height;
        SDL_QueryTexture(restartTexture, NULL, NULL, &width, &height);
        renderText(gameRenderer, restartTexture, (WINDOW_WIDTH - width) / 2, (WINDOW_HEIGHT - height) / 2 + 150);
        SDL_DestroyTexture(restartTexture);
    }

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
        switch(currentState){
        case PLAYING:
            normal_play(gameRunning);
            break;
        case GAME_OVER:
            lose_state(gameRunning);
            break;
        }
    }
    closeGame();
    return 0;
}

