#include <iostream>
#include <unistd.h>
#include <fcntl.h>

#include "FrameBuffer.h"
#include "SDLManager.h"
#include <ctime>

using namespace std;



struct tm * getCurrentTime() {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return timeinfo;
}

int main() {



    int fd = open("/dev/fb1", O_RDWR);
    char textBuffer[80];

    if (fd >= 0) {
        FrameBuffer *buffer = new FrameBuffer;
        buffer->init(fd);
        SDLManager *sdlManager = new SDLManager(buffer);
        sdlManager->init();
        SDL_Color textColor = {46, 199, 110};
        while(1) {
            struct tm * timeInfo = getCurrentTime();
            strftime(textBuffer, 80, "%I:%M:%S", timeInfo);
            if (!sdlManager->renderText(textBuffer, textColor, 48, 60, 60)) {
                break;
            };
            strftime(textBuffer, 80, "%d-%m-%Y", timeInfo);
            if (!sdlManager->renderText(textBuffer, textColor, 24, 95, 120)) {
                break;
            };
            sdlManager->updateFrameBuffer();
            sleep(1);
        }

        delete buffer;
        delete sdlManager;
    } else {
        perror("open");
    }

    /*
    * Clean up
    */


    if (fd >= 0) {
        close(fd);
    }

    return 0;
};