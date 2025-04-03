#ifndef PLAYER_H
#define PLAYER_H

class Player{
public:
    static const int VEL = 15;
    Player();
    void clearData();

    int getX();
    int getY();
    Vector2D getPosition();
    int getTextureWidth();
    int getTextureHeight();

    bool loadTexture(SDL_Renderer *renderer, std::string path);
    void render(SDL_Renderer *renderer, int screenX, int screenY, double angle);

    void accelerateInDirection(const int key_pressed);
    void decelerateInDirection(const int key_pressed);
    void movePlayer();

private:
    //int x, y;
    //int vX, vY;
    Vector2D position, velocity;
    SDL_Texture *shipTexture;
    int textureHeight, textureWidth;
};

#endif
