#ifndef TILED_BACKGROUND_H
#define TILED_BACKGROUND_H

class TiledBackground{
public:
    TiledBackground();
    void clearData();
    void setParalaxStrength(const double _paralaxStrength);
    bool loadTexture(SDL_Renderer *renderer, std::string path);
    void renderTile(SDL_Renderer *renderer, int x, int y);
    void renderSurroundedTiles(SDL_Renderer *renderer, ViewPort camera);
private:
    double paralaxStrength;
    SDL_Texture *tileTexture;
    int tileHeight, tileWidth;
};

#endif
