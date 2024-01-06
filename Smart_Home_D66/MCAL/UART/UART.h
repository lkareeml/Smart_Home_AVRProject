/*
 * UART.h
 *
 * Created: 12/20/2023 7:24:43 AM
 *
 */ 


#ifndef UART_H_
#define UART_H_

#define Disabled           0
#define Enabled            1
#define Async_Mode         0
#define Sync_Mode          1
#define Parity_Disable     0
#define Parity_Even        1
#define Parity_Odd         2
#define Stop1_Bit          0
#define Stop2_Bit          1
#define Comm_Size_5        5
#define Comm_Size_6        6
#define Comm_Size_7        7
#define Comm_Size_8        8
#define Comm_Size_9        9
#define TX_Rising_XCK      0
#define TX_Falling_XCK     1

// void   UART_Send_Byte_Polling16(uint16 Data);
// uint16 UART_Receive_Byte16(void);
// void   UART_Send_String_Polling16(uint16 *String);
// void   UART_Recieve_String16(uint16 *String);


//		Option_Name							Selected_Option			Available_Options
#define Double_Speed_Mode					Disabled				// Enabled -- Disabled
#define UART_BAUD							9600UL					// 2400UL ~~ 250000UL
#define Sync_Mode_Select					Async_Mode				// Async_Mode -- Sync_Mode
#define Parity_Mode							Parity_Disable			// Parity_Even -- Parity_Odd
#define Stop_bit_size						Stop1_Bit				// Stop2_Bit -- Stop1_Bit
#define Comm_Character_Size					Comm_Size_8				// Comm_Size_5.6.7.8.9
#define Sync_Mode_Clock_Polarity			TX_Rising_XCK			// TX_Rising_XCK -- TX_Falling_XCK
#define UART_TX_Enable						Enabled					// Enabled -- Disabled
#define UART_RX_Enable						Enabled					// Enabled -- Disabled
#define UART_TX_Complete_Interrupt			Disabled				// Enabled -- Disabled
#define UART_RX_Complete_Interrupt			Disabled				// Enabled -- Disabled
#define UART_Data_Reg_Empty_Interrupt		Disabled				// Enabled -- Disabled

//#define Multi_processor_Com_Mode	 >>>> (UCSRA_Reg,0);	Only operate for
// (the first stop or the ninth bit) is one  the frame contains an address.
// (the first stop or the ninth bit) is zero the frame is a data frame

void UART_Dynamic_INIT(uint8 CommSize,uint8 SyncMode,uint8 SyncPolarity,uint8 ParityMode,uint8 Stopbit,uint8 X2SpeedMode,uint32 baudRate);
void UART_Enable_TX();
void UART_Disable_TX();
void UART_Enable_RX();
void UART_Disable_RX();
void UART_Enable_TX_Interrupt();
void UART_Disable_TX_Interrupt();
void UART_Enable_RX_Interrupt();
void UART_Disable_RX_Interrupt();
void UART_Enable_DataReg_Empty_Interrupt();
void UART_Disable_DataReg_Empty_Interrupt();

void   UART_Init();
void   UART_Send_Byte_Polling_8(uint8 Data);
uint8  UART_Receive_Byte_8(void);
void   UART_Send_String_Polling_8(sint8 *String);
void   UART_Recieve_String_8(sint8 *String);
void   UART_Send_Number_Polling_32(uint32 Number);
uint32 UART_Recieve_Number_Polling_32(void);


void UART_User_Login(uint8 * global_Flag, sint8 * global_UART_Username, sint8 * global_EEPROM_Password, sint8 * global_UART_Password);
void UART_Show_MainMenu_Inside(uint8 choice_1);
void UART_Show_Control_Appliances_Inside(uint8 choice_2,sint8 * global_UART_UserID);


void UART_Show_Invalid();
void UART_Show_MainMenu();
void UART_Show_Control_Appliances();
void UART_Show_Control_Leds();
void UART_Show_Control_AC();
void UART_Show_Control_Door();
void UART_Show_Control_Dimmer();
void UART_Show_UserManagement();
void UART_Show_Settings();
void UART_Show_User_List();
void UART_EEPROM_Delete_All_Users();

#endif /* UART_H_ */