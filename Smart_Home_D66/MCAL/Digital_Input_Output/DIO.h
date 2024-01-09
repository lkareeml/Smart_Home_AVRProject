/*
 * DIO.h
 *
 * Created: 12/20/2023 5:05:54 AM
 *
 */ 


#ifndef DIO_H_
#define DIO_H_

typedef enum{
	PORTAx,
	PORTBx,
	PORTCx,
	PORTDx
}PORT;

typedef enum{
	Pin0, Pin1, Pin2, Pin3, Pin4, Pin5, Pin6, Pin7
}PIN;

typedef enum{
	In = 0, Out = 1
}DIO_IO;

typedef enum{
	Low, High,Toggle
}DIO_VALUE;

void DIO_Set_Port_Direction	(PORT Port,DIO_IO Direction);
void DIO_Set_Port_Output	(PORT Port,DIO_VALUE Output);
void DIO_Set_Pin_Direction	(PORT Port,PIN pin,DIO_IO Direction);
void DIO_Set_Pin_Output		(PORT Port,PIN pin,DIO_VALUE Output);
DIO_VALUE DIO_Get_Pin_Value	(PORT Port,PIN pin);


#endif /* DIO_H_ */