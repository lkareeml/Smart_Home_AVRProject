/*****************************************************/
/*          Author : Developed by Kareem Atef        */
/*          Date: 10-11-2023		                 */
/*          Version: 1.0				             */
/*          Module: LCD                              */
/*****************************************************/

#define F_CPU 16000000UL
#include "../../STD_Types.h"
#include "../../MCAL/Digital_Input_Output/DIO.h"
#include "../../HAL/EEPROM/EEPROM.h"
#include "../../HAL/Keypad_4x4/Keypad_4x4.h"
#include "../../HAL/Buzzer/Buzzer.h"
#include "../../MCAL/UART/UART.h"
#include <stdlib.h>
#include <string.h>

#include "LCD.h"
#include <util/delay.h>


static uint8 Cursor_Position = 0;
extern uint8 Max_Failure_Login_Count;


extern uint8 LCD_g_step;
extern uint8 LCD_UserInput;
extern uint8 LCD_user_count;
extern uint8 LCD_pass_count;
extern uint8 LCD_ID;
extern sint8 LCD_UserID[8];
extern sint8 LCD_Password[8];
extern sint8 LCD_EE_Password[8];
extern sint8 LCD_EE_Username[8];
extern uint8 LCD_Fail_Count;
extern uint8 LCD_g_choice1;
extern uint8 LCD_g_choice2;



#define Get_Bit_Value(Byte_Data,Bit_Order)	((Byte_Data>>Bit_Order)&(0x01)) /// 0,1

void Send_Data_FallingEdge(){
	DIO_Set_Pin_Output(PORTBx,3,High);///E HIGH
	_delay_ms(3);
	DIO_Set_Pin_Output(PORTBx,3,Low);///E LOW
	_delay_ms(3);
}

void LCD_Init(){
	DIO_Set_Pin_Direction(PORTBx,1,Out); ///B1 RS OUT
	DIO_Set_Pin_Direction(PORTBx,2,Out); ///B2 RW OUT
	DIO_Set_Pin_Direction(PORTBx,3,Out); ///B3 E  OUT
	DIO_Set_Pin_Direction(PORTAx,4,Out); ///D4 OUT
	DIO_Set_Pin_Direction(PORTAx,5,Out); ///D5 OUT
	DIO_Set_Pin_Direction(PORTAx,6,Out); ///D6 OUT
	DIO_Set_Pin_Direction(PORTAx,7,Out); ///D7 OUT
	_delay_ms(35);
	LCD_Send_Cmd(0x33);
	_delay_ms(2);
	LCD_Send_Cmd(0x32);
	_delay_ms(2);
	LCD_Send_Cmd(0x28);
	_delay_ms(2);
	LCD_Send_Cmd(0x0F);
	_delay_ms(2);
	LCD_Send_Cmd(0x06);
	_delay_ms(2);
	LCD_Send_Cmd(0x01);
	_delay_ms(2);
	Cursor_Position = 0;
}

void LCD_Send_Cmd(uint8 cmd){
	DIO_Set_Pin_Output(PORTBx,1,Low);///RS LOW
	DIO_Set_Pin_Output(PORTBx,2,Low);///RW LOW
	
	DIO_Set_Pin_Output(PORTAx,4,Get_Bit_Value(cmd,4));///A4 CMD
	DIO_Set_Pin_Output(PORTAx,5,Get_Bit_Value(cmd,5));///A5 CMD
	DIO_Set_Pin_Output(PORTAx,6,Get_Bit_Value(cmd,6));///A6 CMD
	DIO_Set_Pin_Output(PORTAx,7,Get_Bit_Value(cmd,7));///A7 CMD
	Send_Data_FallingEdge();_delay_ms(5);
	DIO_Set_Pin_Output(PORTAx,4,Get_Bit_Value(cmd,0));///A0 CMD
	DIO_Set_Pin_Output(PORTAx,5,Get_Bit_Value(cmd,1));///A1 CMD
	DIO_Set_Pin_Output(PORTAx,6,Get_Bit_Value(cmd,2));///A2 CMD
	DIO_Set_Pin_Output(PORTAx,7,Get_Bit_Value(cmd,3));///A3 CMD
	Send_Data_FallingEdge();_delay_ms(5);
}

void LCD_Send_Char(uint8 character){
	if((character >= 32) && (character <= 125))
	{
		DIO_Set_Pin_Output(PORTBx,1,High);///RS High For DATA
		DIO_Set_Pin_Output(PORTBx,2,Low);///RW LOW
		DIO_Set_Pin_Output(PORTAx,4,Get_Bit_Value(character,4));///A4 CMD
		DIO_Set_Pin_Output(PORTAx,5,Get_Bit_Value(character,5));///A5 CMD
		DIO_Set_Pin_Output(PORTAx,6,Get_Bit_Value(character,6));///A6 CMD
		DIO_Set_Pin_Output(PORTAx,7,Get_Bit_Value(character,7));///A7 CMD
		Send_Data_FallingEdge();_delay_ms(5);
		DIO_Set_Pin_Output(PORTAx,4,Get_Bit_Value(character,0));///A0 CMD
		DIO_Set_Pin_Output(PORTAx,5,Get_Bit_Value(character,1));///A1 CMD
		DIO_Set_Pin_Output(PORTAx,6,Get_Bit_Value(character,2));///A2 CMD
		DIO_Set_Pin_Output(PORTAx,7,Get_Bit_Value(character,3));///A3 CMD
		Send_Data_FallingEdge();_delay_ms(5);
		LCD_Position_Handler(RIGHT);
	}
}

void LCD_Set_Cursor_Position(uint8 row, uint8 col){
	uint8 address = 0;
	address = ((row*(0x40))+(col));
	///Address + 1 bit for enable location move
	LCD_Send_Cmd(address|0x80);
	if(row == 0 && col <= 15){
		Cursor_Position = col;
		}else if(row == 1 && col <= 15){
		Cursor_Position = col + 16;
	}
}

void LCD_Send_Clear_Screen(){
	LCD_Send_Cmd(0x01);
	_delay_ms(2);
	Cursor_Position = 0;
}

void LCD_Cursor_Shift(Dir Direction){
	if (Direction == RIGHT && Cursor_Position < 31)
	{
		LCD_Send_Cmd(0x14);
		LCD_Position_Handler(Direction);
	}
	else if (Direction == LEFT && Cursor_Position >=0)
	{
		LCD_Send_Cmd(0x10);
		LCD_Position_Handler(Direction);
	}
}

void LCD_Delete_Last_Written(){
	if(Cursor_Position >=0){
		LCD_Cursor_Shift(LEFT);_delay_ms(2);
		LCD_Send_Char(' ');_delay_ms(2);
		LCD_Cursor_Shift(LEFT);_delay_ms(2);
	}
}

void LCD_Position_Handler(Dir Direction){
	if(Direction == RIGHT){// INCREASE
		if(Cursor_Position <= 14){
			Cursor_Position++;
			}else if(Cursor_Position == 15){
			Cursor_Position++;
			LCD_Set_Cursor_Position(1,0);
			}else if(Cursor_Position > 15 && Cursor_Position <= 30){
			Cursor_Position++;
			}else if(Cursor_Position == 31){
			Cursor_Position = 0;
			LCD_Set_Cursor_Position(0,0);
		}
	}
	if(Direction == LEFT){// DECREASE
		if(Cursor_Position > 0 && Cursor_Position <=15 ){
			Cursor_Position--;
			}else if(Cursor_Position == 16){
			LCD_Set_Cursor_Position(0,15);
			}else if(Cursor_Position > 16 && Cursor_Position <= 31){
			Cursor_Position--;
			}else if(Cursor_Position == 0){
			LCD_Set_Cursor_Position(1,15);
		}
	}
	
}

void LCD_Send_String(sint8 * str){
	uint8 index =0;
	while(str[index] != '\0'){
		LCD_Send_Char(str[index]);
		index++;
		_delay_ms(10);
	}
	_delay_ms(5);
}








void LCD_Show_Get_UserID(){
	LCD_Send_Clear_Screen();
	LCD_Send_String("Enter UserID:   ");
}
void LCD_Show_Get_Password(){
	LCD_Send_Clear_Screen();
	LCD_Send_String("Enter Password: ");
}
void LCD_Show_Welcome_User(){
	LCD_Send_Clear_Screen();
	LCD_Send_String("Welcome Back!   ");
}
void LCD_Show_AuthFailed(){
	//In case Wrong Password :
	LCD_Send_Clear_Screen();
	LCD_Send_String("Auth Failed!    ");
	LCD_Send_String("Try Again       ");
}
void LCD_Show_Invalid(){
	LCD_Send_Clear_Screen();
	LCD_Send_String("Invalid Selected");
	LCD_Send_String("Try Again       ");
}

void LCD_Show_Main_Options(){
	LCD_Send_Clear_Screen();
	LCD_Send_String("(1)AC  (2)User  ");
	LCD_Send_String("(3)Led (4)Dimmer");
}
void LCD_Show_Choose_AC(){
	LCD_Send_Clear_Screen();
	LCD_Send_String("AC(1)On  (3)Auto");
	LCD_Send_String("AC(2)Off (9)Exit");
}
void LCD_Show_Choose_LED(){
	LCD_Send_Clear_Screen();
	LCD_Send_String("Led No.(1)(2)(3)");
	LCD_Send_String("(4)(5)   (9)Exit");
}
void LCD_Show_Choose_Dimmer(){
	LCD_Send_Clear_Screen();
	LCD_Send_String("Dimmer Higher(1)");
	LCD_Send_String("(0)Lower (9)Exit");
}
void LCD_Show_Locked(){
	LCD_Send_Clear_Screen();
	LCD_Send_String("Auth Failed!    ");
	LCD_Send_String("System Locked   ");
}


void LCD_Process(){
	switch(LCD_g_step){
		case 1:LCD_Show_Get_UserID(); LCD_g_step++; break;
		case 2:
			LCD_UserInput = KEYPAD_Get_Pressed_Key();
			if (LCD_UserInput == '\0') break;
			if (LCD_UserInput >= '0' && LCD_UserInput <= '9'){
				LCD_Send_Char(LCD_UserInput);
				LCD_UserID[LCD_user_count] = LCD_UserInput;
				LCD_user_count++;
			}else if (LCD_UserInput == '*' && LCD_user_count > 0){
				LCD_Delete_Last_Written();
				LCD_UserID[LCD_user_count] = '\0';
				LCD_user_count--;
			}else if (LCD_UserInput >= 'A' && LCD_UserInput <= 'D') {
				LCD_g_step++;
			}
			LCD_UserInput = '\0';
			break;
		case 3:LCD_Show_Get_Password();LCD_g_step++;break;
		case 4:
			// Get user Input 
			LCD_UserInput = KEYPAD_Get_Pressed_Key();
			if (LCD_UserInput == '\0') break;
			if (LCD_UserInput >= '0' && LCD_UserInput <= '9'){
				LCD_Send_Char(LCD_UserInput);
				LCD_Password[LCD_pass_count] = LCD_UserInput;
				LCD_pass_count++;
				}else if (LCD_UserInput == '*' && LCD_pass_count > 0){
				LCD_Delete_Last_Written();
				LCD_Password[LCD_pass_count] = '\0';
				LCD_pass_count--;
				}else if (LCD_UserInput >= 'A' && LCD_UserInput <= 'D') {
				LCD_g_step++;
			}
			LCD_UserInput = '\0';
			break;
		case 5: 
			//User/Password Login Process
			// If User ELCD_UserInputist, Fetch Password
			LCD_ID = atoi(LCD_UserID);

			if(EEPROM_Read_UserID_Exist(LCD_ID)){
				EEPROM_Read_8Data(LCD_ID,LCD_EE_Password,1);
			}
			//If password Match, Login Success!
			if(strcmp(LCD_Password,LCD_EE_Password) == 0){
				EEPROM_Read_8Data(LCD_ID,LCD_EE_Username,2);
				LCD_Show_Welcome_User();
				LCD_Send_String(LCD_EE_Username);
				LCD_g_step++;
			}else{
				//Else Login Failed, Increase Fail Counter
				LCD_Show_AuthFailed();
				LCD_Fail_Count++;
				if(LCD_Fail_Count == Max_Failure_Login_Count){
					UART_Send_String_Polling_8("Login Failed, \nSystem Locked! \n");
					LCD_Show_Locked();
					BUZZER_ALARM_TILL_RESET();
				}
				LCD_g_step = 1;
			}
			break;
			
			
		case 6:_delay_ms(1000);LCD_Show_Main_Options();break;
		case 7:
			LCD_UserInput = KEYPAD_Get_Pressed_Key();
			if (LCD_UserInput == '\0') break;
			if (LCD_UserInput >= '0' && LCD_UserInput <= '4'){
				LCD_g_choice1 = LCD_UserInput - 48;
				LCD_UserInput = '\0';
				LCD_g_step++;
			}else{
				LCD_Show_Invalid();
				_delay_ms(1000);
				LCD_g_step--;
			}
			break;
			
		case 8:
			switch(LCD_g_choice1){
				case 1: break;
				case 2: break;
				case 3: break;
				case 4: break;
			}
	}

}


// First we show user the options
// Then we wait for user choice
//User show list code
//Handle Dimmer choice ...
//Dimmer Light Control Mode
//Now Search EEPROM For That User!!
// if User is created it will have ID in EEPROM
// User ELCD_UserInputist, Get Password From EEPROM
// User Exist Process Next to GetPassword
//Now Get Password from User EEPROM !!
// Now Flag the EEPROM THAT LCD USer Is Logged IN!
		
		
void LCD_Idle(uint8 AC_State, uint8 Temp){
	//    LCD-KEYPAD-IDLE_NO_LOGIN
	//[AC(ON) Temp(30c)] -- [AC(OFF)Temp(20c)]
	//[****(0) To Login] -- [****(0) To Login]

	if(Temp >60) return;// Protection as max temp is 60 degree
	sint8 snum[3];
	itoa(Temp, snum, 10);
	LCD_Send_Clear_Screen(); // Clears anything on screen
	LCD_Send_String("AC(O"); // AC(O as it can be On or Off
	if     (AC_State == 0) LCD_Send_String("N) ");
	else if(AC_State == 1) LCD_Send_String("FF)");
	LCD_Send_String("Temp(");
	LCD_Send_String(snum);
	if     (Temp <  10) LCD_Send_String("c) ");
	else if(Temp >= 10) LCD_Send_String("c)");
	LCD_Send_String("        (0) More");
}
