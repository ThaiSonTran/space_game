#ifndef BULLET_H
#define BULLET_H
class Bullet{
public:
    //bool loadTexture(SDL_Renderer *renderer, std::string path);
    //void render(SDL_Renderer *renderer, int x, int y);
    Bullet(int _x, int _y, int _dirX, int _dirY, double _rotateAngle);
    void renderBullet(SDL_Renderer *renderer, int cellIndexX, int cellIndexY, TextureAtlas &bulletAtlas, Vector2D &camera);
    void moveBullet();
    bool isTooFar(int x, int y);
private:
    Vector2D position;
    Vector2D direction;
    double rotateAngle;
};
#endif // BULLET_H
