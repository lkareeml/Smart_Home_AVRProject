/*
 * Door_Servo.h
 *
 * Created: 12/20/2023 4:21:03 PM
 *
 */ 


#ifndef DOOR_SERVO_H_
#define DOOR_SERVO_H_

//	**********   Servo Configuration Port And PIN    **********
#define Servo_Pin		Pin7
#define Servo_Port		PORTDx
// **********************************************************

void Servo_Init();
void Door_Feedback(uint8 New_Feed,Requester Type);

#endif /* DOOR_SERVO_H_ */