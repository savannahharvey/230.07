/*************************************************************
 * 1. Name:
 *      The Key
 * 2. Assignment Name:
 *      Lab 08: M777 Howitzer
 * 3. Assignment Description:
 *      Simulate firing the M777 howitzer 15mm artillery piece
 * 4. What was the hardest part? Be as specific as possible.
 *      ??
 * 5. How long did it take for you to complete the assignment?
 *      ??
 *****************************************************************/

#include <cassert>      // for ASSERT
#include <math.h>       // for PI
#include "uiInteract.h" // for INTERFACE
#include "uiDraw.h"     // for RANDOM and DRAW*
#include "ground.h"     // for GROUND
#include "position.h"   // for POSITION
#include "bullet.h"     // for BULLET
#include "angle.h"      // for ANGLE

#include <vector>
#include <tuple>


#define initial_speed         827      // m/s
#define bullet_weight         46.7     // kg
#define bullet_diameter       154.89   // mm
#define bullet_radius         bullet_diameter * 0.5 * 0.001
#define bullet_surface_area   M_PI*bullet_radius*bullet_radius
#define air_den               0.6
#define drag_co               0.3

using namespace std;

/*************************************************************************
 * Demo
 * Test structure to capture the LM that will move around the screen
 *************************************************************************/
class Demo
{
public:
   Demo(const Position& ptUpperRight) :
      ptUpperRight(ptUpperRight),
      ground(ptUpperRight), bullet()
   {
      // Set the horizontal position of the howitzer. This should be random.
      // See uiDraw.h which has random() defined.
      ptHowitzer.setPixelsX(Position(ptUpperRight).getPixelsX() / 2.0);

      // Generate the ground and set the vertical position of the howitzer.
      ground.reset(ptHowitzer);

      

      // This is to make the bullet travel across the screen. Notice how there are 
      // 20 pixels, each with a different age. This gives the appearance
      // of a trail that fades off in the distance.



      //set initial 
      double degrees;
      cout << "What is the angle of the howitzer where 0 is up?  ";
      cin  >> degrees;
      
      //bullet.setStartPos(ptHowitzer);
      bullet.setPos(0,0);

      angle.setDegrees(degrees);
      bullet.setDegrees(angle.getDegrees());
      bullet.setVelocity(angle, initial_speed);
      
      t = 0.01;
      double altitude;
      vector <pair<int, double>> gravityTable = {
            {0   ,   9.807},
            {1000,	9.804},
            {2000,	9.801},
            {3000,	9.797},
            {4000,	9.794},
            {5000,	9.791},
            {6000,	9.788},
            {7000,	9.785},
            {8000,	9.782},
            {9000,	9.779},
            {10000,	9.776},
            {15000,	9.761},
            {20000,	9.745},
            {25000,	9.730},
            {30000,	9.715},
            {40000,	9.684},
            {50000,	9.654},
            {60000,	9.624},
            {70000,	9.594},
            {80000,	9.564} };

      vector<pair<int, double>> airDensityTable = {
           {0, 1.2250000},
           {1000, 1.1120000},
           {2000, 1.0070000},
           {3000, 0.9093000},
           {4000, 0.8194000},
           {5000, 0.7364000},
           {6000, 0.6601000},
           {7000, 0.5900000},
           {8000, 0.5258000},
           {9000, 0.4671000},
           {10000, 0.4135000},
           {15000, 0.1948000},
           {20000, 0.0889100},
           {25000, 0.0400800},
           {30000, 0.0184100},
           {40000, 0.0039960},
           {50000, 0.0010270},
           {60000, 0.0003097},
           {70000, 0.0000828},
           {80000, 0.0000185}
      };

      vector<pair<double, double>> dragCoefficientTable = {
        {0.300, 0.1629},
        {0.500, 0.1659},
        {0.700, 0.2031},
        {0.890, 0.2597},
        {0.920, 0.3010},
        {0.960, 0.3287},
        {0.980, 0.4002},
        {1.000, 0.4258},
        {1.020, 0.4335},
        {1.060, 0.4483},
        {1.240, 0.4064},
        {1.530, 0.3663},
        {1.990, 0.2897},
        {2.870, 0.2297},
        {2.890, 0.2306},
        {5.000, 0.2656}
      };

      vector<pair<int, int>> speedOfSound = {
        {0, 340},
        {1000, 336},
        {2000, 332},
        {3000, 328},
        {4000, 324},
        {5000, 320},
        {6000, 316},
        {7000, 312},
        {8000, 308},
        {9000, 303},
        {10000, 299},
        {15000, 295},
        {20000, 295},
        {25000, 295},
        {30000, 305},
        {40000, 324},
        {50000, 337},
        {60000, 319},
        {70000, 289},
        {80000, 269}
      };


      hangTime = 0.0;
      Acceleration gravity;
      do {
         altitude = bullet.getYPosition();
         currentGravity = bullet.interpolation(altitude, gravityTable);
         gravity.setDDY(-currentGravity);
         currentAirDensity = bullet.interpolation(altitude, airDensityTable);
         currentDragCoefficient = bullet.interpolation(altitude, dragCoefficientTable);   //NOT altitude

         bullet.setDrag( currentAirDensity, bullet_surface_area, bullet_weight);
         bullet.addAcceleration(gravity);

         bullet.travel(t);

         
         hangTime += 0.01;
      }
      while (bullet.getYPosition() > 0.0);
      
        
      for (int i = 0; i < 20; i++)
      {
         // interia
         projectilePath[i].setPixelsX((double)i * 2.0);
         projectilePath[i].setPixelsY(ptUpperRight.getPixelsY() / 1.5);
      }
      cout << "Distance:      " << bullet.getXPosition() << "m   "
           << "Altitude:      " << bullet.getYPosition() << "m   "
           << "Hang Time:     " << hangTime              << "s"
           << endl;
   }



   Ground ground;                 // the ground, described in ground.h
   Position  projectilePath[20];  // path of the projectile, described in position.h
   Position  ptHowitzer;          // location of the howitzer
   Position  ptUpperRight;        // size of the screen
   Angle angle;                   // angle of the howitzer, in radians 
   double time;                   // amount of time since the last firing, in seconds
   Bullet bullet;                 // information of the projectile fired
   double t;
   double hangTime;               // time bullet is in air
   double currentGravity;
   double currentAirDensity;
   double currentDragCoefficient;
};



/*************************************
 * All the interesting work happens here, when
 * I get called back from OpenGL to draw a frame.
 * When I am finished drawing, then the graphics
 * engine will wait until the proper amount of
 * time has passed and put the drawing on the screen.
 **************************************/
void callBack(const Interface* pUI, void* p)
{
   // the first step is to cast the void pointer into a game object. This
   // is the first step of every single callback function in OpenGL. 
   Demo* pDemo = (Demo*)p;

   //
   // accept input
   //

   // move a large amount
   if (pUI->isRight())
      pDemo->angle.add(0.05);
   if (pUI->isLeft())
      pDemo->angle.add(- 0.05);

   // move by a little
   if (pUI->isUp())
      pDemo->angle.add(pDemo->angle.getRadians() >= 0 ? -0.003 : 0.003);
   if (pUI->isDown())
      pDemo->angle.add(pDemo->angle.getRadians() >= 0 ? 0.003 : -0.003);

   // fire that gun
   if (pUI->isSpace())
      pDemo->time = 0.0;

   //
   // perform all the game logic
   //

   // advance time by half a second.
   pDemo->time += 0.5;

   // move the projectile across the screen
   for (int i = 0; i < 20; i++)
   {
      // this bullet is moving left at 1 pixel per frame
      double x = pDemo->projectilePath[i].getPixelsX();
      x -= 1.0;
      if (x < 0)
         x = pDemo->ptUpperRight.getPixelsX();
      pDemo->projectilePath[i].setPixelsX(x);
   }

   //
   // draw everything
   //

   ogstream gout(Position(10.0, pDemo->ptUpperRight.getPixelsY() - 20.0));

   // draw the ground first
   pDemo->ground.draw(gout);

   // draw the howitzer
   gout.drawHowitzer(pDemo->ptHowitzer, pDemo->angle.getRadians(), pDemo->time);

   // draw the projectile
   for (int i = 0; i < 20; i++)
      gout.drawProjectile(pDemo->projectilePath[i], 0.5 * (double)i);

   // draw some text on the screen
   gout.setf(ios::fixed | ios::showpoint);
   gout.precision(1);
   gout << "Time since the bullet was fired: "
        << pDemo->time << "s\n";
}

double Position::metersFromPixels = 40.0;

/*********************************
 * Initialize the simulation and set it in motion
 *********************************/
#ifdef _WIN32_X
#include <windows.h>
int WINAPI wWinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ PWSTR pCmdLine,
   _In_ int nCmdShow)
#else // !_WIN32
int main(int argc, char** argv)
#endif // !_WIN32
{
   // Initialize OpenGL
   Position ptUpperRight;
   ptUpperRight.setPixelsX(700.0);
   ptUpperRight.setPixelsY(500.0);
   Position().setZoom(40.0 /* 42 meters equals 1 pixel */);
   Interface ui(0, NULL,
      "Demo",   /* name on the window */
      ptUpperRight);

   // Initialize the demo
   Demo demo(ptUpperRight);

   // set everything into action
   ui.run(callBack, &demo);


   return 0;
}
