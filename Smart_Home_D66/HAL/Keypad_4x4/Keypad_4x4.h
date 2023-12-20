/****************************************/
/*          Author : Kareem Atef        */
/*          Date: 10-11-2023		    */
/*          Version: 1.0				*/
/*          Module: Keypad              */
/****************************************/

#ifndef KEYPAD_4X4_H_
#define KEYPAD_4X4_H_

#define COL_PORTB 	PORTB
#define COL_Pin_0	Pin4
#define COL_Pin_1	Pin5
#define COL_Pin_2	Pin6
#define COL_Pin_3	Pin7

#define ROW_PORTC	PORTC
#define ROW_Pin_0	Pin3
#define ROW_Pin_1	Pin4
#define ROW_Pin_2	Pin5
#define ROW_Pin_3	Pin6

#define Output_R_0_C_0	'1'
#define Output_R_0_C_1	'4'
#define Output_R_0_C_2	'7'
#define Output_R_0_C_3	'*'
#define Output_R_1_C_0	'2'
#define Output_R_1_C_1	'5'
#define Output_R_1_C_2	'8'
#define Output_R_1_C_3	'0'
#define Output_R_2_C_0	'3'
#define Output_R_2_C_1	'6'
#define Output_R_2_C_2	'9'
#define Output_R_2_C_3	'#'
#define Output_R_3_C_0	'A'
#define Output_R_3_C_1	'B'
#define Output_R_3_C_2	'C'
#define Output_R_3_C_3	'D'

void KEYPAD_Initialization();
uint8 KEYPAD_Get_Pressed_Key();

#endif /* KEYPAD_4X4_H_ */