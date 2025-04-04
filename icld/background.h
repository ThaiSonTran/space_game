#ifndef TILED_BACKGROUND_H
#define TILED_BACKGROUND_H

class TiledBackground{
public:
    static const int SCALE_RATIO = 4;
    TiledBackground();
    void clearData();
    void setParallaxStrength(const double _strengthFactor);
    bool loadTexture(SDL_Renderer *renderer, std::string path);
    void renderTile(SDL_Renderer *renderer, int screenX, int screenY);
    void renderSurroundedTiles(SDL_Renderer *renderer, Vector2D &camera);
private:
    float parallaxStrength;
    SDL_Texture *tileTexture;
    int tileHeight, tileWidth;
};

#endif
