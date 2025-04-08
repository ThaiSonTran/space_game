#ifndef PLAYER_H
#define PLAYER_H

class Player{
public:
    static const int VEL = 12; //15 pixel per frame
    static constexpr double SCALE_RATIO = 0.5;
    Player();
    void clearData();

    int getXCoord();
    int getYCoord();
    Vector2D getPosition();
    int getShipWidth();
    int getShipHeight();

    bool loadTexture(SDL_Renderer *renderer, std::string path);
    void render(SDL_Renderer *renderer, int screenX, int screenY, double angle);

    void accelerateInDirection(const int keyPressed);
    void decelerateInDirection(const int keyPressed);
    void movePlayer();

    void decreaseCurrentHealth(int decreaseAmount);
    bool isDead();
    float calculateHealthRatio();

    void resetHealth() {
        currentHealth = 50;
    }


private:
    Vector2D position, velocity;
    SDL_Texture *shipTexture;
    int shipHeight, shipWidth;
    int currentHealth;
};

#endif
