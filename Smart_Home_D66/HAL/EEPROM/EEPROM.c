/****************************************/
/*          Author : Kareem Atef        */
/*          Date: 4-12-2023             */
/*          Version: 1.0                */
/*          Module: EEPROM              */
/****************************************/
#define F_CPU 16000000UL
#include "../../STD_Types.h"
#include "../../MCAL/TWI_I2C/TWI_I2C.h"
#include "EEPROM.h"
#include <util/delay.h>


void  EEPROM_Init(void){TWI_Init();}

uint8 EEPROM_Read_Byte(uint16 Address){
	uint8 Data = 0;
	Address &= 0b0000011111111111 ; // Filter the address for protection
	uint8 SLA_W_address = 0b10100000;
	SLA_W_address |= ((((Address >> 10)&1)<<3) | (((Address >> 9)&1)<<2) | (((Address >> 8)&1)<<1));
	uint8 SLA_R_address = SLA_W_address +1;
	TWI_MSTR_Send_Start();
	if(TWI_Check_Status(MSTR_TR_START)==1){
		TWI_Send_Data(SLA_W_address);
		if(TWI_Check_Status(MSTR_T_SLA_W_ACK)==1){
			TWI_Send_Data((uint8)Address);
			if(TWI_Check_Status(MSTR_T_DATA_ACK)==1){
				TWI_MSTR_Send_Start();
				if(TWI_Check_Status(MSTR_TR_REPEAT_START)==1){
					TWI_Send_Data(SLA_R_address);
					if(TWI_Check_Status(MSTR_R_SLA_R_ACK)==1){
						Data = TWI_Receive_Data(0);
						TWI_Send_Stop();
					}
				}
			}
		}
	}
	_delay_ms(10);
	return Data;
}

void  EEPROM_Write_Byte(uint16 Address , uint8 Data){
	//Address &= 0b0000011111111111 ; // Filter the address for protection
	uint8 SLA_W_address = 0b10100000; // Frame For EXT EEPROM
	SLA_W_address |= ((((Address >> 10)&1)<<3) | (((Address >> 9)&1)<<2) | (((Address >> 8)&1)<<1));
	TWI_MSTR_Send_Start();
	if(TWI_Check_Status(MSTR_TR_START)==1){
		TWI_Send_Data(SLA_W_address);
		if(TWI_Check_Status(MSTR_T_SLA_W_ACK)==1){
			TWI_Send_Data((uint8)Address);
			if(TWI_Check_Status(MSTR_T_DATA_ACK)){
				TWI_Send_Data(Data);
				if(TWI_Check_Status(MSTR_T_DATA_ACK)){
					TWI_Send_Stop();
					_delay_ms(10);
				}
			}
		}
	}
}



//strlen() function doesn't count the null character \0

uint16 EEPROM_Write_String(uint16 Address , sint8* String){
	uint8 i = 0;
	while(String[i] != '\0'){
		EEPROM_Write_Byte((Address+i), String[i]);
		i++;
	}
	EEPROM_Write_Byte((Address+i), String[i]);
	return (Address+i+1);
}

void  EEPROM_Read_String(uint16 Address ,sint8* String){
	uint8 i =0;
	String[i] = EEPROM_Read_Byte(Address+i);
	while(String[i] != '\0'){
		i++;
		_delay_ms(10);
		String[i] = EEPROM_Read_Byte(Address+i);
	}
	String[i] = '\0';
}


void  EEPROM_Write_Number_32(uint16 Address , uint32 Number){
	//00011000 01100000 00011000 00000110
	// Sec1 = Number >> 24; // Sec2 = Number >> 16;
	// Sec3 = Number >> 8; // Sec4 = (Number >> 0);
	//4,294,967,296 unsigned 32bit //65535 unsigned 16 bit
	int i =0;
	for(i =0;i<4;i++){
		EEPROM_Write_Byte(Address + i, (uint8)((Number>> ((3-i)*8)) & 0xFF) );
		_delay_ms(5);
		i++;
	}
}

uint32 EEPROM_Read_Number_32(uint16 Address){
	uint32 Number = 0;
	for(int i =0;i<4;i++){
		Number |= (uint32)(EEPROM_Read_Byte(Address+i)<<((3-i)*8));
	}
	return Number;
}


















/* IN PROGRESS */
void  EEPROM_Remove_Byte(uint16 Address);
void  EEPROM_Remove_String(uint16 Start_Address, uint16 End_Address);
void EEPROM_ALL_Wiper(){}
void EEPROM_Address_Checker(){}
