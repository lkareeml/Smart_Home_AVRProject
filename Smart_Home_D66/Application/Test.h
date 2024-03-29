/*
 * Test.h
 *
 */ 


#ifndef TEST_H_
#define TEST_H_


/*

From 0 to 1024 for UART
From 1024 to 2048 For LCD

0001           00000000 // DOOR STATE OPEN/CLOSE
0002           00000000 // AC STATE ON/OFF
0003           00000000 // AC STATE Manual/Auto
0004           00000000 // Lock State (admin unlock)
0005           00000000 // UART admin Logged In (UserID)
0006           00000000 // KEYPAD USER LOGGED IN (UserID) if 0 means non
0007           00000000 // UART user Logged In (UserID)
0008           00000000 // 
0009           00000000 // 

0010           00000000 // Admin ID MUST BE 99
0011           00000000 // Admin Password
0012           00000000 //2,3,4,5,6,7
0018           00000000 //8
0019           00000000	//END EMPTY
0020           00000000 // Admin UserName
0021           00000000 //2,3,4,5,6,7
0027           00000000 //8
0028           00000000	//END EMPTY
0029           00000000	//Filler

0030           00000000 // 1nd User ID (if exist it will write 1 here)
0031           00000000 // 1nd User Password
0032           00000000 // 2,3,4,5,6,7
0038           00000000 // 8
0039           00000000	// END EMPTY
0040           00000000 // 1nd User Name
0041           00000000 // 2,3,4,5,6,7
0047           00000000 // 8
0048           00000000	// END EMPTY
0049           00000000	// Filler

0050           00000000 // 2st User ID (if exist it will write 2 here)
0069           00000000	// END EMPTY

0070           00000000 // 3st User ID (if exist it will write 3 here)
0089           00000000	// END EMPTY

2048           00000000

*/


#endif /* TEST_H_ */