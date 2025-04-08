#ifndef BULLET_H
#define BULLET_H
class Bullet{
public:
    static const int VEL = 20;
    static constexpr double SCALE_RATIO = 0.05;
    bool isEnemyBullet;

    Bullet(int _x, int _y, int _dirX, int _dirY, double _rotateAngle, bool _isEnemyBullet);

    Vector2D getPosition();

    void renderBullet(SDL_Renderer *renderer, int cellIndexX, int cellIndexY, TextureAtlas &bulletAtlas, Vector2D &camera);

    void moveBullet();
    bool isTooFar(int x, int y);
private:
    Vector2D position;
    Vector2D direction;
    double rotateAngle;
};
#endif // BULLET_H
