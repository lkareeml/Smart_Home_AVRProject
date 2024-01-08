/*
 * Temp_Sensor.c
 *
 * Created: 12/20/2023 4:25:04 PM
 *
 */ 

/*
 * Temp_Sensor.c
 *
 * Created: 12/20/2023 4:25:04 PM
 *
 */ 
// 
// #include "../../MCAL/Interrupt/Interrupt.h"
// #include "../../MCAL/Analog_To_Digital_Converter/ADC.h"
// #include "../../STD_Types.h"
// #include "../../MCAL/Digital_Input_Output/DIO.h"
// #include "../../HAL/AirConditioner/AC.h"
// #include <avr/io.h>
// #include <avr/interrupt.h>
// 
// 
// static uint8 globaltemp;
// 
// ISR(ADC_vect)
// {
// 	
// 	uint8 result=ADC_Return_Result();
// 	globaltemp=((float)result/255.0)*50;
// 	uint8 ac_status=EEPROM_Read_Byte(0003);
// 	if(ac_status=='m')//m stands for manual mode
// 	{
// 		//return 0;
// 	}
// 	else if(ac_status=='a')///a stands for auto mode
// 	{
// 		if(globaltemp>28)
// 		{
// 			AC_On();
// 		}
// 		else if(globaltemp<21)
// 		{
// 			AC_Off();
// 		}
// 	}
// }
// 
// 
// void intializeTemperatureSensor()
// {
// 	ADC_Timer_With_Interrupt();
// }
// uint16 get_temperature_number()
// {
// 	uint16 result=ADC_ISR_Operation();//this returns the value of the adc
// 	uint16 temperature=result*15;
// 	return temperature;
// }
// uint16 get_temperature_string()
// {
// 	uint16 x=get_temperature_number();
// 	uint8 value_string;
// 	itoa(x,value_string,10);
// 	return value_string;/// print in the LCD
// }
// 
// 
