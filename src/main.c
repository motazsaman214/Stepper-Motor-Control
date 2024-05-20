/**
 * @file main.c
 * @brief Stepper motor control with AVR ATmega32 using push buttons for direction and mode control.
 *
 * This file contains the implementation of stepper motor control using an AVR ATmega32
 * microcontroller. It includes functionality to control the motor direction and stepping
 * mode (full step or half step) through external interrupts triggered by push buttons.
 *
 * @date May 18, 2024
 * @author Motaz
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../include/lcd.h"
#include "../include/common_macros.h"
#include "../include/micro_config.h"
#include "../include/std_types.h"

#define HALF_WAVE_STEPS 8 ///< Number of steps in half wave mode
#define FULL_WAVE_STEPS 4 ///< Number of steps in full wave mode

#define CW  1 ///< Clockwise direction
#define CCW 0 ///< Counter-clockwise direction

volatile uint8 MotorDirection = CW; ///< Motor direction (CW or CCW)
volatile uint8 StepMode = 0;        ///< Step mode (0 for full step, 1 for half step)
volatile uint16_t timer0_ticks = 0; ///< Timer0 ticks counter

uint8 FULL_WAVE_motor_steps[FULL_WAVE_STEPS] = {12, 6, 3, 9}; ///< Full wave stepping sequence
uint8 HALF_WAVE_motor_steps[HALF_WAVE_STEPS] = {12, 4, 6, 2, 3, 1, 9, 8}; ///< Half wave stepping sequence

/**
 * @brief Initialize Timer0.
 *
 * This function initializes Timer0 for generating a delay.
 */
void timer0_init() {
    TCCR0 |= (1 << CS00);  // Set prescaler to 1
    TCNT0 = 0;             // Initialize counter value
    TIMSK |= (1 << TOIE0); // Enable Timer0 overflow interrupt
    sei();                 // Enable global interrupts
}

/**
 * @brief Delay function using Timer0.
 *
 * This function generates a delay in milliseconds using Timer0.
 *
 * @param milliseconds The duration of the delay in milliseconds.
 */
void delay_ms(uint16_t milliseconds) {
    // Calculate the number of ticks required for the given milliseconds
    uint16_t ticks = milliseconds * (F_CPU / 1000) / 256;

    // Reset Timer0 ticks
    timer0_ticks = 0;

    // Wait until the required number of ticks have elapsed
    while (timer0_ticks < ticks);
}

/**
 * @brief Timer0 overflow interrupt service routine.
 *
 * This ISR increments the Timer0 ticks counter.
 */
ISR(TIMER0_OVF_vect) {
    // Increment Timer0 ticks
    timer0_ticks++;
}

/**
 * @brief Initialize external interrupts INT0 and INT1.
 *
 * This function sets up external interrupts on PD2 (INT0) and PD3 (INT1) with internal pull-up
 * resistors and triggers them on the falling edge.
 */
void INT_int() {
    DDRD &= ~(1 << PD2) & ~(1 << PD3);  ///< Set PD2 and PD3 as inputs
    PORTD |= (1 << PD2) | (1 << PD3);   ///< Enable internal pull-up resistors for PD2 and PD3
    GICR |= (1 << INT0) | (1 << INT1);  ///< Enable external interrupts INT0 and INT1

    MCUCR |= (1 << ISC01);  ///< Trigger INT0 on falling edge
    MCUCR |= (1 << ISC11);  ///< Trigger INT1 on falling edge
}

/**
 * @brief Interrupt Service Routine for INT0 (PD2).
 *
 * This ISR toggles the motor direction when the button connected to PD2 is pressed.
 */
ISR(INT0_vect) {
    MotorDirection = (MotorDirection == CW) ? CCW : CW; ///< Toggle motor direction
}

/**
 * @brief Interrupt Service Routine for INT1 (PD3).
 *
 * This ISR toggles the stepping mode between full step and half step when the button
 * connected to PD3 is pressed.
 */
ISR(INT1_vect) {
    StepMode = (StepMode == 0) ? 1 : 0; ///< Toggle stepping mode
}

/**
 * @brief Update the LCD display with the current motor direction and step mode.
 *
 * @param direction The current motor direction.
 * @param mode The current step mode.
 */
void updateLCD(uint8 direction, uint8 mode) {
    static uint8 prevDirection = -1;
    static uint8 prevMode = -1;

    if (direction != prevDirection || mode != prevMode) {
        LCD_clearScreen();
        LCD_goToRowColumn(0, 0);
        LCD_displayString("Dir:");
        LCD_displayString((direction == CW) ? "CW " : "CCW");

        LCD_goToRowColumn(1, 0);
        LCD_displayString("Mode:");
        LCD_displayString((mode == 0) ? "Full" : "Half");

        prevDirection = direction;
        prevMode = mode;
    }
}

/**
 * @brief Main function.
 *
 * This function initializes the motor control, sets up the interrupts, and enters a loop
 * that controls the motor based on the current direction and stepping mode.
 *
 * @return int This function does not return.
 */
int main(void) {
    DDRC = 0xFF;  ///< Set PORTC as output for motor control
    LCD_init();   ///< Initialize the LCD
    INT_int();    ///< Initialize external interrupts
    timer0_init(); ///< Initialize Timer0 for delay

    uint8 stepIndex = 0; ///< Index to track the current step in the sequence

    while (1) {
        const uint8* sequence = (StepMode == 0) ? FULL_WAVE_motor_steps : HALF_WAVE_motor_steps; ///< Select step sequence based on mode
        uint8 steps = (StepMode == 0) ? FULL_WAVE_STEPS : HALF_WAVE_STEPS; ///< Determine number of steps based on mode
        PORTC = sequence[stepIndex]; ///< Set motor control pins to the current step value

        // Increment or decrement stepIndex based on MotorDirection
        if (MotorDirection == CW) {
            stepIndex = (stepIndex + 1) % steps;
        } else {
            stepIndex = (stepIndex == 0) ? steps - 1 : stepIndex - 1;
        }

        updateLCD(MotorDirection, StepMode); ///< Update LCD with current direction and mode if changed
        delay_ms(500); ///< Fixed delay for stepping speed
    }

    return 0;
}
