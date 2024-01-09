/****************************************/
/* Atef		    */
/*          Date: 10-11-2023		    */
/*          Version: 1.0				*/
/*          Module : ADC                */
/****************************************/

#include "../../STD_Types.h"
#include "../../Bit_Manipulation.h"
#include "../../AVR32_Reg_Private.h"
#include "../../MCAL/Digital_Input_Output/DIO.h"
#include "ADC.h"

//ADCSRA  Bit 5 – ADATE: ADC Auto Trigger Enable
void ADC_Auto_Trigger_Enable()	{	Set(ADCSRA_Reg,5);}
void ADC_Auto_Trigger_Disable()	{	Clear(ADCSRA_Reg,5);}

//Bit 3 – ADIE: ADC Interrupt Enable
void ADC_Interrupt_Enable()		{	Set(ADCSRA_Reg,3);}
void ADC_Interrupt_Disable()	{	Clear(ADCSRA_Reg,3);}

void ADC_Enable(void)		{	Set(ADCSRA_Reg,7);				}
void ADC_Disable(void)		{	Clear(ADCSRA_Reg,7);			}
void ADC_Start(void)		{	Set(ADCSRA_Reg,6);				}
void ADC_Wait(void)			{	while(Get(ADCSRA_Reg,6) == 1);	}
uint8 ADC_Return_Result()	{	return ADCH_Reg;				}

uint8 ADC_Timer_Polling_Result(){
	while(Get(ADCSRA_Reg,6) == 1);
	uint8 Data = ADCH_Reg; Set(ADCSRA_Reg,6);
	return Data;
}

void ADC_Init(void)
{
	//ADMUX_Reg : AREF(0,0)          AVCC(1,0)     Internal 2.56V(1,1)
	#if ADC_REF_VOLT == ADC_AREF
	Clear(ADMUX_Reg,6);
	Clear(ADMUX_Reg,7);
	#elif ADC_REF_VOLT == ADC_AVCC
	Set  (ADMUX_Reg,6);
	Clear(ADMUX_Reg,7);
	#elif ADC_REF_VOLT == ADC_INTERNAL
	Set  (ADMUX_Reg,6);
	Set  (ADMUX_Reg,7);
	#endif
	///Bit 5 – ADCMUX(ADLAR)           Left Adjust(1)     Right Adjust(0)
	#if ADC_ADJUSTMENT == ADC_Left_Adjust
	Set(ADMUX_Reg,5);
	#elif ADC_ADJUSTMENT == ADC_Right_Adjust
	Clear(ADMUX_Reg,5);
	#endif
	/// Bits 2:0 – ADPS2:0: ADC Prescaler  (2,4,8,...128) - (1,2,3...7)
	#if ADC_PRESCALER == ADC_PRE_2///001
	Clear(ADCSRA_Reg,0);
	Clear(ADCSRA_Reg,1);
	Set(ADCSRA_Reg,2);
	#elif ADC_PRESCALER == ADC_PRE_4///010
	Clear(ADCSRA_Reg,0);
	Set(ADCSRA_Reg,1);
	Clear(ADCSRA_Reg,2);
	#elif ADC_PRESCALER == ADC_PRE_8///011
	Clear(ADCSRA_Reg,0);
	Set(ADCSRA_Reg,1);
	Set(ADCSRA_Reg,2);
	#elif ADC_PRESCALER == ADC_PRE_16///100
	Set(ADCSRA_Reg,0);
	Clear(ADCSRA_Reg,1);
	Clear(ADCSRA_Reg,2);
	#elif ADC_PRESCALER == ADC_PRE_32///101
	Set(ADCSRA_Reg,0);
	Clear(ADCSRA_Reg,1);
	Set(ADCSRA_Reg,2);
	#elif ADC_PRESCALER == ADC_PRE_64///110
	Set(ADCSRA_Reg,0);
	Set(ADCSRA_Reg,1);
	Clear(ADCSRA_Reg,2);
	#elif ADC_PRESCALER == ADC_PRE_128///111
	Set(ADCSRA_Reg,0);
	Set(ADCSRA_Reg,1);
	Set(ADCSRA_Reg,2);
	#endif
}

void ADC_Manual_Interrupt(){
	ADC_Disable();
	ADMUX_Reg = ((ADMUX_Reg & 0b11100000) | ADC_Ch1);
	ADC_Interrupt_Enable();
	ADC_Enable();
	ADC_Start();// Don't Forget :add ADC_Start at end of interrupt call
	// As after Starting once, it will flag interrupt and stop working
}
/*
void ADC_ISR(ADC_vect){
	ADC_Start();
}*/
void ADC_Timer_No_Interrupt(){
	ADMUX_Reg = ((ADMUX_Reg & 0b11100000) | ADC_Ch1);
	//  choose auto trigger source > Timer0 COMP Match 3 bit in SIFOR
	// 	SFIOR_Reg = SFIOR_Reg | 0b1000xxxx;
	Set(SFIOR_Reg,7);
	Clear(SFIOR_Reg,6);
	Clear(SFIOR_Reg,5);
	Clear(SFIOR_Reg,4);
	ADC_Auto_Trigger_Enable();
	ADC_Init();
	ADC_Enable();
	ADC_Start();// Timer will Start it Automatically
}

void ADC_Timer_With_Interrupt(){
	ADMUX_Reg = ((ADMUX_Reg & 0b11100000) | ADC_Ch1);
	//  choose auto trigger source > Timer0 COMP Match 3 bit in SIFOR
	// 	SFIOR_Reg = SFIOR_Reg | 0b1000xxxx;
	Set(SFIOR_Reg,7);
	Clear(SFIOR_Reg,6);
	Clear(SFIOR_Reg,5);
	Clear(SFIOR_Reg,4);
	ADC_Auto_Trigger_Enable();
	ADC_Interrupt_Enable();
	ADC_Enable();
	ADC_Start();// Timer will Start it Automatically
}

void ADC_Interrupt_Flag_Clear(){
	//Bit 4 – ADIF: ADC Interrupt Flag
	Set(ADCSRA_Reg,4);
}


#if ADC_ADJUSTMENT == ADC_Left_Adjust
uint8 ADC_Convert_8bit(uint8 channel)
{
	ADC_Disable();
	ADMUX_Reg = (ADMUX_Reg & 0b11100000);
	ADMUX_Reg = ADMUX_Reg | channel;
	ADC_Enable();
	ADC_Start();
	ADC_Wait();
	return ADCH_Reg;
}
#elif ADC_ADJUSTMENT == ADC_Right_Adjust
uint16 ADC_Convert_16bit(uint8 channel)
{
	ADC_Disable();
	ADMUX_Reg = ((ADMUX_Reg & 0b11100000 ) | channel);
	ADC_Enable();
	ADCL_Reg = 0;
	ADCH_Reg = 0;
	ADC_Start();
	ADC_Wait();
	/// ADCLR = 0 --- ADCH (00000011) ADCL (11111111) --- Right Adjust
	uint8 Right_Data = ((ADCH_Reg & 0b00000011) << 8);
	uint8 Left_Data = ADCL_Reg;
	uint16 Collect_Data = (Right_Data | Left_Data) ;
	return Collect_Data;
}
#endif