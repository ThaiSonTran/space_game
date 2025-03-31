#ifndef PLAYER_H
#define PLAYER_H

class Player{
public:
    static const int VEL = 15;
    Player();
    void freeData();
    int getX();
    int getY();
    bool loadTexture(SDL_Renderer *renderer, std::string path);
    void render(SDL_Renderer *renderer, int x, int y);
    void increaseVelocity(const SDL_Keysym key_pressed);
    void decreaseVelocity(const SDL_Keysym key_pressed);
    void movePlayer();
private:
    int x, y;
    int vX, vY;
    SDL_Texture *shipTexture;
    int textureHeight, textureWidth;
};

#endif
