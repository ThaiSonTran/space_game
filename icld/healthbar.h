#ifndef HEALTHBAR_H
#define HEALTHBAR_H
struct HealthBar{
    bool loadTexture(SDL_Renderer *renderer, std::string path, SDL_Texture *&texture);
    void updateHealthBarTexture(SDL_Renderer* renderer, float healthPercent);
    void render(SDL_Renderer *renderer, int screenX, int screenY);

    SDL_Texture *borderTexture, *fillTexture;
    SDL_Texture *healthBarTexture;
};

#endif // HEALTHBAR_H
