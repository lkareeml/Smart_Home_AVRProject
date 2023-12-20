/*
01    $000    RESET External Pin, Power-on Reset, Brown-out Reset, Watchdog Reset, and JTAG AVR Reset
02    $002    INT0 External Interrupt Request 0
03    $004    INT1 External Interrupt Request 1
04    $006    INT2 External Interrupt Request 2
// 05    $008    TIMER2 COMP Timer2 Compare Match
// 06    $00A    TIMER2 OVF Timer2 Overflow
// 07    $00C    TIMER1 CAPT Timer1 Capture Event
// 08    $00E    TIMER1 COMPA Timer1 Compare Match A
// 09    $010    TIMER1 COMPB Timer1 Compare Match B
// 10    $012    TIMER1 OVF Timer1 Overflow
// 11    $014    TIMER0 COMP Timer0 Compare Match
// 12    $016    TIMER0 OVF Timer0 Overflow
13    $018    SPI, STC Serial Transfer Complete
14    $01A    USART, RXC USART, Rx Complete
15    $01C    USART, UDRE USART Data Register Empty
16    $01E    USART, TXC USART, Tx Complete
17    $020    ADC ADC Conversion Complete
18    $022    EE_RDY EEPROM Ready
19    $024    ANA_COMP Analog Comparator
20    $026    TWI Two-wire Serial Interface
21    $028    SPM_RDY Store Program Memory Ready
*/

#define EXTI0_Raising_Edge    0
#define EXTI0_Falling_Edge    1
#define EXTI0_Any_Change      2
#define EXTI0_Low_Level       3

#define EXTI1_Raising_Edge    0
#define EXTI1_Falling_Edge    1
#define EXTI1_Any_Change      2
#define EXTI1_Low_Level       3

#define EXTI2_Raising_Edge    0
#define EXTI2_Falling_Edge    1


//EXTI0 Peripheral Interrupt
/// Options : EXTI0_Raising_Edge , EXTI0_Falling_Edge , EXTI0_Any_Change, EXTI0_Low_Level
#define EXTI0_ISC EXTI0_Raising_Edge

//EXTI1 Peripheral Interrupt
/// Options : EXTI1_Raising_Edge , EXTI1_Falling_Edge , EXTI1_Any_Change, EXTI1_Low_Level
#define EXTI1_ISC EXTI1_Raising_Edge

//EXTI02 Peripheral Interrupt
/// Options : EXTI2_Raising_Edge , EXTI2_Falling_Edge
#define EXTI2_ISC EXTI2_Raising_Edge


#ifndef INTERRUPT_H_
#define INTERRUPT_H_

void GIE_Enable();
void GIE_Disable();

void EXTI0_Init();
void EXTI0_Set_Direction_Input();
void EXTI0_Set_Internal_Pull_Up();
void EXTI0_Enable();
void EXTI0_Disable();

void EXTI1_Init();
void EXTI1_Enable();
void EXTI1_Disable();

void EXTI2_Init();
void EXTI2_Enable();
void EXTI2_Disable();


#endif /* INTERRUPT_H_ */