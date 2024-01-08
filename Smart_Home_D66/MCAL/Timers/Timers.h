/*****************************************************************************/
/*****************************************************************************/
/*******************    Timer 0/2 8-Bit **********************/
#define Timer_M_Normal      0
#define Timer_M_CTC         1
#define Timer_M_FastPWM     2
#define Timer_M_PhasePWM    3
/*************************************************************/
#define Timer_Pre_1         1
#define Timer_Pre_8         2
#define Timer_Pre_64        3
#define Timer_Pre_256       4
#define Timer_Pre_1024      5
/*************************************************************/
#define Timer_PinOFF       0
#define Timer_PinTOGGLE    1
#define Timer_PinSET       2    //INVERT
#define Timer_PinCLEAR     3    //NON INVERT

/*****************************************************************************/
/*****************************************************************************/
/*******************    Timer 1 16-Bit  **********************/
#define    Timer1_Normal                               0
#define    Timer1_PWM_Phase_Correct_8bit               1
#define    Timer1_PWM_Phase_Correct_9bit               2
#define    Timer1_PWM_Phase_Correct_10bit              3
#define    Timer1_CTC_OCR1A                            4
#define    Timer1_Fast_PWM_8bit                        5
#define    Timer1_Fast_PWM_9bit                        6
#define    Timer1_Fast_PWM_10bit                       7
#define    Timer1_PWM_Phase_Frequency_Correct_ICR1     8
#define    Timer1_PWM_Phase_Frequency_Correct_OCR1A    9
#define    Timer1_PWM_Phase_Correct_ICR1               10
#define    Timer1_PWM_Phase_Correct_OCR1A              11
#define    Timer1_CTC_ICR1                             12
#define    Timer1_FastPWM_ICR1                         14
#define    Timer1_FastPWM_OCR1A                        15 /// SEND PWM TO OCR1A
/*************************************************************/
// #define Timer_Pre_1                     1
// #define Timer_Pre_8                     2
// #define Timer_Pre_64                    3
// #define Timer_Pre_256                   4
// #define Timer_Pre_1024                  5
#define Timer_Pre_STOP                     0
#define Timer_Pre_External_T1_Falling      6
#define Timer_Pre_External_T1_Raising      7
/*************************************************************/
#define Enabled                      1
#define Disabled                     0
#define Timer1_IC_Raising_Edge       1
#define Timer1_IC_Falling_Edge       0
#define Normal_Disconnect            0
#define Toggle_WGM_9_14_15_OC1A      1
#define Clear_OC1A_OC1B_Non_Inv      2
#define Set_OC1A_OC1B_Inv            3

/*****************************************************************************/
/*****************************************************************************/

/**************************    Configuration    ******************************/
//    Timer_Normal   Timer_CTC   Timer_FastPWM   Timer_PhasePWM
#define Timer0_Mode            Timer_M_PhasePWM
#define Timer2_Mode            Timer_M_CTC
//    Timer_Pre_1,8,64,256,1024
#define Timer0_PreScaler    Timer_Pre_1
#define Timer2_PreScaler    Timer_Pre_1024
//    Timer_PinOFF    Timer_PinCLEAR    Timer_PinTOGGLE    Timer_PinSET
#define Timer0_OC0_Pin        Timer_PinSET
#define Timer2_OC2_Pin        Timer_PinOFF
/********************************    Timer 1    ************************************/
// OC2 == PD7 // OC1B == PD4 // OC1A == PD5 // OC0 == PB3 //
#define Timer1_Mode                    Timer1_Normal
#define Timer1_PreScaler               Timer_Pre_1024
#define Timer1_IC_Noise_Canceller      Disabled
#define Timer1_IC_Edge_Select          Timer1_IC_Falling_Edge
#define Compare_Output_Mode_A          Normal_Disconnect
#define Compare_Output_Mode_B          Normal_Disconnect
/*********************************************************/



#ifndef TIMERS_H_
#define TIMERS_H_

void Timer1_Init();

void Timer1_Set_ICR1_Reg(uint16 value);
void Timer1_Set_TCNT1_Reg(uint16 value);
void Timer1_Set_OCR1A_Reg(uint16 value);
void Timer1_Set_OCR1B_Reg(uint16 value);

void Timer1_Input_Capture_Interrupt_Enable();
void Timer1_Input_Capture_Interrupt_Disable();
void Timer1_Compare_A_Interrupt_Enable();
void Timer1_Compare_A_Interrupt_Disable();
void Timer1_Compare_B_Interrupt_Enable();
void Timer1_Compare_B_Interrupt_Disable();
void Timer1_Overflow_Interrupt_Enable();
void Timer1_Overflow_Interrupt_Disable();


/***********************************************************************************/
void Timer0_Init();

void Timer0_Set_TCNT0_Reg(uint8 value);
void Timer0_Set_OCR0_Reg(uint8 value);

void Timer0_Enable_Overflow_Interrupt();
void Timer0_Disable_Overflow_Interrupt();
void Timer0_Enable_CTC_Interrupt();
void Timer0_Disable_CTC_Interrupt();


void Timer2_Init();

void Timer2_Set_TCNT2_Reg(uint8 value);
void Timer2_Set_OCR2_Reg(uint8 value);

void Timer2_Enable_Overflow_Interrupt();
void Timer2_Disable_Overflow_Interrupt();
void Timer2_Enable_CTC_Interrupt();
void Timer2_Disable_CTC_Interrupt();


#endif /* TIMERS_H_ */