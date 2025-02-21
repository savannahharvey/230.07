/***********************************************************************
 * Source File:
 *    bullet : The representation the bullet
 * Author:
 *    Sam Evans
 * Summary:
 *    Everything we need to know about a bullet on the screen.
 ************************************************************************/

 #include "bullet.h"
 #include <cmath>

 /******************************************
 * BULLET  drag
 * sets the drag force in newtons (N)
 * d = ½ c ρ v^2 a
 *    c = drag coefficient
 *    ρ = density of the fluid/gas
 *    v = velocity of the projectile
 *    a = surface area
 *****************************************/

void Bullet::setDrag(double p, double v, double area, double weight)
{
   double drag_force = 0.5 * 0.3 * p * v * v * area;
   double drag_accel = drag_force / weight;

   double currentAngle = atan2(getDX(), getDY())+M_PI;
   angle.setRadians(currentAngle);
   cout << "current angle: " << currentAngle << endl;

   dragDDX = -1 * drag_accel * sin(currentAngle);
   //a.addDDX(dragDDX);

   dragDDY = -1 * drag_accel * cos(currentAngle); // Vertical drag
   //a.addDDY(dragDDY);
}

double Bullet::getDragDDX()
{
   return dragDDX;
}

double Bullet::getDragDDY()
{
   return dragDDY;
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

 //double r0, double r1, double d0, double d1,
double Bullet::interpolation(const double altitude, vector <pair<int, double>> table)
{
   double r0 = 4.0;
   double r1;
   double d0;
   double d1;
   int i = 0;

   while (i < 19)
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
   if (altitude > 80000)
   {
      cout << "altitude super high!" << endl;
      return 9.564;
   }
   }




