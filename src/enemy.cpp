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
Vector2D Enemy::getPosition(){
    return this->position;
}
Enemy::Enemy(int _type, Vector2D _position, Vector2D _direction, int _frameFireCycle, int _currentHealth){
    type = _type;
    position = _position;
    direction = _direction;
    frameFireCycle = _frameFireCycle;
    currentHealth = _currentHealth;
}
void Enemy::renderEnemy(SDL_Renderer *renderer, TextureAtlas &enemyAtlas, Vector2D &camera){

    int cellIndexX = mapToAtlas[type][0];
    int cellIndexY = mapToAtlas[type][1];

    Vector2D WinCoord = position - camera;

    SDL_Rect clip = {
        cellIndexX * enemyAtlas.cellWidth,
        cellIndexY * enemyAtlas.cellHeight,
        enemyAtlas.cellWidth, enemyAtlas.cellHeight
    };
    SDL_Rect renderQuad = {
        WinCoord.x - int(SCALE_RATIO * enemyAtlas.cellWidth) / 2,
        WinCoord.y - int(SCALE_RATIO * enemyAtlas.cellHeight) / 2,
        int(SCALE_RATIO * enemyAtlas.cellWidth), int(SCALE_RATIO * enemyAtlas.cellHeight)
    };
    SDL_RenderCopyEx(renderer, enemyAtlas.texture, &clip, &renderQuad, rotateAngle, NULL, SDL_FLIP_NONE);
}
void Enemy::moveEnemy(Vector2D playerPosition, std::list<Bullet> &bulletList){
    direction = playerPosition - position;
    double len = sqrt(1LL * direction.x * direction.x + 1LL * direction.y * direction.y);
    if(len == 0){
        direction.x = 1;
        position.x += Enemy::VEL;
    }
    else{
        double deltaX = direction.x / len;
        double deltaY = direction.y / len;
        position += Vector2D(int(deltaX * Enemy::VEL), int(deltaY * Enemy::VEL));
    }
    rotateAngle = std::atan2(direction.y, direction.x) * 180 / M_PI + 90;

    if(frameFireCycle <= 0){
        frameFireCycle = 30;
        bulletList.push_back(Bullet(
            position.x, position.y,
            direction.x, direction.y, rotateAngle, 1
        ));
    }
    else --frameFireCycle;
}
void Enemy::decreaseCurrentHealth(int decreaseAmount){
        currentHealth -= decreaseAmount;
}
bool Enemy::isDead(){
    return currentHealth <= 0;
}

const Vector2D EnemySpawner::spawnDirection[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

void EnemySpawner::createInto(std::list<Enemy> &enemyList, Vector2D playerPostion){
    enemyList.push_back(Enemy(
        rand() % 4,
        spawnDirection[std::rand() % 8] * spawnDistance + playerPostion,
        Vector2D(1, 0),
        15, 4
    ));
}
