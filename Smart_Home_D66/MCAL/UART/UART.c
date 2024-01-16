/****************************************/
/*			Auther: Kareem Atef         */
/*          Date: 26-11-2023            */
/*          Version: 1.0                */
/*          Module: UART                */
/****************************************/

#include "../../STD_Types.h"
#include "../../Bit_Manipulation.h"
#include "../../AVR32_Reg_Private.h"
#include "../../HAL/EEPROM/EEPROM.h"
#include "../../HAL/Buzzer/Buzzer.h"
#include "../../HAL/Leds_5/Leds.h"
#include "UART.h"
#include <stdio.h>
#include <string.h>

extern uint8 MAX_Users_Count;
extern uint8 FailCount;

void UART_Enable_TX() { Set(UCSRB_Reg, 4);}
void UART_Disable_TX() {Clear(UCSRB_Reg, 4);}
void UART_Enable_RX() {	Set(UCSRB_Reg, 3);}
void UART_Disable_RX() {Clear(UCSRB_Reg, 3);}
void UART_Enable_TX_Interrupt() {Set(UCSRB_Reg, 6);}
void UART_Disable_TX_Interrupt() {Clear(UCSRB_Reg, 6);}
void UART_Enable_RX_Interrupt() {Set(UCSRB_Reg, 7);}
void UART_Disable_RX_Interrupt() {Clear(UCSRB_Reg, 7);}
void UART_Enable_DataReg_Empty_Interrupt() {Set(UCSRB_Reg, 5);}
void UART_Disable_DataReg_Empty_Interrupt() {Clear(UCSRB_Reg, 5);}

void UART_Init() {
	/****************************   Setting Frame Format :   *******************************/
	//Setting Communication Character Size:
	#if(Comm_Character_Size == Comm_Size_5)
	Clear(UCSRB_Reg, 2);
	Clear(UCSRC_Reg, 2);
	Clear(UCSRC_Reg, 1); //000
	#elif(Comm_Character_Size == Comm_Size_6)
	Clear(UCSRB_Reg, 2);
	Clear(UCSRC_Reg, 2);
	Set(UCSRC_Reg, 1); //001
	#elif(Comm_Character_Size == Comm_Size_7)
	Clear(UCSRB_Reg, 2);
	Set(UCSRC_Reg, 2);
	Clear(UCSRC_Reg, 1); //010
	#elif(Comm_Character_Size == Comm_Size_8)
	Clear(UCSRB_Reg, 2);
	Set(UCSRC_Reg, 2);
	Set(UCSRC_Reg, 1); //011
	#elif(Comm_Character_Size == Comm_Size_9)
	Set(UCSRB_Reg, 2);
	Set(UCSRC_Reg, 2);
	Set(UCSRC_Reg, 1); //111
	#endif
	//Setting Sync Mode:
	#if(Sync_Mode_Select == Async_Mode)
	Clear(UCSRC_Reg, 6);
	#elif(Sync_Mode_Select == Sync_Mode)
	Set(UCSRC_Reg, 6);
	#if(Sync_Mode_Clock_Polarity == TX_Rising_XCK)
	Clear(UCSRC_Reg, 0);
	#elif(Sync_Mode_Clock_Polarity == TX_Falling_XCK)
	Set(UCSRC_Reg, 0);
	#endif
	#endif
	//Setting Parity Mode:
	Set(UCSRC_Reg, 7); // To Select UCSRC Reg
	#if(Parity_Mode == Parity_Even)
	Set(UCSRC_Reg, 5);
	Clear(UCSRC_Reg, 4);
	#elif(Parity_Mode == Parity_Disable)
	Clear(UCSRC_Reg, 5);
	Clear(UCSRC_Reg, 4);
	#elif(Parity_Mode == Parity_Odd)
	Set(UCSRC_Reg, 5);
	Set(UCSRC_Reg, 4);
	#endif
	//Setting Stop Bit Count:
	#if(Stop_bit_size == Stop1_Bit)
	Clear(UCSRC_Reg, 3);
	#elif(Stop_bit_size == Stop2_Bit)
	Set(UCSRC_Reg, 3);
	#endif
	/****************************   Setting The Double Speed Configuration  *******************************/
	// 	#if    (Double_Speed_Mode == Disabled)
	// 	Clear(UCSRA_Reg,1);
	// 	uint16 UBRR_value = UBRR_16;
	// 	#elif (Double_Speed_Mode == Enabled)
	// 	Set(UCSRA_Reg,1);
	// 	uint16 UBRR_value = UBRR_8;
	// 	#endif
	/****************************   Setting The Baud Rate Configuration  *******************************/
	//Clear(UBRRH_Reg,7);    // To Select UBBRR Reg
	//UBRR_value = UBRR_value & 0b0000111111111111; //0b 0000 1111 11111111 FILTER
	uint16 UBRR_value = 103;
	UBRRH_Reg = (UBRR_value >> 8);
	UBRRL_Reg = (uint8) UBRR_value;
	/****************************   Setting The TX/RX State  *******************************/
	//Enabling the Transmitter
	#if(UART_TX_Enable == Enabled)
	Set(UCSRB_Reg, 4);
	#endif
	//Enabling the Receiver
	#if(UART_RX_Enable == Enabled)
	Set(UCSRB_Reg, 3);
	#endif
	/****************************   Setting The Interrupt  *******************************/
	//Enabling the RX Complete Interrupt
	#if(UART_TX_Complete_Interrupt == Enabled)
	Set(UCSRB_Reg, 6);
	#endif
	//Enabling the Complete Interrupt
	#if(UART_RX_Complete_Interrupt == Enabled)
	Set(UCSRB_Reg, 7);
	#endif
	//Enabling the  Data Register Empty Interrupt
	#if(UART_Data_Reg_Empty_Interrupt == Enabled)
	Set(UCSRB_Reg, 5);
	#endif
}
void UART_Send_Byte_Polling_8(uint8 Data) {
	while (!Get(UCSRA_Reg, 5)); // Polling Method On Pin5 UCSRA Reg /Data Register Empty
	UDR_Reg = Data;
}
void UART_Send_String_Polling_8(sint8 * String) {
	uint8 count = 0;
	while (String[count] != '\0') {
		UART_Send_Byte_Polling_8(String[count]);
		count++;
	}
}
uint8 UART_Receive_Byte_8(void) {
	while (Get(UCSRA_Reg, 7) == 0); // Polling Method
	return UDR_Reg;
}
void UART_Recieve_String_8(sint8 * String) {
	uint8 count = 0;
	while (1) {
		String[count] = UART_Receive_Byte_8();
		if (String[count] == 13){String[count] = 0;}//Remove at Carriage Return \r
		if (String[count] == '\n') { // ASCII  NEW LINE Bluetooth END TRANSMISSION
			String[count] = '\0';
			break;
			} else {
			count++;
		}
	}
}
void UART_Send_Number_Polling_32(uint32 Number){
	char string[10];
	UART_Send_String_Polling_8(string);
	sprintf(string, "%lu", Number);
}
uint32 UART_Recieve_Number_Polling_32(void){
	uint32 Number;
	Number = 0;
	return Number;
}


void UART_Show_Request_UserID(){
	UART_Send_String_Polling_8(" Enter UserID:(1~99)\n");
}
void UART_Show_Request_Password(){
	UART_Send_String_Polling_8(" Enter Password:\n");
}
void UART_Show_Invalid(){
	UART_Send_String_Polling_8("Invalid input  try Again \n");
}
void UART_Show_MainMenu(){
	UART_Send_String_Polling_8("Choose Option: \n");
	UART_Send_String_Polling_8("(1) Control Led \n");
	UART_Send_String_Polling_8("(2) Control AC \n");
	UART_Send_String_Polling_8("(3) Control Door \n");
	UART_Send_String_Polling_8("(4) Control Dimmer \n");
	UART_Send_String_Polling_8("(5) Show Users list \n");
	UART_Send_String_Polling_8("(6) Create New User \n");
	UART_Send_String_Polling_8("(7) Delete User \n");
}
void UART_Show_Control_Leds(){
	UART_Send_String_Polling_8("(1) Toggle Led 1 \n");
	UART_Send_String_Polling_8("(2) Toggle Led 2 \n");
	UART_Send_String_Polling_8("(3) Toggle Led 3 \n");
	UART_Send_String_Polling_8("(4) Toggle Led 4 \n");
	UART_Send_String_Polling_8("(5) Toggle Led 5 \n");
	UART_Send_String_Polling_8("(0) Back \n");
}
void UART_Show_Control_AC(){
	UART_Send_String_Polling_8("(1) AC Auto \n");
	UART_Send_String_Polling_8("(2) AC Manual Turn Off \n");
	UART_Send_String_Polling_8("(3) AC Manual Turn On  \n");
	UART_Send_String_Polling_8("(0) Back \n");
}
void UART_Show_Control_Door(){
	UART_Send_String_Polling_8("(1) Open Door Lock \n");
	UART_Send_String_Polling_8("(2) Close Door Lock \n");
	UART_Send_String_Polling_8("(0) Back \n");

}
void UART_Show_Control_Dimmer(){
	UART_Send_String_Polling_8("(1) Dimmer Up \n");
	UART_Send_String_Polling_8("(2) Dimmer Down \n");
	UART_Send_String_Polling_8("(3) Dimmer Off \n");
	UART_Send_String_Polling_8("(4) Dimmer On \n");
	UART_Send_String_Polling_8("(0) Back \n");

}

void UART_Show_User_List(){
	sint8 Usernamex[8];
	for(uint8 id = 1;id<MAX_Users_Count;id++){
		if(EEPROM_Read_UserID_Exist(id)){
			UART_Send_String_Polling_8("UserID: ");
			UART_Send_Byte_Polling_8(id + 48);
			UART_Send_String_Polling_8(" Username: ");
			EEPROM_Read_8Data(id,Usernamex,2);
			UART_Send_String_Polling_8(Usernamex);
			UART_Send_String_Polling_8("\n");
 		}
	}
}


void UART_Show_Delete_ID(){
	UART_Send_String_Polling_8("Enter UserID To Delete : \n");
}
void UART_Show_Request_NewUsername(){
	UART_Send_String_Polling_8("Enter The NewUser Username: \n");
}
void UART_Show_Request_NewUserPassword(){
	UART_Send_String_Polling_8("Enter The NewUser Password: \n");
}

void UART_Logged_OFF(){
	UART_Send_String_Polling_8("Logged off Success! \n");
	UART_Show_Request_UserID();
}
void UART_Choice_Handler_1(uint8 g_choice_1){
	switch(g_choice_1){
		case 1:UART_Show_Control_Leds();break;		//NEXT EXIST
		case 2:UART_Show_Control_AC();	break;		//NEXT EXIST
		case 3:UART_Show_Control_Door();break;		//NEXT EXIST
		case 4:UART_Show_Control_Dimmer();break;		//NEXT EXIST
		case 5:UART_Show_User_List();break;				// NO NEXT
		case 6:UART_Show_Request_NewUsername();break;	//NEXT EXIST
		case 7:UART_Show_Delete_ID();break;				//NEXT EXIST
		case 8:UART_Logged_OFF();break;					//NO NEXT
	}
}


void UART_Show_Login_Failed(){
	UART_Send_String_Polling_8("Login Failed, Try again! ");
	UART_Send_Byte_Polling_8(FailCount + 48);
	UART_Send_String_Polling_8(" \n");
}