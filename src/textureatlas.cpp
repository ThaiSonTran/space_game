#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "textureatlas.h"

void TextureAtlas::clearData(){
    if(texture == NULL ) return;
    SDL_DestroyTexture(texture);
    texture = NULL;
}
bool TextureAtlas::loadTexture(SDL_Renderer *renderer, std::string path){
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
    textureWidth = loadedSurface->w;
    textureHeight = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
    texture = newTexture;

    return texture != NULL;
}
void TextureAtlas::calculateCellSize(int numCellWide, int numCellTall){
    cellWidth = textureWidth / numCellWide;
    cellHeight = textureHeight / numCellTall;
}
