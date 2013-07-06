
#include "Timer.h"


#define ledPin 13
	
Timer t;
uint64_t rpmCounter = 0;
int targetRPM = 700;
int cycle = 50;
int lowDuty = 20;
float highDuty = 80;
float highVolt = 1024;
double duty = 50;
int on = trunc(cycle * duty);
int off = cycle - on;
int potPin = 2;
float dutyMult = highDuty / highVolt;
void setup()
{
	attachInterrupt(0, pulse, RISING);
	Serial.begin(9600); 

	int tickEvent = t.every(1000, tickHandler);
	pinMode(3, OUTPUT);   // OCR2B sets duty cycle
	TCCR2A = 0x23;     // COM2B1, WGM21, WGM20 
	// Set prescaler  
	TCCR2B = 0x0A;   // WGM21, Prescaler = /8
	// Set TOP and initialize duty cycle to zero(0)
	OCR2A = 79;    // TOP DO NOT CHANGE, SETS PWM PULSE RATE
	OCR2B = 0;    // duty cycle for Pin 3 (0-79) generates 1 500nS pulse even when 0 :(
}

void writeLog(double rpm, float pot, double _duty)
{
	Serial.print(tuple("RPM", rpm));
	Serial.print(tuple("POT", pot));
	Serial.print(tuple("DUTY", _duty));
	Serial.println();
}

void tickHandler()
{
	double rpm = 0;
	if(rpmCounter > 0) rpm = (rpmCounter / 2) * 60;
	double grey = targetRPM * .05;
	float val = analogRead(potPin);
	rpmCounter = 0;
	duty = trunc(val * dutyMult);
	writeLog(rpm, val, duty);	
	OCR2B = duty;
}


void pulse()
{
	rpmCounter++;
}



void loop()
{
	t.update();
	if(Serial.available() > 0)
	{
		int key = Serial.read();
	}
}


String tuple(String name, String val, String terminator = ";")
{
	return name + ":" + val + terminator;
}

String tuple(String name, float val)
{
	int converted = trunc(val);
	return tuple(name, String(converted));
}

String tuple(String name, double val)
{
	int converted = trunc(val);
	return tuple(name, String(converted));
}
String tuple(String name, int val)
{
	return tuple(name, String(val));
}