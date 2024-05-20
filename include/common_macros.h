/*
 * common_macros.h
 *
 *  Created on: Dec 22, 2023
 *      Author: Motaz
 */

#ifndef SRC_COMMON_MACROS_H_
#define SRC_COMMON_MACROS_H_

/* Set a certain bit in any register */
#define SET_BIT(REG,BIT) (REG|=(1<<BIT))

/* Clear a certain bit in any register */
#define CLEAR_BIT(REG,BIT) (REG&=(~(1<<BIT)))

/* Toggle a certain bit in any register */
#define TOGGLE_BIT(REG,BIT) (REG^=(1<<BIT))

/* AndING two numbers with each other */
#define AND(DATA1,DATA2) ((DATA1)&(DATA2))

/* ORING two numbers with each other */
#define OR(DATA1,DATA2) ((DATA1)|(DATA2))
/* Rotate right the register value with specific number of rotates */
#define ROR(REG,num) ( REG= (REG>>num) | (REG<<(8-num)) )

/* Rotate left the register value with specific number of rotates */
#define ROL(REG,num) ( REG= (REG<<num) | (REG>>(8-num)) )

/* Check if a specific bit is set in any register and return true if yes */
#define BIT_IS_SET(REG,BIT) ( REG & (1<<BIT) )
/*Shift a number to Right with certain number of Bits */
#define SIFT_TO_RIGHT(DATA,N_SHIFT) ( (DATA) >> (N_SHIFT))

/*Shift a number to Left with certain number of Bits */
#define SIFT_TO_LEFT(DATA,N_SHIFT) ( (DATA) << (N_SHIFT))

/* Check if a specific bit is cleared in any register and return true if yes */
#define BIT_IS_CLEAR(REG,BIT) ( !(REG & (1<<BIT)) )


#endif /* SRC_COMMON_MACROS_H_ */
