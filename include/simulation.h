#include "data.h"

void Start();
void Update(double delta);
void IntegrateForces(double delta);
Vector2d GetGravity(const Planet &bodyA, const Planet &bodyB);