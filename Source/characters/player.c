#include "player.h"

static uint8_t pacman_frame = 0;
static uint8_t ghostAreVulnerable = 0;

Player pacman = {
    .posx = 0,
    .posy = 0,
    .newDirection = RIGHT,
    .oldDirection = STOP,
    .isAlive = 1,
    .id = 0,
    .isVulnerable = 1,
    .image = (Image*) &right_1_image
};

Player red_ghost = {
    .posx = 0,
    .posy = 0,
    .newDirection = STOP,
    .oldDirection = STOP,
    .isAlive = 0,
    .id = 1,
    .isVulnerable = 0,
    .image = (Image*) &blinky_image
};

/*
Player pink_ghost = {
    .posx = 0,
    .posy = 0,
    .newDirection = STOP,
    .oldDirection = STOP,
    .isAlive = 0,
    .id = 2,
    .isVulnerable = 0,
    .image = (Image*) &pinky_image
};

Player orange_ghost = {
    .posx = 0,
    .posy = 0,
    .newDirection = STOP,
    .oldDirection = STOP,
    .isAlive = 0,
    .id = 3,
    .isVulnerable = 0,
    .image = (Image*) &clyde_image
};

Player green_ghost = {
    .posx = 0,
    .posy = 0,
    .newDirection = STOP,
    .oldDirection = STOP,
    .isAlive = 0,
    .id = 4,
    .isVulnerable = 0,
    .image = (Image*) &inky_image
};
*/

const Image* updateImage(Player* player) {
    if (player->id==0) {
        switch (player->oldDirection) {
						case UP:
								player->image = pacman_frame == 0 ? &up_1_image :
																					pacman_frame == 1 ? &up_2_image : &up_3_image;
								break;
						case DOWN:
								player->image = pacman_frame == 0 ? &down_1_image :
																					pacman_frame == 1 ? &down_2_image : &down_3_image;
								break;
						case LEFT:
								player->image = pacman_frame == 0 ? &left_1_image :
																					pacman_frame == 1 ? &left_2_image : &left_3_image;
								break;
						case RIGHT:
								player->image = pacman_frame == 0 ? &right_1_image :
																					pacman_frame == 1 ? &right_2_image : &right_3_image;
								break;
						default:
								break;
				}
        pacman_frame = (pacman_frame + 1) % 3;
    } else {
        if (getIsVulnerable(player)) {
            player->image = (player->image == &blue_ghost_image) ? &white_ghost_image : &blue_ghost_image ;
        } else {
            player->image = &blinky_image;
            /*
                        switch (player->id) {
                            case 1:
                                player->image = &blinky_image;
                                break;
                            case 2:
                                player->image = &pinky_image;
                                break;
                            case 3:
                                player->image = &clyde_image;
                                break;
                            case 4:
                                player->image = &inky_image;
                                break;
                            default:
                                break;
                        }
            */
        }
    }
    return player->image;
}

void movePlayer(Player* player, Direction dir, uint16_t pixels) {
    switch (dir) {
        case UP:
            player->posy -= pixels;
            break;
        case DOWN:
            player->posy += pixels;
            break;
        case LEFT:
            player->posx -= pixels;
            break;
        case RIGHT:
            player->posx += pixels;
            break;
        case STOP:
        default:
            // Do nothing if the player is stopped or direction is invalid
            break;
    }
}

void pacmanIsHunter() {
    setIsVulnerable(&pacman, 0);
    setIsVulnerable(&red_ghost, 1);
    // setIsVulnerable(&pink_ghost, 1);
    // setIsVulnerable(&orange_ghost, 1);
    // setIsVulnerable(&green_ghost, 1);
}

void ghostAreHunters(){
    setIsVulnerable(&pacman, 1);
    setIsVulnerable(&red_ghost, 0);
    // setIsVulnerable(&pink_ghost, 0);
    // setIsVulnerable(&orange_ghost, 0);
    // setIsVulnerable(&green_ghost, 0);
}

