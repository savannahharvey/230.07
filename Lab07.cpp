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
#include <cmath>
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
      Acceleration a;
      double gravity;
      cout << "What is the angle of the howitzer where 0 is up?  ";
      cin  >> degrees;
      
      //bullet.setStartPos(ptHowitzer);
      bullet.setPos(0,0);

      angle.setDegrees(degrees);
      bullet.setDegrees(angle.getDegrees());
      bullet.setVelocity(angle, initial_speed);
      gravity = 9.807;
      bullet.setDDY(gravity);
      
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
      hangTime = 0.0;
      double v;
      double dragForce;
      double dAccel;
      double dragAcceleration;
      double dragAngle;
      double dragDDX;
      double dragDDY;
      double DDX;
      double DDY;
      do {
         a.setDDX(0.0);
         a.setDDY(0.0);
         altitude = bullet.getYPosition();
         gravity = bullet.interpolation(altitude, gravityTable);
         bullet.setDDY(-gravity);
         //bullet.setDDY(-gravity + bullet.getDDY());
         //bullet.setDDX(bullet.getDDX());
         v = bullet.getSpeed();
         bullet.setDrag( air_den, v, bullet_surface_area, bullet_weight);
         cout << "DDX of Drag is " << bullet.getDragDDX() << endl;
         cout << "DDY of Drag is " << bullet.getDragDDY() << endl;
         
         cout << "DDX of bullet is " << bullet.getDDX() << endl;
         cout << "DDY of bullet is " << bullet.getDDY() << endl;
         bullet.addDrag();
         cout << "after Drag" << endl;
         cout << "DDX of bullet is " << bullet.getDDX() << endl;
         cout << "DDY of bullet is " << bullet.getDDY() << endl;
         a = bullet.getAccleration();

         bullet.travel(a, t);
         cout << "after travel" << endl;
         cout << "DDX of bullet is " << bullet.getDDX() << endl;
         cout << "DDY of bullet is " << bullet.getDDY() << endl;
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
};




vector <pair<int, double>> getGravityTable()
{
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

   return gravityTable;

}



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
