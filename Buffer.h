//
// Created by dejan on 20.5.2015.
//

#ifndef CCLOCK_BUFFER_H
#define CCLOCK_BUFFER_H


#include <stddef.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include <SDL2/SDL_pixels.h>

class Buffer {
protected:
    char *buffer = NULL;
    struct fb_var_screeninfo screen_info;
    struct fb_fix_screeninfo fixed_info;
    size_t bufferLength;

    unsigned short rgbToPixelValue(SDL_Color color) {
        return (unsigned short) (color.r << 11 | color.b << 5 | color.g);
    }

public:
    int width;
    int height;
    int bitsPerPixel;
    Buffer() {


    };

    bool init(int frameBufferHandle) {

        if (frameBufferHandle >= 0) {
            if (!ioctl(frameBufferHandle, FBIOGET_VSCREENINFO, &screen_info) &&
                !ioctl(frameBufferHandle, FBIOGET_FSCREENINFO, &fixed_info)) {
                fprintf(stderr, "%d %d\n", screen_info.bits_per_pixel, screen_info.width);
                bufferLength = screen_info.yres_virtual * fixed_info.line_length;
                buffer = (char *) mmap(NULL,
                                       bufferLength,
                                       PROT_READ | PROT_WRITE,
                                       MAP_SHARED,
                                       frameBufferHandle,
                                       0);
                width = screen_info.width;
                height = screen_info.height;
                bitsPerPixel = screen_info.bits_per_pixel;

                return buffer != MAP_FAILED;
            }
        }
        return false;
    }

    void drawPixel(int x, int y, SDL_Color color) {
        if (   x > screen_info.width
            || x < 0
            || y > screen_info.height
            || y < 0
        ) {

            return;
        };

        long int location = x * screen_info.bits_per_pixel / 8 + y * fixed_info.line_length;
        *((unsigned short int *) (buffer + location)) = rgbToPixelValue(color);
    }
    void clear() {
        memset(&buffer[0], 0, bufferLength);
    }

    ~Buffer() {
        if (buffer && buffer != MAP_FAILED) {
            munmap(buffer, bufferLength);
        }
    };
};

#endif //CCLOCK_BUFFER_H
