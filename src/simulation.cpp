#include "data.h"
#include "vector2.h"

Planet planets[9];
bool followEarth = false;

Vector2d GetGravity(const Planet& bodyA, const Planet& bodyB);
void IntegrateForces(double delta);

void Start()
{
	Planet sun;
	sun.pos = Vector2d(0.0, 0.0);
	sun.mass = 1.989e30;
	sun.radius = 4.0;

	Planet mercury;
	mercury.pos = Vector2d(-0.39 * astroUnit, 0.0);
	mercury.mass = 3.285e23;
	mercury.vel = Vector2d(0.0, 47900.0);
	mercury.radius = 0.8;

	Planet venus;
	venus.pos = Vector2d(-0.72 * astroUnit, 0.0);
	venus.mass = 4.867e24;
	venus.vel = Vector2d(0.0, 35000.0);
	venus.radius = 1.5;

	Planet earth;
	earth.pos = Vector2d(-astroUnit, 0.0);
	earth.mass = 5.972e24;
	earth.vel = Vector2d(0.0, 29783.0);
	earth.radius = 2.0;

	Planet mars;
	mars.pos = Vector2d(-1.524 * astroUnit, 0.0);
	mars.mass = 6.39e23;
	mars.vel = Vector2d(0.0, 24077.0);
	mars.radius = 1.0;

	Planet jupiter;
	jupiter.pos = Vector2d(-5.2 * astroUnit, 0.0);
	jupiter.mass = 1.89813e27;
	jupiter.vel = Vector2d(0.0, 13100.0);
	jupiter.radius = 7.0;

	Planet saturn;
	saturn.pos = Vector2d(-9.58 * astroUnit, 0.0);
	saturn.mass = 5.683e26;
	saturn.vel = Vector2d(0.0, 9700.0);
	saturn.radius = 5.0;

	Planet uranus;
	uranus.pos = Vector2d(-19.22 * astroUnit, 0.0);
	uranus.mass = 8.681e25;
	uranus.vel = Vector2d(0.0, 6800.0);
	uranus.radius = 4.5;

	Planet neptune;
	neptune.pos = Vector2d(-30.07 * astroUnit, 0.0);
	neptune.mass = 1.024e26;
	neptune.vel = Vector2d(0.0, 5400.0);
	neptune.radius = 4.0;

	planets[Sun] = sun;
	planets[Mercury] = mercury;
	planets[Venus] = venus;
	planets[Earth] = earth;
	planets[Mars] = mars;
	planets[Jupiter] = jupiter;
	planets[Saturn] = saturn;
	planets[Uranus] = uranus;
	planets[Neptune] = neptune;

	/*for (int i = 0; i < NUM_PLANETS; i++)
	{
		for (int j = 0; j < TRAIL_LENGTH; j++) { planets[i].trail[j] = planets[i].pos; }
		planets[i].trailIndex = 0;
	}*/
}

void Update(double delta)
{
	static bool initialized = false;
	if (!initialized)
	{
		for (int i = 0; i < NUM_PLANETS; i++)
		{
			if (i == Sun) continue;
			planets[i].accel = Vector2d(0.0, 0.0);
			for (auto& bodyB : planets) planets[i].accel += GetGravity(planets[i], bodyB) / planets[i].mass;
		}
		initialized = true;
	}
	IntegrateForces(delta * 86400.0 * 30.0);
}

void IntegrateForces(double delta)
{
	for (int i = 0; i < NUM_PLANETS; i++)
	{
		planets[i].prevAccel = planets[i].accel;
		planets[i].pos += planets[i].vel * delta + planets[i].accel * (0.5 * delta * delta);
	}

	for (int i = 0; i < NUM_PLANETS; i++)
	{
		planets[i].accel = Vector2d(0.0, 0.0);
		for (auto& bodyB : planets) { planets[i].accel += GetGravity(planets[i], bodyB) / planets[i].mass; }
	}

	for (int i = 0; i < NUM_PLANETS; i++)
	{
		planets[i].vel += (planets[i].prevAccel + planets[i].accel) * (0.5 * delta);
	}

	/*for (int i = 0; i < NUM_PLANETS; i++)
	{
		Planet& planet = planets[i];
		planet.trail[planet.trailIndex] = planet.pos;
		planet.trailIndex = (planet.trailIndex + 1) % TRAIL_LENGTH;
	}*/
}

Vector2d GetGravity(const Planet& bodyA, const Planet& bodyB)
{
	if (bodyA.pos == bodyB.pos) return Vector2d(0.0, 0.0);
	Vector2d offset = bodyB.pos - bodyA.pos;

	Vector2d dir = offset.normalized();
	double radius2 = offset.length_squared();
	if (radius2 < 1e16) return Vector2d(0.0, 0.0);

	double force = gravConst * bodyA.mass * bodyB.mass / radius2;

	return dir * force;
}
