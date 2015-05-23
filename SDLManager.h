#ifndef CCLOCK_SDLMANAGER_H
#define CCLOCK_SDLMANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "FrameBuffer.h"
#include "SDLFontManager.h"

class SDLManager {
protected:
    SDL_Surface *mainSurface = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Font *fonts = nullptr;
    FrameBuffer *frameBuffer = nullptr;

    Uint32 getpixel(SDL_Surface *surface, int x, int y) {
        int bpp = surface->format->BytesPerPixel;
        /* Here p is the address to the pixel we want to retrieve */
        Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

        switch (bpp) {
            case 1:
                return *p;
            case 2:
                return *(Uint16 *) p;
            case 3:
                if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                    return p[0] << 16 | p[1] << 8 | p[2];
                else
                    return p[0] | p[1] << 8 | p[2] << 16;
            case 4:
                return *(Uint32 *) p;
            default:
                return 0;
        }
    }

public:
    SDLManager(FrameBuffer *_buffer) {
        frameBuffer = _buffer;
    };

    ~SDLManager() {
        if (nullptr != mainSurface) {
            SDL_FreeSurface(mainSurface);
        }

        if (nullptr != renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (nullptr != fonts) {
            delete fonts;
        }


    }

    bool init() {
        //init main surface
        mainSurface = SDL_CreateRGBSurface(0, frameBuffer->width, frameBuffer->width, frameBuffer->bitsPerPixel, 0, 0, 0, 255);
        if (mainSurface == nullptr) {
            fprintf(stderr, "MAin surface creation failed %s", SDL_GetError());
            return false;
        }
        //prepare renderer
        renderer = SDL_CreateSoftwareRenderer(mainSurface);

        //prepare fonts


        fonts = new SDL_Font();
        fonts->load("resources/fonts/UbuntuMono.ttf");

        return true;
    }

    bool renderText(const char *text, SDL_Color color, int size, int x = 0, int y = 0) {
        TTF_Font* font = fonts->getFont(size);
        SDL_Surface *fontSurface = TTF_RenderText_Solid(font, text, color);

        if (fontSurface == nullptr) {
            fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
            return false;
        }

        SDL_Rect SrcR;
        SDL_Rect DestR;

        SrcR.h = fontSurface->h;
        SrcR.w = fontSurface->w;
        SrcR.x = 0;
        SrcR.y = 0;

        DestR.x = mainSurface->w / 2 - fontSurface->w/2 ;
        DestR.y = y;
        DestR.w = fontSurface->w;
        DestR.h = fontSurface->h;

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, fontSurface);
        if (texture == nullptr) {
            fprintf(stderr, "Faild to create texture: %s\n", SDL_GetError());
            return false;
        }
        SDL_FreeSurface(fontSurface);

        SDL_RenderCopy(renderer, texture, &SrcR, &DestR);
        return true;
    }

    void updateFrameBuffer() {
        frameBuffer->clear();
        SDL_RenderPresent(renderer);
        for (int x = 0; x < mainSurface->w; x++) {
            for (int y = 0; y < mainSurface->h; y++) {
                Uint32 pixel = getpixel(mainSurface, x, y);
                SDL_Color color;
                SDL_GetRGB(pixel, mainSurface->format, &color.r, &color.g, &color.b);
                frameBuffer->updatePixel(x, y, color);
            }
        }
        SDL_RenderClear(renderer);
    }


};

#endif //CCLOCK_SDLMANAGER_H
