/**
 * @mainpage Stepper Motor Control with AVR ATmega32
 *
 * @section intro_sec Introduction
 *
 * This documentation provides detailed information about the stepper motor control project
 * implemented on an AVR ATmega32 microcontroller. The project involves controlling a stepper
 * motor's direction and stepping mode using push buttons for user interaction.
 *
 * @section features_sec Features
 *
 * - Motor direction control using external interrupt INT0.
 * - Stepping mode selection (full step or half step) using external interrupt INT1.
 * - LCD display for real-time feedback on motor direction and stepping mode.
 * - Timer-based delay for controlling motor speed.
 *
 * @section hardware_sec Hardware
 *
 * - AVR ATmega32 microcontroller.
 * - Stepper motor.
 * - LCD display (16x4).
 * - Push buttons for motor direction and mode control.
 *
 * @section setup_sec Setup
 *
 * - Connect the stepper motor to PORTC of the AVR ATmega32.
 * - Connect the LCD display to appropriate data and control pins.
 * - Connect push buttons for motor direction and mode control to PD2 (INT0) and PD3 (INT1) respectively.
 * - Ensure proper power supply and grounding.
 *
 * @section usage_sec Usage
 *
 * - Press the direction button to toggle between clockwise (CW) and counter-clockwise (CCW) rotation.
 * - Press the mode button to switch between full step and half step mode.
 * - View the motor direction and mode on the LCD display.
 *
 * @section author_sec Author
 *
 * Written by Motaz Ahmed Elsaman.
 *

 *
 */
