/****************************************/
/*          Author : Kareem Atef        */
/*          Date: 4-12-2023             */
/*          Version: 5.1                */
/*          Module: EEPROM              */
/****************************************/

#ifndef EEPROM_H_
#define EEPROM_H_

void  EEPROM_Init(void);
void  EEPROM_Write_Byte(uint16 Address , uint8 Data);
void  EEPROM_Write_String(uint16 Address , sint8* Strings);
uint8 EEPROM_Read_Byte(uint16 Address);
void  EEPROM_Read_String(uint16 Address , sint8* String);
void  EEPROM_Write_Number_32(uint16 Address ,uint32 Number);
uint32 EEPROM_Read_Number_32(uint16 Address);


void EEPROM_Read_Data(uint8 UserID,sint8 * Data, uint8 DataType);
uint8 EEPROM_Read_UserID_Exist(uint8 UserID);
void EEPROM_Delete_User(uint8 UserID);
void EEPROM_Edit_Data(uint8 UserID,sint8 * Data, uint8 DataType);
void EEPROM_Reg_New_User(uint8 UserID, sint8 * Username, sint8 * Password);

#endif /* EEPROM_H_ */