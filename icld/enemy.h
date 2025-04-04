#ifndef ENEMY_H
#define ENEMY_H
class Enemy{
public:
    static const int mapToAtlas[4][2];
    Enemy(int _type, Vector2D _position, Vector2D _direction, int _frameFireCycle);
    void renderEnemy(SDL_Renderer *renderer, TextureAtlas &enemyAtlas, Vector2D &camera);
    void moveEnemy(Vector2D playerPosition, std::list<Bullet> &bulletList);
private:
    int type;
    Vector2D position;
    Vector2D direction;
    int currentHealth;
    int frameFireCycle;
    int maximumHealth;
    double rotateAngle;
};
class EnemySpawner{
public:
    static const int spawnDistance = 200;
    static const Vector2D spawnDirection[8];
    void createInto(std::list<Enemy> &enemyList, Vector2D playerPostion);

};
#endif // ENEMY_H
