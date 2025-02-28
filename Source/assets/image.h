#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

typedef struct {
    const int width;
    const int height;
    const uint8_t *data;
} Image;

extern const Image blinky_image;
extern const Image blue_ghost_image;
//extern const Image clyde_image;
extern const Image down_1_image;
extern const Image down_2_image;
extern const Image down_3_image;
//extern const Image inky_image;
extern const Image left_1_image;
extern const Image left_2_image;
extern const Image left_3_image;
//extern const Image pinky_image;
extern const Image right_1_image;
extern const Image right_2_image;
extern const Image right_3_image;
extern const Image up_1_image;
extern const Image up_2_image;
extern const Image up_3_image;
extern const Image white_ghost_image;
extern const Image big_dot_image;
extern const Image small_dot_image;
extern const Image ready_image;
extern const Image maze_image;

inline int getWidth(const Image *img) {
    return img->width;
}

inline int getHeight(const Image *img) {
    return img->height;
}

inline const uint8_t* getData(const Image *img) {
    return img->data;
}

#endif // IMAGE_H