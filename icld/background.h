#ifndef TILED_BACKGROUND_H
#define TILED_BACKGROUND_H

class TiledBackground{
public:
    TiledBackground();
    void resetData();
    bool loadTexture(SDL_Renderer *renderer, std::string path);
    void render(SDL_Renderer *renderer, int x, int y);
    void renderTiles(SDL_Renderer *renderer, int playerX, int playerY, ViewPort camera);
private:
    int posX, posY;
    SDL_Texture *texture;
    int textureHeight, textureWidth;
};

#endif
