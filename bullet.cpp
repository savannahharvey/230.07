/***********************************************************************
 * Header File:
 *    Bullet : The representation of the Bullet
 * Authors:
 *    Sam Evans
 *    Savannah Harvey
 * Summary:
 *    Everything we need to know about the bullet
 *		Also Usain bullet
 ************************************************************************/

 #include "bullet.h"
 #include <cmath>
 #include <math.h>

 /******************************************
 * BULLET  applyDrag
 *  Applies drag to the bullet's acceleration   
 *    Calculates the drag force in newtons (N)
 *    then converts it into drag acceleration
 *    next it adds the drag acceleration to bullet''s acceleration
 * 
 * d = ½ c ρ v^2 a
 *    c = drag coefficient
 *    ρ = density of the fluid/gas
 *    v = velocity of the projectile
 *    a = surface area
 *****************************************/

void Bullet::applyDrag(double c, double p, double area, double weight)
{
   double v = getSpeed();
   double drag_force = 0.5 * c * p * v * v * area;
   double drag_accel = drag_force / weight;

   double currentAngle = atan2(getDX(), getDY());
   angle.setRadians(currentAngle);
   angle.reverse();
   a.set(angle, drag_accel);
   return;

}


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
void Bullet::travel(double t)
{
   // update position
   pos.addMetersX(v.getDX() * t + 0.5 * a.getDDX() * t * t);
   pos.addMetersY(v.getDY() * t + 0.5 * a.getDDY() * t * t);
   //compute speed
   v.add(a, t);
}


/******************************************
 * Bullet  linearInterpolation
 * math Magic
 *    finds a value for either gravity or air denesity
 *    given any altitude
 * (r - r0)/(d - d0) = (r1 - r0) / (d1 - d0)
 *
 *    (r,d) (r0,d0) (r1,d1)
 *
 *    r  = The value we are calculating
 *    r0 = The value given the altitude below
 *    r1 = The value given the altitude above
 *
 *    d  = The altitude for the value we want to calculate
 *       **For this value we will be using bulletgetYPosition()
 *    d0 = The altitude for the value below
 *    d1 = The altitude for the value above
 *****************************************/

double Bullet::interpolation(const double altitude, 
                             vector <pair<double, double>> table)
{
   double r0 = 4.0;
   double r1;
   double d0;
   double d1;
   int i = 0;

   while (i < table.size() - 1)
   {
      if (altitude < table[i + 1].first && altitude >= table[i].first)
      {
         r0 = table[i].second;
         r1 = table[i + 1].second;
         d0 = table[i].first;
         d1 = table[i + 1].first;
         return ((r1 - r0) / (d1 - d0)) * (altitude - d0) + r0;
      }
      i++;
   }
   return table[i].second;

}



