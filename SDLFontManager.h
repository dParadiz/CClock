//
// Created by dejan on 23.5.2015.
//

#ifndef CCLOCK_SDLFONTMANAGER_H
#define CCLOCK_SDLFONTMANAGER_H

#include <assert.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <iostream>

class SDL_Font {
    std::string FontLocation;
    std::map<int, TTF_Font *> FontSizes;

public:
    //Save the path to the TTF file and do some error checking string cant be empty
    bool load(const std::string &FileLocation) {
        FontLocation = FileLocation;

        if (FileLocation == "")
            assert(false);
        TTF_Init();
        return true;
    }

    // loads and returns a TTF_Font.
    // looks in the map if the font already exists if not create a new font with the requested size
    TTF_Font *getFont(int FontSize) {
        std::map<int, TTF_Font *>::iterator it;
        it = FontSizes.find(FontSize);

        if (it != FontSizes.end())
            return it->second;
        else {
            TTF_Font *mFont = TTF_OpenFont(FontLocation.c_str(), FontSize);
            FontSizes.insert(std::pair<int, TTF_Font *>(FontSize, mFont));
            return mFont;
        }
    }

    ~SDL_Font() {
        for (std::map<int, TTF_Font *>::iterator it = FontSizes.begin(); it != FontSizes.end(); ++it) {
            TTF_CloseFont(it->second);
        }
    }
};

#endif //CCLOCK_SDLFONTMANAGER_H
