#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include "player.h"

void Player::freeData(){
    if(shipTexture == NULL ) return;
    SDL_DestroyTexture(shipTexture);
    shipTexture = NULL;
    delete(&textureHeight);
    delete(&textureWidth);
    delete(&x);
    delete(&y);
}
bool Player::loadTexture(SDL_Renderer *renderer, std::string path){
	freeData();
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
	shipTexture = newTexture;

	return shipTexture != NULL;
}
void Player::render(SDL_Renderer *renderer, int x, int y){
	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, textureWidth, textureHeight};

	//Render to screen
	SDL_RenderCopy(renderer, shipTexture, NULL, &renderQuad);
}
