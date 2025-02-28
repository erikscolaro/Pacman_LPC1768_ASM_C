#ifndef PLAYER_H
#define PLAYER_H

#include "assets/image.h"
#include <stdint.h>

typedef enum {
    DOWN,
		RIGHT,
		UP, 
		LEFT,
		STOP    
} Direction;

typedef struct {
    uint16_t posx;
    uint16_t posy;
    Direction newDirection; // Change direction to enum
    Direction oldDirection;
    uint8_t isAlive; // Add an attribute for visibility
    uint8_t id;
    uint8_t isVulnerable;
    const Image* image; // Change image attribute to a pointer
} Player;

extern Player pacman;
extern Player red_ghost;
//extern Player pink_ghost;
//extern Player orange_ghost;
//extern Player green_ghost;

inline uint8_t getIsAlive(Player* player) {
    return player->isAlive;
}

inline uint8_t getId(Player* player) {
    return player->id;
}

inline uint16_t getPosX(Player* player) {
    return player->posx;
}

inline uint16_t getPosY(Player* player) {
    return player->posy;
}
inline void setPosX(Player* player, uint16_t posx) {
    player->posx = posx;
}

inline void setPosY(Player* player, uint16_t posy) {
    player->posy = posy;
}

inline void setDirection(Player* player, Direction direction) {
    player->newDirection = direction;
}

inline Direction setOldDirection(Player* player, Direction oldDirection) {
    return player->oldDirection = oldDirection;
}

inline Direction getDirection(Player* player) {
    return player->newDirection;
}

inline Direction getOldDirection(Player* player) {
    return player->oldDirection;
}

inline void setIsAlive(Player* player, uint8_t isAlive) {
    player->isAlive = isAlive;
}

inline const Image* getImage(Player* player) {
    return player->image;
}

inline uint8_t getIsVulnerable(Player* player) {
    return player->isVulnerable;
}

inline void setIsVulnerable(Player* player, uint8_t isVulnerable) {
    player->isVulnerable = isVulnerable;
}

const Image* updateImage(Player* player);

void movePlayer(Player* player, Direction dir, uint16_t pixels);
void pacmanIsHunter();
void ghostAreHunters();

#endif // PLAYER_H
