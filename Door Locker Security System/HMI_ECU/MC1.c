/*
 * main.c
 *
 *  Created on: 28 Oct 2022
 *      Author: Mahmoud Ehab
 */

#include"MC1.h"

int main(void) {

	UART_ConfigType UART_config = { EIGHT_BIT, EVEN, ONE_BIT, UART_BAUDRATE };/*inputs for the uart*/
	UART_init(&UART_config);/*initiation of the uart*/
	LCD_init();/*initiation of the lcd*/
	Timer1_ConfigType Timer1_config = { TIMER1_INITIAL_VALUE, TIMER1_OCRA_VALUE,
			F_CPU_256, COMPARE_OCR };/*inputs of timer1*/
	Timer1_init(&Timer1_config);/*initiation of timer1*/
	Timer1_setCallBack(secondPassed);/*getting the address of the callback function*/
	SREG |= (1 << 7);/*enabling interrupts*/
	/*Create System Password*/
	createPassword();/*sending instrucions and data to create the password for the first time*/
	while (1) {
		status = mainOptionsMenu();/*storing the choice of the user in a global variable to send it to the other MC*/
		_delay_ms(500);
		if (status == '+') {
			openDoorOption();/*a function to send instructions to the other MC to open the door*/
		} else if (status == '-') {
			changePasswordOption();/*a function to send instructions to the other MC to change the password stored in the eeprom*/
			if (g_flag_change_pass == 1) {
				g_flag_change_pass = 0;
				createPassword();/*sending instrucions and data to initiate creating a new password in case the user want to change the password*/
			}
		}
	}
}
/*
 * Description :
 * a function to send instructions and data to the other MC to create the password for the system
 */
void createPassword(void) {
	while (1) {
		LCD_clearScreen();
		LCD_displayString("Plz Enter Pass:");
		LCD_moveCursor(1, 0);
		for (i = 0; i < PASSWORD_LENGTH; i++) {
			key[i] = KEYPAD_getPressedKey();
			LCD_displayCharacter('*');
			_delay_ms(500);
		}
		while (KEYPAD_getPressedKey() != enter)/*waiting for user to press enter*/
			;
		_delay_ms(500);
		UART_sendByte(getready);/*sending a signal to the other MC to get ready*/
		while (UART_recieveByte() != ready)/*waiting for a confirmation that the other MC is ready*/
			;
		sendPassword(key);/*sending the password through uart*/
		while (UART_recieveByte() != done)/*waiting for confirmation that the password was recieved*/
			;
		LCD_clearScreen();
		LCD_displayString("Plz re-Enter the");
		LCD_displayStringRowColumn(1, 0, "same Pass: ");
		LCD_moveCursor(1, 10);
		for (i = 0; i < PASSWORD_LENGTH; i++) {
			key[i] = KEYPAD_getPressedKey();
			LCD_displayCharacter('*');
			_delay_ms(500);
		}
		while (KEYPAD_getPressedKey() != enter)/*waiting for user to press enter*/
			;
		_delay_ms(500);
		UART_sendByte(getready);/*sending a signal to the other MC to get ready*/
		while (UART_recieveByte() != ready)/*waiting for a confirmation that the other MC is ready*/
			;
		sendPassword(key);/*sending the password through uart*/
		while (UART_recieveByte() != done)/*waiting for confirmation that the password was recieved*/
			;
		if (UART_recieveByte() != MATCHED) {
			/*send pass to control_mcu*/
			LCD_clearScreen();
			LCD_displayString("The 2 Passwords ");
			LCD_displayStringRowColumn(1, 0, "arenot the same");
			wait(ONE_SECOND_DELAY);
			continue;
		} else {
			break;
		}
	}
}
/*
 * Description :
 * a function display options for the user and return the user's choice
 */
uint8 mainOptionsMenu(void) {
	LCD_clearScreen();
	LCD_displayString("+ : Open Door");
	LCD_displayStringRowColumn(1, 0, "- : Change Pass");
	return (KEYPAD_getPressedKey());
}
/*
 * Description :
 * a function to send instructions to the other MC to open the door
 * the function send the password entered by the user to the other MC then wait answer
 * if the answer is that the passwords are matched it displays the status of the door
 * if not it let the user send the password two more times to the other MC and if the user failed to get the password right it locks the system for 1 minute and prevent any input form the user
 */
void openDoorOption(void) {
	while (1) {
		UART_sendByte('+');
		LCD_clearScreen();
		LCD_displayString("Plz Enter Pass:");
		LCD_moveCursor(1, 0);
		for (i = 0; i < PASSWORD_LENGTH; i++) {
			key[i] = KEYPAD_getPressedKey();
			LCD_displayCharacter('*');
			_delay_ms(500);
		}
		while (KEYPAD_getPressedKey() != enter)/*waiting for user to press enter*/
			;
		_delay_ms(500);
		UART_sendByte(getready);/*sending a signal to the other MC to get ready*/
		while (UART_recieveByte() != ready)/*waiting for a confirmation that the other MC is ready*/
			;
		sendPassword(key);/*sending the password through uart*/
		while (UART_recieveByte() != done)/*waiting for confirmation that the password was recieved*/
			;
		/*if the two passwords are matched*/
		status = UART_recieveByte();
		if (status == MATCHED) {
			LCD_clearScreen();
			LCD_displayString("Correct");
			wait(ONE_SECOND_DELAY);
			counter = 0;
			UART_sendByte(UNLOCK);/*sending a signal to unlock the door*/
			LCD_clearScreen();
			LCD_displayString("Door Unlocking");
			waitWithTimer(TIME_OF_UNLOCKING);/*waiting for the door to unlock*/

			LCD_clearScreen();
			LCD_displayString("Door Unlocked   ");
			waitWithTimer(TIME_HOLDING_THE_MOTOR);/*time of the door kept open*/

			LCD_clearScreen();
			LCD_displayString("Door Locking   ");
			waitWithTimer(TIME_OF_LOCKING);/*waiting for the door to close*/

			LCD_clearScreen();
			LCD_displayString("Door Locked    ");
			wait(ONE_SECOND_DELAY);
			break;
		} else if (status == UNMATCHED) {
			counter++;
			LCD_clearScreen();
			LCD_displayString("Wrong");
			wait(ONE_SECOND_DELAY);
			if (counter == 3) {
				counter = 0;
				UART_sendByte(WARNING);/*sending signal to activate the buzzer*/
				LCD_clearScreen();
				LCD_displayString("System Locked");
				waitWithTimer(TIME_SYSTEM_IS_LOCKED);/*waiting for the system to unlock*/
				UART_sendByte(UNLOCK);/*sending a signal to unlock the system and turn off the buzzer*/
				break;
			} else {
				UART_sendByte(CONTINUE);/*sending a signal to continue recieveing the other passwords*/
				continue;
			}
		}
	}
}
/*
 * Description :
 * a function to send instructions to the other MC to change the password stored in the eeprom
 * the function asks for the old password and then send it to the other MC and waits for an answer
 * if the answer is that the passwords are matched it sends the user to the option of creating a new password and send it to the other MC
 * if not it let the user send the password two more times to the other MC and if the user failed to get the password right it locks the system for 1 minute and prevent any input form the user
 */
void changePasswordOption(void) {
	while (1) {
		UART_sendByte('-');
		LCD_clearScreen();
		LCD_displayString("Plz Enter Pass:");
		LCD_moveCursor(1, 0);
		for (i = 0; i < PASSWORD_LENGTH; i++) {
			key[i] = KEYPAD_getPressedKey();
			LCD_displayCharacter('*');
			_delay_ms(500);
		}
		while (KEYPAD_getPressedKey() != enter)/*waiting for user to press enter*/
			;
		_delay_ms(500);
		UART_sendByte(getready);/*sending a signal to the other MC to get ready*/
		while (UART_recieveByte() != ready)/*waiting for a confirmation that the other MC is ready*/
			;
		sendPassword(key);/*sending the password through uart*/
		while (UART_recieveByte() != done)/*waiting for confirmation that the password was recieved*/
			;
		status = UART_recieveByte();
		if (status == MATCHED) {
			LCD_clearScreen();
			LCD_displayString("Correct");
			wait(ONE_SECOND_DELAY);
			counter = 0;
			g_flag_change_pass = 1;
			break;
		} else if (status == UNMATCHED) {
			counter++;
			LCD_clearScreen();
			LCD_displayString("Wrong");
			wait(ONE_SECOND_DELAY);
			if (counter == 3) {
				counter = 0;
				UART_sendByte(WARNING);/*sending a signal to activate the buzzer*/
				LCD_clearScreen();
				LCD_displayString("System Locked");
				waitWithTimer(TIME_SYSTEM_IS_LOCKED);/*waiting for the system to unlock*/
				UART_sendByte(UNLOCK);/*sending a signal to unlock the system and turn off the buzzer*/
				break;
			} else {
				UART_sendByte(CONTINUE);/*sending a signal to continue recieveing the other passwords*/
				continue;
			}
		}
	}
}
/*
 * Description :
 * a function to send password using uart
 * Inputs:an array to send the password in it
 */
void sendPassword(uint8 *pass) {
	for (i = 0; i < PASSWORD_LENGTH; i++) {
		UART_sendByte(pass[i]);
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
 * a function to calculate time using timer and displaying the remaining time
 * Inputs: time to be counted
 */
void waitWithTimer(uint16 time) {
	g_second = 0;
	while (g_second < time) {
		LCD_moveCursor(1, 0);
		LCD_intgerToString(time - g_second);
		if ((time - g_second) < 10) {
			LCD_displayString(" ");
		}
	}
}
