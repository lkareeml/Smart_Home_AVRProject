/***********************************************************************
 * Smart_Home_D66.c
 *
 * Created: 12/20/2023 3:23:04 AM
 * Author : Kareem
 */ 
/***********************************************************************/

#include "Application/SmartHome_Core_Header.h"



int main(void){
	Smart_Initialization();
	//Smart_Configurator();
	while (1) 
    {
		Smart_Idle();
	}
}
