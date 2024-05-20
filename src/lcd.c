/*
 * lcd.c
 *
 *  Created on: May 19, 2024
 *      Author: Motaz
 */

/**
 * @file lcd.c
 * @brief LCD driver source file.
 *
 * This file contains the implementation of functions for controlling an LCD.
 * It supports both 4-bit and 8-bit modes.
 *
 * @date May 19, 2024
 * @author Motaz
 */

#include "../include/lcd.h"

/**
 * @brief Initialize the LCD.
 *
 * This function initializes the LCD in either 4-bit or 8-bit mode.
 */
void LCD_init(void) {
    LCD_CTRL_PORT_DIR |= (1 << RS) | (1 << RW) | (1 << E);
    #if (DATA_BITS_MODE == 4)
        #ifdef UPPER_PORT_PINS
            LCD_DATA_PORT_DIR |= 0xF0;
        #else
            SET_BIT(LCD_DATA_PORT, 0x0F);
        #endif
        LCD_sendCommand(FOUR_BITS_DATA_MODE);
        LCD_sendCommand(TWO_LINE_4_BIT_MODE);
    #elif (DATA_BITS_MODE == 8)
        LCD_DATA_PORT_DIR = 0xFF;
        LCD_sendCommand(TWO_LINE_8_BIT_MODE);
    #endif
    LCD_sendCommand(DISPLAY_ON_CURSOR_BLINKING);
    LCD_sendCommand(CLEAR_DISPLAY);
}

/**
 * @brief Send a command to the LCD.
 *
 * This function sends a command to the LCD in either 4-bit or 8-bit mode.
 *
 * @param command The command to send.
 */
void LCD_sendCommand(uint8 command) {
    CLEAR_BIT(LCD_CTRL_PORT, RS);
    CLEAR_BIT(LCD_CTRL_PORT, RW);
    _delay_ms(1);
    SET_BIT(LCD_CTRL_PORT, E);
    _delay_ms(1);
    #if (DATA_BITS_MODE == 4)
        #ifdef UPPER_PORT_PINS
            LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (command & 0xF0);
        #else
            LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | ((command & 0xF0) >> 4);
        #endif
        _delay_ms(1);
        CLEAR_BIT(LCD_CTRL_PORT, E);
        _delay_ms(1);
        SET_BIT(LCD_CTRL_PORT, E);
        _delay_ms(1);
        #ifdef UPPER_PORT_PINS
            LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | ((command & 0x0F) << 4);
        #else
            LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | (command & 0x0F);
        #endif
        _delay_ms(1);
        CLEAR_BIT(LCD_CTRL_PORT, E);
        _delay_ms(1);
    #elif (DATA_BITS_MODE == 8)
        LCD_DATA_PORT = command;
        _delay_ms(1);
        CLEAR_BIT(LCD_CTRL_PORT, E);
        _delay_ms(1);
    #endif
}

/**
 * @brief Display a character on the LCD.
 *
 * This function displays a character on the LCD in either 4-bit or 8-bit mode.
 *
 * @param data The character to display.
 */
void LCD_displayCharacter(uint8 data) {
    SET_BIT(LCD_CTRL_PORT, RS);
    CLEAR_BIT(LCD_CTRL_PORT, RW);
    _delay_ms(1);
    SET_BIT(LCD_CTRL_PORT, E);
    _delay_ms(1);
    #if (DATA_BITS_MODE == 4)
        #ifdef UPPER_PORT_PINS
            LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (data & 0xF0);
        #else
            LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | ((data & 0xF0) >> 4);
        #endif
        _delay_ms(1);
        CLEAR_BIT(LCD_CTRL_PORT, E);
        _delay_ms(1);
        SET_BIT(LCD_CTRL_PORT, E);
        _delay_ms(1);
        #ifdef UPPER_PORT_PINS
            LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | ((data & 0x0F) << 4);
        #else
            LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | (data & 0x0F);
        #endif
        _delay_ms(1);
        CLEAR_BIT(LCD_CTRL_PORT, E);
        _delay_ms(1);
    #elif (DATA_BITS_MODE == 8)
        LCD_DATA_PORT = data;
        _delay_ms(1);
        CLEAR_BIT(LCD_CTRL_PORT, E);
        _delay_ms(1);
    #endif
}

/**
 * @brief Display a string on the LCD.
 *
 * This function displays a null-terminated string on the LCD.
 *
 * @param Str The string to display.
 */
void LCD_displayString(const char *Str) {
    uint8 i = 0;
    while (Str[i] != '\0') {
        LCD_displayCharacter(Str[i]);
        i++;
    }
}

/**
 * @brief Move the cursor to a specified position on the LCD.
 *
 * This function moves the cursor to a specified row and column on the LCD.
 *
 * @param row The row number (0-1).
 * @param col The column number (0-15).
 */
void LCD_goToRowColumn(uint8 row, uint8 col) {
    uint8 Address;
    switch (row) {
        case 0:
            Address = col;
            break;
        case 1:
            Address = col + 0x40;
            break;
        case 2:
            Address = col + 0x10;
            break;
        case 3:
            Address = col + 0x50;
            break;
    }
    LCD_sendCommand(SET_DDRAM_ADDRESS | Address);
}

/**
 * @brief Display a string at a specified position on the LCD.
 *
 * This function displays a null-terminated string at a specified row and column on the LCD.
 *
 * @param row The row number (0-1).
 * @param col The column number (0-15).
 * @param Str The string to display.
 */
void LCD_displayStringRowColum(uint8 row, uint8 col, const char *Str) {
    LCD_goToRowColumn(row, col);
    LCD_displayString(Str);
}

/**
 * @brief Convert an integer to a string and display it on the LCD.
 *
 * This function converts an integer to a string and displays it on the LCD.
 *
 * @param data The integer to display.
 */
void LCD_intgerToString(int data) {
    char buff[16];
    itoa(data, buff, 10);
    LCD_displayString(buff);
}

/**
 * @brief Convert a double to a string and display it on the LCD.
 *
 * This function converts a double to a string and displays it on the LCD.
 * It displays up to two decimal places.
 *
 * @param data The double to display.
 */
void LCD_doubleToString(double data) {
    int32_t intPart = (int32_t)data;  ///< Integer part
    int32_t fracPart = (int32_t)((data - intPart) * 100.0);  ///< Assuming 2 decimal places

    char buff[16];
    sprintf(buff, "%ld.%02ld", intPart, fracPart);
    LCD_displayString(buff);
}

/**
 * @brief Clear the LCD screen.
 *
 * This function clears the LCD screen.
 */
void LCD_clearScreen(void) {
    LCD_sendCommand(CLEAR_DISPLAY);
}
