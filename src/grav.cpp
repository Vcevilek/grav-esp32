#include <Arduino.h>
#include <Wire.h>

#include "display.h"
#include "simulation.h"

AstroCam camera;

unsigned long currentTime = 0, oldTime = 0;

void setup()
{
	Wire.begin(8, 9);
	Wire.setClock(400000);

	pinMode(19, INPUT_PULLUP);
	pinMode(20, INPUT_PULLUP);
	pinMode(21, INPUT_PULLUP);
	pinMode(47, INPUT_PULLUP);

	Serial.begin(9600);

	DisplayInit();

	Start();
}

void loop()
{
	currentTime = micros();
	double delta = (currentTime - oldTime) / 1000000.0;
	oldTime = currentTime;

	Update(delta);

	ChangeZoom(delta);
	Draw();
}