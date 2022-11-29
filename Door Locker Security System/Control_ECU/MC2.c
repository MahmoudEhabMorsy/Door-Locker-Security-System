/*
 * MC2.c
 *
 *  Created on: 28 Oct 2022
 *      Author: Mahmoud Ehab
 */

#include"MC2.h"

int main(void) {
	UART_ConfigType UART_config = { EIGHT_BIT, EVEN, ONE_BIT, UART_BAUDRATE };/*inputs for the uart*/
	UART_init(&UART_config);/*initiation of the uart*/
	TWI_ConfigType TWI_config = { TWI_MC_SLAVE_ADDRESS, FAST };/*inputs for the i2c*/
	TWI_init(&TWI_config);/*initiation of the i2c*/
	DcMotor_init();/*initiation of the dc motor*/
	Buzzer_init();/*initiation of the buzzer*/
	Timer1_ConfigType Timer1_config = { TIMER1_INITIAL_VALUE, TIMER1_OCRA_VALUE,
			F_CPU_256, COMPARE_OCR };/*inputs of timer1*/
	Timer1_init(&Timer1_config);/*initiation of timer1*/
	Timer1_setCallBack(secondPassed);/*getting the address of the callback function*/
	SREG |= (1 << 7);/*enabling interrupts*/
	createPassword();/*create the password for the first time*/
	while (1) {
		g_status = UART_recieveByte();/*storing the choice of the user in a global variable*/
		if (g_status == '+') {
			openDoorOption();/*a function for the ('+': open door) option*/
			continue;
		} else if (g_status == '-') {
			changePasswordOption();/*a function to change the password stored in the eeprom*/
			if (g_flag_change_pass == 1) {
				g_flag_change_pass = 0;
				createPassword();/*creating a new password in case the user want to change the password*/
			}
		}
	}
}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * a function to create the password for the system
 */
void createPassword(void) {
	while (1) {
		uint8 pass1[PASSWORD_LENGTH];
		while (UART_recieveByte() != getready)/*waiting for the other MC to be ready*/
			;
		UART_sendByte(ready);/*send to the other MC that it is ready*/
		recievePassword(pass1);/*recieveing the password from the other MC*/
		UART_sendByte(done);/*sending confirmation that the password had been recieved successfully*/
		while (UART_recieveByte() != getready)/*waiting for the other MC to be ready*/
			;
		UART_sendByte(ready);/*send to the other MC that it is ready*/
		recievePassword(g_passRecieved);/*recieveing the second password from the other MC*/
		UART_sendByte(done);/*sending confirmation that the password had been recieved successfully*/
		if (comparePassword(pass1, g_passRecieved, PASSWORD_LENGTH)) {
			storePassword(PASS_ADD, g_passRecieved);/*storing the recieved password in the eeprom*/
			UART_sendByte(MATCHED);/*sending confirmation that the two password matched and a password had been created in the eeprom*/
			return;
		} else {
			UART_sendByte(UNMATCHED);/*sending accnolegment that the two password are not the same so for the user to send another two passwords*/
			continue;
		}
	}
}
/*
 * Description :
 * callback function for the timer's ISR
 */
void secondPassed(void) {
	g_second++;
}
/*
 * Description :
 * a function to calculate time using timer
 * Inputs: time to be counted
 */
void wait(uint16 time) {
	g_second = 0;
	while (g_second < time)
		;
}
/*
 * Description :
 * a function to recieve password using uart
 * Inputs:an array to recieve the password in it
 */
void recievePassword(uint8 *pass_r) {
	for (i = 0; i < PASSWORD_LENGTH; i++) {
		pass_r[i] = UART_recieveByte();
	}
}
/*
 * Description :
 * a function to store the password recieved in the eeprom
 * Inputs: 1-address of the location in the eeprom where the password is to be stored
 * 		   2-an array in which the password to be stored in the eeprom is located in it
 */
void storePassword(uint16 pass_s_addr, uint8 *pass_s) {
	for (i = 0; i < PASSWORD_LENGTH; i++) {
		EEPROM_writeByte(pass_s_addr + i, pass_s[i]);
		_delay_ms(100);
	}
}
/*
 * Description :
 * a function to compare two (equal in size) arrays (the two passwords)
 * Inputs: 1&2-two arrays	3-size of those arrays
 * returns: 1 if the two arrays match 0 if the two arrays are diffrent
 */
uint8 comparePassword(uint8 *pass1_c, uint8 *pass2_c, uint8 pass_size) {
	for (i = 0; i < pass_size; i++) {
		if (pass1_c[i] != pass2_c[i])
			return PASS_NOT_MATCHED;
	}
	return PASS_MATCH;
}
/*
 * Description :
 * a function to retrive the password from eeprom and put it in a global variable (g_passStored)
 */
void retrivePassword(void) {
	for (i = 0; i < PASSWORD_LENGTH; i++) {
		EEPROM_readByte(PASS_ADD + i, g_passStored + i);
	}
}
/*
 * Description :
 * a function for the ('+': open door) option
 * the function opens the door if the password entered is matched with the one stored in the eeprom
 * the function also lock the system if the password entered doesnot match the password stored in the eeprom three consecutive time
 */
void openDoorOption(void) {
	while (1) {
		while (UART_recieveByte() != getready)/*waiting for the other MC to be ready*/
			;
		UART_sendByte(ready);/*send to the other MC that it is ready*/
		recievePassword(g_passRecieved);/*recieveing the password from the other MC*/
		UART_sendByte(done);/*sending confirmation that the password had been recieved successfully*/
		retrivePassword();/*retrieving the password stored in the eeprom to compare it with the password recieved from the other Mc*/
		if (comparePassword(g_passRecieved, g_passStored,
		PASSWORD_LENGTH) == PASS_MATCH) {
			UART_sendByte(MATCHED);/*send confirmation that the two passwords are matched*/
			while (UART_recieveByte() != UNLOCK)/*waiting for answer*/
				;
			DcMotor_Rotate(CW, MAX_SPEED);/*rotating the motor to unlock the door*/
			wait(TIME_OF_UNLOCKING);/*keep the motor rotating for a specified time*/

			DcMotor_Rotate(STOP, MAX_SPEED);/*Stopping the motor*/
			wait(TIME_HOLDING_THE_MOTOR);/*keep the motor stopped for a specified time*/

			DcMotor_Rotate(ACW, MAX_SPEED);/*rotating the motor to lock the door*/
			wait(TIME_OF_LOCKING);/*keep the motor rotating for a specified time*/

			DcMotor_Rotate(STOP, MAX_SPEED);/*stopping the motor*/
			return;
		} else if (comparePassword(g_passRecieved, g_passStored,
		PASSWORD_LENGTH) == PASS_NOT_MATCHED) {
			UART_sendByte(UNMATCHED);/*sending acknowledgement that the two passwords didn't match*/
			g_status = UART_recieveByte();
			if (g_status == WARNING) {
				Buzzer_on();/*turning the buzzer on*/
				while (UART_recieveByte() != UNLOCK)/*waiting for a signal to turn of the buzzer*/
					;
				Buzzer_off();/*turning the buzzer off*/
				return;
			} else if (g_status == CONTINUE) {
				continue;
			}
		}
	}
}
/*
 * Description :
 * a function to change the password stored in the eeprom
 * the function asks for the old password and if matched it allows the user to change the password stored in the eeprom
 * if the password is not matched three consecutive times the function locks the system for 1 minute
 */
void changePasswordOption(void) {
	while (1) {
		while (UART_recieveByte() != getready)/*waiting for the other MC to be ready*/
			;
		UART_sendByte(ready);/*send to the other MC that it is ready*/
		recievePassword(g_passRecieved);/*recieveing the password from the other MC*/
		UART_sendByte(done);/*sending confirmation that the password had been recieved successfully*/
		retrivePassword();/*retrieving the password stored in the eeprom to compare it with the password recieved from the other Mc*/
		if (comparePassword(g_passRecieved, g_passStored, PASSWORD_LENGTH)) {
			UART_sendByte(MATCHED);/*send confirmation that the two passwords are matched*/
			g_flag_change_pass = 1;/*a flag for re-routing the code for the desired result*/
			break;
		} else {
			UART_sendByte(UNMATCHED);/*sending acknowledgement that the two passwords didn't match*/
			g_status = UART_recieveByte();
			if (g_status == WARNING) {
				Buzzer_on();/*turning the buzzer on*/
				while (UART_recieveByte() != UNLOCK)/*waiting for a signal to turn of the buzzer*/
					;
				Buzzer_off();/*turning the buzzer off*/
				break;
			} else if (g_status == CONTINUE) {
				continue;
			}
		}
	}
}
