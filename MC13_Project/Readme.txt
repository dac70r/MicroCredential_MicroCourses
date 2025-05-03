## Project Setup Instructions: 
1. Acquire the necessary components and connect them according to pin out as stated in "Pin Plan" in main program. Refer 
to the circuit diagram in "MC13_Project_Report_DennisWongGuanMing" for a visual illustration of physical connection.

2. Run the program and verify that the 7segment display displaying "dEnn" then the number of remaining car park spaces.


## Code Functionality explanation 
1. This program uses a library for the 7-segment display. The library was originally written for Arduino and has been
converted into C program for use with our microcontroller. 

2. In short, the code does the following: 
	(a) configure the microcontroller pins using registers - eg Pin PB2 configured as output to drive Exit LED
	(b) Interrupt Service Routine (ISR) for pins PC0 and PC1 - these pins receive interrupts from the Entry and
	Exit Sensors. Exit Sensor has higher priority than Entry Sensor. 
	(c) Time Delay Implementation - Implements a crude delay function.
	(d) TM1637 library - This library has been converted into C, register implementation.
	(e) Main program