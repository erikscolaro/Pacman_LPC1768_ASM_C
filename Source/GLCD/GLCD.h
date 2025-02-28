/**************************************** Copyright (c) ***********************************************
 *
 *                      http://www.powermcu.com
 *
 *-------------- File Info ---------------------------------------------------------------------------
 * File name:       GLCD.h
 * Description:     Tested with SSD1289, ILI9320, R61505U, SSD1298, ST7781, SPFD5408B, ILI9325, ILI9328,
 *                  HX8346A, HX8347A.
 *----------------------------------------------------------------------------------------------------
 * Created by:      AVRman
 * Created date:    2012-3-10
 * Version:         1.3
 * Description:     Original version
 *
 *----------------------------------------------------------------------------------------------------
 * Modified by:
 * Modified date:
 * Version:
 * Description:
 ******************************************************************************************************/

#ifndef __GLCD_H 
#define __GLCD_H

#include "LPC17xx.h"
#include <stdint.h>

/* LCD Interface Pins */
#define PIN_EN      (1 << 19)
#define PIN_LE      (1 << 20)
#define PIN_DIR     (1 << 21)
#define PIN_CS      (1 << 22)
#define PIN_RS      (1 << 23)
#define PIN_WR      (1 << 24)
#define PIN_RD      (1 << 25)

#define LCD_EN(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_EN) : (LPC_GPIO0->FIOCLR = PIN_EN))
#define LCD_LE(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_LE) : (LPC_GPIO0->FIOCLR = PIN_LE))
#define LCD_DIR(x)  ((x) ? (LPC_GPIO0->FIOSET = PIN_DIR) : (LPC_GPIO0->FIOCLR = PIN_DIR))
#define LCD_CS(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_CS) : (LPC_GPIO0->FIOCLR = PIN_CS))
#define LCD_RS(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_RS) : (LPC_GPIO0->FIOCLR = PIN_RS))
#define LCD_WR(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_WR) : (LPC_GPIO0->FIOCLR = PIN_WR))
#define LCD_RD(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_RD) : (LPC_GPIO0->FIOCLR = PIN_RD))

/* Display Orientation */
#define DISP_ORIENTATION  0  /* Options: 0 or 90 or 180 or 270 */

#if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)

#define MAX_X  320
#define MAX_Y  240

#elif (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)

#define MAX_X  240
#define MAX_Y  320

#endif

/* LCD Colors */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

/******************************************************************************
* Function Name  : RGB565CONVERT
* Description    : Convert 24-bit color to 16-bit color
* Input          : red, green, blue
* Output         : None
* Return         : 16-bit RGB color
*******************************************************************************/
#define RGB565CONVERT(red, green, blue) (uint16_t)( ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3) )

#define RGB332toRGB565(rgb) (uint16_t)( \
    (((((rgb) >> 5) & 0x07) << 2) | (((rgb) >> 5) & 0x07) >> 1) << 11 | /* R */ \
    (((((rgb) >> 2) & 0x07) << 3) | (((rgb) >> 2) & 0x07)) << 5  |      /* G */ \
    ((((rgb) & 0x03) << 3) | (((rgb) & 0x03) << 1))                  /* B */ \
)

/* Function Prototypes */
void LCD_Initialization(void);
void LCD_Clear(uint16_t Color);
uint16_t LCD_GetPoint(uint16_t Xpos, uint16_t Ypos);
void LCD_SetPoint(uint16_t Xpos, uint16_t Ypos, uint16_t point);
void LCD_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void PutChar(uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor);
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str, uint16_t Color, uint16_t bkColor);
void drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void LCD_ShowImage(const uint8_t* data, uint16_t width, uint16_t height, uint16_t startX, uint16_t startY);
void LCD_ShowImagecolorAvoid(const uint8_t* data, uint16_t width, uint16_t height, uint16_t startX, uint16_t startY, uint16_t colorToAvoid);

/* Personal variables */
typedef enum {WAITING, PLAYING, PAUSED, RESUMED, WON, OVER} Status;

/* Personal functions*/
void gameWaiting();
void gameStart();
void gameOver();
void gameWon();
void gamePaused();
void gameResumed();
void gameRefresh();

void updateStatus(Status newStatus);
Status getStatus();
void setSeed(uint16_t seed);
void decrementGameCountdown();
int getCountdown();
void readCANstats();
void sendCANstats(uint8_t time, uint8_t lives, uint16_t score);

#endif //__GLCD_H 