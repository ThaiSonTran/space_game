#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cmath>
#include <string>
#include "textureatlas.h"
#include "vector2d.h"
#include "bullet.h"

Bullet::Bullet(int _x, int _y, int _dirX, int _dirY, double _rotateAngle){
    position = Vector2D(_x, _y);
    direction = Vector2D(_dirX, _dirY);
    rotateAngle = _rotateAngle;
}
void Bullet::renderBullet(SDL_Renderer *renderer, int cellIndexX, int cellIndexY, TextureAtlas &bulletAtlas, Vector2D &camera){
    const double ScaleRatio = 0.05;

    Vector2D WinCoord = position - camera;
    SDL_Rect clip = {
        cellIndexX * bulletAtlas.cellWidth,
        cellIndexY * bulletAtlas.cellHeight,
        bulletAtlas.cellWidth, bulletAtlas.cellHeight
    };
    SDL_Rect renderQuad = {
        WinCoord.x - int(ScaleRatio * bulletAtlas.cellWidth) / 2,
        WinCoord.y - int(ScaleRatio * bulletAtlas.cellHeight) / 2,
        int(ScaleRatio * bulletAtlas.cellWidth), int(ScaleRatio * bulletAtlas.cellHeight)
    };
    SDL_RenderCopyEx(renderer, bulletAtlas.texture, &clip, &renderQuad, rotateAngle, NULL, SDL_FLIP_NONE);
}
void Bullet::moveBullet(){
    double len = sqrt(1LL * direction.x * direction.x + 1LL * direction.y * direction.y);
    if(len == 0){
        position.x += 25;
        return;
    }
    double deltaX = direction.x / len;
    double deltaY = direction.y / len;
    position += Vector2D(int(deltaX * 25), int(deltaY * 25));
}
bool Bullet::isTooFar(int x, int y){
    int deltaX = position.x - x;
    int deltaY = position.y - y;
    return 1LL * deltaX * deltaX + 1LL * deltaY * deltaY >= 100000000;
}
