/*
 * DIO.c
 *
 * Created: 12/20/2023 5:03:21 AM
 *
 */ 

#include "DIO.h"
#include "../../AVR32_Reg_Private.h"
#include "../../Bit_Manipulation.h"


void DIO_Set_Port_Direction(PORT Port,DIO_IO Direction){
	if(Direction == Out){
		switch(Port){
			case PORTA:DDRA_Reg=0xFF;break;
			case PORTB:DDRB_Reg=0xFF;break;
			case PORTC:DDRC_Reg=0xFF;break;
			case PORTD:DDRD_Reg=0xFF;break;
			default:break;
		}
		}else if(Direction == In){
		switch(Port){
			case PORTA:DDRA_Reg=0x00;break;
			case PORTB:DDRB_Reg=0x00;break;
			case PORTC:DDRC_Reg=0x00;break;
			case PORTD:DDRD_Reg=0x00;break;
			default:break;
		}
	}
}

void DIO_Set_Pin_Direction(PORT port,PIN pin,DIO_IO Direction){
	if(Direction == Out){
		switch(port){
			case PORTA: Set(DDRA_Reg,pin) ;break;
			case PORTB: Set(DDRB_Reg,pin) ;break;
			case PORTC: Set(DDRC_Reg,pin) ;break;
			case PORTD: Set(DDRD_Reg,pin) ;break;
			default:break;
		}
		}else if(Direction == In){
		switch(port){
			case PORTA: Clear(DDRA_Reg,pin);break;
			case PORTB: Clear(DDRB_Reg,pin);break;
			case PORTC: Clear(DDRC_Reg,pin);break;
			case PORTD: Clear(DDRD_Reg,pin);break;
			default:break;
		}
	}
}

void DIO_Set_Port_Output(PORT port,DIO_VALUE Output){
	if(Output == High){
		switch(port){
			case PORTA:PORTA_Reg=0xFF;break;
			case PORTB:PORTB_Reg=0xFF;break;
			case PORTC:PORTC_Reg=0xFF;break;
			case PORTD:PORTD_Reg=0xFF;break;
			default:break;
		}
		}else if(Output == Low){
		switch(port){
			case PORTA:PORTA_Reg=0x00;break;
			case PORTB:PORTB_Reg=0x00;break;
			case PORTC:PORTC_Reg=0x00;break;
			case PORTD:PORTD_Reg=0x00;break;
			default:break;
		}
	}
}

void DIO_Set_Pin_Output(PORT port,PIN pin,DIO_VALUE Output){
	if(Output == High){
		switch(port){
			case PORTA: Set(PORTA_Reg,pin) ;break;
			case PORTB: Set(PORTB_Reg,pin) ;break;
			case PORTC: Set(PORTC_Reg,pin) ;break;
			case PORTD: Set(PORTD_Reg,pin) ;break;
			default:break;
		}
		}else if(Output == Low){
		switch(port){
			case PORTA: Clear(PORTA_Reg,pin);break;
			case PORTB: Clear(PORTB_Reg,pin);break;
			case PORTC: Clear(PORTC_Reg,pin);break;
			case PORTD: Clear(PORTD_Reg,pin);break;
			default:break;
		}
	}
}

DIO_VALUE DIO_Get_Pin_Value(PORT port,PIN pin){
	switch(port){
		case PORTA: return Get(PINA_Reg,pin);break;
		case PORTB: return Get(PINB_Reg,pin);break;
		case PORTC: return Get(PINC_Reg,pin);break;
		case PORTD: return Get(PIND_Reg,pin);break;
		default:return 0;break;
	}
}