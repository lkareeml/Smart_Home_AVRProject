#ifndef GLOBAL_VAR_H_
#define GLOBAL_VAR_H_
// ****************     UART VARIABLES     ****************
extern uint8 Max_Failure_Login_Count;
extern uint8 MAX_Users_Count;// Without Admin
extern uint8 door_state;// 0 is closed, 1 is open
extern uint8 G_Dimmer_Value;
extern sint8 UART_Buffer[8];
extern uint8 FailCount;
extern uint8 UART_Received_Flag;
extern uint8 UART_global_Flag;
extern sint8 UART_UserID[8];
extern uint8 User_ID;
extern sint8 UART_Password[8];
extern sint8 EEPROM_Username[8];
extern sint8 EEPROM_Password[8];
extern uint8 g_choice_1;
extern uint8 g_choice_2;
extern uint8 g_choice_2;
extern uint8 New_UserID;
extern sint8 New_Username[8];
extern sint8 New_Password[8];
extern uint8 AC_State_Auto; // Automatic 1 is on , 0 off allow manual
extern uint8 AC_State; // 0 means off 1 means on
// ****************     KEYPAD LCD VARIABLES     ****************
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
// **************************************************************
#endif /* GLOBAL_VAR_H_ */