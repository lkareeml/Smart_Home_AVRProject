/*
 * ADC.h
 *
 * Created: 12/20/2023 6:36:41 AM
 *
 */ 


#ifndef ADC_H_
#define ADC_H_

#define ADC_AVCC          0
#define ADC_AREF          1
#define ADC_INTERNAL      2
#define ADC_Left_Adjust   3
#define ADC_Right_Adjust  4
#define ADC_PRE_2         1
#define ADC_PRE_4         2
#define ADC_PRE_8         3
#define ADC_PRE_16        4
#define ADC_PRE_32        5
#define ADC_PRE_64        6
#define ADC_PRE_128       7


#define ADC_REF_VOLT      ADC_AVCC            /// Options: ADC_AVCC - ADC_AREF - ADC_INTERNAL
#define ADC_ADJUSTMENT    ADC_Left_Adjust     /// Options: ADC_Left_Adjust - ADC_Right_Adjust
#define ADC_PRESCALER     ADC_PRE_2         /// Options: ADC_PRE_2 - ADC_PRE_4 ... ADC_PRE_128


#define ADC_Ch0 0
#define ADC_Ch1 1
#define ADC_Ch2 2
#define ADC_Ch3 3
#define ADC_Ch4 4
#define ADC_Ch5 5
#define ADC_Ch6 6
#define ADC_Ch7 7

void ADC_Init(void);
void ADC_Enable(void);
void ADC_Disable(void);
void ADC_Wait(void);
void ADC_Start(void);

uint8 ADC_Return_Result();
uint8 ADC_Timer_Polling_Result();

void ADC_Interrupt_Enable();
void ADC_Interrupt_Disable();
void ADC_Auto_Trigger_Enable();
void ADC_Auto_Trigger_Disable();

void ADC_Manual_Interrupt();
void ADC_Timer_No_Interrupt();
void ADC_Timer_With_Interrupt();
void ADC_Interrupt_Flag_Clear();


uint8 ADC_Convert_8bit(uint8 channel);
uint16 ADC_Convert_16bit(uint8 channel);





#endif /* ADC_H_ */