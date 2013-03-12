// Define Intel 8255A register structure
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct i8255A
{
  u32 PortA; // only lowest 8 bits used. Offset 0x00
  u32 PortB; // only lowest 8 bits used. Offset 0x04
  u32 PortC; // only lowest 8 bits used. Offset 0x08
  u32 Control;	// only lowest 8 bits used. Offset 0x0C
};

// Define a pointer to the 8255 structure
volatile i8255A *PPI = (i8255A *) 0xFFFF1C00;

// PortB output bits for the washing machine
#define BuzzerMask      0x80
#define SwitchReset     0x40
#define MotorDirMask    0x20
#define MotorDriveMask  0x10
#define DisplayMask     0x0F

// PortC input bits bits for the washing machine
#define MotorSpeedFeed 0x40
#define AcceptSwitch   0x20
#define CancelSwitch   0x10
#define Prog3Select    0x08
#define Prog2Select    0x04
#define Prog1Select    0x02
#define DoorSwitch     0x01
#define ResetMask      0x00

// Bit masks to enable mode setting for the Control port
#define ModeSel 0x80
#define AMode0  0x00
#define AMode1  0x20
#define AMode2  0x40
#define AInp    0x10
#define AOut    0x00
#define BMode0  0x00
#define BMode1  0x04
#define BInp    0x02
#define BOut    0x00
#define CHInp   0x08
#define CHOut   0x00
#define CLInp   0x01
#define CLOut   0x00

u8 PortCData = 0;
u8 PortBData = 0;




class Displays {			//Defines class Display type
u8 address;					//Physical address of the display
int value;
public:
Displays(u8);


void set_display_value(u8);

};

Displays::Displays(u8 a) {			//
address = a;
}


class Counters {					//Class used to define Counter structure

time_t start;
time_t current_time;
double timerlength;

public:
Counters(u8);

bool start_timer(u32);

};

Counters::Counters(u8 a) {
timerlength = a;
}


class Motors {					//Class definition of motors
u8 driveaddress;				//Physical motor address
u8 diraddress;					//Direction controller address
bool speed;						// set speed. 1=moving 0=stationary
bool direction;					//set direction 0=clockwise 1=anti-clockwise

public:
Motors(u8, u8);


void set_drum_movement(bool, bool);

};

Motors::Motors(u8 a, u8 b) {
driveaddress = a;
diraddress = b;
}



class Buzzers {
u8 address;				//Buzzer address
bool state;				//State of the buzzer 1=active

public:
Buzzers (u8);

void set_buzzer(u8);
};

Buzzers::Buzzers(u8 a) {
address = a;
}


class Buttons {					//Class definition of button type
u8 address;						//Physical address of the buttons
bool state;						//State of the button 1=pressed

public:
Buttons(u8,u8);

bool set_button_state();
};

Buttons::Buttons (u8 a, u8 b) {
address = a;
state = b;
}




bool Counters::start_timer(u32 len)					//Delay routine
{

double timeleft;									//Counts running delay length
timerlength = len;

  time(&start);
  while (1)
  {
  time(&current_time);
  
  timeleft = difftime(current_time, start);			//Works out length of time left
  
  if (timeleft >= timerlength) {					//If the length of time left is 
  cout << "Timer finished" << endl;					//If current length of delay is equal to or greater than the desired length of the delay
break;												//the delay routine is finished
  }

}
}


void Motors::set_drum_movement(bool s, bool d)		//Controls movement of the motor
{
speed = s;											// Speed. 1=moving 0=stationary
direction = d;										//Rotation direction. 0=clockwise 1=anti-clockwise

if (direction == 1) {
    PortBData = (PortBData | (diraddress));			//Sets motor ports
  }			
  else {
    PortBData = (PortBData & (~diraddress));		//Clear motor ports
  }
   if (speed == 1) {
    PortBData = (PortBData | (driveaddress));
  }
  else {
    PortBData = (PortBData & (~driveaddress));
  }
  (*PPI).PortB = (u32) PortBData;

}


void Buzzers::set_buzzer(u8 on)							//Turns the buzzer on or off
{
state = on;
if (state == 1) {
PortBData = (PortBData | (address));					//Turns the buzzer on
}
else {
PortBData = (PortBData & (~address));					//Turns the buzzer off
}
(*PPI).PortB = (u32) PortBData;
}


bool Buttons::set_button_state()							//Member function of buttons which toggles the button state when pressed or released.
{
  PortCData = (u8)(*PPI).PortC; // read the input port
  PortBData = (u8)(*PPI).PortB;
  if (state == 1) {
;
  if (PortCData & address) { 								//If the button is pressed

    state = 1;												//Set the state for that button to 1
  }
  else {
    state = 0;												//Otherwise state=0 for the button
  }
  }
  else {
  
  if (PortCData & address) { 							//If state is 0
  PortBData = (PortBData | (SwitchReset));				//and the switch is pressed
(*PPI).PortB = (u32) PortBData;							//latch the switch on
    state = 1;
  }
    else {
    state = 0;
      }
  }
return state;
}


void Displays::set_display_value(u8 val)				//Sets the displays output value
{
PortBData = (PortBData & 0xF0);							//Clear the current display value
value = (val & 0x0F);									//New value has 8 bits. Set the upper un-needed 4 bits to 0
PortBData = (PortBData | (value));						//Send the new value to the display
(*PPI).PortB = (u32) PortBData;

}

