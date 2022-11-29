/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: DcMotor.h
 *
 * Description: header file for the ATmega32 DC Motor driver
 *
 * Author: Mahmoud Ehab
 *
 *******************************************************************************/

#ifndef DCMOTOR_H_
#define DCMOTOR_H_
#include"std_types.h"
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum {
	STOP, CW, ACW
} DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * The Function responsible for setup the direction for the two motor pins through the GPIO driver.
 * Stop the DC-Motor at the beginning through the GPIO driver.
 */
void DcMotor_init(void);

/*
 * Description :
 * The function responsible for rotating the DC Motor CW/ or A-CW or stop the motor based on the state input state value.
 * Send the required duty cycle to the PWM driver based on the required speed value.
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed);

#endif /* DCMOTOR_H_ */
