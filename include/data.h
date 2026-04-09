#pragma once
#include "vector2.h"

#define gravConst 6.6743e-11
#define astroUnit 1.496e11
#define NUM_PLANETS 9
#define TRAIL_LENGTH 50

struct Planet
{
	Vector2d pos;
	Vector2d vel;
	Vector2d accel;
	Vector2d prevAccel;
	double mass = 1.0;
	double radius = 10.0;

	// Vector2d trail[TRAIL_LENGTH];
	// int trailIndex = 0;
};

struct AstroCam
{
	Vector2d center = Vector2d(0.0, 0.0);
	float zoom = 1.0;
};

enum
{
	Sun,
	Mercury,
	Venus,
	Earth,
	Mars,
	Jupiter,
	Saturn,
	Uranus,
	Neptune,
};

extern Planet planets[NUM_PLANETS];
extern AstroCam camera;