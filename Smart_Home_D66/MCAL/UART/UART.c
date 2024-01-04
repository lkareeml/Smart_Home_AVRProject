/****************************************/
/* Atef         */
/*          Date: 26-11-2023            */
/*          Version: 1.0                */
/*          Module: UART                */
/****************************************/

#include "../../STD_Types.h"
#include "../../Bit_Manipulation.h"
#include "../../AVR32_Reg_Private.h"
#include "../../HAL/EEPROM/EEPROM.h"
#include "UART.h"
#include <stdio.h>

void UART_Dynamic_INIT(uint8 CommSize, uint8 SyncMode, uint8 SyncPolarity, uint8 ParityMode, uint8 Stopbit, uint8 X2SpeedMode, uint32 baudRate) {
	//Setting Communication Character Size:
	switch (CommSize) {
		case 5:
		Clear(UCSRB_Reg, 2);Clear(UCSRC_Reg, 2);Clear(UCSRC_Reg, 1);break;
		case 6:
		Clear(UCSRB_Reg, 2);Clear(UCSRC_Reg, 2);Set(UCSRC_Reg, 1);break;
		case 7:
		Clear(UCSRB_Reg, 2);Set(UCSRC_Reg, 2);Clear(UCSRC_Reg, 1);break;
		case 8:
		Clear(UCSRB_Reg, 2);Set(UCSRC_Reg, 2);Set(UCSRC_Reg, 1);break;
		case 9:
		Set(UCSRB_Reg, 2);Set(UCSRC_Reg, 2);Set(UCSRC_Reg, 1);break;
	} //Setting Sync Mode:
	switch (SyncMode) {
		case 1:
		Clear(UCSRC_Reg, 6);
		break; //Async_Mode
		case 0:
		Set(UCSRC_Reg, 6); //Sync_Mode //TX_Rising_XCK (1) TX_Falling_XCK (0)
		if (SyncPolarity == 0) {
			Clear(UCSRC_Reg, 0);
			} else if (SyncPolarity == 1) {
			Set(UCSRC_Reg, 0);
		}
		break;
	} //Setting Parity Mode:
	Set(UCSRC_Reg, 7); // To Select UCSRC Reg
	switch (ParityMode) {
		case 0:
		Clear(UCSRC_Reg, 5);
		Clear(UCSRC_Reg, 4);
		break; //Parity_Disable
		case 1:
		Set(UCSRC_Reg, 5);
		Set(UCSRC_Reg, 4);
		break; //Parity Odd
		case 2:
		Set(UCSRC_Reg, 5);
		Clear(UCSRC_Reg, 4);
		break; //Parity_Even
	} //Setting Stop Bit Count:
	switch (Stopbit) {
		case 1:
		Clear(UCSRC_Reg, 3);
		break; //1_Bit
		case 2:
		Set(UCSRC_Reg, 3);
		break; //0_Bit
	}
	uint16 UBRR_value = 0;// = 1000000/ (baudRate - 1) ;//103;
	switch(X2SpeedMode){//0 Disabled 1 Enabled
		case 0:Clear(UCSRA_Reg,1);UBRR_value = (1000000/(baudRate-1));break;
		case 1:Set(UCSRA_Reg,1);UBRR_value = (2000000/(baudRate-1));break;
	}
	UBRRH_Reg = (UBRR_value >> 8);
	UBRRL_Reg = (uint8) UBRR_value;
}

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



static uint8 Global_UART_count = 0;
static uint8 Global_UART_Array[9];
static uint8 Global_UART_Send_Response_State = 0;

// 0 means nothing send nothing to receive
// 1 means Sending now
// 2 means Sent waiting for reply
// 3 means Sent and Processing Reply
// 4 means Sent and Got Reply Processed
// 10 means Idle

void Reset_UART_Counter(){
	Global_UART_count = 0;
}
void Reset_UART_VARS(){
	Global_UART_count = 0;
	for(int i =0;i<9;i++){
		Global_UART_Array[i] = 0;
	}
}

void UART_Choice_Handler(){
	
}
// 
// void UART_Interrupt_Handler(USART0_RX_vect){
// 	Global_UART_Array[Global_UART_count] = UDR_Reg;
// 	if (Global_UART_Array[Global_UART_count] == 13)
// 	{//Remove at Carriage Return \r
// 		Global_UART_Array[Global_UART_count] = 0;
// 	}
// 	if (Global_UART_Array[Global_UART_count] == '\n')
// 	{ // ASCII  NEW LINE Bluetooth END TRANSMISSION
// 		Global_UART_Array[Global_UART_count] = '\0';
// 		//Reset_UART_Counter();
// 		
// 	}
// 	else{
// 		Global_UART_count++;
// 	}
// }








uint8 UART_Receive_Byte_8(void) {
	while (Get(UCSRA_Reg, 7) == 0); // Polling Method
	return UDR_Reg;
}

uint8 UART_Receive_CHECK(){
	return Get(UCSRA_Reg, 7);// 0 means nothing sent, 1 something sent
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

void   UART_Send_Number_Polling_32(uint32 Number){
	char string[10];
	UART_Send_String_Polling_8(string);
	sprintf(string, "%lu", Number);
}

uint32 UART_Recieve_Number_Polling_32(void){
	uint32 Number;
	Number = 0;
	return Number;
}


















// #if (Comm_Character_Size == Comm_Size_9)
//     void UART_Send_Byte_Polling16(uint16 Data){
//         while(!Get(UCSRA_Reg,5));// Polling Method On Pin5 UCSRA Reg /Data Register Empty
//         if(1&(Data>>8)){
//             Set(UCSRB_Reg,0);
//         }else{
//             Clear(UCSRB_Reg,0);
//         }
//         UDR_Reg = (uint8)(Data & 0b0000000011111111);
//     }
//     uint16 UART_Receive_Byte16(void)
//     {
//         while(!Get(UCSRA_Reg,7));
//         uint16 Received_Data = 0;
//         if(Get(UCSRB_Reg,1)){
//             Received_Data = ((1<<8) | UDR_Reg);
//             }else{
//             Received_Data = UDR_Reg;
//         }
//         return Received_Data;
//     }
//     void UART_Send_String_Polling16(uint16 String){
//         uint8 count = 0;
//         while(String[count] != '\0'){
//             UART_Send_Byte_Polling16(String[count]);
//             count++;
//         }
//     }
//     void UART_Recieve_String16(uint16 *String){
//         uint8 count = 0;
//         while(1){
//             String[count] = UART_Receive_Byte16();
//             if(String[count] == 46){// ASCII 46 Point  //4 is [END TRANSMISSION]
//                 break;
//             }
//             else{
//                 count++;
//             }
//         }
//     }
// #endif














void UART_Show_Invalid(){
	UART_Send_String_Polling_8("Invalid input Please try Again \n");
}




void UART_Show_MainMenu(){
	UART_Send_String_Polling_8("Choose Option: \n");
	UART_Send_String_Polling_8("(1) Control Appliances \n");
	UART_Send_String_Polling_8("(2) User Management \n");
	UART_Send_String_Polling_8("(3) Settings \n");
}
void UART_Show_Control_Appliances(){
	UART_Send_String_Polling_8("(1) Control Leds \n");
	UART_Send_String_Polling_8("(2) Control AC \n");
	UART_Send_String_Polling_8("(3) Control Door \n");
	UART_Send_String_Polling_8("(4) Control Dimmer \n");
}
void UART_Show_Control_Leds(){
	UART_Send_String_Polling_8("Led 1 : (11)On, (01)Off \n");
	UART_Send_String_Polling_8("Led 2 : (12)On, (02)Off \n");
	UART_Send_String_Polling_8("Led 3 : (13)On, (03)Off \n");
	UART_Send_String_Polling_8("Led 4 : (14)On, (04)Off \n");
	UART_Send_String_Polling_8("Led 5 : (15)On, (05)Off \n");
}
void UART_Show_Control_AC(){
	UART_Send_String_Polling_8("(1) AC Auto \n");
	UART_Send_String_Polling_8("(2) AC Manual Turn Off \n");
	UART_Send_String_Polling_8("(3) AC Manual Turn On  \n");
}
void UART_Show_Control_Door(){
	UART_Send_String_Polling_8("(1) Open Door Lock \n");
	UART_Send_String_Polling_8("(2) Close Door Lock \n");
}
void UART_Show_Control_Dimmer(){
	UART_Send_String_Polling_8("(1) Dimmer Up \n");
	UART_Send_String_Polling_8("(2) Dimmer Down \n");
	UART_Send_String_Polling_8("(3) Dimmer Off \n");
	UART_Send_String_Polling_8("(4) Dimmer On \n");
}
void UART_Show_UserManagement(){
	UART_Send_String_Polling_8("(1) Show Users list \n");
	UART_Send_String_Polling_8("(2) Create New User \n");
	UART_Send_String_Polling_8("(3) Delete Existing User \n");
	UART_Send_String_Polling_8("(4) Delete All Users \n");
	UART_Send_String_Polling_8("(5) Change User Password \n");
	UART_Send_String_Polling_8("(6) Change User Username \n");
}
void UART_Show_Settings(){
	UART_Send_String_Polling_8("(1) Test for more options \n");
	UART_Send_String_Polling_8("(2) Factory Reset \n");
}


void UART_Show_User_List(){
	for(uint8 id = 0;id<4;id++){
		sint8 Username[8];
		if(EEPROM_Read_User(id,Username)){
			UART_Send_String_Polling_8("UserID: ");
			UART_Send_Byte_Polling_8(id);
			UART_Send_String_Polling_8(" Username: ");
			UART_Send_String_Polling_8(Username);
			UART_Send_String_Polling_8("/n");
		}
	}
}
void UART_EEPROM_Delete_All_Users(){
	for(uint8 id = 0;id<4;id++){
		if(EEPROM_Delete_User(id)){
			UART_Send_String_Polling_8("UserID: ");
			UART_Send_Byte_Polling_8(id);
			UART_Send_String_Polling_8(" Successfully\n");
		}
	}
	UART_Send_String_Polling_8("Deleted All Users Successfully \n");
}



