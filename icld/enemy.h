#ifndef ENEMY_H
#define ENEMY_H
class Enemy{
public:
    static const int VEL = 8;
    static const int mapToAtlas[4][2];
    static constexpr double SCALE_RATIO = 0.5;

    Enemy(int _type, Vector2D _position, Vector2D _direction, int _frameFireCycle, int _currentHealth);

    Vector2D getPosition();

    void renderEnemy(SDL_Renderer *renderer, TextureAtlas &enemyAtlas, Vector2D &camera);

    void moveEnemy(Vector2D playerPosition, std::list<Bullet> &bulletList);

    void decreaseCurrentHealth(int decreaseAmount);
    bool isDead();
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
    static const int spawnDistance = 700;
    static const Vector2D spawnDirection[8];
    void createInto(std::list<Enemy> &enemyList, Vector2D playerPostion);

};
#endif // ENEMY_H
