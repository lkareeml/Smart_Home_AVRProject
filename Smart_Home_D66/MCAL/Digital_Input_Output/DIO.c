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
			case PORTAx:DDRA_Reg=0xFF;break;
			case PORTBx:DDRB_Reg=0xFF;break;
			case PORTCx:DDRC_Reg=0xFF;break;
			case PORTDx:DDRD_Reg=0xFF;break;
			default:break;
		}
		}else if(Direction == In){
		switch(Port){
			case PORTAx:DDRA_Reg=0x00;break;
			case PORTBx:DDRB_Reg=0x00;break;
			case PORTCx:DDRC_Reg=0x00;break;
			case PORTDx:DDRD_Reg=0x00;break;
			default:break;
		}
	}
}

void DIO_Set_Pin_Direction(PORT port,PIN pin,DIO_IO Direction){
	if(Direction == Out){
		switch(port){
			case PORTAx: Set(DDRA_Reg,pin) ;break;
			case PORTBx: Set(DDRB_Reg,pin) ;break;
			case PORTCx: Set(DDRC_Reg,pin) ;break;
			case PORTDx: Set(DDRD_Reg,pin) ;break;
			default:break;
		}
		}else if(Direction == In){
		switch(port){
			case PORTAx: Clear(DDRA_Reg,pin);break;
			case PORTBx: Clear(DDRB_Reg,pin);break;
			case PORTCx: Clear(DDRC_Reg,pin);break;
			case PORTDx: Clear(DDRD_Reg,pin);break;
			default:break;
		}
	}
}

void DIO_Set_Port_Output(PORT port,DIO_VALUE Output){
	if(Output == High){
		switch(port){
			case PORTAx:PORTA_Reg=0xFF;break;
			case PORTBx:PORTB_Reg=0xFF;break;
			case PORTCx:PORTC_Reg=0xFF;break;
			case PORTDx:PORTD_Reg=0xFF;break;
			default:break;
		}
		}else if(Output == Low){
		switch(port){
			case PORTAx:PORTA_Reg=0x00;break;
			case PORTBx:PORTB_Reg=0x00;break;
			case PORTCx:PORTC_Reg=0x00;break;
			case PORTDx:PORTD_Reg=0x00;break;
			default:break;
		}
	}
}

void DIO_Set_Pin_Output(PORT port,PIN pin,DIO_VALUE Output){
	if(Output == High){
		switch(port){
			case PORTAx: Set(PORTA_Reg,pin) ;break;
			case PORTBx: Set(PORTB_Reg,pin) ;break;
			case PORTCx: Set(PORTC_Reg,pin) ;break;
			case PORTDx: Set(PORTD_Reg,pin) ;break;
			default:break;
		}
	}
	else if(Output == Low){
		switch(port){
			case PORTAx: Clear(PORTA_Reg,pin);break;
			case PORTBx: Clear(PORTB_Reg,pin);break;
			case PORTCx: Clear(PORTC_Reg,pin);break;
			case PORTDx: Clear(PORTD_Reg,pin);break;
			default:break;
		}
	}
	else if(Output == Toggle){
		switch(port){
			case PORTAx: Toggle(PORTA_Reg,pin);break;
			case PORTBx: Toggle(PORTB_Reg,pin);break;
			case PORTCx: Toggle(PORTC_Reg,pin);break;
			case PORTDx: Toggle(PORTD_Reg,pin);break;
			default:break;
		}			
	}
}

DIO_VALUE DIO_Get_Pin_Value(PORT port,PIN pin){
	switch(port){
		case PORTAx: return Get(PINA_Reg,pin);break;
		case PORTBx: return Get(PINB_Reg,pin);break;
		case PORTCx: return Get(PINC_Reg,pin);break;
		case PORTDx: return Get(PIND_Reg,pin);break;
		default:return 0;break;
	}
}