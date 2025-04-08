#include <string>
#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include "healthbar.h"

bool HealthBar::loadTexture(SDL_Renderer *renderer, std::string path, SDL_Texture *&texture){
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

    SDL_FreeSurface(loadedSurface);
	texture = newTexture;

	return texture != NULL;
}
void HealthBar::updateHealthBarTexture(SDL_Renderer* renderer, float healthPercent){
    // Save the current render target
    SDL_Texture* currentTarget = SDL_GetRenderTarget(renderer);

    SDL_SetRenderTarget(renderer, healthBarTexture);

    // Clear the texture with transparent pixels
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int fillWidth = (int)(56 * healthPercent);

    SDL_Rect fillRect = { 19, 8, fillWidth, 4};
    SDL_Rect borderRect = { 0, 0, 76, 20};

    SDL_RenderCopy(renderer, fillTexture, NULL, &fillRect);
    SDL_RenderCopy(renderer, borderTexture, NULL, &borderRect);

    SDL_SetRenderTarget(renderer, currentTarget);
}
void HealthBar::render(SDL_Renderer *renderer, int screenX, int screenY){
	SDL_Rect renderQuad = {screenX, screenY, 76 * 2, 20 * 2};
	SDL_RenderCopy(renderer, healthBarTexture, NULL, &renderQuad);
}

