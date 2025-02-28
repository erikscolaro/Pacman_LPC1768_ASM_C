#include "GLCD.h"
#include <stdlib.h>
#include <stdio.h>
#include "characters/player.h"
#include "timer/timer.h"
#include "adc/adc.h"
#include "button_EXINT/button.h"
#include "speaker/speaker.h"
#include "CAN/CAN.h"

// Game settings
#define REFRESH_RATE           60  // 30fps

// Player speeds
#define PACMAN_SPEED           240  // pixel per second
#define GHOST_SPEED            120  // pixel per second
uint16_t PACMAN_PPF = PACMAN_SPEED / REFRESH_RATE;  // 2 pixels per frame
uint16_t GHOST_PPF = GHOST_SPEED / REFRESH_RATE;    // 2 pixels per frame
uint8_t needToIncrement=0;

// Hunting mode settings
#define HUNTING_MODE_DURATION_MS 10000
#define RESPAWN_COUNTDOWN_MS 3000

// Points
#define SMALL_DOT_POINTS       10  // Points for small dot
#define BIG_DOT_POINTS         50  // Points for big dot
#define GHOST_ATE_POINTS 			 100 // Points for eating ghost in hunting mode

// Board dimensions
#define XPOS_NUM               26  // Number of horizontal positions
#define YPOS_NUM               29  // Number of vertical positions
#define HORZ_SPACING_DOTS      8   // Horizontal spacing between dots
#define VERT_SPACING_DOTS      8   // Vertical spacing between dots
#define DOT_NUM                240 // Total number of dots
#define BIG_DOT_NUM                       6   // Total number of big dots
#define BIG_DOT_RANDOM_MODULE    11  // Value used in evaluating random number generation

// Image printing tuning
#define PLAYERS_OFFSET 1
#define BIG_DOT_OFFSET 4
#define SMALL_DOT_OFFSET 7

const uint16_t xcc[XPOS_NUM] = {
    12, 20, 28, 36, 44, 52, 60, 68, 76, 84, 92, 100, 108, 116, 124, 132, 140, 148, 156, 164, 172, 180, 188, 196, 204, 212
};

const uint16_t ycc[YPOS_NUM] = {
    33, 41, 49, 57, 65, 73, 81, 89, 97, 105, 113, 121, 129, 137, 145, 153, 161, 169, 177, 185, 193, 201, 209, 217, 225, 233, 241, 249, 257
};

const uint8_t positionConst[YPOS_NUM][XPOS_NUM] = { 
    {1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1},
    {1,1,1,1,1,1,0,0,1,1,2,2,0,0,2,2,1,1,0,0,1,1,1,1,1,1},
    {0,0,0,0,0,1,0,0,0,0,0,2,0,0,2,0,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,0,2,0,0,2,0,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,2,2,2,2,2,2,2,2,2,2,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,2,0,0,0,5,5,0,0,0,2,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,2,0,0,0,2,2,0,0,0,2,0,0,1,0,0,0,0,0},
    {3,2,2,2,2,1,2,2,2,0,0,0,2,2,0,0,0,2,2,2,1,2,2,2,2,3},
    {0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,2,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,2,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,2,2,2,2,2,2,2,2,2,2,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,2,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,2,0,0,1,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1},
    {1,1,1,0,0,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,0,0,1,1,1},
    {0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0},
    {0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0},
    {1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Definizione di un enum per identificare gli indici delle stringhe
typedef enum {
    GAME_OVER = 0,
    YOUR_SCORE,
    PRESS_RESTART,
    YOU_WIN,
    CONGRATULATIONS,
    SCORE_LABEL,
	PAUSED_LABEL,
	COUNTDOWN_LABEL
} GameStringsIndex;

// Unica variabile per tutte le stringhe
const char *gameStrings[] = {
    "GAME OVER",               // 0
    "Your Score:",             // 1
    "Press INT0 to restart",   // 2
    "YOU WIN!",                // 3
    "Congratulations!",        // 4
    "SCORE",             	   // 5
	"PAUSED",				   // 6
	"COUNTDOWN"								
	
};

typedef struct {
    uint16_t points;
    uint16_t randomSeed;
    uint16_t eatenDots;
    uint8_t lifes;
    uint8_t remainingTime;
    Status status;
    uint8_t isStatusChanged;
    uint8_t position[YPOS_NUM][XPOS_NUM];
} GameState;

volatile GameState gameState;

// Functions for player management
uint8_t isValidMove(Player* player, Direction direction); // Check if a move is valid for the player
void updatePlayerPosition(Player* player); // Update the position of a single player
void updatePlayersPosition(void); // Update the positions of all players

// Functions for graphics management
void printDots(void); // Render dots on the screen
void printPlayers(void); // Render players on the screen
void printGameCountdown(uint8_t withText); // Display the countdown timer, optionally with text
void printLives(void); // Display the remaining lives
void printPoints(uint8_t withText); // Display the current points, optionally with text
void printAllInfo(); // Display all game information (lives, points, etc.)

// Functions for game state management
void updatePoints(uint16_t points); // Update the player's points
void subtractLife(void); // Decrease the player's remaining lives
void addLife(void); // Increase the player's remaining lives

Direction findNextMove(uint8_t ghostXindex, uint8_t ghostYindex, 
                      uint8_t pacmanXindex, uint8_t pacmanYindex, uint8_t isHunted);

/**
 * @brief Initializes the game state and waits for the INT0 input to begin the game.
 * 
 * This function sets up the initial game state, including points, lives, remaining time,
 * status, and eaten dots. It also initializes the ADC for a random seed, clears the LCD
 * screen, sets the cursor to the center, and prints a message prompting the user to press
 * INT0 to start the game. Additionally, it creates an IRQ for button 0.
 * 
 * @note This function should be called to prepare the game before starting.
 */
void gameWaiting() {
		// init at first to let the adc read a random seed value
		ADC_init();                          // Initialize ADC for random seed
    ADC_start_conversion();
    LCD_Clear(Black);                    // Clear screen
    GUI_Text(20, 160, (uint8_t *) gameStrings[PRESS_RESTART] , White, Black); // Display start message		
    set_match_register(0, 0, MS_TO_CC(100000000, 1000 / REFRESH_RATE), 0, 1, 1); // Set the match register to 33ms (30fps)
	
    gameState.points = 0;
    gameState.lifes = 2;
    gameState.remainingTime = 60;
		sendCANstats(gameState.remainingTime, gameState.lifes, gameState.points);
    gameState.status = WAITING;
    gameState.isStatusChanged = 0;
    gameState.eatenDots = 0;
		GHOST_PPF = GHOST_SPEED / REFRESH_RATE;    // 2 pixels per frame
		
		set_match_register(2,1,0,0,0,0);
		
		enable_timer(0);                            // Enable the timer
	
		setTrack(STANDBY_TRACK); //play the first track for standby
		playTrack(1);
}


/**
 * @brief Starts the game by initializing the game screen and characters.
 * 
 * This function clears the screen, displays the maze, ready message, lives, points, and countdown.
 * It also sets the initial positions and visibility of the characters, displays them, and starts the game timers.
 * 
 * @note This function should be called to start the game.
 */
void gameStart() {
    LCD_Clear(Black);                               // Clear the screen
    LCD_ShowImagecolorAvoid(getData(&maze_image), getWidth(&maze_image), getHeight(&maze_image),0, 0, Black);               // Display the maze
		LCD_ShowImagecolorAvoid(getData(&ready_image), getWidth(&ready_image), getHeight(&ready_image), 96, 164, Black);    // Display the ready message
    printLives();                                   // Display the lives
    printPoints(1);                                 // Display the points
    printGameCountdown(1);                          // Display the time

    // Set the position and visibility of the characters
    setIsAlive(&red_ghost, 1);                      // Set red ghost as alive
    setPosX(&red_ghost, xcc[13]-4);                 // Set red ghost's x position
    setPosY(&red_ghost, ycc[13]);                   // Set red ghost's y position
		setOldDirection(&red_ghost, RIGHT);

    setIsAlive(&pacman, 1);                         // Set pacman as alive
    setPosX(&pacman, xcc[13]- 4);                   // Set pacman's x position
    setPosY(&pacman, ycc[22]);                      // Set pacman's y position
		setOldDirection(&pacman, RIGHT);
		setDirection(&pacman, RIGHT);

    printPlayers();                                 // Display the players
    printDots();                                    // Display the dots

    drawRectangle(96, 164, getWidth(&ready_image), getHeight(&ready_image), Black);     // Clear the ready message

    set_match_register(1, 0, MS_TO_CC(100000000, 1000), 0, 1, 1);                       // Set the match register to 1000ms (1s) for the countdown
    enable_timer(1);          

		setTrack(PLAYING_0_TRACK); 
		playTrack(1);
}

/**
 * @brief Ends the game and displays the game over screen.
 * 
 * This function disables the game timers, clears the screen, and displays the "GAME OVER" message,
 * the player's score, and a message prompting the user to press INT0 to restart the game.
 * 
 * @note This function should be called when the game is over.
 */
void gameOver() {
    disable_timer(1); // Disable the timer for countdown
		pauseTrack();
    LCD_Clear(Black); // Clear the screen
    GUI_Text(10, 120, (uint8_t *) gameStrings[GAME_OVER], Red, Black); // Display the "GAME OVER" message
    GUI_Text(10, 140, (uint8_t *) gameStrings[YOUR_SCORE], Yellow, Black); // Display the "Your Score:" message
    char points[10];
    sprintf(points, "%d", gameState.points);
    GUI_Text(10, 160, (uint8_t *) points, Yellow, Black); // Display the points
    GUI_Text(10, 180, (uint8_t *) gameStrings[PRESS_RESTART], Green, White); // Display the restart message
}

/**
 * @brief Displays the game won screen and congratulates the player.
 * 
 * This function disables the game timers, clears the screen, and displays the "YOU WIN!" message,
 * the player's score, and a message prompting the user to press INT0 to restart the game.
 * 
 * @note This function should be called when the player wins the game.
 */
void gameWon() {
    disable_timer(1); // Disable the timer for countdown
		pauseTrack();
    LCD_Clear(Black); // Clear the screen
    GUI_Text(10, 120, (uint8_t *) gameStrings[YOU_WIN], Yellow, Black); // Display the "YOU WIN!" message
    GUI_Text(10, 140, (uint8_t *) gameStrings[CONGRATULATIONS], Yellow, Black); // Display the "Congratulations!" message
    GUI_Text(10, 160, (uint8_t *) gameStrings[YOUR_SCORE], Yellow, Black); // Display the "Your Score:" message
    char points[10];
    sprintf(points, "%d", gameState.points);
    GUI_Text(10, 180, (uint8_t *) points, Yellow, Black); // Display the points
    GUI_Text(10, 200, (uint8_t *) gameStrings[PRESS_RESTART], Green, White); // Display the restart message
}

/**
 * @brief Pauses the game and displays the paused message.
 * 
 * This function disables the game timers and displays the "PAUSED" message on the screen.
 * 
 * @note This function should be called when the game is paused.
 */
void gamePaused() {
    disable_timer(1); // Disable the timer for countdown
		pauseTrack();
    GUI_Text(96, 161, (uint8_t *)gameStrings[PAUSED_LABEL], Yellow, Black); // Display the paused message
}


/**
 * @brief Resumes the game from a paused state.
 * 
 * This function clears the paused message from the screen, redraws the players,
 * and re-enables the game timers for countdown and refresh.
 * 
 * @note This function should be called to resume the game after it has been paused.
 */
void gameResumed() {
    drawRectangle(96, 161, 48, 16, Black); // Clear the paused message
    printPlayers();                                  // Redraw the players
		resumeTrack();
    enable_timer(1);                                 // Enable the timer for countdown
}

/**
 * @brief Refreshes the game screen when timer 0 is triggered.
 * 
 * This function handles the game state transitions and updates the game screen accordingly.
 * It checks the current game state and performs the necessary actions, such as initializing
 * the game, updating player positions, pausing, resuming, winning, or ending the game.
 * 
 * @note This function should be called when timer 0 is triggered to refresh the game screen.
 */
void gameRefresh() {
    switch (gameState.status) {
        case WAITING:
            if (gameState.isStatusChanged) {
                gameWaiting();
                gameState.isStatusChanged = 0;
            }
            break;
        case PLAYING:
            if (gameState.isStatusChanged) { // About to start the game
                gameStart();
                gameState.isStatusChanged = 0;
                gamePaused();
                updateStatus(PAUSED);
            } else { // Game is running
                updatePlayersPosition();
                printPlayers();
            }
            break;
        case PAUSED:
            if (gameState.isStatusChanged) {
                gamePaused();
                gameState.isStatusChanged = 0;
            }
            break;
        case RESUMED:
            if (gameState.isStatusChanged) {
                gameResumed();
                gameState.status = PLAYING;
                gameState.isStatusChanged = 0;
            }
            break;
        case WON:
            if (gameState.isStatusChanged) {
                gameWon();
                gameState.isStatusChanged = 0;
            }
            break;
        case OVER:
            if (gameState.isStatusChanged) {
                gameOver();
                gameState.isStatusChanged = 0;
            }
            break;
    }
}

/**
 * @brief Updates the game status and sets the status change flag.
 * 
 * This function updates the game status to the new status and sets the status change flag
 * if the new status is different from the current status.
 * 
 * @param newStatus The new status to set.
 */
void updateStatus(Status newStatus) {
    if (gameState.status != newStatus) {
        gameState.isStatusChanged = 1;
        gameState.status = newStatus;
    }
}

/**
 * @brief Gets the current game status.
 * 
 * This function returns the current game status.
 * 
 * @return Status The current game status.
 */
Status getStatus() {
    return gameState.status;
}

/**
 * @brief Sets the random seed for the game.
 * 
 * This function sets the random seed for the game and ensures that the random seed is not zero.
 * 
 * @param seed The random seed to set.
 */
void setSeed(uint16_t seed) {
    srand(seed);
    gameState.randomSeed = 0;
    while (gameState.randomSeed == 0) {
        gameState.randomSeed = (uint16_t)(rand() % BIG_DOT_RANDOM_MODULE);
    }
		
		// caluclating big dots locations and copying the map
		uint16_t printedDotsNum = 0;
    uint8_t printedBigDotsNum = 0;
    for (int i = 0; i < YPOS_NUM; i++) {
        for (int j = 0; j < XPOS_NUM; j++) {
            gameState.position[i][j] = positionConst[i][j];
						if (gameState.position[i][j] == 1) {
                printedDotsNum++; // Increment before to avoid 0%n always equal 0
                if (printedDotsNum % (gameState.randomSeed + DOT_NUM / BIG_DOT_NUM * printedBigDotsNum) == 0) {
                    printedBigDotsNum++;
                    gameState.position[i][j] = 4;
                } 
            }
        }
    }
}

/**
 * @brief Displays the dots on the game board.
 * 
 * This function iterates through the game board and displays small and big dots
 * based on the game state.
 * 
 * @note This function should be called to refresh the dots on the game board.
 */
void printDots() {
    for (int i = 0; i < YPOS_NUM; i++) {
        for (int j = 0; j < XPOS_NUM; j++) {
            uint8_t dotType = gameState.position[i][j];
            
            if (dotType == 4) {
                // Display big dot
                LCD_ShowImagecolorAvoid(getData(&big_dot_image), 
                                        getWidth(&big_dot_image), 
                                        getHeight(&big_dot_image), 
                                        xcc[j] + BIG_DOT_OFFSET, 
                                        ycc[i] + BIG_DOT_OFFSET, 
                                        Black);
            } else if (dotType == 1) {
                // Display small dot
                LCD_ShowImagecolorAvoid(getData(&small_dot_image), 
                                        getWidth(&small_dot_image), 
                                        getHeight(&small_dot_image), 
                                        xcc[j] + SMALL_DOT_OFFSET, 
                                        ycc[i] + SMALL_DOT_OFFSET, 
                                        Black);
            }
        }
    }
}


/**
 * @brief Updates the position of a player (Pacman or ghost) on the game board.
 * 
 * This function checks if the player can move in the current direction or the old direction.
 * If the move is valid, it updates the player's position. If the player is Pacman and crosses
 * a dot, it updates the points and the game state accordingly. It also handles teleportation.
 * 
 * @param player Pointer to the player (Pacman or ghost) to update.
 */
void updatePlayerPosition(Player* player) {
    uint16_t posXold = getPosX(player), 
             posYold = getPosY(player), 
             posXindex = 0,
             posYindex = 0,
             id = getId(player);
    Direction dirNow = getDirection(player), 
              dirOld = getOldDirection(player);
    
    // Identify if the pacman is in a valid turning spot
    uint8_t inTurningSpot = !((posXold - xcc[0]) % HORZ_SPACING_DOTS || (posYold - ycc[0]) % VERT_SPACING_DOTS);
    
    if (inTurningSpot == 1) { // pacman is in a valid turning spot, where dots and teleport are
        if (isValidMove(player, dirNow)) {
            setOldDirection(player, dirNow);
        } else {
            if (!isValidMove(player, dirOld)) {
                setOldDirection(player, STOP);
            }
        }
				
				if (id==0){
					posXindex = (posXold - xcc[0]) / HORZ_SPACING_DOTS;
					posYindex = (posYold - ycc[0]) / VERT_SPACING_DOTS;
					switch (gameState.position[posYindex][posXindex]) {
							case 1: // Small dot
									updatePoints(SMALL_DOT_POINTS);
									gameState.position[posYindex][posXindex] = 2;
									break;
							case 4: // Big dot
									updatePoints(BIG_DOT_POINTS);
									pacmanIsHunter();
									setTrack(PLAYING_HUNTER_TRACK);
									playTrack(1);
									disable_timer(2);
									reset_timer(2);
									set_match_register(2, 0, MS_TO_CC(100000000, HUNTING_MODE_DURATION_MS), 1, 1, 1);
									enable_timer(2);
									gameState.position[posYindex][posXindex] = 2;
									break;
							case 3: // Teleport
									if (getPosX(player) == xcc[0] && dirOld == LEFT) {
											setPosX(player, xcc[XPOS_NUM - 2]);
									} else if (getPosX(player) == xcc[XPOS_NUM - 1] && dirOld == RIGHT) {
											setPosX(player, xcc[1]);
									}
									break;
							default:
									break;
					}
				} else {  //ghost in a valid turning spot
					if (needToIncrement!=0){
							GHOST_PPF*=2;				
							needToIncrement=0;
					}
				}
		}
    dirOld = getOldDirection(player);
    movePlayer(player, dirOld, id == 0 ? PACMAN_PPF : GHOST_PPF);
    // Cancel the previous position
    drawRectangle(posXold, posYold, getWidth(getImage(player)) + PLAYERS_OFFSET, getHeight(getImage(player)) + PLAYERS_OFFSET, Black);
}


/* ------------------------------ IA FOR RED GHOST CODE ---------------------------*/

// Structure for D* Lite nodes
typedef struct {
    uint16_t g;           // Cost from start
    uint16_t rhs;         // One-step lookahead value
    uint16_t key[2];      // Key for priority queue
} DStarNode;

// Structure for priority queue nodes
typedef struct {
    uint8_t x;
    uint8_t y;
    uint16_t key[2];
} PQNode;

// Priority queue implementation
#define MAX_PQ_SIZE 100
static PQNode pq[MAX_PQ_SIZE];
static uint8_t pq_size = 0;

// Manhattan distance heuristic for hunting: calculate Manhattan distance between two points
static uint16_t hunting_heuristic(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Manhattan distance heuristic for being hunted: calculate inverse Manhattan distance to flee from Pacman
static uint16_t hunted_heuristic(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    return XPOS_NUM + YPOS_NUM - (abs(x1 - x2) + abs(y1 - y2));  // Invert the distance
}

// Priority queue operations
static void pq_push(uint8_t x, uint8_t y, uint16_t key[2]) {
    if (pq_size >= MAX_PQ_SIZE) return;
    
    uint8_t i = pq_size++;
    while (i > 0) {
        uint8_t parent = (i - 1) / 2;
        if (pq[parent].key[0] < key[0] || 
           (pq[parent].key[0] == key[0] && pq[parent].key[1] <= key[1])) break;
        
        pq[i] = pq[parent];
        i = parent;
    }
    pq[i].x = x;
    pq[i].y = y;
    pq[i].key[0] = key[0];
    pq[i].key[1] = key[1];
}

static PQNode pq_pop(void) {
    PQNode top = pq[0];
    pq_size--;
    
    PQNode last = pq[pq_size];
    uint8_t i = 0;
    while (2*i + 1 < pq_size) {
        uint8_t child = 2*i + 1;
        if (child + 1 < pq_size && 
            (pq[child+1].key[0] < pq[child].key[0] || 
            (pq[child+1].key[0] == pq[child].key[0] && pq[child+1].key[1] < pq[child].key[1])))
            child++;
            
        if (last.key[0] < pq[child].key[0] || 
           (last.key[0] == pq[child].key[0] && last.key[1] <= pq[child].key[1]))
            break;
            
        pq[i] = pq[child];
        i = child;
    }
    pq[i] = last;
    return top;
}

Direction findNextMove(uint8_t ghostXindex, uint8_t ghostYindex, 
                      uint8_t pacmanXindex, uint8_t pacmanYindex, uint8_t isHunted) {
    static DStarNode nodes[YPOS_NUM][XPOS_NUM];
    static const int8_t dx[] = {0, 1, 0, -1};  // DOWN, RIGHT, UP, LEFT
    static const int8_t dy[] = {1, 0, -1, 0};
    
    // Initialize nodes
    for (uint8_t y = 0; y < YPOS_NUM; y++) {
        for (uint8_t x = 0; x < XPOS_NUM; x++) {
            nodes[y][x].g = UINT16_MAX;
            nodes[y][x].rhs = UINT16_MAX;
        }
    }

    // Set target coordinates based on isHunted
    uint8_t targetX, targetY;
    if (isHunted) {
        // When hunted, find the farthest corner from Pacman
        uint8_t corners[4][2] = {{0,0}, {0,YPOS_NUM-1}, {XPOS_NUM-1,0}, {XPOS_NUM-1,YPOS_NUM-1}};
        uint16_t maxDist = 0;
        for(int i = 0; i < 4; i++) {
            uint16_t dist = abs(corners[i][0] - pacmanXindex) + abs(corners[i][1] - pacmanYindex);
            if(dist > maxDist && gameState.position[corners[i][1]][corners[i][0]] != 0) {
                maxDist = dist;
                targetX = corners[i][0];
                targetY = corners[i][1];
            }
        }
    } else {
        // When hunting, target Pacman
        targetX = pacmanXindex;
        targetY = pacmanYindex;
    }
    
    // Initialize goal
    nodes[targetY][targetX].rhs = 0;
    pq_size = 0;
    
    // Calculate initial key using appropriate heuristic
    uint16_t key[2];
    if (isHunted) {
        key[0] = hunted_heuristic(ghostXindex, ghostYindex, targetX, targetY);
    } else {
        key[0] = hunting_heuristic(ghostXindex, ghostYindex, targetX, targetY);
    }
    key[1] = 0;
    pq_push(targetX, targetY, key);
    
    // Main loop
    while (pq_size > 0 && (nodes[ghostYindex][ghostXindex].rhs > nodes[ghostYindex][ghostXindex].g || 
           nodes[ghostYindex][ghostXindex].g == UINT16_MAX)) {
        PQNode current = pq_pop();
        
        if (nodes[current.y][current.x].g > nodes[current.y][current.x].rhs) {
            nodes[current.y][current.x].g = nodes[current.y][current.x].rhs;
            
            for (uint8_t i = 0; i < 4; i++) {
                uint8_t nx = current.x + dx[i];
                uint8_t ny = current.y + dy[i];
                
                if (nx < XPOS_NUM && ny < YPOS_NUM && gameState.position[ny][nx] != 0) {
                    uint16_t new_rhs = nodes[current.y][current.x].g + 1;
                    if (new_rhs < nodes[ny][nx].rhs) {
                        nodes[ny][nx].rhs = new_rhs;
                        key[0] = new_rhs + (isHunted ? 
                                          hunted_heuristic(ghostXindex, ghostYindex, nx, ny) :
                                          hunting_heuristic(ghostXindex, ghostYindex, nx, ny));
                        key[1] = new_rhs;
                        pq_push(nx, ny, key);
                    }
                }
            }
        }
    }
    
    // Find best next move
    uint16_t min_cost = UINT16_MAX;
    Direction best_dir = STOP;
    
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t nx = ghostXindex + dx[i];
        uint8_t ny = ghostYindex + dy[i];
        
        if (nx < XPOS_NUM && ny < YPOS_NUM && gameState.position[ny][nx] != 0) {
            if (nodes[ny][nx].g < min_cost) {
                min_cost = nodes[ny][nx].g;
                best_dir = (Direction)(i);  // Convert to Direction enum
            }
        }
    }
    
    return best_dir;
}

/* ------------------------------ END OF IA FOR RED GHOST CODE ---------------------------*/

/**
 * @brief Updates the positions of all players (Pacman and ghosts) on the game board.
 * 
 * This function updates the positions of Pacman and all ghosts if alive.
 * 
 * @note This function should be called to refresh the positions of all players.
 */
void updatePlayersPosition() {
    updatePlayerPosition(&pacman);
	
		if (abs(getPosX(&pacman)-getPosX(&red_ghost))+abs(getPosY(&pacman)-getPosY(&red_ghost))<=2){
			if (getIsVulnerable(&red_ghost)){
				setIsAlive(&red_ghost, 0);                      // Set red ghost as alive
				drawRectangle(getPosX(&red_ghost), getPosY(&red_ghost), getWidth(getImage(&red_ghost)), getHeight(getImage((&red_ghost))), Black);
				setPosX(&red_ghost, xcc[13]);                 // Set red ghost's x position
				setPosY(&red_ghost, ycc[13]);                   // Set red ghost's y position
				setOldDirection(&red_ghost, RIGHT);
				ghostAreHunters();
				if (gameState.remainingTime>20){
					setTrack(PLAYING_0_TRACK);
				} else if (gameState.remainingTime<=20 && gameState.remainingTime>10){
					setTrack(PLAYING_1_TRACK);
				} else if (gameState.remainingTime<=10) {
					setTrack(PLAYING_2_TRACK);
				}
				playTrack(1);
				disable_timer(2);
				reset_timer(2);
				set_match_register(2, 0, 0, 0,0,0); //cleaning match register used to countdwn for hunting mode
				set_match_register(2, 1, MS_TO_CC(100000000, RESPAWN_COUNTDOWN_MS), 1, 1, 1);
				enable_timer(2);
				updatePoints(GHOST_ATE_POINTS);
			} else {
				subtractLife();
				drawRectangle(getPosX(&pacman), getPosY(&pacman), getWidth(getImage(&pacman)), getHeight(getImage((&pacman))), Black);
				setPosX(&pacman, xcc[13]- 4);                   // Set pacman's x position
				setPosY(&pacman, ycc[22]);                      // Set pacman's y position
				setOldDirection(&pacman, RIGHT);
				setDirection(&pacman, RIGHT);
				
			}
		}
		

		if (red_ghost.isAlive==1){
			// Get current position indices for pacman
			uint8_t pacmanPosX = (getPosX(&pacman) - xcc[0]) / HORZ_SPACING_DOTS;
			uint8_t pacmanPosY = (getPosY(&pacman) - ycc[0]) / VERT_SPACING_DOTS;

			// Get current position indices for red ghost 
			uint8_t ghostPosX = (getPosX(&red_ghost) - xcc[0]) / HORZ_SPACING_DOTS;
			uint8_t ghostPosY = (getPosY(&red_ghost) - ycc[0]) / VERT_SPACING_DOTS; 

			// Update red ghost direction using pathfinding
			Direction newGhostDir = findNextMove(ghostPosX, ghostPosY, pacmanPosX, pacmanPosY, red_ghost.isVulnerable);
			setDirection(&red_ghost, newGhostDir);
			
			// Update red ghost position
			updatePlayerPosition(&red_ghost);
			
			// redraw rectangle cell gate
			for (int y= 128; y< 132; y++){
				for (int x = 100 ; x< 140; x++){
					LCD_SetPoint(x,y,RGB332toRGB565(maze_image.data[y*maze_image.width +x]));
				}
			}
		}
		
		printDots();
}


/**
 * @brief Checks if a move in the specified direction is valid for the player.
 * 
 * This function checks if the player can move in the specified direction without hitting a wall or obstacle.
 * 
 * @param player Pointer to the player (Pacman or ghost) to check.
 * @param direction The direction to check for validity.
 * @return uint8_t Returns 1 if the move is valid, 0 otherwise.
 * 
 * @note This function should be called to validate a player's move.
 */
uint8_t isValidMove(Player* player, Direction direction) {
    uint16_t posix = (getPosX(player) - xcc[0]) / HORZ_SPACING_DOTS;
    uint16_t posiy = (getPosY(player) - ycc[0]) / VERT_SPACING_DOTS;
    uint8_t posVal = 0;

    int16_t newPosX = posix, newPosY = posiy;

    switch (direction) {
        case UP:    newPosY--; break;
        case DOWN:  newPosY++; break;
        case LEFT:  newPosX--; break;
        case RIGHT: newPosX++; break;
        default:    return 1;
    }

    // Verifica validità della posizione
    if (newPosX < 0 || newPosX >= XPOS_NUM || newPosY < 0 || newPosY >= YPOS_NUM) {
        return 0;
    }

    posVal = gameState.position[newPosY][newPosX];
		if (posVal == 0 || (getId(player) == 0 && posVal == 5) ) return 0;

    return 1;
}


/**
 * @brief Displays all players (Pacman and ghosts) on the screen.
 * 
 * This function displays Pacman and all ghosts on the screen by calling the LCD_ShowImagecolorAvoid() function for each player.
 * 
 * @note This function should be called to refresh the display of all players.
 */
void printPlayers() {
    void *players[] = {&pacman, &red_ghost};
    // void *players[] = {&pacman, &red_ghost, &pink_ghost, &orange_ghost, &green_ghost};
    const Image *temp;
    for (int i = 0; i < sizeof(players) / sizeof(players[0]); i++) {
        if (getIsAlive(players[i])) {
            temp = updateImage(players[i]);
            LCD_ShowImagecolorAvoid(getData(temp), getWidth(temp), getHeight(temp), getPosX(players[i]) + PLAYERS_OFFSET, getPosY(players[i]) + PLAYERS_OFFSET, Black);
        }
    }
}


/**
 * @brief Updates the player's points and lives.
 * 
 * This function adds the specified points to the player's total points. If the total points
 * reach a multiple of 1000, an extra life is awarded. It also increments the count of eaten dots
 * and checks if all dots have been eaten to trigger the game won state.
 * 
 * @param points The number of points to add.
 * 
 * @note This function should be called to update the player's points and lives.
 */
void updatePoints(uint16_t points) {
		sendCANstats(gameState.remainingTime, gameState.lifes, gameState.points+points);
	
    if (gameState.points % 1000 + points >= 1000) {
        addLife();
    }
    gameState.eatenDots++;
    if (gameState.eatenDots == DOT_NUM) {
        gameWon();
    }
}

/**
 * @brief Decrements the game countdown timer.
 * 
 * This function decreases the remaining game time by one second. If the time reaches zero,
 * it updates the game status to WON if all dots are eaten, or to OVER if there are remaining dots.
 * 
 * @note This function should be called every second to update the countdown.
 */
void decrementGameCountdown() {
		if (gameState.remainingTime == 0 && gameState.eatenDots == DOT_NUM) {
        updateStatus(WON);
				return;
    } else if (gameState.remainingTime == 0) {
        updateStatus(OVER);
				return;
    }
		sendCANstats(gameState.remainingTime-1, gameState.lifes, gameState.points);
		
		//doubling speed at 20 and 10 seconds remaining
		if (gameState.remainingTime == 20){
			needToIncrement++;
			setTrack(PLAYING_1_TRACK);
			playTrack(1);
		}
		if (gameState.remainingTime == 10){
			needToIncrement++;
			setTrack(PLAYING_2_TRACK);
			playTrack(1);
		}
			
}

/**
 * @brief Subtracts a life from the player.
 * 
 * This function decreases the player's lives by one. If the player has no lives left,
 * it updates the game status to OVER.
 * 
 * @note This function should be called when the player loses a life.
 */
void subtractLife() {
	  if (gameState.lifes == 1) {
        updateStatus(OVER);
    }
    sendCANstats(gameState.remainingTime, gameState.lifes-1, gameState.points);
}

/**
 * @brief Adds a life to the player.
 * 
 * This function increases the player's lives by one and updates the display.
 * 
 * @note This function should be called when the player gains an extra life.
 */
void addLife() {
    sendCANstats(gameState.remainingTime, gameState.lifes+1, gameState.points);
}



int getCountdown(){
	return gameState.remainingTime;
}

/****************************PRINT METHODS FOR CAN MANAGEMENT **************************/

/**
 * @brief Displays the player's remaining lives on the screen.
 * 
 * This function draws the remaining lives as images on the screen.
 * 
 * @note This function should be called to update the lives display.
 */
void printLives() {
    drawRectangle(10, 300, 30 + 20 * gameState.lifes, 300, Black); // Clear previous lives display
    for (int i = 0; i < gameState.lifes && i < 9; i++) {
        LCD_ShowImagecolorAvoid(getData(&left_1_image), getWidth(&left_1_image), getHeight(&left_1_image), 10 + i * 20, 300, Black); // Display each life as an image
    }
}

/**
 * @brief Displays the game countdown timer on the screen.
 * 
 * This function formats the remaining game time as a string and displays it on the screen.
 * If `withText` is set to 1, it also displays the countdown label.
 * 
 * @param withText If set to 1, displays the countdown label.
 * 
 * @note This function should be called to update the countdown display.
 */
void printGameCountdown(uint8_t withText) {
    char time[5];
    sprintf(time, "%2d s", gameState.remainingTime);
    if (withText == 1) {
        GUI_Text(10, 0, (uint8_t *) gameStrings[COUNTDOWN_LABEL], White, Black); // Display the countdown message
    }
    GUI_Text(10, 12, (uint8_t *) time, White, Black); // Display the remaining time
}

/**
 * @brief Displays the player's points on the screen.
 * 
 * This function formats the player's points as a string and displays it on the screen.
 * If `withText` is set to 1, it also displays the score label.
 * 
 * @param withText If set to 1, displays the score label.
 * 
 * @note This function should be called to update the points display.
 */
void printPoints(uint8_t withText) {
    char points[10];
    sprintf(points, "%08d", gameState.points);
    if (withText == 1) {
        GUI_Text(120, 0, (uint8_t *) gameStrings[SCORE_LABEL], White, Black);
    }
    GUI_Text(120, 12, (uint8_t *) points, White, Black); // Display the points
}


void printAllInfo(){
	printPoints(0);
	printGameCountdown(0);
	printLives();
}

void sendCANstats(uint8_t time, uint8_t lives, uint16_t score){
	CAN_TxMsg.data[0] = time;
	CAN_TxMsg.data[1] = lives;
	CAN_TxMsg.data[2] = ((score ) & 0xFF00) >> 8;
	CAN_TxMsg.data[3] = (score) & 0xFF;
	CAN_TxMsg.len = 4;
	CAN_TxMsg.id = 2;
	CAN_TxMsg.format = STANDARD_FORMAT;
	CAN_TxMsg.type = DATA_FRAME;
	CAN_wrMsg(1, &CAN_TxMsg); 
	#ifdef SIMULATTOR
		gameState.remainingTime=time;
		gameState.lifes=lives;
		gameState.points=score;
		if (gameState.status==PLAYING) printAllInfo();
	#endif
}

void readCANstats() {
	#ifndef SIMULATTOR
		gameState.remainingTime = CAN_RxMsg.data[0];  // Tempo rimanente
		gameState.lifes = CAN_RxMsg.data[1];          // Vite rimanenti
		gameState.points = (CAN_RxMsg.data[2] << 8) | CAN_RxMsg.data[3]; // Punteggio (unione dei due byte)
		if (gameState.status==PLAYING) printAllInfo();
	#endif
}
