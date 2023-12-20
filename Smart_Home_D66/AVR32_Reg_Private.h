
/****************************************/
/*          Author: D66_Team_X          */
/*          Date: 06-12-2023            */
/*          Version: 1.0                */
/*          Module: AC                  */
/****************************************/

// All Atmega32 Registers 
// Change only if you need to make the project work on other micro controllers

/**********************************************************************/
#ifndef STD_TYPES_H_
#include "STD_Types.h"
#endif

#ifndef AVR32_REG_PRIVATE_H_
#define AVR32_REG_PRIVATE_H_

#define SREG_Reg        *((volatile uint8*)0x5F) // ////
#define SPH_Reg         *((volatile uint8*)0x5E)
#define SPL_Reg         *((volatile uint8*)0x5D)
#define GICR_Reg        *((volatile uint8*)0x5B) // General Interrupt
#define GIFR_Reg        *((volatile uint8*)0x5A) // General Interrupt
#define MCUCR_Reg       *((volatile uint8*)0x55) // Interrupt
#define SPMCR_Reg       *((volatile uint8*)0x57)
#define OSCCAL_Reg      *((volatile uint8*)0x51)
#define OCDR_Reg        *((volatile uint8*)0x51)
#define SFIOR_Reg       *((volatile uint8*)0x50) // ////
#define ASSR_Reg        *((volatile uint8*)0x42)
#define ACSR_Reg        *((volatile uint8*)0x28) // TIMER1 BIT1

#define WDTCR_Reg       *((volatile uint8*)0x41) // WatchDog
#define MCUCSR_Reg      *((volatile uint8*)0x54) // Reset Flags

#define PORTA_Reg       *((volatile uint8*)0x3B) // GPIO
#define DDRA_Reg        *((volatile uint8*)0x3A) // GPIO
#define PINA_Reg        *((volatile uint8*)0x39) // GPIO
#define PORTB_Reg       *((volatile uint8*)0x38) // GPIO
#define DDRB_Reg        *((volatile uint8*)0x37) // GPIO
#define PINB_Reg        *((volatile uint8*)0x36) // GPIO
#define PORTC_Reg       *((volatile uint8*)0x35) // GPIO
#define DDRC_Reg        *((volatile uint8*)0x34) // GPIO
#define PINC_Reg        *((volatile uint8*)0x33) // GPIO
#define PORTD_Reg       *((volatile uint8*)0x32) // GPIO
#define DDRD_Reg        *((volatile uint8*)0x31) // GPIO
#define PIND_Reg        *((volatile uint8*)0x30) // GPIO

#define ADMUX_Reg       *((volatile uint8*)0x27) // ADC
#define ADCSRA_Reg      *((volatile uint8*)0x26) // ADC
#define ADCH_Reg        *((volatile uint8*)0x25) // ADC
#define ADCL_Reg        *((volatile uint8*)0x24) // ADC

#define TIMSK_Reg       *((volatile uint8*)0x59) // TIMER
#define TIFR_Reg        *((volatile uint8*)0x58) // TIMER
#define TCCR0_Reg       *((volatile uint8*)0x53) // TIMER
#define TCNT0_Reg       *((volatile uint8*)0x52) // TIMER
#define OCR0_Reg        *((volatile uint8*)0x5C) // TIMER
#define TCCR1A_Reg      *((volatile uint8*)0x4F) // TIMER
#define TCCR1B_Reg      *((volatile uint8*)0x4E) // TIMER
#define TCNT1H_Reg      *((volatile uint8*)0x4D) // TIMER
#define TCNT1L_Reg      *((volatile uint8*)0x4C) // TIMER
#define OCR1AH_Reg      *((volatile uint8*)0x4B) // TIMER
#define OCR1AL_Reg      *((volatile uint8*)0x4A) // TIMER
#define OCR1BH_Reg      *((volatile uint8*)0x49) // TIMER
#define OCR1BL_Reg      *((volatile uint8*)0x48) // TIMER
#define ICR1H_Reg       *((volatile uint8*)0x47) // TIMER
#define ICR1L_Reg       *((volatile uint8*)0x46) // TIMER
#define TCCR2_Reg       *((volatile uint8*)0x45) // TIMER
#define TCNT2_Reg       *((volatile uint8*)0x44) // TIMER
#define OCR2_Reg        *((volatile uint8*)0x43) // TIMER

#define UBRRH_Reg       *((volatile uint8*)0x40) // UART
#define UBRRL_Reg       *((volatile uint8*)0x29) // UART
#define UCSRC_Reg       *((volatile uint8*)0x40) // UART
#define UDR_Reg         *((volatile uint8*)0x2C) // UART
#define UCSRA_Reg       *((volatile uint8*)0x2B) // UART
#define UCSRB_Reg       *((volatile uint8*)0x2A) // UART

#define SPDR_Reg        *((volatile uint8*)0x2F) // SPI
#define SPSR_Reg        *((volatile uint8*)0x2E) // SPI
#define SPCR_Reg        *((volatile uint8*)0x2D) // SPI

#define TWCR_Reg        *((volatile uint8*)0x56) // TWI (I2C)
#define TWDR_Reg        *((volatile uint8*)0x23) // TWI (I2C)
#define TWAR_Reg        *((volatile uint8*)0x22) // TWI (I2C)
#define TWSR_Reg        *((volatile uint8*)0x21) // TWI (I2C)
#define TWBR_Reg        *((volatile uint8*)0x20) // TWI (I2C)

#define EEARH_Reg       *((volatile uint8*)0x3F) // EEPROM INTERNAL
#define EEARL_Reg       *((volatile uint8*)0x3E) // EEPROM INTERNAL
#define EEDR_Reg        *((volatile uint8*)0x3D) // EEPROM INTERNAL
#define EECR_Reg        *((volatile uint8*)0x3C) // EEPROM INTERNAL



#endif /* AVR32_REG_PRIVATE_H_ */