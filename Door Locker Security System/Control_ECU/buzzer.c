/*
 * buzzer.c
 *
 *  Created on: 28 Oct 2022
 *      Author: Mahmoud Ehab
 */
#include"buzzer.h"
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * initiate the buzzer
 */
void Buzzer_init(void) {
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}
/*
 * Description :
 * turn on the buzzer
 */
void Buzzer_on(void) {
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH);
}
/*
 * Description :
 * turn off the buzzer
 */
void Buzzer_off(void) {
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}

