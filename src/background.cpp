#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include "vector2d.h"
#include "background.h"
#include <iostream>

TiledBackground::TiledBackground(){

}
void TiledBackground::clearData(){
    if(tileTexture == NULL ) return;
    SDL_DestroyTexture(tileTexture);
    tileTexture = NULL;
}
void TiledBackground::setParallaxStrength(const double strengthFactor){
    parallaxStrength = strengthFactor;
}
bool TiledBackground::loadTexture(SDL_Renderer *renderer, std::string path){
    clearData();
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
    tileWidth = SCALE_RATIO * loadedSurface->w;
    tileHeight = SCALE_RATIO * loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
    tileTexture = newTexture;

    return tileTexture != NULL;
}
void TiledBackground::renderTile(SDL_Renderer *renderer, int screenX, int screenY){
    SDL_Rect renderQuad = {screenX, screenY, tileWidth, tileHeight};
    SDL_RenderCopy(renderer, tileTexture, NULL, &renderQuad);
}
void TiledBackground::renderSurroundedTiles(SDL_Renderer *renderer, Vector2D &camera){
    Vector2D backgroundShift = camera * parallaxStrength;
    for(int offsetIndexX = -1; offsetIndexX <= 1; ++offsetIndexX){
        for(int offsetIndexY = -1; offsetIndexY <= 1; ++offsetIndexY){

            Vector2D tileIndex(
                offsetIndexX + (camera.x + backgroundShift.x) / tileWidth,
                offsetIndexY + (camera.y + backgroundShift.y) / tileHeight
            );

            Vector2D adjustedTile(
                tileIndex.x * tileWidth - backgroundShift.x,
                tileIndex.y * tileHeight - backgroundShift.y
            );

            renderTile(renderer, adjustedTile.x - camera.x, adjustedTile.y - camera.y);
        }
    }
}
