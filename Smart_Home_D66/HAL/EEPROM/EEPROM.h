/****************************************/
/*          Author : Kareem Atef        */
/*          Date: 4-12-2023             */
/*          Version: 1.0                */
/*          Module: EEPROM              */
/****************************************/

#ifndef EEPROM_H_
#define EEPROM_H_

void  EEPROM_Init(void);
void  EEPROM_Remove_Byte(uint16 Address);
void  EEPROM_Remove_String(uint16 Start_Address, uint16 End_Address);
void EEPROM_ALL_Wiper();
void EEPROM_Address_Checker();

void  EEPROM_Write_Byte(uint16 Address , uint8 Data);
uint16  EEPROM_Write_String(uint16 Address , sint8* Strings);//Returns Last address

uint8 EEPROM_Read_Byte(uint16 Address);
void  EEPROM_Read_String(uint16 Address , sint8* String);

void  EEPROM_Write_Number_32(uint16 Address ,uint32 Number);
uint32 EEPROM_Read_Number_32(uint16 Address);


uint8 EEPROM_Reg_New_User(uint8 UserID, sint8 * Username, sint8 * Password);
uint8 EEPROM_Edit_User_Password(uint8 UserID,sint8 * Password);
uint8 EEPROM_Edit_User_Username(uint8 UserID,sint8 * Username);
uint8 EEPROM_Read_User(uint8 UserID,sint8 * Username);
uint8 EEPROM_Read_Password(uint8 UserID,sint8 * Password);

uint8 EEPROM_Delete_User(uint8 UserID);


#endif /* EEPROM_H_ */