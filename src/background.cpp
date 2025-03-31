#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include "viewport.h"
#include "background.h"
#include <iostream>

TiledBackground::TiledBackground(){

}
void TiledBackground::resetData(){
    if(texture == NULL ) return;
    SDL_DestroyTexture(texture);
    texture = NULL;
}
bool TiledBackground::loadTexture(SDL_Renderer *renderer, std::string path){
    resetData();
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if(loadedSurface == NULL){
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        return false;
    }

    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    if(newTexture == NULL){
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return false;
    }

    textureWidth = 4 * loadedSurface->w;
    textureHeight = 4 * loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
    texture = newTexture;

    return texture != NULL;
}
void TiledBackground::render(SDL_Renderer *renderer, int x, int y){
    SDL_Rect renderQuad = {x, y, textureWidth, textureHeight};
    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
}
void TiledBackground::renderTiles(SDL_Renderer *renderer, int playerX, int playerY, ViewPort camera){
    posX = playerX / textureWidth;
    posY = playerY / textureHeight;
    for(int offsetPosX = -1; offsetPosX <= 1; ++offsetPosX){
        for(int offsetPosY = -1; offsetPosY <= 1; ++offsetPosY){
            int renderX = (posX + offsetPosX) * textureWidth - camera.x;
            int renderY = (posY + offsetPosY) * textureHeight - camera.y;
            render(renderer, renderX, renderY);
            std::cout << (posX + offsetPosX) * textureWidth << ' ' << (posY + offsetPosY) * textureHeight << ":";
        }
    }
    std::cout << camera.x << ' ' << camera.y;
    std::cout << "()" << playerX << ' ' << playerY;
    std::cout << '\n';
}
