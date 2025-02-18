/***********************************************************************
 * Header File:
 *    Inertia : The representation of a Inertia
 * Authors:
 *    Sam Evans
 *    Savannah 
 * Summary:
 *    Everything we need to know about the Inertia of an object
 ************************************************************************/

#pragma once
#include "angle.h"
#include	"acceleration.h"
#include	"velocity.h"


class angle;
class Acceleration;
class Velocity;

class Inertia
{
public:
	Inertia()
	{
		time = 1;
		drag = 0;
		gravity = 0;
		angle.setDegrees(15.0);
	};

private:
	float time;
	Acceleration a;
	float drag;
	float gravity;
	Angle angle;
	float inertia;
	Velocity v;

};

Inertia::Inertia()
{
}

Inertia::~Inertia()
{
}