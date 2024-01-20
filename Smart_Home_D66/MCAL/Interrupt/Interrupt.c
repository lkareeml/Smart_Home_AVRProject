/*
 * Interrupt.c
 *
 * Created: 12/20/2023 5:23:40 AM
 *
 */ 

#include "../../STD_Types.h"
#include "../../Bit_Manipulation.h"
#include "../../AVR32_Reg_Private.h"
#include "../Digital_Input_Output/DIO.h"
#include "Interrupt.h"

void EXTI0_Init()
{
	#if EXTI0_ISC == EXTI0_Low_Level
	Clear(MCUCR_Reg,0);
	Clear(MCUCR_Reg,1);
	#elif EXTI0_ISC == EXTI0_Any_Change
	Set(MCUCR_Reg,0);
	Clear(MCUCR_Reg,1);
	#elif EXTI0_ISC == EXTI0_Falling_Edge
	Clear(MCUCR_Reg,0);
	Set(MCUCR_Reg,1);
	#elif EXTI0_ISC == EXTI0_Raising_Edge
	Set(MCUCR_Reg,0);
	Set(MCUCR_Reg,1);
	#endif
}

void EXTI0_Set_Direction_Input(){
	DIO_Set_Pin_Direction(PORTDx,Pin2,In);
}

void EXTI0_Set_Internal_Pull_Up(){
	DIO_Set_Pin_Output(PORTDx,Pin2,High);
}

void EXTI1_Init()
{
	#if EXTI1_ISC == EXTI1_Low_Level
	Clear(MCUCR_Reg,2);
	Clear(MCUCR_Reg,3);
	#elif EXTI1_ISC == EXTI1_Any_Change
	Set(MCUCR_Reg,2);
	Clear(MCUCR_Reg,3);
	#elif EXTI1_ISC == EXTI1_Falling_Edge
	Clear(MCUCR_Reg,2);
	Set(MCUCR_Reg,3);
	#elif EXTI1_ISC == EXTI1_Raising_Edge
	Set(MCUCR_Reg,2);
	Set(MCUCR_Reg,3);
	#endif
}

void EXTI2_Init()
{
	#if EXTI2_ISC == EXTI2_Falling_Edge
	Clear(MCUCSR_Reg,6);
	#elif EXTI2_ISC == EXTI2_Raising_Edge
	Set(MCUCSR_Reg,6);
	#endif
}

void EXTI0_Enable()
{
	//EXTI0 Peripheral Interrupt Enable by Set bit 6 On GICR Register
	Set(GICR_Reg,6);
}

void EXTI0_Disable()
{
	//EXTI0 Peripheral Interrupt Disable by Clear bit 6 On GICR Register
	Clear(GICR_Reg,6);
}

void EXTI1_Enable()
{
	//EXTI1 Peripheral Interrupt Enable by Set bit 7 On GICR Register
	Set(GICR_Reg,7);
}

void EXTI1_Disable()
{
	//EXTI1 Peripheral Interrupt Disable by Clear bit 7 On GICR Register
	Clear(GICR_Reg,7);
}

void EXTI2_Enable()
{
	//EXTI2 Peripheral Interrupt Enable by Set bit 5 On GICR Register
	Set(GICR_Reg,5);
}

void EXTI2_Disable()
{
	//EXTI1 Peripheral Interrupt Disable by Set bit 5 On GICR Register
	Clear(GICR_Reg,5);
}

void GIE_Enable()
{
	//General Interrupt Enable by Set bit 7 On SREG Register
	Set(SREG_Reg,7);
}

void GIE_Disable()
{
	//General Interrupt Disable by Set bit 7 On SREG Register
	Clear(SREG_Reg,7);
}