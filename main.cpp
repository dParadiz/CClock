#include <iostream>
#include <unistd.h>
#include <fcntl.h>

#include "FrameBuffer.h"
#include "SDLManager.h"
#include <ctime>
#include <stdexcept>

using namespace std;



struct tm * getCurrentTime() {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return timeinfo;
}

int main() {

    FrameBuffer *buffer = new FrameBuffer;
    SDLManager *sdlManager = new SDLManager();
    SDL_Color textColor = {46, 199, 110};


    char textBuffer[80];
    try {

            buffer->init("/dev/fb1");
            sdlManager->init(buffer->width, buffer->height, buffer->bitsPerPixel);

            while (1) {
                struct tm *timeInfo = getCurrentTime();
                strftime(textBuffer, 80, "%I:%M:%S", timeInfo);

                if (!sdlManager->renderText(textBuffer, textColor, 48, 60, 60)) {
                    break;
                };
                strftime(textBuffer, 80, "%d-%m-%Y", timeInfo);
                if (!sdlManager->renderText(textBuffer, textColor, 24, 95, 120)) {
                    break;
                };
                sdlManager->updateFrameBuffer(buffer);

                sleep(1);
            }

            delete buffer;
            delete sdlManager;

    } catch (exception &e) {
        fprintf(stderr, "%s\n", e.what());
        delete buffer;
        delete sdlManager;
    }


    return 0;
};