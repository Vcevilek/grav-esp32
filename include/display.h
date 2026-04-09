#pragma once
#include "vector2.h"

extern int planetIndex;

Vector2i WorldToScreen(Vector2d worldPos);
void DisplayInit();
void ChangeZoom(double delta);
void Draw();