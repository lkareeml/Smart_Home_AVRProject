/****************************************/
/*          Author: D66_Team_X          */
/*          Date: 06-12-2023            */
/*          Version: 1.0                */
/*          Module: STD                 */
/****************************************/
/*
 * Standard Types 
 * Change if needed only
 */ 


#ifndef STD_TYPES_H_
#define STD_TYPES_H_

typedef unsigned char uint8;
typedef char sint8;
typedef unsigned short uint16;
typedef short sint16;
typedef long unsigned int uint32;
typedef int sint32;
typedef enum {FALSE = 0, TRUE = 1} bool;
typedef enum{
	LCD = 0,
	UART = 1
}Requester;


#endif /* STD_TYPES_H_ */