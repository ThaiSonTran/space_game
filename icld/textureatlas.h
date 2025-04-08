#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H
struct TextureAtlas{
    SDL_Texture *texture;
    int textureWidth, textureHeight;
    int cellWidth, cellHeight;

    void clearData();

    bool loadTexture(SDL_Renderer *renderer, std::string path);

    void calculateCellSize(int numCellWide, int numCellTall);
};

#endif // TEXTUREATLAS_H
