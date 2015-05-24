#ifndef CCLOCK_FRAMEBUFFER_H
#define CCLOCK_FRAMEBUFFER_H


#include <stddef.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <stdexcept>


class FrameBuffer {
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

    FrameBuffer() {


    };

    bool init(const char *frameBufferPath) {
        int frameBufferHandle = open(frameBufferPath, O_RDWR);

        if (frameBufferHandle == -1) {
            throw std::runtime_error(strerror(errno));
        }

        if (!ioctl(frameBufferHandle, FBIOGET_VSCREENINFO, &screen_info) &&
            !ioctl(frameBufferHandle, FBIOGET_FSCREENINFO, &fixed_info)) {
            width = screen_info.width;
            height = screen_info.height;

            bufferLength = screen_info.yres_virtual * fixed_info.line_length;
            buffer = (char *) mmap(NULL,
                                   bufferLength,
                                   PROT_READ | PROT_WRITE,
                                   MAP_SHARED,
                                   frameBufferHandle,
                                   0);

            bitsPerPixel = screen_info.bits_per_pixel;
            close(frameBufferHandle);
            if (buffer == MAP_FAILED) {
                throw std::runtime_error("mapping failed");
            }
        } else {
            close(frameBufferHandle);
            throw std::runtime_error(strerror(errno));
        }
    }

    void updatePixel(int x, int y, SDL_Color color) {
        if (x > width || x < 0
            || y > height || y < 0) {
            return;
        };

        long int location = x * bitsPerPixel / 8 + y * fixed_info.line_length;
        *((unsigned short int *) (buffer + location)) = rgbToPixelValue(color);
    }

    void clear() {
        memset(&buffer[0], 0, bufferLength);
    }

    ~FrameBuffer() {
        if (buffer && buffer != MAP_FAILED) {
            munmap(buffer, bufferLength);
        }
    };
};

#endif //CCLOCK_BUFFER_H
