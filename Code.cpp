// ELB018 Real Time Software Engineering
// Washing machine lab demo - uses the washing machine door switch to start and 
// stop the motor  

#include <iostream>
#include <ctime>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "Classes.h"


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;


	//Creates class object for physical elements on the board to apply the correct variables to the type
  Buttons Door(DoorSwitch,0);										//Physical object "Door switch" of type Buttons
  Buttons Accept(AcceptSwitch,0);
  Buttons Cancel(CancelSwitch,0);
  Buttons Prog3(Prog3Select,0);
  Buttons Prog2(Prog2Select,0);
  Buttons Prog1(Prog1Select,0);  
  Motors DrumDrive(MotorDriveMask, MotorDirMask);					//Physical object "drum motor" of type Motors
  Buzzers Buzzer(BuzzerMask);										//Physical object "Buzzer" of type Buzzers
  Displays SevenSeg(DisplayMask);									//Physical object "SevenSeg" (Seven Segment Display) of type Displays
  Counters Counter(0);												//Creates "Counter" of type Counters. This is not a physical object but used with delays
  
 char choice;
  int sel;




void displaymenu()
{
//Initialise the user interface options
//that the user has to choose from
//to test the washing machine elements
  cout << "What would you like to do?" << endl;
  cout << "1. Press 1 to test the buzzer" << endl;
  cout << "2. Press 2 to start the motor turning in a clockwide direction" << endl;
  cout << "3. Press 3 to star the motor turning in an anti-clockwise direction" << endl;
  cout << "4. Press 4 to then choose a number (1-8) which will be displayed on the 7 segment display" << endl;
  cout << "5. Press 5 to see which button you've pressed" << endl;
  cout << "6. Press 6 to test timer" << endl;
  cout << "7. Press 7 to exit the program" << endl << endl;
  choice = 0x00; 		//Clear the menu selection veriable at every menu redraw

  }


bool button_test() {
//User choice number 4. The washing machine input buttons are checked for input.
//If an input is found, the button is latched on for 3 seconds. The name of the input
//pressed is also printed to screen

bool exit = 0;
  while (1)
  {
  while (exit == 0)
  {
    if (Accept.set_button_state()) {
  cout << "You pressed the Accept button" << endl; 	//Print button label to screen
  Counter.start_timer(3);		//3 second latch
   PortBData = (PortBData & (~SwitchReset));		//Unlatch the button
        (*PPI).PortB = (u32) PortBData;
  exit = 1;
  }
  if (Prog3.set_button_state()) {
  cout << "You pressed the Program3 button" << endl;	//Print button label to screen
 Counter.start_timer(3);		//3 second latch
   PortBData = (PortBData & (~SwitchReset));			//Unlatch the button
        (*PPI).PortB = (u32) PortBData;
  exit = 1;
  }
    if (Cancel.set_button_state()) {
  cout << "You pressed the Cancel button" << endl;	//Print button label to screen
   Counter.start_timer(3);		//3 second latch
   PortBData = (PortBData & (~SwitchReset));		//Unlatch the button
        (*PPI).PortB = (u32) PortBData;
  exit = 1;
  }
    if (Prog2.set_button_state()) {
  cout << "You pressed the Program2 button" << endl;	//Print button label to screen
   Counter.start_timer(3);		//3 second latch
   PortBData = (PortBData & (~SwitchReset));			//Unlatch the button
        (*PPI).PortB = (u32) PortBData;
  exit = 1;
  }
    if (Prog1.set_button_state()) {
  cout << "You pressed the Program1 button" << endl;	//Print button label to screen
   Counter.start_timer(3);		//3 second latch
   PortBData = (PortBData & (~SwitchReset));			//Unlatch the button
        (*PPI).PortB = (u32) PortBData;
  exit = 1;
  }
   if (Door.set_button_state()) {			//Depress door switch to open the door
  cout << "You closed the door" << endl;	//Release door switch to show door has been closed
   Counter.start_timer(3);		//3 second latch
   PortBData = (PortBData & (~SwitchReset));	//Unlatch the button
        (*PPI).PortB = (u32) PortBData;
  exit = 1;
  }
  }
   cin.ignore();		//Flush the cin buffer
  cout << "Would you like to exit? (1 for yes or 0 for no)" << endl; //Allows user to break out of testing or continue
    cin >> exit;
  if (exit == 1)
  {
  return exit;
  }
  else  {
  exit = 0;
  }
  
  }
  }

int main (void)	
{
 
  // enable actions to control the interface
  u8 ControlByte = (ModeSel | AMode0 | AInp | BMode0 | BOut | CHInp | CLInp);
  (*PPI).Control = (u32) ControlByte;

  	//Reset the drum, buzzer and seven seg display
   DrumDrive.set_drum_movement(0,0);
   Buzzer.set_buzzer(0);
   
   SevenSeg.set_display_value(0);


   
  // announce the program
  cout << "Welcome to the Washing Machine Simulator Program" << endl << endl;
  displaymenu();

  // Stay in loop to enable the user to select which element to test. Using the menu options defined above
  // activated by the users numeric input
  while(1) {

 
 cin >> choice;		//Read the users input to select which element to test

  switch (choice)
  {
  case '1':
  cin.ignore();		//Flush the input buffer
   Buzzer.set_buzzer(1);		//Turn the buzzer on
   Counter.start_timer(5);		//keep the buzzer turned on for this length of time
   Buzzer.set_buzzer(0);		//turn the buzzer off
displaymenu();					//Redraw user selection options

  break;						//Return to user selection
  
  case '2':
  cin.ignore();

    DrumDrive.set_drum_movement(1,0);		//Turn the drum motor in a clockwise direction
    Counter.start_timer(10);				//Maintain this state for 10 seconds
    DrumDrive.set_drum_movement(0,0);		//Turn motor off
	choice = 0x00;							//Clear user choice input
  displaymenu();							//Redraw user selection options
  
  break;									//Return to user selection
  
  case '3':
  cin.ignore();								//Flush input buffer

    DrumDrive.set_drum_movement(1,1);		//Pass speed and direction variable to the motor drive routine
    Counter.start_timer(10);				//10 second delay. Hold motor condition for this length of time
DrumDrive.set_drum_movement(0,1);			//Stop the motor from turning
  displaymenu();							//Redraw user selection options
  
  break;
  
    case '4':
    cin.ignore();								//Flush the input buffer
    char number_choice;							//Stores the number the user wishes to output
cout << "Choose a number to display: " << endl;
    cin >> number_choice;						//Read in the number the user enters
SevenSeg.set_display_value(number_choice);		//Pass the user entered value to the Seven segment class
cin.ignore();
  displaymenu();								//Redraw user selection options
  break;
  
  case '5':
  
button_test();									//Calls the button test routine so the user can test the input buttons
  displaymenu();								//Return to menu
  cin.ignore();									//Flush the input buffer
  break;
  
  
  
  case '6':
  cin.ignore();												//Flush the input buffer
  u32 length;												//Store a user entered value used to determin delay length
  cout << "Enter a length of time to test: " << endl;
  cin >> length;											//Read in the user entered value for delay length
Counter.start_timer(length);								//Call the delay subroutine with the users delay length value
cin.ignore();
  displaymenu();											//Redraw user selection options
  break;
  
    case '7':
  cin.ignore();
  cout << "We hope you enjoyed using the Bytronic Washing Machine Simulator, hope to see you again soon :)" << endl;
//exit(EXIT_FAILURE);
return 0;													//Exit the routine
  
  break;
  //Should the user enter an invalid value on the option selection menu this will be triggered. This will
  //tell the user to re-enter a value that is within the required range
  default:
  cin.ignore();																		//Flush the buffer of the invalid value
  cout << "You have entered an incorrect option, please choose again" << endl;
  displaymenu();																	//Redraw options menu
  break;
  }
  
  }
  
  
    	
  } 

