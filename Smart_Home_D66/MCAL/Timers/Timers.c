/****************************************/
/* Atef         */
/*          Date: 19-11-2023            */
/*          Version: 1.0                */
/*          Module: Timers              */
/****************************************/
#include "../../STD_Types.h"
#include "../../Bit_Manipulation.h"
#include "../../AVR32_Reg_Private.h"
#include "Timers.h"

/********************************    Timer 1    ************************************/
// In Timer1 There Are 8 Main Registers :
// TCCR1A -- TCCR1B -- (TCNT1H/L) -- (OCR1AH/L) -- (OCR1BH/L) -- (ICR1H/L) -- (TIMSK) -- (TIFR)
// ASCR

void Timer1_Init(){
	/***********************   Timer1 ALL 15 Modes   **************************/
	#if  (Timer1_Mode == Timer1_Normal)                        //0        ICR1-YES
	Clear(TCCR1B_Reg,4);Clear(TCCR1B_Reg,3);Clear(TCCR1A_Reg,1);Clear(TCCR1A_Reg,0);
	
	#elif (Timer1_Mode == Timer1_PWM_Phase_Correct_8bit)    //1
	Clear(TCCR1B_Reg,4);Clear(TCCR1B_Reg,3);Clear(TCCR1A_Reg,1);Set(TCCR1A_Reg,0);
	
	#elif (Timer1_Mode == Timer1_PWM_Phase_Correct_9bit)    //2
	Clear(TCCR1B_Reg,4);Clear(TCCR1B_Reg,3);Set(TCCR1A_Reg,1);Clear(TCCR1A_Reg,0);
	
	#elif (Timer1_Mode == Timer1_PWM_Phase_Correct_10bit)    //3
	Clear(TCCR1B_Reg,4);Clear(TCCR1B_Reg,3);Set(TCCR1A_Reg,1);Set(TCCR1A_Reg,0);
	
	#elif (Timer1_Mode == Timer1_CTC_OCR1A)                    //4
	Clear(TCCR1B_Reg,4);Set(TCCR1B_Reg,3);Clear(TCCR1A_Reg,1);Clear(TCCR1A_Reg,0);
	
	#elif (Timer1_Mode == Timer1_Fast_PWM_8bit)                //5
	Clear(TCCR1B_Reg,4);Set(TCCR1B_Reg,3);Clear(TCCR1A_Reg,1);Set(TCCR1A_Reg,0);
	
	#elif (Timer1_Mode == Timer1_Fast_PWM_9bit)                //6
	Clear(TCCR1B_Reg,4);Set(TCCR1B_Reg,3);Set(TCCR1A_Reg,1);Clear(TCCR1A_Reg,0);
	
	#elif (Timer1_Mode == Timer1_Fast_PWM_10bit)            //7
	Clear(TCCR1B_Reg,4);Set(TCCR1B_Reg,3);Set(TCCR1A_Reg,1);Set(TCCR1A_Reg,0);
	
	#elif (Timer1_Mode == Timer1_PWM_Phase_Frequency_Correct_ICR1)            //8
	Set(TCCR1B_Reg,4);Clear(TCCR1B_Reg,3);Clear(TCCR1A_Reg,1);Clear(TCCR1A_Reg,0);
	
	#elif (Timer1_Mode == Timer1_PWM_Phase_Frequency_Correct_OCR1A)            //9
	Set(TCCR1B_Reg,4);Clear(TCCR1B_Reg,3);Clear(TCCR1A_Reg,1);Set(TCCR1A_Reg,0);
	
	#elif (Timer1_Mode == Timer1_PWM_Phase_Correct_ICR1)                    //10
	Set(TCCR1B_Reg,4);Clear(TCCR1B_Reg,3);Set(TCCR1A_Reg,1);Clear(TCCR1A_Reg,0);

	#elif (Timer1_Mode == Timer1_PWM_Phase_Correct_OCR1A)                    //11
	Set(TCCR1B_Reg,4);Clear(TCCR1B_Reg,3);Set(TCCR1A_Reg,1);Set(TCCR1A_Reg,0);

	#elif (Timer1_Mode == Timer1_CTC_ICR1)                                    //12
	Set(TCCR1B_Reg,4);Set(TCCR1B_Reg,3);Clear(TCCR1A_Reg,1);Clear(TCCR1A_Reg,0);

	#elif (Timer1_Mode == Timer1_FastPWM_ICR1)                                //14
	Set(TCCR1B_Reg,4);Set(TCCR1B_Reg,3);Set(TCCR1A_Reg,1);Clear(TCCR1A_Reg,0);

	#elif (Timer1_Mode == Timer1_FastPWM_OCR1A)                                //15    ICR1-NO
	Set(TCCR1B_Reg,4);Set(TCCR1B_Reg,3);Set(TCCR1A_Reg,1);Set(TCCR1A_Reg,0);

	#endif
	/***********************   Timer1 PreScaler   **************************/
	#if  (Timer1_PreScaler == Timer_Pre_STOP)
	Clear(TCCR1B_Reg,2);    Clear(TCCR1B_Reg,1);    Clear(TCCR1B_Reg,0);
	#elif (Timer1_PreScaler == Timer_Pre_1)
	Clear(TCCR1B_Reg,2);    Clear(TCCR1B_Reg,1);    Set(TCCR1B_Reg,0);
	#elif (Timer1_PreScaler == Timer_Pre_8)
	Clear(TCCR1B_Reg,2);    Set(TCCR1B_Reg,1);        Clear(TCCR1B_Reg,0);
	#elif (Timer1_PreScaler == Timer_Pre_64)
	Clear(TCCR1B_Reg,2);    Set(TCCR1B_Reg,1);        Set(TCCR1B_Reg,0);
	#elif (Timer1_PreScaler == Timer_Pre_256)
	Set(TCCR1B_Reg,2);        Clear(TCCR1B_Reg,1);    Clear(TCCR1B_Reg,0);
	#elif (Timer1_PreScaler == Timer_Pre_1024)
	Set(TCCR1B_Reg,2);        Clear(TCCR1B_Reg,1);    Set(TCCR1B_Reg,0);
	#elif (Timer1_PreScaler == Timer_Pre_External_T1_Falling)
	Set(TCCR1B_Reg,2);        Set(TCCR1B_Reg,1);        Clear(TCCR1B_Reg,0);
	#elif (Timer1_PreScaler == Timer_Pre_External_T1_Raising)
	Set(TCCR1B_Reg,2);        Set(TCCR1B_Reg,1);        Set(TCCR1B_Reg,0);
	#endif
	
	/**********************  Input Capture  Options  **********************/
	
	#if (Timer1_IC_Edge_Select == Timer1_IC_Raising_Edge)
	Set(TCCR1B_Reg,6);
	#elif (Timer1_IC_Edge_Select == Timer1_IC_Falling_Edge)
	Clear(TCCR1B_Reg,6);
	#endif
	
	#if (Timer1_IC_Noise_Canceller == Disabled)
	Clear(TCCR1B_Reg,7);
	#elif (Timer1_IC_Noise_Canceller == Enabled)
	Set(TCCR1B_Reg,7);
	#endif
	
	/***********************   Timer1 OCR1A Pin   **************************/
	#if  (Compare_Output_Mode_A == Normal_Disconnect)
	Clear(TCCR1A_Reg,7);Clear(TCCR1A_Reg,6);
	#elif  (Compare_Output_Mode_A == Toggle_WGM_9_14_15_OC1A)
	Clear(TCCR1A_Reg,7);Set(TCCR1A_Reg,6);
	#elif  (Compare_Output_Mode_A == Clear_OC1A_OC1B_Non_Inv)
	Set(TCCR1A_Reg,7);Clear(TCCR1A_Reg,6);
	#elif  (Compare_Output_Mode_A == Set_OC1A_OC1B_Inv)
	Set(TCCR1A_Reg,7);Set(TCCR1A_Reg,6);
	#endif

	/***********************   Timer1 OCR1B Pin   **************************/
	#if  (Compare_Output_Mode_B == Normal_Disconnect)
	Clear(TCCR1A_Reg,7);Clear(TCCR1A_Reg,6);
	#elif  (Compare_Output_Mode_B == Toggle_WGM_9_14_15_OC1A)//TOGGLE FOR OC1A ONLY Data sheet
	Clear(TCCR1A_Reg,7);Set(TCCR1A_Reg,6);
	#elif  (Compare_Output_Mode_B == Clear_OC1A_OC1B_Non_Inv)
	Set(TCCR1A_Reg,7);Clear(TCCR1A_Reg,6);
	#elif  (Compare_Output_Mode_B == Set_OC1A_OC1B_Inv)
	Set(TCCR1A_Reg,7);Set(TCCR1A_Reg,6);
	#endif
	//Clear the reserved pin to ensure compatibility :
	Clear(TCCR1B_Reg,5);
}

void Timer1_Enable_Input_Capture_Interrupt()    {      Set(TIMSK_Reg,5);     } // TIMSK
void Timer1_Disable_Input_Capture_Interrupt()   {    Clear(TIMSK_Reg,5);     } // TIMSK

void Timer1_Enable_Compare_A_Interrupt()        {      Set(TIMSK_Reg,4);     } // TIMSK
void Timer1_Disable_Compare_A_Interrupt()       {    Clear(TIMSK_Reg,4);     } // TIMSK

void Timer1_Enable_Compare_B_Interrupt()        {      Set(TIMSK_Reg,3);     } // TIMSK
void Timer1_Disable_Compare_B_Interrupt()       {    Clear(TIMSK_Reg,3);     } // TIMSK

void Timer1_Enable_Overflow_Interrupt()         {      Set(TIMSK_Reg,2);     } // TIMSK
void Timer1_Disable_Overflow_Interrupt()        {    Clear(TIMSK_Reg,2);     } // TIMSK

uint8 Timer1_Read_IC_Interrupt_Flag()           {    return Get(TIFR_Reg,5); } // TIFR
uint8 Timer1_Read_OCA_Interrupt_Flag()          {    return Get(TIFR_Reg,4); } // TIFR
uint8 Timer1_Read_OCB_Interrupt_Flag()          {    return Get(TIFR_Reg,3); } // TIFR
uint8 Timer1_Read_OVF_Interrupt_Flag()          {    return Get(TIFR_Reg,2); } // TIFR
uint16 Timer1_Read_IC_Reg()                     { return ((ICR1H_Reg << 8) | ICR1L_Reg);} // ICR1H/L

void Timer1_Set_ICR1_Reg(uint16 value){
	ICR1H_Reg= (uint8)(value >> 8);
	ICR1L_Reg= (uint8)(value & 0x00FF);
}
void Timer1_Set_TCNT1_Reg(uint16 value){
	TCNT1H_Reg= (uint8)(value >> 8);
	TCNT1L_Reg= (uint8)(value & 0x00FF);
}
void Timer1_Set_OCR1A_Reg(uint16 value){
	OCR1AH_Reg =  (uint8)(value >> 8);
	OCR1AL_Reg =  (uint8)(value & 0x00FF);
}
void Timer1_Set_OCR1B_Reg(uint16 value){
	OCR1BH_Reg =  (uint8)(value >> 8);
	OCR1BL_Reg =  (uint8)(value & 0x00FF);
}
/******************************    END Timer 1    **********************************/







/********************************    Timer 0    ************************************/
void Timer0_Init(){
	/***********************   Timer0 Mode   **************************/
	#if  (Timer0_Mode == Timer_M_Normal)
	Clear(TCCR0_Reg,6);        Clear(TCCR0_Reg,3);
	#elif (Timer0_Mode == Timer_M_CTC)
	Clear(TCCR0_Reg,6);          Set(TCCR0_Reg,3);
	#elif (Timer0_Mode == Timer_M_FastPWM)
	Set(TCCR0_Reg,6);          Set(TCCR0_Reg,3);
	#elif (Timer0_Mode == Timer_M_PhasePWM)
	Set(TCCR0_Reg,6);        Clear(TCCR0_Reg,3);
	#endif
	/***********************   Timer0 PreScaler   **************************/
	#if  (Timer0_PreScaler == Timer_Pre_1)
	Clear(TCCR0_Reg,2);Clear(TCCR0_Reg,1);  Set(TCCR0_Reg,0);
	#elif (Timer0_PreScaler == Timer_Pre_8)
	Clear(TCCR0_Reg,2);  Set(TCCR0_Reg,1);Clear(TCCR0_Reg,0);
	#elif (Timer0_PreScaler == Timer_Pre_64)
	Clear(TCCR0_Reg,2);  Set(TCCR0_Reg,1);  Set(TCCR0_Reg,0);
	#elif (Timer0_PreScaler == Timer_Pre_256)
	Set(TCCR0_Reg,2);Clear(TCCR0_Reg,1);Clear(TCCR0_Reg,0);
	#elif (Timer0_PreScaler == Timer_Pre_1024)
	Set(TCCR0_Reg,2);Clear(TCCR0_Reg,1);  Set(TCCR0_Reg,0);
	#endif
	/***********************   Timer0 OCR0 Pin   **************************/
	#if  (Timer0_OC0_Pin == Timer_COM_Pin_OFF)
	Clear(TCCR0_Reg,5);Clear(TCCR0_Reg,4);
	#elif (Timer0_OC0_Pin == Timer_COM_Pin_TOGGLE)
	Clear(TCCR0_Reg,5);     Set(TCCR0_Reg,4);
	#elif (Timer0_OC0_Pin == Timer_COM_Pin_CLEAR)
	Set(TCCR0_Reg,5);Clear(TCCR0_Reg,4);
	#elif (Timer0_OC0_Pin == Timer_COM_Pin_SET)
	Set(TCCR0_Reg,5);  Set(TCCR0_Reg,4);
	#endif
}
void Timer0_Set_TCNT0_Reg(uint8 value)   {    TCNT0_Reg= value;    }//Timer Register
void Timer0_Set_OCR0_Reg(uint8 value)    {    OCR0_Reg = value;    }//Output Compare Register
void Timer0_Enable_Overflow_Interrupt()  {    Set(TIMSK_Reg,0);    }
void Timer0_Disable_Overflow_Interrupt() {    Clear(TIMSK_Reg,0);  }
void Timer0_Enable_CTC_Interrupt()       {    Set(TIMSK_Reg,1);    }
void Timer0_Disable_CTC_Interrupt()      {    Clear(TIMSK_Reg,1);  }

/******************************    END Timer 0    **********************************/

/********************************    Timer 2    ************************************/
void Timer2_Init(){
	/***********************   Timer2 Mode   **************************/
	#if   (Timer2_Mode == Timer_M_Normal)
	Clear(TCCR2_Reg,6);        Clear(TCCR2_Reg,3);
	#elif (Timer2_Mode == Timer_M_CTC)
	Clear(TCCR2_Reg,6);          Set(TCCR2_Reg,3);
	#elif (Timer2_Mode == Timer_M_FastPWM)
	Set(TCCR2_Reg,6);          Set(TCCR2_Reg,3);
	#elif (Timer2_Mode == Timer_M_PhasePWM)
	Set(TCCR2_Reg,6);        Clear(TCCR2_Reg,3);
	#endif
	/***********************   Timer0 PreScaler   **************************/
	#if   (Timer2_PreScaler == Timer_Pre_1)
	Clear(TCCR2_Reg,2);Clear(TCCR2_Reg,1);  Set(TCCR2_Reg,0);
	#elif (Timer2_PreScaler == Timer_Pre_8)
	Clear(TCCR2_Reg,2);  Set(TCCR2_Reg,1);Clear(TCCR2_Reg,0);
	#elif (Timer2_PreScaler == Timer_Pre_64)
	Clear(TCCR2_Reg,2);  Set(TCCR2_Reg,1);  Set(TCCR2_Reg,0);
	#elif (Timer2_PreScaler == Timer_Pre_256)
	Set(TCCR2_Reg,2);Clear(TCCR2_Reg,1);Clear(TCCR2_Reg,0);
	#elif (Timer2_PreScaler == Timer_Pre_1024)
	Set(TCCR2_Reg,2);Clear(TCCR2_Reg,1);  Set(TCCR2_Reg,0);
	#endif
	/***********************   Timer0 OCR0 Pin   **************************/
	#if   (Timer2_OC2_Pin == Timer_COM_Pin_OFF)
	Clear(TCCR2_Reg,5);Clear(TCCR2_Reg,4);
	#elif (Timer2_OC2_Pin == Timer_COM_Pin_TOGGLE)
	Clear(TCCR2_Reg,5);     Set(TCCR2_Reg,4);
	#elif (Timer2_OC2_Pin == Timer_COM_Pin_CLEAR)
	Set(TCCR2_Reg,5);Clear(TCCR2_Reg,4);
	#elif (Timer2_OC2_Pin == Timer_COM_Pin_SET)
	Set(TCCR2_Reg,5);  Set(TCCR2_Reg,4);
	#endif
}
void Timer2_Set_TCNT2_Reg(uint8 value)    {    TCNT2_Reg= value;    }//Timer Register
void Timer2_Set_OCR2_Reg(uint8 value)     {    OCR2_Reg = value;    }//Output Compare Register
void Timer2_Enable_Overflow_Interrupt()   {      Set(TIMSK_Reg,6);  }
void Timer2_Disable_Overflow_Interrupt()  {    Clear(TIMSK_Reg,6);  }
void Timer2_Enable_CTC_Interrupt()        {      Set(TIMSK_Reg,7);  }
void Timer2_Disable_CTC_Interrupt()       {    Clear(TIMSK_Reg,7);  }

/******************************    END Timer 2    **********************************/
