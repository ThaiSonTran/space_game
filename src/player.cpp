#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <player.h>


Player::Player(){
    x = INT_MAX / 2;
    y = INT_MAX / 2;
    vX = 0;
    vY = 0;
}
int Player::getX(){
    return this->x;
}
int Player::getY(){
    return this->y;
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

    textureWidth = loadedSurface->w;
    textureHeight = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
	shipTexture = newTexture;

	return shipTexture != NULL;
}
void Player::render(SDL_Renderer *renderer, int x, int y){
	SDL_Rect renderQuad = {x, y, textureWidth, textureHeight};
	SDL_RenderCopy(renderer, shipTexture, NULL, &renderQuad);
}
void Player::increaseVelocity(const int key_pressed){
    switch(key_pressed){
    case SDLK_s: vY += Player::VEL; break;
    case SDLK_w: vY -= Player::VEL; break;
    case SDLK_d: vX += Player::VEL; break;
    case SDLK_a: vX -= Player::VEL; break;
    }
}
void Player::decreaseVelocity(const int key_pressed){
    switch(key_pressed){
    case SDLK_s: vY -= Player::VEL; break;
    case SDLK_w: vY += Player::VEL; break;
    case SDLK_d: vX -= Player::VEL; break;
    case SDLK_a: vX += Player::VEL; break;
    }
}
void Player::movePlayer(){
    x += vX;
    y += vY;
}
