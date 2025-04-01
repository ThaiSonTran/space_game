#ifndef BULLET_H
#define BULLET_H
class Bullet{
public:
    //bool loadTexture(SDL_Renderer *renderer, std::string path);
    //void render(SDL_Renderer *renderer, int x, int y);

private:
    int x, y;
    int dirX, dirY;
    //SDL_Texture bulletTexture;
    int textureWidth, textureHeight;
};
#endif // BULLET_H
