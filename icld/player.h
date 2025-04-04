#ifndef PLAYER_H
#define PLAYER_H

class Player{
public:
    static const int VEL = 15; //15 pixel per frame
    Player();
    void clearData();

    int getXCoord();
    int getYCoord();
    Vector2D getPosition();
    int getShipWidth();
    int getShipHeight();

    bool loadTexture(SDL_Renderer *renderer, std::string path);
    void render(SDL_Renderer *renderer, int screenX, int screenY, double angle);

    void accelerateInDirection(const int key_pressed);
    void decelerateInDirection(const int key_pressed);
    void movePlayer();

private:
    Vector2D position, velocity;
    SDL_Texture *shipTexture;
    int shipHeight, shipWidth;
};

#endif
