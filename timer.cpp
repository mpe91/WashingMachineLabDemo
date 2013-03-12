#include <ctimer>
#include <time.h>
#include <Code.cpp>

class timer
{
bool started;
bool paused;
bool stopped;

int timerlength;
int time;

public:
timer(int);
void Start(int);
void Stop();
void Pause ();

}

timer::timer(int a)
{
timerlength = a;
}

void Start(int length)
{
 timerlength = int length
 timer = 0;
 current_time = 0;
 
 
 if (current_time >= (double)timerlength*CLOCKS_PER_SEC)
 {
 SevenSeg.set_display_value();
 timer++
 }
 

}