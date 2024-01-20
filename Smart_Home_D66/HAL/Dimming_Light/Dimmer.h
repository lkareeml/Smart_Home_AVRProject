/*
 * Dimmer.h
 *
 * Created: 12/20/2023 4:24:25 PM
 *
 */ 
#ifndef DIMMER_H_
#define DIMMER_H_

//	**********    Dimmer Configuration Port And PIN    **********
#define Dimmer_Pin		3
#define Dimmer_Port		PORTDx
// **********************************************************

void Dimmer_Init();
void Dimmer_FeedBack(uint8 New_Feed,Requester Type);
void Dimmer_Increase();
void Dimmer_Decrease();
void Dimmer_On();
void Dimmer_Off();


#endif /* DIMMER_H_ */