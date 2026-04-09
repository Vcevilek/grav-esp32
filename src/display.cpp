#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "data.h"
#include "display.h"
#include "vector2.h"

#define ZOOM_INC 20
#define ZOOM_DEC 19
#define PLANET_INC 21
#define PLANET_DEC 47

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1		// Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int planetIndex = Sun;
int lastPlanetIndex = Sun;
bool lastIncState = HIGH;
bool lastDecState = HIGH;
unsigned long lastIncTime = 0.0;
unsigned long lastDecTime = 0.0;
unsigned long planetSelectedTime = 0.0;

const char* planetNames[NUM_PLANETS] = {
	"Sun", "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune"};

Vector2i WorldToScreen(Vector2d worldPos)
{
	double zoomScale = ((double)display.height() / (2.0 * astroUnit)) * camera.zoom;
	Vector2d screenCenter = Vector2d((double)display.width() / 2.0, (double)display.height() / 2.0);
	Vector2d offset = ((worldPos - camera.center) * zoomScale);
	Vector2d finalPos = screenCenter + offset;
	return Vector2i((int)finalPos.x, (int)finalPos.y);
}

void DisplayInit()
{
	if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
	{
		Serial.println(F("SSD1306 allocation failed"));
		for (;;); // Don't proceed, loop forever
	}

	display.setTextSize(1);
	display.setTextColor(SSD1306_WHITE);
	display.cp437(true);
}

void ChangeZoom(double delta)
{
	if (digitalRead(ZOOM_INC) == LOW) camera.zoom += delta * 0.5;
	if (digitalRead(ZOOM_DEC) == LOW) camera.zoom -= delta * 0.5;

	unsigned long now = millis();

	bool incState = digitalRead(PLANET_INC);
	if (incState == LOW && lastIncState == HIGH && now - lastIncTime > 200)
	{
		planetIndex++;
		if (planetIndex >= NUM_PLANETS) planetIndex = 0;
		lastIncTime = now;
	}

	bool decState = digitalRead(PLANET_DEC);
	if (decState == LOW && lastDecState == HIGH && now - lastDecTime > 200)
	{
		planetIndex--;
		if (planetIndex < 0) planetIndex = NUM_PLANETS - 1;
		lastDecTime = now;
	}

	if (planetIndex != lastPlanetIndex)
	{
		planetSelectedTime = now;
		lastPlanetIndex = planetIndex;
	}

	lastIncState = incState;
	lastDecState = decState;
	camera.zoom = max(camera.zoom, 0.001f);

	Vector2d target = planets[planetIndex].pos;
	double zoomScale = ((double)display.height() / (2.0 * astroUnit)) * camera.zoom;
	Vector2d diff = (target - camera.center) * zoomScale;

	if (now - planetSelectedTime > 1000) { camera.center = target; }
	else
	{
		camera.center = camera.center * (1 - delta * 3.0) + planets[planetIndex].pos * delta * 3.0;
	}
}

void Draw()
{
	Vector2i screenPos = WorldToScreen(planets[Earth].pos);
	display.clearDisplay();

	display.setCursor(0, 0);
	display.printf(planetNames[planetIndex]);

	for (int i = 0; i < NUM_PLANETS; i++)
	{
		Planet& planet = planets[i];
		Vector2i screenPos = WorldToScreen(planet.pos);
		display.fillCircle(screenPos.x, screenPos.y, planet.radius * camera.zoom, SSD1306_WHITE);

		/*for (int j = 0; j < TRAIL_LENGTH - 1; j++)
		{
			int idx1 = (planet.trailIndex + j) % TRAIL_LENGTH;
			int idx2 = (planet.trailIndex + j + 1) % TRAIL_LENGTH;

			Vector2i a = WorldToScreen(planet.trail[idx1]);
			Vector2i b = WorldToScreen(planet.trail[idx2]);

			display.drawLine(a.x, a.y, b.x, b.y, SSD1306_WHITE);
		}*/
	}

	display.display();
}