#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cmath>
#include <string>
#include "textureatlas.h"
#include "vector2d.h"
#include "bullet.h"

Bullet::Bullet(int _x, int _y, int _dirX, int _dirY, double _rotateAngle, bool _isEnemyBullet = false){
    position = Vector2D(_x, _y);
    direction = Vector2D(_dirX, _dirY);
    rotateAngle = _rotateAngle;
    isEnemyBullet = _isEnemyBullet;
}
Vector2D Bullet::getPosition(){
    return this->position;
}
void Bullet::renderBullet(SDL_Renderer *renderer, int cellIndexX, int cellIndexY, TextureAtlas &bulletAtlas, Vector2D &camera){
    Vector2D WinCoord = position - camera;
    SDL_Rect clip = {
        cellIndexX * bulletAtlas.cellWidth,
        cellIndexY * bulletAtlas.cellHeight,
        bulletAtlas.cellWidth, bulletAtlas.cellHeight
    };
    SDL_Rect renderQuad = {
        WinCoord.x - int(SCALE_RATIO * bulletAtlas.cellWidth) / 2,
        WinCoord.y - int(SCALE_RATIO * bulletAtlas.cellHeight) / 2,
        int(SCALE_RATIO * bulletAtlas.cellWidth), int(SCALE_RATIO * bulletAtlas.cellHeight)
    };
    SDL_RenderCopyEx(renderer, bulletAtlas.texture, &clip, &renderQuad, rotateAngle, NULL, SDL_FLIP_NONE);
}
void Bullet::moveBullet(){
    double len = sqrt(1LL * direction.x * direction.x + 1LL * direction.y * direction.y);
    if(len == 0){
        position.x += Bullet::VEL;
        return;
    }
    double deltaX = direction.x / len;
    double deltaY = direction.y / len;
    position += Vector2D(int(deltaX * Bullet::VEL), int(deltaY * Bullet::VEL));
}
bool Bullet::isTooFar(int x, int y){
    int deltaX = position.x - x;
    int deltaY = position.y - y;
    return 1LL * deltaX * deltaX + 1LL * deltaY * deltaY >= 100000000;
}
