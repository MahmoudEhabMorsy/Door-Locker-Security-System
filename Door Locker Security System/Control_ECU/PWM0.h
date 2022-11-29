/******************************************************************************
 *
 * Module: PWM Timer0 Driver
 *
 * File Name: PWM0.h
 *
 * Description: header file for the ATmega32 PWM driver
 *
 * Author: Mahmoud Ehab
 *
 *******************************************************************************/


#ifndef PWM0_H_
#define PWM0_H_
#include"std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define PWM0_PORT_ID PORTB_ID
#define PWM0_PIN_ID	PIN3_ID
#define DUTY_CYCLE_SCALER 2.55 /*to scale the duty cycle from 0 to 100*/

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * The function responsible for triggering the Timer0 with the PWM Mode.
 * Setup the PWM mode with Non-Inverting.
 * Setup the prescaler with F_CPU/8.
 * Setup the compare value based on the required input duty cycle
 * Setup the direction for OC0 as output pin through the GPIO driver.
 * The generated PWM signal frequency will be 500Hz to control the DC Motor speed.
 */
void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM0_H_ */
