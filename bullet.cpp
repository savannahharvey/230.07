/***********************************************************************
 * Source File:
 *    bullet : The representation the bullet
 * Author:
 *    Sam Evans
 * Summary:
 *    Everything we need to know about a bullet on the screen.
 ************************************************************************/

 #include "bullet.h"

 /******************************************
 * BULLET  drag
 * sets the drag force in newtons (N)
 * d = ½ c ρ v^2 a
 *    c = drag coefficient
 *    ρ = density of the fluid/gas
 *    v = velocity of the projectile
 *    a = surface area
 *****************************************/
void Bullet::setDrag(double c, double p, double v, double a)
{
   drag = 0.5 * c * p * v * v * a;
}; 

/******************************************
 * BULLET  travels
 * changes the bullet's position according to 
 * d = ½ c ρ v^2 a
 *    s = distance in meters (m)
 *    s0 = initial distance (m)
 *    v = velocity (m/s)
 *    t = time (s)
 *    a = acceleration (m/s2)
 *****************************************/
void Bullet::travel(Acceleration& a, double t)
{
   // update position
   pos.addMetersX(v.getDX() * t + 0.5 * a.getDDX() * t * t);
   pos.addMetersY(v.getDY() * t + 0.5 * a.getDDY() * t * t);
   //compute speed
   v.add(a, t);
}

void Bullet::setStartPos(Position& ptHowitzer)
{
   pos.setMeters(ptHowitzer.getMetersX(), ptHowitzer.getMetersY());
}

