/*
 * 3D Bedroom Graphics Project
 * Built by: Ibrahim Rafi
 * Course: Computer Graphics
 */

#include <GL/freeglut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <cstdint>
#include <iostream>

#ifndef RES
#define RES 1
#endif

static double Sin(double deg) { return std::sin(deg * M_PI / 180.0); }
static double Cos(double deg) { return std::cos(deg * M_PI / 180.0); }

static void Fatal(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    std::vfprintf(stderr, format, args);
    va_end(args);
    std::exit(EXIT_FAILURE);
}

static void ErrCheck(const char* where)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        std::fprintf(stderr, "OpenGL error at %s: 0x%04X\n", where, err);
}

static void Print(const char* format, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, format);
    std::vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);

    for (const char* p = buf; *p; ++p)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *p);
}

static void Project(double fov, double asp, double dim)
{
    glMatrixMode(GL_PROJECTION); // Select whether subsequent matrix operations affect projection or model/view transforms.
    glLoadIdentity(); // Reset the current transformation matrix to identity.
    if (fov > 0)
        gluPerspective(fov, asp, dim / 16.0, 16.0 * dim); // Perspective projection: creates the depth-based 3D camera view.
    else
        glOrtho(-dim * asp, dim * asp, -dim, dim, -dim, dim); // Orthographic projection: creates a non-perspective 3D view.
    glMatrixMode(GL_MODELVIEW); // Select whether subsequent matrix operations affect projection or model/view transforms.
    glLoadIdentity(); // Reset the current transformation matrix to identity.
}








int th=-45;
int ph=30;
int axes=0;
int mode=0;
int move=1;
int fov=35;
double asp=1;
double dim=3.6;

int light     =   1;
int one       =   1;
int distance  =   5;
int inc       =  10;
int smooth    =   1;
int local     =   0;
int emission  =   0;
int ambient   =  10;
int diffuse   =  50;
int specular  =   0;
int shininess =   0;
float shiny   =   1;
int zh        =  90;
float ylight  =   2.5;
typedef struct {float x,y,z;} vtx;



static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);


   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}


static void ball(double x,double y,double z,double r)
{

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.

   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glScaled(r,r,r); // 3D scaling: resize the object along the X, Y and Z axes.

   float yellow[]   = {1.0,1.0,0.0,1.0};
   float Emission[] = {0.0,0.0,0.01*emission,1.0};
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

   for (int ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }

   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}

static void normal(float Ax, float Ay, float Az, float Bx, float By, float Bz,float Cx, float Cy, float Cz)
{

   float dx0 = Ax-Bx;
   float dy0 = Ay-By;
   float dz0 = Az-Bz;

   float dx1 = Cx-Ax;
   float dy1 = Cy-Ay;
   float dz1 = Cz-Az;

   float Nx = dy0*dz1 - dy1*dz0;
   float Ny = dz0*dx1 - dz1*dx0;
   float Nz = dx0*dy1 - dx1*dy0;
   glNormal3f(Nx,Ny,Nz);
}


static void triangle(vtx A,vtx B,vtx C)
{

   float dx0 = A.x-B.x;
   float dy0 = A.y-B.y;
   float dz0 = A.z-B.z;

   float dx1 = C.x-A.x;
   float dy1 = C.y-A.y;
   float dz1 = C.z-A.z;

   float Nx = dy0*dz1 - dy1*dz0;
   float Ny = dz0*dx1 - dz1*dx0;
   float Nz = dx0*dy1 - dx1*dy0;

   glNormal3f(Nx,Ny,Nz);
   glBegin(GL_TRIANGLES);
   glVertex3f(A.x,A.y,A.z);
   glVertex3f(B.x,B.y,B.z);
   glVertex3f(C.x,C.y,C.z);
   glEnd();
}


static void penguin(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double a, double b, double c)
{

   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.



   if (y == 0)
   {
      y = .71 * dy;
   }


   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(th,0,1,0); // 3D rotation: rotate the object around the specified axis.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.





   glBegin(GL_QUADS);

   glColor3f(1,1,1);
   normal(-0.5,0.5, 0,0.5,0.5, 0,0.5,0, 0.2);
   glVertex3f(-0.5,0.5, 0);
   glVertex3f(0.5,0.5, 0);
   glVertex3f(0.5,0, 0.2);
   glVertex3f(-0.5,0, 0.2);
   glEnd();


   glBegin(GL_QUADS);
   glColor3f(1,1,1);
   normal(-0.5,0, 0.2,0.5,0, 0.2,0.5,-0.7, 0);
   glVertex3f(-0.5,0, 0.2);
   glVertex3f(0.5,0, 0.2);
   glVertex3f(0.5,-0.7, 0);
   glVertex3f(-0.5,-0.7, 0);
   glEnd();


   glBegin(GL_QUADS);
   glColor3f(a,b,c);
   glNormal3f(-1,0,0);
   glVertex3f(-0.5,0.5, 0);
   glVertex3f(-0.5,-0.7, 0);
   glVertex3f(-0.5,-0.7, -0.5);
   glVertex3f(-0.5,0.5, -0.5);
   glEnd();



   glBegin(GL_QUADS);
   glColor3f(a,b,c);
   glNormal3f(0,0,-1);
   glVertex3f(0.5,0.5, -0.5);
   glVertex3f(-0.5,0.5, -0.5);
   glVertex3f(-0.5,-0.7, -0.5);
   glVertex3f(0.5,-0.7, -0.5);
   glEnd();



   glBegin(GL_QUADS);
   glColor3f(a,b,c);
   glNormal3f(1,0,0);
   glVertex3f(0.5,0.5, 0);
   glVertex3f(0.5,-0.7, 0);
   glVertex3f(0.5,-0.7, -0.5);
   glVertex3f(0.5,0.5, -0.5);




   glColor3f(a,b,c);
   normal(0.5,0.3,-.1,0.5,0.3, -0.4,0.7,-0.3,-.4);
   glVertex3f(0.5,0.3,-.1);
   glVertex3f(0.5,0.3, -0.4);
   glVertex3f(0.7,-0.3,-.4);
   glVertex3f(0.7,-0.3,-.1);


   double offset = 0.05;


   glColor3f(a,b,c);
   normal(0.5-offset,0.3-offset, -0.4,0.5-offset,0.3-offset,-.1,0.7-offset,-0.3-offset,-.1);
   glVertex3f(0.5-offset,0.3-offset, -0.4);
   glVertex3f(0.5-offset,0.3-offset,-.1);
   glVertex3f(0.7-offset,-0.3-offset,-.1);
   glVertex3f(0.7-offset,-0.3-offset,-.4);


   glColor3f(a,b,c);
   normal(0.5-offset,0.3-offset,-.1,0.5,0.3,-.1,0.7,-0.3,-.1);
   glVertex3f(0.5-offset,0.3-offset,-.1);
   glVertex3f(0.5,0.3,-.1);
   glVertex3f(0.7,-0.3,-.1);
   glVertex3f(0.7-offset,-0.3-offset,-.1);

   glColor3f(a,b,c);
   normal(0.5-offset,0.3-offset,-.4,0.7-offset,-0.3-offset,-.4,0.7,-0.3,-.4);
   glVertex3f(0.5-offset,0.3-offset,-.4);
   glVertex3f(0.7-offset,-0.3-offset,-.4);
   glVertex3f(0.7,-0.3,-.4);
   glVertex3f(0.5,0.3,-.4);


   normal(0.7,-0.3,-.1,0.7,-0.3,-.4,0.7-offset,-0.3-offset,-.4);
   glVertex3f(0.7,-0.3,-.1);
   glVertex3f(0.7,-0.3,-.4);
   glVertex3f(0.7-offset,-0.3-offset,-.4);
   glVertex3f(0.7-offset,-0.3-offset,-.1);







   glColor3f(a,b,c);
   normal(-0.5,0.3, -0.4,-0.5,0.3,-.1,-0.7,-0.3,-.1);
   glVertex3f(-0.5,0.3, -0.4);
   glVertex3f(-0.5,0.3,-.1);
   glVertex3f(-0.7,-0.3,-.1);
   glVertex3f(-0.7,-0.3,-.4);


   glColor3f(a,b,c);
   normal(-0.5+offset,0.3-offset,-.1,-0.5+offset,0.3-offset, -0.4,-0.7+offset,-0.3-offset,-.4);
   glVertex3f(-0.5+offset,0.3-offset,-.1);
   glVertex3f(-0.5+offset,0.3-offset, -0.4);
   glVertex3f(-0.7+offset,-0.3-offset,-.4);
   glVertex3f(-0.7+offset,-0.3-offset,-.1);



   glColor3f(a,b,c);
   normal(-0.7+offset,-0.3-offset,-.1,-0.7,-0.3,-.1,-0.5,0.3,-.1);
   glVertex3f(-0.7+offset,-0.3-offset,-.1);
   glVertex3f(-0.7,-0.3,-.1);
   glVertex3f(-0.5,0.3,-.1);
   glVertex3f(-0.5+offset,0.3-offset,-.1);


   glColor3f(a,b,c);
   normal(-0.5+offset,0.3-offset,-.4,-0.5,0.3,-.4,-0.7,-0.3,-.4);
   glVertex3f(-0.5+offset,0.3-offset,-.4);
   glVertex3f(-0.5,0.3,-.4);
   glVertex3f(-0.7,-0.3,-.4);
   glVertex3f(-0.7+offset,-0.3-offset,-.4);



   normal(-0.7,-0.3,-.4,-0.7,-0.3,-.1,-0.7+offset,-0.3-offset,-.1);
   glVertex3f(-0.7,-0.3,-.4);
   glVertex3f(-0.7,-0.3,-.1);
   glVertex3f(-0.7+offset,-0.3-offset,-.1);
   glVertex3f(-0.7+offset,-0.3-offset,-.4);




   glColor3f(a,b,c);
   glNormal3f(0,1,0);
   glVertex3f(-0.5,0.5, 0);
   glVertex3f(0.5,0.5, 0);
   glVertex3f(0.5,0.5, -0.5);
   glVertex3f(-0.5,0.5, -0.5);


   glColor3f(1,1,1);
   glNormal3f(0,-1,0);
   glVertex3f(0.5,-0.7, 0);
   glVertex3f(-0.5,-0.7, 0);
   glVertex3f(-0.5,-0.7, -0.5);
   glVertex3f(0.5,-0.7, -0.5);



   glColor3f(1,.647,0);

   glNormal3f(0,-1,0);
   glVertex3f(0.1,-0.7, 0);
   glVertex3f(0.4,-0.7, 0);
   glVertex3f(0.4,-0.7, 0.3);
   glVertex3f(0.1,-0.7, 0.3);


   glNormal3f(0,1,0);
   glVertex3f(0.1,-0.65, 0);
   glVertex3f(0.4,-0.65, 0);
   glVertex3f(0.4,-0.65, 0.3);
   glVertex3f(0.1,-0.65, 0.3);


   glNormal3f(0,0,1);
   glVertex3f(0.4,-0.65, 0.3);
   glVertex3f(0.4,-0.7, 0.3);
   glVertex3f(0.1,-0.7, 0.3);
   glVertex3f(0.1,-0.65, 0.3);


   glNormal3f(1,0,0);
   glVertex3f(0.4,-0.65, 0.3);
   glVertex3f(0.4,-0.7, 0.3);
   glVertex3f(0.4,-0.7, 0);
   glVertex3f(0.4,-0.65, 0);


   glNormal3f(-1,0,0);
   glVertex3f(0.1,-0.65, 0.3);
   glVertex3f(0.1,-0.7, 0.3);
   glVertex3f(0.1,-0.7, 0);
   glVertex3f(0.1,-0.65, 0);





   glNormal3f(0,-1,0);
   glVertex3f(-0.1,-0.7, 0);
   glVertex3f(-0.4,-0.7, 0);
   glVertex3f(-0.4,-0.7, 0.3);
   glVertex3f(-0.1,-0.7, 0.3);


   glNormal3f(0,1,0);
   glVertex3f(-0.1,-0.65, 0);
   glVertex3f(-0.4,-0.65, 0);
   glVertex3f(-0.4,-0.65, 0.3);
   glVertex3f(-0.1,-0.65, 0.3);


   glNormal3f(-1,0,0);
   glVertex3f(-0.4,-0.65, 0.3);
   glVertex3f(-0.4,-0.7, 0.3);
   glVertex3f(-0.4,-0.7, 0);
   glVertex3f(-0.4,-0.65, 0);


   glNormal3f(1,0,0);
   glVertex3f(-0.1,-0.65, 0.3);
   glVertex3f(-0.1,-0.7, 0.3);
   glVertex3f(-0.1,-0.7, 0);
   glVertex3f(-0.1,-0.65, 0);


   glNormal3f(0,0,1);
   glVertex3f(-0.4,-0.65, 0.3);
   glVertex3f(-0.4,-0.7, 0.3);
   glVertex3f(-0.1,-0.7, 0.3);
   glVertex3f(-0.1,-0.65, 0.3);

   glEnd();

   glColor3f(1,1,1);

   vtx A = {0.5,0,0.2};
   vtx B = {0.5,0.5,0};
   vtx C = {0.5,-0.7,0};
   triangle(A,B,C);


   glColor3f(1,1,1);
   vtx D = {-0.5,0, 0.2};
   vtx E = {-0.5,0.5, 0};
   vtx F = {-0.5,-0.7, 0};
   triangle(E,D,F);


   glColor3f(a,b,c);
   glTranslated(0,.7,-.15); // 3D translation: position the object along the X, Y and Z axes.
   glScaled(.7,.7,.7); // 3D scaling: resize the object along the X, Y and Z axes.
   for (int ph=-90;ph<90;ph+=15)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=15)
      {
         Vertex(th,ph);
         Vertex(th,ph+15);
      }
      glEnd();
   }


   glColor3f(1,.647,0);
   glScaled(6,6,6); // 3D scaling: resize the object along the X, Y and Z axes.

   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      normal(0.0, 0.0, 0.35,0.05*Cos(th+5),0.05*Sin(th+5),0,0.05*Cos(th),0.05*Sin(th),0);
      glVertex3d(0.0, 0.0, 0.3);
      glVertex3d(0.05*Cos(th+5),0.05*Sin(th+5),0);
      glVertex3d(0.05*Cos(th),0.05*Sin(th),0);
      glEnd();
   }



   glColor3f(0,0,0);
   glTranslated(.08,.06,.13); // 3D translation: position the object along the X, Y and Z axes.
   glScaled(.01,.01,.01); // 3D scaling: resize the object along the X, Y and Z axes.
   for (int ph=-90;ph<90;ph+=15)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=15)
      {
         Vertex(th,ph);
         Vertex(th,ph+15);
      }
      glEnd();
   }


   glColor3f(0,0,0);
   glTranslated(-15,0,0); // 3D translation: position the object along the X, Y and Z axes.
   glScaled(1.1,1.1,1.1); // 3D scaling: resize the object along the X, Y and Z axes.
   for (int ph=-90;ph<90;ph+=15)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=15)
      {
         Vertex(th,ph);
         Vertex(th,ph+15);
      }
      glEnd();
   }

   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}


static void door(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.


   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(th,0,1,0); // 3D rotation: rotate the object around the specified axis.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.
   glBegin(GL_QUADS);
   glColor3d(1,1,1);
   glNormal3d(0,0,1);
   glVertex3d(-1,4,0);
   glVertex3d(1,4,0);
   glVertex3d(1,0,0);
   glVertex3d(-1,0,0);

   glNormal3d(1,0,0);
   glVertex3d(1,0,0);
   glVertex3d(1,4,0);
   glVertex3d(1,4,-0.2);
   glVertex3d(1,0,-0.2);

   glNormal3d(-1,0,0);
   glVertex3d(-1,0,0);
   glVertex3d(-1,4,0);
   glVertex3d(-1,4,-0.2);
   glVertex3d(-1,0,-0.2);

   glNormal3d(0,0,-1);
   glVertex3d(-1,4,-0.2);
   glVertex3d(1,4,-0.2);
   glVertex3d(1,0,-0.2);
   glVertex3d(-1,0,-0.2);

   glNormal3d(0,1,0);
   glVertex3d(-1,4,0);
   glVertex3d(-1,4,-0.2);
   glVertex3d(1,4,-0.2);
   glVertex3d(1,4,0);

   glNormal3d(0,-1,0);
   glVertex3d(-1,0,0);
   glVertex3d(-1,0,-0.2);
   glVertex3d(1,0,-0.2);
   glVertex3d(1,0,0);


      glEnd();

   glColor3f(.2,.2,2);
   glTranslated(0.75,1.8,.1); // 3D translation: position the object along the X, Y and Z axes.
   glScaled(.1,.1,.1); // 3D scaling: resize the object along the X, Y and Z axes.
   glColor3f(.83,.69,.21);
   for (int ph=-90;ph<90;ph+=5)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=5)
      {
         Vertex(th,ph);
         Vertex(th,ph+15);
      }
      glEnd();
   }

   glTranslated(0,0,-4); // 3D translation: position the object along the X, Y and Z axes.
   for (int ph=-90;ph<90;ph+=5)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=5)
      {
         Vertex(th,ph);
         Vertex(th,ph+15);
      }
      glEnd();
   }
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.

}


static void sphere(double x, double y, double z, double dx, double dy, double dz)
{
   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.

   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.
   for (int ph=-90;ph<90;ph+=5)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=5)
      {
         Vertex(th,ph);
         Vertex(th,ph+15);
      }
      glEnd();
   }

   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}

static void sideTableDisplay(double x,double y,double z,double s)
{
   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.
   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glScaled(s,s,s); // 3D scaling: resize the object along the X, Y and Z axes.

   glColor3d(.03,.03,.03);
   glBegin(GL_QUADS);
   glVertex3d(-.5,0,-.28);
   glVertex3d(-.5,0,.28);
   glVertex3d(.5,0,.28);
   glVertex3d(.5,0,-.28);
   glVertex3d(-.5,-.08,-.28);
   glVertex3d(.5,-.08,-.28);
   glVertex3d(.5,-.08,.28);
   glVertex3d(-.5,-.08,.28);
   glEnd();

   glColor3d(.02,.02,.025);
   glBegin(GL_QUADS);
   glVertex3d(-.42,.05,.03);
   glVertex3d(-.42,.48,.03);
   glVertex3d(.42,.48,.03);
   glVertex3d(.42,.05,.03);
   glEnd();

   glColor3d(.06,.22,.32);
   glBegin(GL_QUADS);
   glVertex3d(-.35,.12,.035);
   glVertex3d(-.35,.41,.035);
   glVertex3d(.35,.41,.035);
   glVertex3d(.35,.12,.035);
   glEnd();

   glColor3d(.12,.12,.12);
   glBegin(GL_QUADS);
   glVertex3d(-.07,.05,.03);
   glVertex3d(.07,.05,.03);
   glVertex3d(.15,-.03,.03);
   glVertex3d(-.15,-.03,.03);
   glEnd();
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}

static void plant(double x,double y,double z,double s)
{
   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.
   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glScaled(s,s,s); // 3D scaling: resize the object along the X, Y and Z axes.

   glColor3d(.45,.20,.08);
   glBegin(GL_QUADS);
   glVertex3d(-.22,0,-.22); glVertex3d(-.16,.30,-.16); glVertex3d(.16,.30,-.16); glVertex3d(.22,0,-.22);
   glVertex3d(.22,0,-.22); glVertex3d(.16,.30,-.16); glVertex3d(.16,.30,.16); glVertex3d(.22,0,.22);
   glVertex3d(.22,0,.22); glVertex3d(.16,.30,.16); glVertex3d(-.16,.30,.16); glVertex3d(-.22,0,.22);
   glVertex3d(-.22,0,.22); glVertex3d(-.16,.30,.16); glVertex3d(-.16,.30,-.16); glVertex3d(-.22,0,-.22);
   glEnd();

   glColor3d(.06,.40,.12);
   for (int leaf=0; leaf<8; ++leaf)
   {
      double angle=leaf*45.0;
      glBegin(GL_TRIANGLES);
      glVertex3d(0,.25,0);
      glVertex3d(.42*Cos(angle),.68,.42*Sin(angle));
      glVertex3d(.23*Cos(angle+28),.42,.23*Sin(angle+28));
      glEnd();
   }
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}

static void wallArt(double x,double y,double z,double s)
{
   glColor3d(.10,.07,.04);
   glBegin(GL_QUADS);
   glVertex3d(x,y-.52*s,z-.72*s);
   glVertex3d(x,y+.52*s,z-.72*s);
   glVertex3d(x,y+.52*s,z+.72*s);
   glVertex3d(x,y-.52*s,z+.72*s);
   glEnd();

   glColor3d(.10,.30,.52);
   glBegin(GL_QUADS);
   glVertex3d(x-.006,y-.42*s,z-.62*s);
   glVertex3d(x-.006,y+.42*s,z-.62*s);
   glVertex3d(x-.006,y+.42*s,z+.62*s);
   glVertex3d(x-.006,y-.42*s,z+.62*s);
   glEnd();

   glColor3d(.95,.62,.12);
   glBegin(GL_TRIANGLES);
   glVertex3d(x-.012,y-.34*s,z-.45*s);
   glVertex3d(x-.012,y+.30*s,z);
   glVertex3d(x-.012,y-.34*s,z+.45*s);
   glEnd();
}


static void cuboid(double x,double y,double z, double dx,double dy,double dz,double th)
{

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.


   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(th,0,1,0); // 3D rotation: rotate the object around the specified axis.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.
   glBegin(GL_QUADS);


   glNormal3d(0,0,1);
   glVertex3d(0,1,0);
   glVertex3d(1,1,0);
   glVertex3d(1,0,0);
   glVertex3d(0,0,0);


   glNormal3d(1,0,0);
   glVertex3d(1,0,0);
   glVertex3d(1,1,0);
   glVertex3d(1,1,-1);
   glVertex3d(1,0,-1);


   glNormal3d(-1,0,0);
   glVertex3d(0,0,0);
   glVertex3d(0,1,0);
   glVertex3d(0,1,-1);
   glVertex3d(0,0,-1);


   glNormal3d(0,0,-1);
   glVertex3d(0,1,-1);
   glVertex3d(1,1,-1);
   glVertex3d(1,0,-1);
   glVertex3d(0,0,-1);


   glNormal3d(0,1,0);
   glVertex3d(0,1,0);
   glVertex3d(0,1,-1);
   glVertex3d(1,1,-1);
   glVertex3d(1,1,0);


   glNormal3d(0,-1,0);
   glVertex3d(0,0,0);
   glVertex3d(0,0,-1);
   glVertex3d(1,0,-1);
   glVertex3d(1,0,0);


   glEnd();



   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.

}

// Modern horizontal wall light.  Its local front faces toward +Z, so it can sit
// directly in front of the room's rear wall without needing a texture.
static void drawWallLight(float x,float y,float z,float height)
{
   // Keep the supplied point as the fixture centre: the former vertical span
   // becomes the horizontal span, producing a long modern wall bar.
   const float width = height;
   const float lampHeight = 0.22f;
   const float depth = 0.060f;
   const float border = 0.026f;
   const float panelDepth = 0.014f;
   const float bottom = height*0.5f - lampHeight*0.5f;
   const GLfloat noEmission[] = {0.0f,0.0f,0.0f,1.0f};
   const GLfloat warmEmission[] = {0.62f,0.45f,0.20f,1.0f};

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.
   glTranslatef(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotatef(0.0f,0.0f,1.0f,0.0f); // 3D rotation: rotate the object around the specified axis.
   glScalef(1.0f,1.0f,1.0f); // 3D scaling: resize the object along the X, Y and Z axes.

   // Black-metal backplate and thin raised border around the diffuser.
   glColor3f(0.045f,0.050f,0.060f);
   cuboid(-width*0.5f,bottom,0.0f,width,lampHeight,depth,0);
   glColor3f(0.16f,0.17f,0.19f);
   cuboid(-width*0.5f-border,bottom+border,0.008f,
          width+2.0f*border,lampHeight-2.0f*border,0.020f,0);

   // The luminous inset panel is deliberately slightly in front of the frame.
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,warmEmission);
   glColor3f(1.0f,0.86f,0.60f);
   cuboid(-width*0.5f+border,bottom+0.050f,0.035f,
          width-2.0f*border,lampHeight-0.10f,panelDepth,0);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,noEmission);

   // Two small standoffs visually connect the fixture to the wall.
   glColor3f(0.10f,0.11f,0.13f);
   cuboid(-width*0.36f,bottom+0.075f,0.045f,0.070f,0.045f,0.045f,0);
   cuboid( width*0.36f-0.070f,bottom+0.075f,0.045f,0.070f,0.045f,0.045f,0);
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}

// Low modern TV console.  The origin is centred at the front edge on the floor.
static void drawTVConsole(float x,float y,float z,float width,float depth,float angle)
{
   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.
   glTranslatef(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotatef(angle,0.0f,1.0f,0.0f); // 3D rotation: rotate the object around the specified axis.

   // Four short legs lift the cabinet just above the floor.
   glColor3f(0.07f,0.08f,0.10f);
   cuboid(-width*0.44f,0.0f,0.00f,0.075f,0.13f,0.075f,0);
   cuboid( width*0.44f-0.075f,0.0f,0.00f,0.075f,0.13f,0.075f,0);
   cuboid(-width*0.44f,0.0f,-depth+0.075f,0.075f,0.13f,0.075f,0);
   cuboid( width*0.44f-0.075f,0.0f,-depth+0.075f,0.075f,0.13f,0.075f,0);

   // Main lower storage cabinet and its slightly projecting tabletop.
   glColor3f(0.16f,0.18f,0.21f);
   cuboid(-width*0.5f,0.13f,0.0f,width,0.31f,depth,0);
   glColor3f(0.05f,0.06f,0.075f);
   cuboid(-width*0.53f,0.44f,0.025f,width*1.06f,0.055f,depth+0.05f,0);

   // Two inset drawer fronts and small pull details make the cabinet read as storage.
   glColor3f(0.105f,0.12f,0.15f);
   cuboid(-width*0.46f,0.19f,0.012f,width*0.43f,0.19f,0.018f,0);
   cuboid( width*0.03f,0.19f,0.012f,width*0.43f,0.19f,0.018f,0);
   glColor3f(0.42f,0.44f,0.46f);
   cuboid(-width*0.27f,0.275f,0.033f,width*0.10f,0.018f,0.012f,0);
   cuboid( width*0.17f,0.275f,0.033f,width*0.10f,0.018f,0.012f,0);
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}

// Flat-screen TV with a narrow frame, reflective dark screen, and centre stand.
static void drawTV(float x,float y,float z,float width,float height,float angle)
{
   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.
   glTranslatef(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotatef(angle,0.0f,1.0f,0.0f); // 3D rotation: rotate the object around the specified axis.

   // Base and neck connect the display firmly to the console top.
   glColor3f(0.045f,0.050f,0.060f);
   cuboid(-0.24f,0.0f,0.050f,0.48f,0.045f,0.17f,0);
   cuboid(-0.045f,0.040f,-0.020f,0.090f,0.15f,0.065f,0);

   // Thin black housing, with the screen mounted just ahead of it.
   const float frame = 0.035f;
   glColor3f(0.025f,0.028f,0.035f);
   cuboid(-width*0.5f,0.18f,0.0f,width,height,0.065f,0);
   glColor3f(0.055f,0.105f,0.145f);
   cuboid(-width*0.5f+frame,0.18f+frame,0.010f,
          width-2.0f*frame,height-2.0f*frame,0.012f,0);

   // A dim blue-grey band gives the otherwise dark display a subtle reflection.
   glColor3f(0.13f,0.22f,0.28f);
   cuboid(-width*0.5f+frame*1.5f,0.18f+height*0.62f,0.024f,
          width-3.0f*frame,0.035f,0.006f,0);
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}

// Draw a hardcover book whose origin is the front-left corner of its bottom cover.
// The separate cover and page blocks give the book a visible binding and page edge.
static void drawClosedBook(float x,float y,float z,float width,float height,float depth,
                           float red,float green,float blue,float angle)
{
   const float cover = 0.022f;
   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.
   glTranslatef(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotatef(angle,0.0f,1.0f,0.0f); // 3D rotation: rotate the object around the specified axis.
   glScalef(1.0f,1.0f,1.0f); // 3D scaling: resize the object along the X, Y and Z axes.

   // Covers extend just beyond the page block on every exposed edge.
   glColor3f(red,green,blue);
   cuboid(-0.025f,0.0f,0.025f,width+0.05f,cover,depth+0.05f,0);
   cuboid(-0.025f,height-cover,0.025f,width+0.05f,cover,depth+0.05f,0);

   // The warm, inset block makes the paper pages visible around the cover.
   glColor3f(0.94f,0.90f,0.76f);
   cuboid(0.020f,cover,-0.020f,width-0.040f,height-2.0f*cover,depth-0.040f,0);

   // A darker narrow strip suggests the bound spine along the front edge.
   glColor3f(red*0.55f,green*0.55f,blue*0.55f);
   cuboid(-0.025f,0.0f,0.025f,width+0.05f,height,0.035f,0);
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}

// Draw one sloped, slightly thick sheet for the left or right half of an open book.
static void drawOpenPageHalf(float outerX,float outerY,float centerY,float depth,bool left)
{
   const float pageThickness = 0.018f;
   const float centerX = 0.0f;
   const float zFront = 0.0f;
   const float zBack = -depth;
   const float bottomOuterY = outerY-pageThickness;
   const float bottomCenterY = centerY-pageThickness;

   glBegin(GL_QUADS);
   // Top and bottom surfaces
   glNormal3f(left ? -0.45f : 0.45f,0.90f,0.0f);
   glVertex3f(outerX,outerY,zFront); glVertex3f(outerX,outerY,zBack);
   glVertex3f(centerX,centerY,zBack); glVertex3f(centerX,centerY,zFront);
   glNormal3f(0.0f,-1.0f,0.0f);
   glVertex3f(outerX,bottomOuterY,zFront); glVertex3f(centerX,bottomCenterY,zFront);
   glVertex3f(centerX,bottomCenterY,zBack); glVertex3f(outerX,bottomOuterY,zBack);
   // Outer, center, front, and rear edges make the page visibly thick.
   glNormal3f(left ? -1.0f : 1.0f,0.0f,0.0f);
   glVertex3f(outerX,outerY,zFront); glVertex3f(outerX,bottomOuterY,zFront);
   glVertex3f(outerX,bottomOuterY,zBack); glVertex3f(outerX,outerY,zBack);
   glNormal3f(left ? 1.0f : -1.0f,0.0f,0.0f);
   glVertex3f(centerX,centerY,zFront); glVertex3f(centerX,centerY,zBack);
   glVertex3f(centerX,bottomCenterY,zBack); glVertex3f(centerX,bottomCenterY,zFront);
   glNormal3f(0.0f,0.0f,1.0f);
   glVertex3f(outerX,outerY,zFront); glVertex3f(centerX,centerY,zFront);
   glVertex3f(centerX,bottomCenterY,zFront); glVertex3f(outerX,bottomOuterY,zFront);
   glNormal3f(0.0f,0.0f,-1.0f);
   glVertex3f(outerX,outerY,zBack); glVertex3f(outerX,bottomOuterY,zBack);
   glVertex3f(centerX,bottomCenterY,zBack); glVertex3f(centerX,centerY,zBack);
   glEnd();
}

// Draw an open book with two raised page halves, a dark cover, and a center spine.
static void drawOpenBook(float x,float y,float z,float width,float depth,float angle)
{
   const float coverThickness = 0.022f;
   const float pageRise = 0.105f;
   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.
   glTranslatef(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotatef(angle,0.0f,1.0f,0.0f); // 3D rotation: rotate the object around the specified axis.
   glScalef(1.0f,1.0f,1.0f); // 3D scaling: resize the object along the X, Y and Z axes.

   // Two dark cover boards stay flat below the open pages.
   glColor3f(0.13f,0.08f,0.06f);
   cuboid(-width*0.5f,0.0f,0.0f,width*0.5f,coverThickness,depth,0);
   cuboid(0.0f,0.0f,0.0f,width*0.5f,coverThickness,depth,0);

   glColor3f(0.97f,0.94f,0.82f);
   drawOpenPageHalf(-width*0.5f,coverThickness+0.008f,pageRise,depth,true);
   glColor3f(0.93f,0.90f,0.78f);
   drawOpenPageHalf( width*0.5f,coverThickness+0.008f,pageRise,depth,false);

   // The upright center strip remains visible between the two page halves.
   glColor3f(0.26f,0.15f,0.10f);
   cuboid(-0.018f,coverThickness,-0.002f,0.036f,pageRise,depth+0.004f,0);
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}



static void dresser(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.


   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(th,0,1,0); // 3D rotation: rotate the object around the specified axis.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.
   double cx = .63;
   double cy = .50;
   double cz = .48;

   double kx = .83;
   double ky = .69;
   double kz = .21;


   glColor3d(cx,cy,cz);
   cuboid(x,y,z,2,3,1,0);


   cuboid(x+.15,y+2.3,z+.15,1.7,.5,.25,0);
   cuboid(x+.15,y+1.6,z+.15,1.7,.5,.25,0);
   cuboid(x+.15,y+0.9,z+.15,1.7,.5,.25,0);
   cuboid(x+.15,y+0.2,z+.15,1.7,.5,.25,0);
   cuboid(x-.05,y+3,z+.15,2.1,.1,1.3,0);


   glColor3f(kx,ky,kz);
   sphere(x+.15+.25,y+2.55,z+.19,.05,.05,.05);
   sphere(x+.15+1.45,y+2.55,z+.19,.05,.05,.05);

   sphere(x+.15+.25,y+1.85,z+.19,.05,.05,.05);
   sphere(x+.15+1.45,y+1.85,z+.19,.05,.05,.05);

   sphere(x+.15+.25,y+1.15,z+.19,.05,.05,.05);
   sphere(x+.15+1.45,y+1.15,z+.19,.05,.05,.05);

   sphere(x+.15+.25,y+0.45,z+.19,.05,.05,.05);
   sphere(x+.15+1.45,y+0.45,z+.19,.05,.05,.05);
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.

}


static void nightstand(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.


   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(th,0,1,0); // 3D rotation: rotate the object around the specified axis.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.
   double cx = .43;
   double cy = .30;
   double cz = .28;

   double kx = .83;
   double ky = .69;
   double kz = .21;


   glColor3d(cx,cy,cz);
   cuboid(x,y,z,1,1.6,1,th);
   cuboid(x-.05,y+1.6,z+.15,1.1,.1,1.3,0);


   cuboid(x+.15,y+0.9,z+.15,0.7,.5,.25,0);
   cuboid(x+.15,y+0.2,z+.15,0.7,.5,.25,0);


   glColor3d(kx,ky,kz);
   sphere(x+.5,y+1.15,z+.19,.05,.05,.05);

   sphere(x+.5,y+0.45,z+.19,.05,.05,.05);
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.

}


static void pillow(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th
                 )
{

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.


   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(th,0,0,1); // 3D rotation: rotate the object around the specified axis.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.
   glScaled(.5,4,1); // 3D scaling: resize the object along the X, Y and Z axes.
   glColor3d(.8,.8,.8);


   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(.2*Cos(th),.2*Sin(th),0);
      glVertex3d(.2*Cos(th),.2*Sin(th),0);
      glVertex3d(.2*Cos(th),.2*Sin(th),2.5);
      glNormal3f(.2*Cos(th+5),.2*Sin(th+5),0);
      glVertex3d(.2*Cos(th+5),.2*Sin(th+5),2.5);
      glVertex3d(.2*Cos(th+5),.2*Sin(th+5),0);
      glEnd();
   }


   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(.2*Cos(th),.2*Sin(th),0);
      glVertex3d(.2*Cos(th),.2*Sin(th),0);
      glNormal3f(.2*Cos(th+5),.2*Sin(th+5),0);
      glVertex3d(.2*Cos(th+5),.2*Sin(th+5),0);
      glNormal3f(0,0,-1);
      glVertex3d(0,0,0);
      glEnd();
   }



   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(.2*Cos(th),.2*Sin(th),2.5);
      glVertex3d(.2*Cos(th),.2*Sin(th),2.5);
      glNormal3f(.2*Cos(th+5),.2*Sin(th+5),2.5);
      glVertex3d(.2*Cos(th+5),.2*Sin(th+5),2.5);
      glNormal3f(0,0,1);
      glVertex3d(0,0,2.5);
      glEnd();
   }

   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.

}


static void bed(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th
                 )
{

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.


   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(th,0,1,0); // 3D rotation: rotate the object around the specified axis.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.


   pillow(.4,.6,-3.3,.5,.5,.5,75);
   pillow(.4,.6,-1.6,.5,.5,.5,75);
   cuboid(0,0.01,0,4,.5,3.5,0);
   glColor3d(0,0,.5);
   cuboid(-.1,0,.1,4.2,.4,3.7,0);
   cuboid(-.1,0,.1,.1,1.3,3.7,0);
   glColor3d(.06,.12,.48);
   cuboid(.18,.52,-.12,3.55,.08,3.10,0);

   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}


static void lamp(double x,double y,double z,
                 double dx,double dy,double dz)
{

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.
   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(-90,1,0,0); // 3D rotation: rotate the object around the specified axis.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.
   glColor3d(.3,.3,.5);


   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(2*Cos(th),2*Sin(th),0);
      glVertex3d(2*Cos(th),2*Sin(th),0);
      glNormal3f(Cos(th),Sin(th),3);
      glVertex3d(Cos(th),Sin(th),3);
      glNormal3f(Cos(th+5),Sin(th+5),3);
      glVertex3d(Cos(th+5),Sin(th+5),3);
      glNormal3f(2*Cos(th+5),2*Sin(th+5),0);
      glVertex3d(2*Cos(th+5),2*Sin(th+5),0);
      glEnd();
   }
   glColor3d(1,1,1);
   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.
   glTranslated(0,0,-3.5); // 3D translation: position the object along the X, Y and Z axes.
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(.2*Cos(th),.2*Sin(th),0);
      glVertex3d(.2*Cos(th),.2*Sin(th),0);
      glVertex3d(.2*Cos(th),.2*Sin(th),5);
      glNormal3f(.2*Cos(th+5),.2*Sin(th+5),0);
      glVertex3d(.2*Cos(th+5),.2*Sin(th+5),5);
      glVertex3d(.2*Cos(th+5),.2*Sin(th+5),0);
      glEnd();
   }


   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(1.5*Cos(th),1.5*Sin(th),0);
      glVertex3d(1.5*Cos(th),1.5*Sin(th),0);
      glVertex3d(1.5*Cos(th),1.5*Sin(th),.3);
      glNormal3f(1.5*Cos(th+5),1.5*Sin(th+5),0);
      glVertex3d(1.5*Cos(th+5),1.5*Sin(th+5),.3);
      glVertex3d(1.5*Cos(th+5),1.5*Sin(th+5),0);
      glEnd();
   }


   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(1.5*Cos(th),1.5*Sin(th),0);
      glVertex3d(1.5*Cos(th),1.5*Sin(th),0);
      glNormal3f(1.5*Cos(th+5),1.5*Sin(th+5),0);
      glVertex3d(1.5*Cos(th+5),1.5*Sin(th+5),0);
      glNormal3f(0,0,-1);
      glVertex3d(0,0,0);
      glEnd();
   }


   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(1.5*Cos(th),1.5*Sin(th),.3);
      glVertex3d(1.5*Cos(th),1.5*Sin(th),.3);
      glNormal3f(1.5*Cos(th+5),1.5*Sin(th+5),.3);
      glVertex3d(1.5*Cos(th+5),1.5*Sin(th+5),.3);
      glNormal3f(0,0,1);
      glVertex3d(0,0,.3);
      glEnd();
   }
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.

   glColor3d(1,1,.9);
   glDisable(GL_LIGHTING);
   sphere(0,0,2,.6,.6,.6);
   if(light)
      glEnable(GL_LIGHTING);

   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.


}


static void luxo(double x,double y,double z,
                 double dx,double dy,double dz, double th)
{

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.


   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(th,0,1,0); // 3D rotation: rotate the object around the specified axis.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.
   glColor3d(.8,.8,.8);

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.
   glRotated(-20,1,0,0); // 3D rotation: rotate the object around the specified axis.


   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(2*Cos(th),2*Sin(th),0);
      glVertex3d(2*Cos(th),2*Sin(th),0);
      glNormal3f(Cos(th),Sin(th),2.5);
      glVertex3d(Cos(th),Sin(th),2.5);
      glNormal3f(Cos(th+5),Sin(th+5),2.5);
      glVertex3d(Cos(th+5),Sin(th+5),2.5);
      glNormal3f(2*Cos(th+5),2*Sin(th+5),0);
      glVertex3d(2*Cos(th+5),2*Sin(th+5),0);
      glEnd();
   }

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.
   glTranslated(0,0,2.5); // 3D translation: position the object along the X, Y and Z axes.



   double r = 1;
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(r*Cos(th),r*Sin(th),0);
      glVertex3d(r*Cos(th),r*Sin(th),0);
      glVertex3d(r*Cos(th),r*Sin(th),1);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),0);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),1);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),0);
      glEnd();
   }


   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(r*Cos(th),r*Sin(th),1);
      glVertex3d(r*Cos(th),r*Sin(th),1);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),1);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),1);
      glNormal3f(0,0,1);
      glVertex3d(0,0,1);
      glEnd();
   }


   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(r*Cos(th),r*Sin(th),0);
      glVertex3d(r*Cos(th),r*Sin(th),0);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),0);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),0);
      glNormal3f(0,0,-1);
      glVertex3d(0,0,0);
      glEnd();
   }

   r = .3;

   glTranslated(0,0,1); // 3D translation: position the object along the X, Y and Z axes.


   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(r*Cos(th),r*Sin(th),0);
      glVertex3d(r*Cos(th),r*Sin(th),0);
      glVertex3d(r*Cos(th),r*Sin(th),.2);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),0);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),.2);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),0);
      glEnd();
   }


   double h = .2;
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(r*Cos(th),r*Sin(th),h);
      glVertex3d(r*Cos(th),r*Sin(th),h);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),h);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),h);
      glNormal3f(0,0,1);
      glVertex3d(0,0,h);
      glEnd();
   }

   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.

   glTranslated(0,0,2.5); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(60,1,0,0); // 3D rotation: rotate the object around the specified axis.

   r = .5;

   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(r*Cos(th),r*Sin(th),0);
      glVertex3d(r*Cos(th),r*Sin(th),0);
      glVertex3d(r*Cos(th),r*Sin(th),4);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),0);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),4);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),0);
      glEnd();
   }


   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(r*Cos(th),r*Sin(th),4);
      glVertex3d(r*Cos(th),r*Sin(th),4);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),4);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),4);
      glNormal3f(0,0,1);
      glVertex3d(0,0,4);
      glEnd();
   }
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.
   glTranslated(0,-6.5,2.5); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(-60,1,0,0); // 3D rotation: rotate the object around the specified axis.

   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(r*Cos(th),r*Sin(th),0);
      glVertex3d(r*Cos(th),r*Sin(th),0);
      glVertex3d(r*Cos(th),r*Sin(th),4);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),0);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),4);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),0);
      glEnd();
   }


   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(r*Cos(th),r*Sin(th),4);
      glVertex3d(r*Cos(th),r*Sin(th),4);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),4);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),4);
      glNormal3f(0,0,1);
      glVertex3d(0,0,4);
      glEnd();
   }

   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.


   sphere(0,-3.1,4.3,.7,.7,.7);
   sphere(0,-6.7,2.5,.7,.7,.7);

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.
   glTranslated(0,-7.5,2.5); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(-90,1,0,0); // 3D rotation: rotate the object around the specified axis.
   glColor3d(.8,.8,.8);



   r = 2.5;
   h = .3;
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(r*Cos(th),r*Sin(th),0);
      glVertex3d(r*Cos(th),r*Sin(th),0);
      glVertex3d(r*Cos(th),r*Sin(th),h);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),0);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),h);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),0);
      glEnd();
   }


   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(r*Cos(th),r*Sin(th),h);
      glVertex3d(r*Cos(th),r*Sin(th),h);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),h);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),h);
      glNormal3f(0,0,1);
      glVertex3d(0,0,h+1);
      glEnd();
   }


   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(r*Cos(th),r*Sin(th),0);
      glVertex3d(r*Cos(th),r*Sin(th),0);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),0);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),0);
      glNormal3f(0,0,-1);
      glVertex3d(0,0,0);
      glEnd();
   }
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.


   glColor3d(1,1,.9);
   glDisable(GL_LIGHTING);
   sphere(0,.35,1,1.3,1.3,1.4);
   if(light)
      glEnable(GL_LIGHTING);


   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}


static void rug(double x,double y,double z,
                 double dx,double dy,double dz, double th)
{
   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.
   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(th,1,0,0); // 3D rotation: rotate the object around the specified axis.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.
   glColor3d(1,1,1);

   double r = 1;

   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(r*Cos(th),r*Sin(th),0);
      glVertex3d(r*Cos(th),r*Sin(th),0);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),0);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),0);
      glNormal3f(0,0,-1);
      glVertex3d(0,0,0);
      glEnd();
   }

   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}


static void room(double x, double y, double z, double dx, double dy, double dz)
{

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.


   glTranslated(x,y,z+dx); // 3D translation: position the object along the X, Y and Z axes.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.

   glColor3d(1,1,1);



   if ((th < 10 && th > -190) || th > 170)
   {
      glBegin(GL_QUADS);
      glNormal3d(1,0,0);
      glVertex3d(0,0,0);
      glVertex3d(0,1,0);
      glVertex3d(0,1,-1);
      glVertex3d(0,0,-1);
      glEnd();
   }


   if((th > -15 && th < 195) || th < -165)
   {
      glBegin(GL_QUADS);
      glNormal3d(-1,0,0);
      glVertex3d(1,0,0);
      glVertex3d(1,1,0);
      glVertex3d(1,1,-1);
      glVertex3d(1,0,-1);
      glEnd();
   }



   if((th < -80 && th > -280) || (th > 80 && th < 280))
   {
      glBegin(GL_QUADS);
      glNormal3d(0,0,-1);
      glVertex3d(0,1,0);
      glVertex3d(1,1,0);
      glVertex3d(1,0,0);
      glVertex3d(0,0,0);
      glEnd();
   }



   if ((th > -100 && th < 100) || (th > 260) || (th < -260))
   {
      glBegin(GL_QUADS);
      glNormal3d(0,0,1);
      glVertex3d(0,1,-1);
      glVertex3d(1,1,-1);
      glVertex3d(1,0,-1);
      glVertex3d(0,0,-1);
      glEnd();
   }

   if (ph > 0)
   {
      glBegin(GL_QUADS);
      glNormal3d(0,1,0);
      glVertex3d(0,0,0);
      glVertex3d(0,0,-1);
      glVertex3d(1,0,-1);
      glVertex3d(1,0,0);
      glEnd();
   }

   if (ph < 15)
   {
      glBegin(GL_QUADS);
      glNormal3d(0,-1,0);
      glVertex3d(0,1,0);
      glVertex3d(0,1,-1);
      glVertex3d(1,1,-1);
      glVertex3d(1,1,0);
      glEnd();
   }

   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.

}


static void flag(double x, double y, double z, double dx, double dy, double dz)
{

   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);


   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.


   glTranslated(x,y,z+dx); // 3D translation: position the object along the X, Y and Z axes.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.
   glColor3d(1,1,1);
   glBegin(GL_QUADS);
   glNormal3f(0,0,1);
   glVertex3d(0,0,0);
   glVertex3d(0,.5,0);
   glVertex3d(1,.5,0);
   glVertex3d(1,0,0);
   glEnd();
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}


static void window(double x, double y, double z, double dx, double dy, double dz, double th)
{


   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.


   glTranslated(x,y,z+dx); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(th,0,1,0); // 3D rotation: rotate the object around the specified axis.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.
   glColor3d(.35,.75,1.0);
   glBegin(GL_QUADS);
   glNormal3f(-1,0,1);
   glVertex3d(0,0,0);
   glVertex3d(0,.5,0);
   glVertex3d(1,.5,0);
   glVertex3d(1,0,0);
   glEnd();

   glColor3d(.08,.12,.24);
   glBegin(GL_QUADS);
   glVertex3d(.08,.05,.003);
   glVertex3d(.08,.24,.003);
   glVertex3d(.61,.24,.003);
   glVertex3d(.61,.05,.003);
   glEnd();

   glColor3d(.12,.18,.32);
   glBegin(GL_TRIANGLES);
   glVertex3d(.04,.24,.003);
   glVertex3d(.345,.34,.003);
   glVertex3d(.65,.24,.003);
   glEnd();

   glColor3d(1,.82,.25);
   for (double wx=.14; wx<.57; wx+=.10)
   {
      for (double wy=.09; wy<.21; wy+=.06)
      {
         glBegin(GL_QUADS);
         glVertex3d(wx,wy,.004);
         glVertex3d(wx,wy+.035,.004);
         glVertex3d(wx+.05,wy+.035,.004);
         glVertex3d(wx+.05,wy,.004);
         glEnd();
      }
   }

   glColor3d(.30,.16,.05);
   glBegin(GL_QUADS);
   glVertex3d(.78,.05,.003);
   glVertex3d(.78,.18,.003);
   glVertex3d(.81,.18,.003);
   glVertex3d(.81,.05,.003);
   glEnd();
   glColor3d(.03,.35,.12);
   glBegin(GL_TRIANGLES);
   glVertex3d(.70,.16,.003);
   glVertex3d(.795,.34,.003);
   glVertex3d(.89,.16,.003);
   glVertex3d(.71,.23,.003);
   glVertex3d(.795,.42,.003);
   glVertex3d(.88,.23,.003);
   glEnd();

   glColor3d(1,1,.72);
   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(.88,.40,.004);
   for (int angle=0; angle<=360; angle+=15)
      glVertex3d(.88+.045*Cos(angle),.40+.045*Sin(angle),.004);
   glEnd();

   glColor3d(.15,.15,.18);
   glBegin(GL_LINE_LOOP);
   glVertex3d(0,0,.002);
   glVertex3d(0,.5,.002);
   glVertex3d(1,.5,.002);
   glVertex3d(1,0,.002);
   glEnd();
   for (double blindY=.04; blindY<.49; blindY+=.05)
   {
      glBegin(GL_QUADS);
      glVertex3d(.015,blindY,.006);
      glVertex3d(.015,blindY+.015,.006);
      glVertex3d(.985,blindY+.015,.006);
      glVertex3d(.985,blindY,.006);
      glEnd();
   }

   glColor3d(.05,.05,.06);
   glBegin(GL_QUADS);
   glVertex3d(0,0,.007);
   glVertex3d(0,.5,.007);
   glVertex3d(.025,.5,.007);
   glVertex3d(.025,0,.007);
   glVertex3d(.975,0,.007);
   glVertex3d(.975,.5,.007);
   glVertex3d(1,.5,.007);
   glVertex3d(1,0,.007);
   glEnd();

   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}


static void desk(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.


   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(th,0,1,0); // 3D rotation: rotate the object around the specified axis.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.
   glColor3d(1,1,1);
   cuboid(0,1,0,3,.1,1.5,0);
   glColor3d(.1,.1,.1);

   cuboid(0.15,.2,-0.08,.1,.8,.1,0);

   cuboid(2.7,.2,-0.08,.1,.8,.1,0);

   cuboid(0.15,.2,-1.3,.1,.8,.1,0);

   cuboid(2.7,.2,-1.3,.1,.8,.1,0);

   // These positions use the desk's local coordinates: its top ends at y = 1.10.
   // The stack and open book therefore rest directly on the tabletop.
   drawClosedBook(.32f,1.101f,-.22f,.80f,.15f,.48f,.72f,.12f,.10f,0.0f);
   drawClosedBook(.39f,1.251f,-.25f,.72f,.12f,.44f,.08f,.28f,.72f,0.0f);
   drawClosedBook(.45f,1.371f,-.27f,.66f,.17f,.40f,.10f,.50f,.24f,0.0f);
   drawOpenBook(1.95f,1.101f,-.32f,.82f,.56f,0.0f);

   glScaled(.9,1.1,.9); // 3D scaling: resize the object along the X, Y and Z axes.

   cuboid(.15,.85,-.08,3,.1,1.5,0);
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}


static void shelf(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.


   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(th,0,1,0); // 3D rotation: rotate the object around the specified axis.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.
   glColor3d(.2,.2,.2);
   cuboid(0,0,0,2,.5,5,0);
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.

}


static void cushon(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.


   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(th,0,0,1); // 3D rotation: rotate the object around the specified axis.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.

   double r = 1;
   double cx = 4;
   double cy= 1;

   for (int th=0;th<=175;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(cx*r*Cos(th),cy*r*Sin(th),2.5);
      glVertex3d(cx*r*Cos(th),cy*r*Sin(th),2.5);
      glNormal3f(cx*r*Cos(th+5),cy*r*Sin(th+5),2.5);
      glVertex3d(cx*r*Cos(th+5),cy*r*Sin(th+5),2.5);
      glNormal3f(0,0,1);
      glVertex3d(0,0,2.5);
      glEnd();
   }


   for (int th=0;th<=175;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(cx*r*Cos(th),cy*r*Sin(th),0);
      glVertex3d(cx*r*Cos(th),cy*r*Sin(th),0);
      glVertex3d(cx*r*Cos(th),cy*r*Sin(th),2.5);
      glNormal3f(cx*r*Cos(th+5),cy*r*Sin(th+5),0);
      glVertex3d(cx*r*Cos(th+5),cy*r*Sin(th+5),2.5);
      glVertex3d(cx*r*Cos(th+5),cy*r*Sin(th+5),0);
      glEnd();
   }


   for (int th=0;th<=175;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(cx*r*Cos(th),cy*r*Sin(th),0);
      glVertex3d(cx*r*Cos(th),cy*r*Sin(th),0);
      glNormal3f(cx*r*Cos(th+5),cy*r*Sin(th+5),0);
      glVertex3d(cx*r*Cos(th+5),cy*r*Sin(th+5),0);
      glNormal3f(0,0,-1);
      glVertex3d(0,0,0);
      glEnd();
   }


   glBegin(GL_QUADS);
   glNormal3f(0,-1,0);
   glVertex3d(cx*r*Cos(180),cy*r*Sin(180),0);
   glVertex3d(cx*r*Cos(0),cy*r*Sin(0),0);
   glVertex3d(cx*r*Cos(0),cy*r*Sin(0),2.5);
   glVertex3d(cx*r*Cos(180),cy*r*Sin(180),2.5);
   glEnd();
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}


static void chair(double x, double y, double z, double dx, double dy, double dz, double th)
{

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.


   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(th,0,1,0); // 3D rotation: rotate the object around the specified axis.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.


   cushon(0,0,0,0.2,0.25,.6,0);

   cushon(1.1,0.97,0,0.25,0.25,.6,75);

   cuboid(.8,-1,0,.1,1,.1,180);

   cuboid(-.7,-1,0,.1,1,.1,180);

   cuboid(-.7,-1,1.4,.1,1,.1,180);

   cuboid(.8,-1,1.4,.1,1,.1,180);


   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.


}


static void baseball(double x, double y, double z, double dx, double dy, double dz, double th)
{

   glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.


   glTranslated(x,y,z); // 3D translation: position the object along the X, Y and Z axes.
   glRotated(th,0,0,1); // 3D rotation: rotate the object around the specified axis.
   glRotated(-90,1,0,0); // 3D rotation: rotate the object around the specified axis.
   glScaled(dx,dy,dz); // 3D scaling: resize the object along the X, Y and Z axes.
   glColor3d(1,1,1);


   for (int ph=-90;ph<90;ph+=5)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=5)
      {
         Vertex(th,ph);
         Vertex(th,ph+15);
      }
      glEnd();
   }
   glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
}


void display()
{

   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


   glEnable(GL_DEPTH_TEST);


   glLoadIdentity(); // Reset the current transformation matrix to identity.

   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim        *Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0); // Camera/view transformation: place the camera and point it at the scene.



   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);


   if (light)
   {

      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};

      float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};

      glEnable(GL_NORMALIZE);

      glEnable(GL_LIGHTING);

      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);

      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);

      glEnable(GL_LIGHT0);

      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);

      // Warm point light placed just in front of the rear-wall fixture.
      // It supplements the existing rotating room light without changing it.
      float WallAmbient[]  = {0.00f,0.00f,0.00f,1.0f};
      float WallDiffuse[]  = {0.34f,0.23f,0.11f,1.0f};
      float WallSpecular[] = {0.12f,0.08f,0.03f,1.0f};
      // Matches the fixture after the bedroom's outer (-1.5,0,-1.5) transform.
      float WallPosition[] = {0.35f,1.29f,-1.38f,1.0f};
      glEnable(GL_LIGHT1);
      glLightfv(GL_LIGHT1,GL_AMBIENT ,WallAmbient);
      glLightfv(GL_LIGHT1,GL_DIFFUSE ,WallDiffuse);
      glLightfv(GL_LIGHT1,GL_SPECULAR,WallSpecular);
      glLightfv(GL_LIGHT1,GL_POSITION,WallPosition);
      glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION,0.75f);
      glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION,0.35f);
   }
   else
   {
      glDisable(GL_LIGHTING);
      glDisable(GL_LIGHT1);
   }


   switch(mode)
   {
      case 1:
         glColor3d(.3,.3,.3);
         chair(0,0,0,1,1,1,180);
         break;
      case 2:
         luxo(0,1,0,.2,.2,.2,-90);
         break;
      case 3:
         nightstand(0,0,0,.7,.7,.7,0);
         break;
      case 4:
         dresser(0,0,0,.5,.5,.5,0);
         break;
      case 5:
         bed(0,0,0,.5,.5,.5,-90);
         break;
      case 6:
         lamp(0,0,0,.5,.5,.5);
         break;
      case 7:
         penguin(0,-.5,0,1,1,1,0,0,0,1);
         break;
      case 8:
         baseball(0,0,0,1,1,1,90);
         break;
      case 9:
         desk(-1.5,-.5,0,1,1,1,0);
         break;
      case 10:
         door(0,0,0,.5,.5,.5,0);
         break;
   }

   if (mode == 0)
   {
      glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.


      glTranslated(-1.5,0,-1.5); // 3D translation: position the object along the X, Y and Z axes.


      room(0,0,0,3,1.8,3);
      // The rear wall is parent-local z=0 after room()'s internal transform.
      // This upper-right position is fully inside the wall and above the bed.
      drawWallLight(1.85f,.85f,.025f,.88f);
      plant(.25,0,2.65,.22);
      plant(2.65,0,.35,.18);

      dresser(1.0,0,3.7,.27,.27,.27,180);


      nightstand(1.5,0,0.25,.25,.25,.25,0);
      nightstand(.25,0,.25,.25,.25,.25,0);
      sideTableDisplay(.5,.42,.5,.20);


      lamp(2,.6,0.17,.05,.05,.05);
      lamp(0.4,.6,0.17,.05,.05,.05);


      bed(.7,0,.1,.3,.3,.3,-90);


      if (((th > -100 && th < 100) || (th > 260) || (th < -260)))
      {
         flag(.725,.5,-.999,1,1,1);
      }


      if ((th < 10 && th > -190) || th > 170)
      {

         glDisable(GL_LIGHTING);
         window(0.01,0.1,0.1,2.8,3,2.8,90);

         if (light)
            glEnable(GL_LIGHTING);
         glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
         glColor3d(.5,.5,.5);


         const bool showBlinds = false;
         if (showBlinds)
         {
            cuboid(-1.5,1.6,1.4,.05,0.01,2.8,0);
            cuboid(-1.5,1.5,1.4,.05,0.01,2.8,0);
            cuboid(-1.5,1.4,1.4,.05,0.01,2.8,0);
            cuboid(-1.5,1.3,1.4,.05,0.01,2.8,0);
            cuboid(-1.5,1.2,1.4,.05,0.01,2.8,0);
            cuboid(-1.5,1.1,1.4,.05,0.01,2.8,0);
            cuboid(-1.5,1,1.4,.05,0.01,2.8,0);
            cuboid(-1.5,.9,1.4,.05,0.01,2.8,0);
            cuboid(-1.5,.8,1.4,.05,0.01,2.8,0);
            cuboid(-1.5,.7,1.4,.05,0.01,2.8,0);
            cuboid(-1.5,.6,1.4,.05,0.01,2.8,0);
            cuboid(-1.5,.5,1.4,.05,0.01,2.8,0);
            cuboid(-1.5,.4,1.4,.05,0.01,2.8,0);
            cuboid(-1.5,.3,1.4,.05,0.01,2.8,0);
            cuboid(-1.5,.2,1.4,.05,0.01,2.8,0);
            cuboid(-1.5,.1,1.4,.05,0.01,2.8,0);
         }

      }
      else
      {
         glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
      }


      luxo(1.3,.75,-.6,0.04,0.04,0.04,140);


      desk(1.05,-.1,-0.9,.4,.5,.3,-90);


      if((th < -80 && th > -280) || (th > 80 && th < 280))
      {
        door(1,0,1.46,.3,.3,.3,180);
      }


      glPushMatrix(); // Save the parent transformation matrix before changing this object's local transform.
      glTranslated(0,-.2,0); // 3D translation: position the object along the X, Y and Z axes.

      // Draw 2 shelves
      shelf(-.1,1,1.5,.1,.1,.1,90);

      // Draw 3 penguins
      penguin(-.2,1.12,1.4,.1,.1,.1,210,.1,.1,1);

      // Draw a baseball
      baseball(-.45,1.1,1.4,.06,.06,.06,90);
      glPopMatrix(); // Restore the parent transformation matrix so later objects are unaffected.
      if (ph > 0)
         rug(.3,0.001,.65,.8,.8,.8,90);

      glColor3d(.15,.15,.15);

      chair(.8,.2,-.2,.2,.2,.2,150);

   }


   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      const double len=2.0;
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();

      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }



   Print("Angle=%d,%d  Dim=%.1f FOV=%d Light=%s",
     th,ph,dim,fov,light?"On":"Off");




   switch(mode)
   {
      case 0:
         Print("Scene=Main");
         break;
      case 1:
         Print("Scene=Chair");
         break;
      case 2:
         Print("Scene=Luxo");
         break;
      case 3:
         Print("Scene=Nightstand");
         break;
      case 4:
         Print("Scene=Dresser");
         break;
      case 5:
         Print("Scene=Bed");
         break;
      case 6:
         Print("Scene=Lamp");
         break;
      case 7:
         Print("Scene=Penguin");
         break;
      case 8:
         Print("Scene=Baseball");
         break;
      case 9:
         Print("Scene=Desk");
         break;
      case 10:
         Print("Scene=Door");
         break;
   }

   if (light)
   {

      Print("Light Distance=%d Light Elevation=%.1f",distance,ylight);
   }


   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}


void idle()
{

   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);

   glutPostRedisplay();
}


void special(int key,int x,int y)
{

   if (key == GLUT_KEY_RIGHT)
      th += 5;

   else if (key == GLUT_KEY_LEFT)
      th -= 5;

   else if (key == GLUT_KEY_UP)
   {
      ph += 5;
   }

   else if (key == GLUT_KEY_DOWN)
   {
      ph -= 5;
   }

   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;

   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;

   else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
   else if (key == GLUT_KEY_F3)
      distance = (distance==1) ? 5 : 1;

   th %= 360;
   ph %= 360;

   Project(fov,asp,dim);

   glutPostRedisplay();
}


void key(unsigned char ch,int x,int y)
{

   if (ch == 27)
      exit(0);

   else if (ch == '0')
      th = ph = 0;

   else if (ch == 't')
      axes = 1-axes;

   else if (ch == 'l' || ch == 'L')
      light = 1-light;

   else if (ch == '-' && fov>1)
      fov--;
   else if (ch == '+' && fov<179)
      fov++;

   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;

   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;

   else if (ch == 'm' || ch == 'M')
      move = 1-move;

   else if (ch == 'c' || ch == 'C')
   {
      mode += 1;
      if (mode > 10)
         mode = 0;
   }

   shiny = shininess<0 ? 0 : pow(2.0,shininess);

   glutIdleFunc(move?idle:NULL);

   Project(fov,asp,dim);

   glutPostRedisplay();
}


void reshape(int width,int height)
{

   asp = (height>0) ? (double)width/height : 1;

   glViewport(0,0, RES*width,RES*height);

   Project(fov,asp,dim);
}


int main(int argc,char* argv[])
{


   glutInit(&argc,argv);

   glutInitWindowSize(600,600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

   glutCreateWindow("3D Bedroom - C++ OpenGL");
   glutFullScreen();

   glutDisplayFunc(display);

   glutReshapeFunc(reshape);

   glutSpecialFunc(special);

   glutKeyboardFunc(key);

   glutIdleFunc(idle);

   ErrCheck("init");

   glutMainLoop();
   return 0;
}
