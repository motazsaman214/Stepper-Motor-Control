

/*
 * lcd.h
 *
 *  Created on: May 19, 2024
 *      Author: Motaz
 */

/**
 * @file lcd.h
 * @brief LCD driver header file.
 *
 * This file contains the macros and function prototypes for controlling an LCD.
 * It supports both 4-bit and 8-bit modes.
 *
 * @date May 19, 2024
 * @author Motaz
 */

#ifndef SRC_LCD_H_
#define SRC_LCD_H_

#include "common_macros.h"
#include "std_types.h"
#include "micro_config.h"

#define DATA_BITS_MODE 4 ///< Data bits mode (4 or 8)
#if (DATA_BITS_MODE == 4)
#define LOWER_PORT_PINS
#endif

/* LCD HW Pins */
#define RS PB0 ///< Register select pin
#define RW PB1 ///< Read/Write pin
#define E  PB2 ///< Enable pin
#define LCD_CTRL_PORT PORTB ///< Control port
#define LCD_CTRL_PORT_DIR DDRB ///< Control port direction

#define LCD_DATA_PORT PORTA ///< Data port
#define LCD_DATA_PORT_DIR DDRA ///< Data port direction

/**
 * @brief LCD Commands
 */
#define ONE_LINE_8_BIT_MODE 0x30
#define TWO_LINE_8_BIT_MODE 0x38
#define ONE_LINE_4_BIT_MODE 0x20
#define TWO_LINE_4_BIT_MODE 0x28
#define FOUR_BITS_DATA_MODE 0x02
#define ENTRY_MODE 0x06
#define DISPLAY_OFF_CURSOR_OFF 0x08
#define DISPLAY_ON_CURSOR_ON 0x0E
#define DISPLAY_ON_CURSOR_OFF 0x0C
#define DISPLAY_ON_CURSOR_BLINKING 0x0F
#define SHIFT_ENTRIE_DISPLAY_LEFT 0x18
#define SHIFT_ENTRIE_DISPLAY_RIGHT 0x1C
#define MOVE_CURSOR_LEFT_BY_ONE_Character 0x10
#define MOVE_CURSOR_RIGHT_BY_ONE_Character 0x14
#define CLEAR_DISPLAY 0x01
#define SET_DDRAM_ADDRESS 0x80
#define SET_CGRAM_ADDRESS 0x40

/**
 * @brief Function prototypes
 */
void LCD_init(void);
void LCD_sendCommand(uint8 command);
void LCD_displayCharacter(uint8 data);
void LCD_displayString(const char *Str);
void LCD_goToRowColumn(uint8 row, uint8 col);
void LCD_displayStringRowColum(uint8 row, uint8 col, const char *Str);
void LCD_intgerToString(int data);
void LCD_doubleToString(double data);
void LCD_clearScreen(void);

#endif /* SRC_LCD_H_ */
