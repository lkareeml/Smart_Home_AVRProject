/****************************************/
/*          Author : Kareem Atef        */
/*          Date: 10-11-2023		    */
/*          Version: 1.0				*/
/*          Module: Keypad              */
/****************************************/

#include "../../STD_Types.h"
#include "../../MCAL/Digital_Input_Output/DIO.h"
#include "Keypad_4x4.h"


void KEYPAD_Initialization(){
	DIO_Set_Pin_Direction(COL_PORTB,COL_Pin_0,Out);
	DIO_Set_Pin_Direction(COL_PORTB,COL_Pin_1,Out);
	DIO_Set_Pin_Direction(COL_PORTB,COL_Pin_2,Out);
	DIO_Set_Pin_Direction(COL_PORTB,COL_Pin_3,Out);
	DIO_Set_Pin_Direction(ROW_PORTC,ROW_Pin_0,In);
	DIO_Set_Pin_Direction(ROW_PORTC,ROW_Pin_1,In);
	DIO_Set_Pin_Direction(ROW_PORTC,ROW_Pin_2,In);
	DIO_Set_Pin_Direction(ROW_PORTC,ROW_Pin_3,In);
	/// OUTPUT HIGH
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_0,High);
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_1,High);
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_2,High);
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_3,High);
	///PULL UP RESISTORES ON
	DIO_Set_Pin_Output(ROW_PORTC,ROW_Pin_0,High);
	DIO_Set_Pin_Output(ROW_PORTC,ROW_Pin_1,High);
	DIO_Set_Pin_Output(ROW_PORTC,ROW_Pin_2,High);
	DIO_Set_Pin_Output(ROW_PORTC,ROW_Pin_3,High);
}

uint8 KEYPAD_Get_Pressed_Key(){
	uint8 key = '\0';
	//KEYPAD_Initialization();
	
	/**************************		ROW 0		************************/
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_0,Low);
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_1,High);
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_2,High);
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_3,High);
	/*****************		Col 0		*****************/
	if(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_0)))
	{
		while(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_0))){}
		key	=	Output_R_0_C_0;
	}
	/*****************		Col 1		*****************/
	else if(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_1)) )
	{
		while(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_1))){}
		key	=	Output_R_0_C_1;
	}
	/*****************		Col 2		*****************/
	else if(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_2)))
	{
		while(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_2))){}
		key	=	Output_R_0_C_2;
	}
	/*****************		Col 3		*****************/
	else if(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_3)))
	{
		while(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_3))){}
		key	=	Output_R_0_C_3;
	}
	
	
	/**************************		ROW 1		************************/
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_0,High);
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_1,Low);
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_2,High);
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_3,High);
	/*****************		Col 0		*****************/
	if(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_0)) )
	{
		while(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_0))){}
		key	=	Output_R_1_C_0;
	}
	/*****************		Col 1		*****************/
	else if(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_1)) ){
		while(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_1))){}
		key	=	Output_R_1_C_1;
	}
	/*****************		Col 2		*****************/
	else if(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_2)) )
	{
		while(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_2))){}
		key	=	Output_R_1_C_2;
	}
	/*****************		Col 3		*****************/
	else if(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_3)) )
	{
		while(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_3))){}
		key	=	Output_R_1_C_3;
	}
	
	
	/**************************		ROW 2		************************/
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_0,High);
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_1,High);
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_2,Low);
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_3,High);
	/*****************		Col 0		*****************/
	if(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_0)) )
	{
		while(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_0))){}
		key	=	Output_R_2_C_0;
	}
	/*****************		Col 1		*****************/
	else if(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_1)) )
	{
		while(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_1))){}
		key	=	Output_R_2_C_1;
	}
	/*****************		Col 2		*****************/
	else if(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_2)) )
	{
		while(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_2))){}
		key	=	Output_R_2_C_2;
	}
	/*****************		Col 3		*****************/
	else if(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_3)) )
	{
		while(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_3))){}
		key	=	Output_R_2_C_3;
	}


	/**************************		ROW 3		************************/
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_0,High);
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_1,High);
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_2,High);
	DIO_Set_Pin_Output(COL_PORTB,COL_Pin_3,Low);
	/*****************		Col 0		*****************/
	if(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_0)) )
	{
		while(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_0))){}
		key	=	Output_R_3_C_0;
	}
	/*****************		Col 1		*****************/
	else if(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_1)))
	{
		while(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_1))){}
		key	=	Output_R_3_C_1;
	}
	/*****************		Col 2		*****************/
	else if(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_2)))
	{
		while(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_2))){}
		key	=	Output_R_3_C_2;
	}
	/*****************		Col 3		*****************/
	else if(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_3)) )
	{
		while(!(DIO_Get_Pin_Value(ROW_PORTC,ROW_Pin_3))){}
		key	=	Output_R_3_C_3;
	}
	
	/**************************		Return		************************/
	return key;
}
