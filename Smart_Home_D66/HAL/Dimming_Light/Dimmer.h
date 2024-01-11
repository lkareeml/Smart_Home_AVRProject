/*
 * Dimmer.h
 *
 * Created: 12/20/2023 4:24:25 PM
 *
 */ 


#ifndef DIMMER_H_
#define DIMMER_H_


void Dimmer_Init();
void Dimmer_Feedback(uint8 New_Feed);
void Dimmer_Increase();
void Dimmer_Decrease();
void Dimmer_On();
void Dimmer_Off();


#endif /* DIMMER_H_ */