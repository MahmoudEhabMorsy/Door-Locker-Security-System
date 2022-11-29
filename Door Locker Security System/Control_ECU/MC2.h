/*
 * MC2.h
 *
 *  Created on: 31 Oct 2022
 *      Author: Mahmoud Ehab
 */

#ifndef MC2_H_
#define MC2_H_

#include<avr/io.h>
#include"util/delay.h"
#include"DcMotor.h"
#include"buzzer.h"
#include"external_eeprom.h"
#include"timer1.h"
#include"uart.h"
#include"twi.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define PASSWORD_LENGTH 5 /*password length*/
#define PASS_ADD 0x00 /*address of the password in eeprom*/
#define UART_BAUDRATE 9600
#define TIMER1_INITIAL_VALUE 0
#define TIMER1_OCRA_VALUE 31250
#define PASS_MATCH 1
#define PASS_NOT_MATCHED 0
/* Definitions used to communicate with MC1 */
#define UNLOCK			0x10
#define LOCK			0x50
#define LOCKED			0xB0
#define UNLOCKED		0xB1
#define CONTINUE		0XAB
#define getready			0xA0
#define ready				0xA1
#define	done				0xA2
#define MATCHED	0x01
#define UNMATCHED 0xff
#define WARNING 0
/*Definitions for calculating time using timer1*/
#define TIME_OF_UNLOCKING 15
#define TIME_HOLDING_THE_MOTOR 3
#define TIME_OF_LOCKING   15
#define TIME_SYSTEM_IS_LOCKED 60

/*******************************************************************************
 *                                Global Variables                                  *
 *******************************************************************************/

uint8 g_passStored[PASSWORD_LENGTH], g_passRecieved[PASSWORD_LENGTH], i,
		g_flag_repeat = 0, g_flag_change_pass = 0, g_status;
uint16 g_second = 0;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * a function to create the password for the system
 */
void createPassword(void);
/*
 * Description :
 * callback function for the timer's ISR
 */
void secondPassed(void);
/*
 * Description :
 * a function to calculate time using timer
 * Inputs: time to be counted
 */
void wait(uint16 time);
/*
 * Description :
 * a function to compare two (equal in size) arrays (the two passwords)
 * Inputs: 1&2-two arrays	3-size of those arrays
 * returns: 1 if the two arrays match 0 if the two arrays are diffrent
 */
uint8 comparePassword(uint8 *pass1_c, uint8 *pass2_c, uint8 pass_size);
/*
 * Description :
 * a function to recieve password using uart
 * Inputs:an array to recieve the password in it
 */
void recievePassword(uint8*);
/*
 * Description :
 * a function to store the password recieved in the eeprom
 * Inputs: 1-address of the location in the eeprom where the password is to be stored
 * 		   2-an array in which the password to be stored in the eeprom is located in it
 */
void storePassword(uint16, uint8*);
/*
 * Description :
 * a function to retrive the password from eeprom and put it in a global variable (g_passStored)
 */
void retrivePassword(void);
/*
 * Description :
 * a function for the ('+': open door) option
 * the function opens the door if the password entered is matched with the one stored in the eeprom
 * the function also lock the system if the password entered doesnot match the password stored in the eeprom three consecutive time
 */
void openDoorOption(void);
/*
 * Description :
 * a function to change the password stored in the eeprom
 * the function asks for the old password and if matched it allows the user to change the password stored in the eeprom
 * if the password is not matched three consecutive times the function locks the system for 1 minute
 */
void changePasswordOption(void);

#endif /* MC2_H_ */
