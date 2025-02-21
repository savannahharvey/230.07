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
#include "position.h"
#include <vector>

class angle;
class Acceleration;
class Velocity;

class Bullet
{
public:
	Bullet()
	{
		time = 1.0;
	};

	Bullet(Position pos)
	{
		time = 1.0;
		angle.setDegrees(15.0);
	};	
	

	//getters
	double getDegrees()			const { return angle.getDegrees();	}	
	double getRadians()			const	{ return angle.getRadians();	}
	double getDX()					const { return v.getDX();				}
	double getDY()					const { return v.getDY();				}
	double getDDX()				const { return a.getDDX();				}
	double getDDY()				const { return a.getDDY();				}
	double getXPosition()		const { return pos.getMetersX(); }
	double getYPosition()		const { return pos.getMetersY(); }
	Acceleration getAccleration() const { return a; }
	double interpolation(const double altitude, vector <pair<int, double>> table);
	double getSpeed()				const { return v.getSpeed(); }
	
	//setter
	void setDegrees(double degrees)		{angle.setDegrees(degrees);	}
	void setRadians(double radians)		{angle.setRadians(radians);	}
	void setDX(double dx)					{v.setDX(dx);						}
	void setDY(double dy)					{v.setDY(dy);						}
	void addVelocity(const Acceleration& acceleration, double time)
	{
		v.add(acceleration, time);
	}
	void setVelocity(const Angle& angle, double magnitude)
	{
		v.set(angle, magnitude);
	}
	void setDrag(double p, double area, double weight);
	void setDDX(double ddx)					{ a.setDDX(ddx);					}
	void setDDY(double ddy)					{ a.setDDY(ddy);					}
	void setPos(double x, double y)		{pos.setMetersX(x); pos.setMetersY(y); }
	void setStartPos(Position& pos);
	void travel(double t);
	void addAcceleration(const Acceleration& accel)
	{
		a.setDDX(accel.getDDX() + a.getDDX());
		a.setDDY(accel.getDDY() + a.getDDY());
	}
	//void addDrag() {a.add(a); }


private:
	double time;
	Acceleration a;
	Angle angle;
	double inertia;
	Velocity v;
	Position  pos;
};
