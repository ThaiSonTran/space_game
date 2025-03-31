#ifndef PLAYER_H
#define PLAYER_H

class Player{
public:
    void freeData();
    bool loadTexture(SDL_Renderer *renderer, std::string path);
    void render(SDL_Renderer *renderer, int x, int y);
private:
    int x, y;
    SDL_Texture *shipTexture;
    int textureHeight, textureWidth;
};

#endif
