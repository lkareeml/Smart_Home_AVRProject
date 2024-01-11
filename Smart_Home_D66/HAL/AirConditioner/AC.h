/****************************************/
/*          Author: Kareem Atef         */
/*          Date: 06-12-2023            */
/*          Version: 1.0                */
/*          Module: AC                  */
/****************************************/

#ifndef AC_H_
#define AC_H_

#define AC_Pin				2
#define AC_Port				PORTAx

void AC_Initialization();
void AC_On();
void AC_Off();
void AC_Feedback(uint8 New_Feed);


#endif /* AC_H_ */