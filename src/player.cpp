#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "vector2d.h"
#include "player.h"


Player::Player(){
    position = Vector2D(INT_MAX / 2, INT_MAX / 2);
    velocity = Vector2D(0, 0);
}
int Player::getX(){
    return this->position.x;
}
int Player::getY(){
    return this->position.y;
}
Vector2D Player::getPosition(){
    return this->position;
}
int Player::getTextureWidth(){
    return this->textureWidth;
}
int Player::getTextureHeight(){
    return this->textureHeight;
}
void Player::clearData(){
    if(shipTexture == NULL ) return;
    SDL_DestroyTexture(shipTexture);
    shipTexture = NULL;
}
bool Player::loadTexture(SDL_Renderer *renderer, std::string path){
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

    textureWidth = 0.5 * loadedSurface->w;
    textureHeight = 0.5 * loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
	shipTexture = newTexture;

	return shipTexture != NULL;
}
void Player::render(SDL_Renderer *renderer, int x, int y, double angle){
	SDL_Rect renderQuad = {x, y, textureWidth, textureHeight};
	SDL_RenderCopyEx(renderer, shipTexture, NULL, &renderQuad, angle, NULL, SDL_FLIP_NONE);
}
void Player::accelerateInDirection(const int key_pressed){
    switch(key_pressed){
    case SDLK_s: velocity.y += Player::VEL; break;
    case SDLK_w: velocity.y -= Player::VEL; break;
    case SDLK_d: velocity.x += Player::VEL; break;
    case SDLK_a: velocity.x -= Player::VEL; break;
    }
}
void Player::decelerateInDirection(const int key_pressed){
    switch(key_pressed){
    case SDLK_s: velocity.y -= Player::VEL; break;
    case SDLK_w: velocity.y += Player::VEL; break;
    case SDLK_d: velocity.x -= Player::VEL; break;
    case SDLK_a: velocity.x += Player::VEL; break;
    }
}
void Player::movePlayer(){
    position += velocity;
}
