#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <cstdlib>
#include <cmath>
#include <list>
#include "vector2d.h"
#include "textureatlas.h"
#include "bullet.h"
#include "enemy.h"

const int Enemy::mapToAtlas[4][2] = {{0, 0},{2, 0},{0, 1},{3, 1}};
const Vector2D EnemySpawner::spawnDirection[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
Enemy::Enemy(int _type, Vector2D _position, Vector2D _direction, int _frameFireCycle){
    type = _type;
    position = _position;
    direction = _direction;
    frameFireCycle = _frameFireCycle;
}
void Enemy::renderEnemy(SDL_Renderer *renderer, TextureAtlas &enemyAtlas, Vector2D &camera){

    int cellIndexX = mapToAtlas[type][0];
    int cellIndexY = mapToAtlas[type][1];
    const double ScaleRatio = 0.5;
    Vector2D WinCoord = position - camera;

    SDL_Rect clip = {
        cellIndexX * enemyAtlas.cellWidth,
        cellIndexY * enemyAtlas.cellHeight,
        enemyAtlas.cellWidth, enemyAtlas.cellHeight
    };
    SDL_Rect renderQuad = {
        WinCoord.x - int(ScaleRatio * enemyAtlas.cellWidth) / 2,
        WinCoord.y - int(ScaleRatio * enemyAtlas.cellHeight) / 2,
        int(ScaleRatio * enemyAtlas.cellWidth), int(ScaleRatio * enemyAtlas.cellHeight)
    };
    SDL_RenderCopyEx(renderer, enemyAtlas.texture, &clip, &renderQuad, rotateAngle, NULL, SDL_FLIP_NONE);
}
void Enemy::moveEnemy(Vector2D playerPosition, std::list<Bullet> &bulletList){
    direction = playerPosition - position;
    if(direction.x == 0 && direction.y == 0) direction.x = 1;
    rotateAngle = std::atan2(direction.y, direction.x) * 180 / M_PI + 90;
    bool canShoot = false;
    if(frameFireCycle <= 0){
        frameFireCycle = 30;
        bulletList.push_back(Bullet(
            position.x, position.y,
            direction.x, direction.y, rotateAngle
        ));
    }
    else --frameFireCycle;
    //let enemy move at velocity <15 so it won't catchup with the bullet, it can't chase player with godspeed
    //maybe i will choose 10
}
void EnemySpawner::createInto(std::list<Enemy> &enemyList, Vector2D playerPostion){
    enemyList.push_back(Enemy(
        rand() % 4,
        spawnDirection[std::rand() % 8] * spawnDistance + playerPostion,
        Vector2D(1, 0),
        15
    ));
}
