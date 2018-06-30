/*
resourses: 
   I used this for praticle
   https://github.com/jlecavalier/computer_graphics_project
   exercises form the moodle
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "CSCIx229.h"
 

#define PI 3.1416

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int side=0;       //  Two sided mode
int ntex=1;       //  Texture mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int Th=0,Ph=30;   //  Light angles
float sco=180;    //  Spot cutoff angle
float Exp=0;      //  Spot exponent
int at0=100;      //  Constant  attenuation %
int at1=0;        //  Linear    attenuation %
int at2=0;        //  Quadratic attenuation %
int fp=0;
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=8.0;   //  Size of world






int rot = 0.0; //rotation var for first person

/* Eye coords */
double Ex = 0;
double Ey = 0;
double Ez = 5;

/* Camera looking coords 
 * Don't need a y coord because up-down view cannot be altered in fp 
 */
double Cx = 0; 
double Cz = 0; 

int move = 1;

// Light values
int num       =   1;  // Number of quads
int inf       =   0;  // Infinite distance light
int distance  =   10;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =   0;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
float X       = 0;    // Light X position
float Y       = 0;    // Light Y position
float Z       = 1;    // Light Z position

unsigned int texture[15]; // Texture names
PARTICLES particles[500];
int textMode = 1;

typedef struct {
   double x;
   double y;
   double z;
} points_t;


/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}


/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}



/*
 *  Set projection
 */
static void myProject()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if(fp) {
      gluPerspective(fov,asp,dim/4,4*dim);
   }
   else {
      if (mode)
         gluPerspective(fov,asp,dim/4,4*dim);
      //  Orthogonal projection
      else
         glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   }  
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}
// ----------------------- my project from here 



void cylender_texture(double x,double y,double z,
                     double dx,double dy,double dz,
                     double th, 
                     double xx, double yy, double zz, 
                     unsigned int body, unsigned int top, unsigned int bt) {
   //double i;
   double radius = 0.3, height = 0.2;

   glPushMatrix();
   //  Transform cube
   glTranslated(x,y,z);
   glRotated(th,xx,yy,zz); //   glRotated(th,0,0,1);
   glScaled(dx,dy,dz);

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);



   //glColor3f(0, 1, 0);
   glBindTexture(GL_TEXTURE_2D,texture[body]);

   double j;
   glBegin(GL_QUAD_STRIP);
   for (j = 0; j <= 360; j+=30) {
      const float tc = (j / (float) 360);

      double x = radius * Cos(j);
      double y = height;
      double z = radius * Sin(j);

      glNormal3d(Cos(j), 0, Sin(j));

      glTexCoord2f(-tc, 0.0); glVertex3d(x, -y, z);
      glTexCoord2f(-tc, 1.0); glVertex3d(x, y, z);
   }
   glEnd();

   //glColor3f(0, 1, 0);

   double i; 

   glBindTexture(GL_TEXTURE_2D,texture[bt]);

   glNormal3d(0,1,0);

   /* Top of Cylinder */
   glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5); 
      glVertex3d(0.0, height, 0.0);

      for(i = 0.0; i <= 360; i+=10) {
         glTexCoord2f(-0.5*Cos(i)+0.5, 0.5*Sin(i)+0.5);
         glVertex3d(radius * Cos(i), height, radius * Sin(i));
      }
   glEnd();

   glBindTexture(GL_TEXTURE_2D,texture[top]);
   glNormal3d(0,-1,0);

   /* Bottom of Cylinder */
   glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5); 
      glVertex3d(0.0, -height, 0.0);

      for(i = 0.0; i <= 360; i+=10) {
         glTexCoord2f(0.5*Cos(i)+0.5, 0.5*Sin(i)+0.5);
         glVertex3d(radius * Cos(i), -height, radius * Sin(i));
      }
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D); 


}

static void cone(double radius1, double radius2, double height, 
                 double x, double y, double z, 
                 double dx, double dy, double dz, 
                 double ang, double xx, double yy, double zz, 
                 unsigned int tex)
{
   const int d = 30;
//   float white[] = {1,1,1,1};
//   float black[] = {0,0,0,1};
//   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shinyvec);
//   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
//   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);

   glPushMatrix();
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);
   glRotated(ang, xx, yy, zz); 
   int th;

   glEnable(GL_TEXTURE_2D);
//   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,texmode?GL_REPLACE:GL_MODULATE);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[tex]);

   glBegin(GL_TRIANGLE_FAN);
   glVertex3f(0, height, 0); //Top of cylinder
   for(th = 0; th <=360; th += d)
   {  
      glVertex3f(radius1 * cos(th), height, radius1 * sin(th));
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glVertex3f(0,0,0);
   for(th = 360; th >= 0; th -= d)
   {
      glVertex3f(radius2 * cos(th), 0, radius2 * sin(th));
   }
   glEnd();

   glBegin(GL_QUAD_STRIP);
   for(th = 0; th < 360; th += d)
   {
      glNormal3f(cos(th), 0, sin(th));
      glTexCoord2f(1 - (th/360), 1); glVertex3f(radius2 * cos(th), 0, radius2 * sin(th));
      glTexCoord2f(th/360, 0); glVertex3f(radius1 * cos(th), height, radius1 * sin(th));
   }

   glVertex3f(radius2, 0, 0);
   glVertex3f(radius1, height, 0);
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}


static void pine_leafe(double x,double y,double z,
                     double dx,double dy,double dz,
                     double th, 
                     double xx, double yy, double zz)
{

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(th,xx,yy,zz);
   glScaled(dx,dy,dz);
   cylender_texture(-2  , 2.5   , 0    , 0.5, 15 , 0.5, 0, 0, 0, 0, 1, 1, 1);
   // end
   cylender_texture(-2  , -1   , 0    , 0.3, 3 , 0.3, 0, 0, 0, 0, 2, 2, 2);
   // left

   cylender_texture(-3.8 , 3, 0  , 0.8, 9 , 0.4, 105, 0, 0, 1, 2, 2, 2);
   cylender_texture(-3.4 , 2, 0  , 0.7, 7 , 0.3, 105, 0, 0, 1, 2, 2, 2);
   cylender_texture(-3.0 , 1, 0  , 0.6, 5 , 0.2, 105, 0, 0, 1, 2, 2, 2);
   cylender_texture(-2.5 , 0, 0  , 0.5, 3 , 0.1, 105, 0, 0, 1, 2, 2, 2);
   // right
   cylender_texture(-0.2 , 3, 0  , 0.8, 9 , 0.4, 75, 0, 0, 1, 2, 2, 2);
   cylender_texture(-0.6 , 2, 0  , 0.7, 7 , 0.3, 75, 0, 0, 1, 2, 2, 2);
   cylender_texture(-1 , 1, 0  , 0.6, 5 , 0.2, 75, 0, 0, 1, 2, 2, 2);
   cylender_texture(-1.5 , 0, 0  , 0.5, 3 , 0.1, 75, 0, 0, 1, 2, 2, 2);   


   glPopMatrix();
}


static void pine_branch(double x,double y,double z,
                     double dx,double dy,double dz,
                     double th, 
                     double xx, double yy, double zz)
{

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(th,xx,yy,zz);
   glScaled(dx,dy,dz);

   cylender_texture(0  , 4   , 0    , 0.5, 20 , 0.5, 1, 0, 0, 0, 0, 1, 1);
   pine_leafe(1, -2, -0.5,    0.5,0.5,0.5,  10, 1,0,0);

   glRotated(15,1,0,0);
   //first
   pine_leafe(2.6, 1, -0.6,    0.5,0.5,0.5,  45, 0,0,1);
   pine_leafe(-1.3, 0, -0.7,   0.5,0.5,0.5, -45, 0,0,1);

   pine_leafe(3.65, 4, -1.55,    0.7,0.7,0.7,  45, 0,0,1);
   pine_leafe(-1.7, 2.5, -1.7,    0.7,0.7,0.7, -45, 0,0,1);

   

   glPopMatrix();
}





static void pine_tree(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double xx, double yy, double zz)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,xx,yy,zz);
   glScaled(dx,dy,dz);


   // ------ tree trunk 
   cylender_texture(0   , 0   , 0    , 1, 16, 1,   0,  0, 0, 1, 0, 0, 0);
   cone(0.35, 0.05, 0.5, 0, 3.6, 0, 1, 1, 1, 180, 0,0, 1, 0);

   // ------ branches 
   int i;
   for (i=0; i<365; i=i+60){
   glRotated(i,0,1,0);
   pine_branch(0, -1.8, 3.5,    0.5,0.5,0.5,  -65, 1,0,0);
   pine_branch(0, -0.5, 3.0,       0.4,0.4,0.4,  -65, 1,0,0);
   pine_branch(0,  1.0, 2.2,       0.3,0.3,0.3,  -65, 1,0,0);
   pine_branch(0,  2.2, 1.5,       0.2,0.2,0.2,  -65, 1,0,0);
   }


   glPopMatrix();
  
}


static void flower(double x,double y,double z,
                     double dx,double dy,double dz,
                     double th, 
                     double xx, double yy, double zz,
                     unsigned int tex)
{

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(th,xx,yy,zz);
   glScaled(dx,dy,dz);
   
   cone(0.8, 0.2, 0.8, 0, 0, 0, 1, 1, 1, 90, 1,0, 0, 2);
   //base
   cylender_texture( 0.00, 0.00,  0.80  , 2.00, 0.40 , 2.00,  90, 1, 0, 0, 4, 4, 4);

   int k;
   for (k=0; k < 360; k = k+45){
   glRotated(k,0,0,1);
      cylender_texture( 0.00, 1.80,  0.80  , 0.80, 0.20 , 4.00,  90, 1, 0, 0, tex, tex, tex);
   }
   
   glPopMatrix();
}


static void stem_flower(double x,double y,double z,
                     double dx,double dy,double dz,
                     double th, 
                     double xx, double yy, double zz,
                     unsigned int tex )
{

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(th,xx,yy,zz);
   glScaled(dx,dy,dz);
   

   cylender_texture( 0.00, -2.00,  0.00  , 0.30, 10.00 , 0.30,  0, 0, 0, 0, 4, 4, 4);

   //leaf
   //glRotated(30,1,0,0);
   unsigned int leaf_tex = 2;
   cylender_texture( 0.00, -2.70, -0.60,   0.40, 0.20 , 2.00,  30, 1, 0, 0, leaf_tex, leaf_tex, leaf_tex);   
   cylender_texture( 0.00, -1.70, -0.60,   0.40, 0.20 , 2.00,  30, 1, 0, 0, leaf_tex, leaf_tex, leaf_tex);   
   cylender_texture( 0.00, -0.70, -0.60,   0.40, 0.20 , 2.00,  30, 1, 0, 0, leaf_tex, leaf_tex, leaf_tex);   

   cylender_texture( 0.00, -2.90, 0.60,   0.40, 0.20 , 2.00,  -30, 1, 0, 0, leaf_tex, leaf_tex, leaf_tex);   
   cylender_texture( 0.00, -1.90, 0.60,   0.40, 0.20 , 2.00,  -30, 1, 0, 0, leaf_tex, leaf_tex, leaf_tex);   
   cylender_texture( 0.00, -0.90, 0.60,   0.40, 0.20 , 2.00,  -30, 1, 0, 0, leaf_tex, leaf_tex, leaf_tex);   

   glRotated(90,0,1,0);
   cylender_texture( 0.00, -2.80, -0.60,   0.40, 0.20 , 2.00,  30, 1, 0, 0, leaf_tex, leaf_tex, leaf_tex);   
   cylender_texture( 0.00, -1.80, -0.60,   0.40, 0.20 , 2.00,  30, 1, 0, 0, leaf_tex, leaf_tex, leaf_tex);   
   cylender_texture( 0.00, -0.80, -0.60,   0.40, 0.20 , 2.00,  30, 1, 0, 0, leaf_tex, leaf_tex, leaf_tex);   

   glRotated(180,0,1,0);
   cylender_texture( 0.00, -2.80, -0.60,   0.40, 0.20 , 2.00,  30, 1, 0, 0, leaf_tex, leaf_tex, leaf_tex);   
   cylender_texture( 0.00, -1.80, -0.60,   0.40, 0.20 , 2.00,  30, 1, 0, 0, leaf_tex, leaf_tex, leaf_tex);   
   cylender_texture( 0.00, -0.80, -0.60,   0.40, 0.20 , 2.00,  30, 1, 0, 0, leaf_tex, leaf_tex, leaf_tex);    

   glRotated(-15,0,0,1);
   flower(0, -0.1, 0,    0.5,0.5,0.5,  -115, 1,0,0, tex);

   glPopMatrix();
}


static void plant_flower(double x,double y,double z,
                     double dx,double dy,double dz,
                     double th, 
                     double xx, double yy, double zz,
                     unsigned int tex)
{

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   
   //base
   cylender_texture( 0.00, -3.60,  -1.00  , 1.00, 1.00 , 1.50,  0, 0, 0, 0, 4, 4, 4);


   stem_flower(0.00, 0.00,  1.20,    1,1,1,  30, 1,0,0, tex);
   glRotated(90,0,1,0);
   stem_flower(1.00, 0.00,  2.20,    1,1,1,  30, 1,0,0, tex);
   glRotated(90,0,1,0);
   stem_flower(0.00, 0.00,  3.20,    1,1,1,  30, 1,0,0, tex);
   glRotated(90,0,1,0);
   
   stem_flower(-1.00, 0.00,  2.20,    1,1,1,  30, 1,0,0, tex);


   glPopMatrix();
}




// Draw lengthy box that will serve as ground for objects
static void drawGround(double x, double y, double z, 
                       double dx,double dy,double dz,
                       double th, 
                       double xx, double yy, double zz, 
                       unsigned int tex)
{
   glPushMatrix();

   glTranslated(x, y, z);   
   glScaled(dx, dy, dz);
   glRotated(th,xx,zz,zz);

   glEnable(GL_TEXTURE_2D);
   //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   //glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D, texture[tex]);

   glBegin(GL_QUADS);
   //glColor3f(.36, .36, .36);

   //Front
   glNormal3f(0,0,+1);   
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glNormal3f(0,0,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glNormal3f(+1, 0, 0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glNormal3f(-1, 0, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);

   //  Top
   glNormal3f(0,+1,0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);


   //  Bottom
   glNormal3f(0, -1, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1); 
   glEnd();  

   glPopMatrix();
   
}

static void ground(double x,double y,double z,
                     double dx,double dy,double dz,
                     double th, 
                     double xx, double yy, double zz, 
                     unsigned int tex,
		     unsigned int t_1, unsigned int t_2)
{

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(th,xx,yy,zz);
   glScaled(dx,dy,dz);
   
   //base
   int i;
   int j;

   for (i=0; i<t_1; i=i+2){
      for (j=0; j<t_2; j=j+2){
          drawGround(i, 0, j, 1, 0.2, 1, 0,0,0,0, tex); 
      }
   }
//   
   glPopMatrix();
}


void front(unsigned int tex){         //Builds bottom of the piece
    
   glEnable(GL_TEXTURE_2D);
//   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_REPLACE);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[tex]);
    glBegin(GL_QUADS);
   
   glNormal3f( 0, 0, +1);
   glTexCoord2f(0,0);glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0);glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1);glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1);glVertex3f(-1,+1, 1);

    glEnd();

  
}

void back(unsigned int tex){

   glEnable(GL_TEXTURE_2D);
//   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_REPLACE);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[tex]);
    glBegin(GL_QUADS);
   
   glNormal3f( 0, 0, -1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);

    glEnd();

}

static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double rx, double ry, double rz, 
                 unsigned int tex)
{


   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,rx,ry,rz); //glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D, texture[tex]);

   glNormal3f( 0, 0, 1);
   front(tex);

   back(tex);

   //glTranslated(1,0,0);
   glRotated(90,0,1,0);  //rigth
   front(tex);
   //glTranslated(1,0,0);
   glRotated(180,0,1,0); //left
   front(tex);
   //glTranslated(2,0,0);
   glNormal3f( 0, -1, 0);
   glRotated(90,1,0,0); // bt
   front(tex);
   //glTranslated(2,0,0);
   glNormal3f( 0, 2, 2);
   glRotated(180,1,0,0);  //top 
   front(tex);

   //  Undo transofrmations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}



static void fountain(double x,double y,double z,
                     double dx,double dy,double dz,
                     double th, 
                     double xx, double yy, double zz,
                     unsigned int tex_f)
{

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(th,xx,yy,zz);
   glScaled(dx,dy,dz);
   
   //base
   cube(0, 0, 0,    4.0, 0.2, 4.0, 0, 0, 0,0, 12);
   // walls 
   cube(0, 1, 3.8,    4.0, 1.0, 0.2, 0, 0, 0,0, tex_f);
   cube(0, 1,-3.8,    4.0, 1.0, 0.2, 0, 0, 0,0, tex_f);
   cube(-3.8, 1, 0,    4.0, 1.0, 0.2, 90, 0, 1,0, tex_f);
   cube(3.8, 1, 0,    4.0, 1.0, 0.2, 90, 0, 1,0, tex_f);


   cylender_texture( 0, 5, 0,   1.5, 20 , 1.5,  0, 0, 0, 0, 12, 12, 12);
   cylender_texture( 0, 9, 0,   3, 1 , 3,  0, 0, 0, 0, tex_f, tex_f, tex_f);
   cone(0, 0.7, 3,  0, 8.8, 0, 1, 1, 1, 0, 0,0, 0, 12);
   // water
   glColor4f(.85,1,.85,2);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE); 
  
   cube(0, 1.5, 0,    3.60, 0.2, 3.60, 0, 0, 0,0, 9);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);

}




static void fence(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double rx, double ry, double rz, 
                 unsigned char ch)
{


   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,rx,ry,rz); //glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   
   if (ch == 'c'){
      cube(-10, 1, 0, 1, 10, 1, 0, 0, 0,0, 11);
      cube( 10, 1, 0, 1, 10, 1, 0, 0, 0,0, 11);
      cube( 1, 9, 0, 0.5, 10, 0.5, 90, 0, 0,1, 11);
      cube( 1, 5, 0, 0.5, 10, 0.5, 90, 0, 0,1, 11);
      cube( 1, 1, 0, 0.5, 10, 0.5, 90, 0, 0,1, 11);
      cube( 1, -3, 0, 0.5, 10, 0.5, 90, 0, 0,1, 11);
   }
   else if (ch == 'r'){
      //cube(-10, 1, 0, 1, 10, 1, 0, 0, 0,0, 11);
      cube( 10, 1, 0, 1, 10, 1, 0, 0, 0,0, 11);
      cube( 1, 9, 0, 0.5, 10, 0.5, 90, 0, 0,1, 11);
      cube( 1, 5, 0, 0.5, 10, 0.5, 90, 0, 0,1, 11);
      cube( 1, 1, 0, 0.5, 10, 0.5, 90, 0, 0,1, 11);
      cube( 1, -3, 0, 0.5, 10, 0.5, 90, 0, 0,1, 11);
   }
   else if (ch == 'l'){
      cube(-10, 1, 0, 1, 10, 1, 0, 0, 0,0, 11);
      //cube( 10, 1, 0, 1, 10, 1, 0, 0, 0,0, 11);
      cube( 1, 9, 0, 0.5, 10, 0.5, 90, 0, 0,1, 11);
      cube( 1, 5, 0, 0.5, 10, 0.5, 90, 0, 0,1, 11);
      cube( 1, 1, 0, 0.5, 10, 0.5, 90, 0, 0,1, 11);
      cube( 1, -3, 0, 0.5, 10, 0.5, 90, 0, 0,1, 11);
   }




   //  Undo transofrmations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}


static void table(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double rx, double ry, double rz)
{


   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,rx,ry,rz); //glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //glRotated(90,1,0,0);
   // top
   cube(-2, 0.3, -0, 4.0, 0.1, 3, 0, 0, 0,0, 11);
   
   // top goes on these
   cube( 1.0, 0,  0, 0.3, 2.5, 0.3, 90, 1, 0,0, 11);
   cube(-5.0, 0,  0, 0.3, 2.5, 0.3, 90, 1, 0,0, 11);

   // poles for top 
   cube( 1.0, -1.5,  0, 0.3, 1.5, 0.3, 0, 0, 0,0, 11);
   cube(-5.0, -1.5,  0, 0.3, 1.5, 0.3, 0, 0, 0,0, 11);
   // holds poles 
   cube(-2, -2.7, -0, 3.2, 0.3, 0.3, 0, 0, 0,0, 11);

   cube(1, -2.7, -0, 0.3, 0.3, 5, 0, 0, 0,0, 11);
   cube(-5, -2.7, -0, 0.3, 0.3, 5, 0, 0, 0,0, 11);


   cube( 1.0, -2.0,  4, 0.3, 0.5, 0.3, 0, 0, 0,0, 11);
   cube(-5.0, -2.0,  4, 0.3, 0.5, 0.3, 0, 0, 0,0, 11);
   cube( 1.0, -2.0,  -4, 0.3, 0.5, 0.3, 0, 0, 0,0, 11);
   cube(-5.0, -2.0,  -4, 0.3, 0.5, 0.3, 0, 0, 0,0, 11);

   cube(-2, -1.5, 4, 4.0, 0.1, 1, 0, 0, 0,0, 11);
   cube(-2, -1.5, -4, 4.0, 0.1, 1, 0, 0, 0,0, 11);

   //  Undo transofrmations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}


// ---------- particle 

void update_particles(PARTICLES particles[500]) {
  int i;
  for (i=0;i<500;i++) {
    // Move the particle accordingly
    particles[i].x += particles[i].x_m;
    particles[i].z += particles[i].z_m;
    particles[i].y += particles[i].accel - particles[i].decel;
    // Increase decel so that it falls faster
    particles[i].decel += .0025;
    // Restart the particle if it falls below the fountain
    if (particles[i].y <= .03) {
        particles[i].x = 0;
  	particles[i].y = 3;
  	particles[i].z = 1;
  	double d = (drand48()*360); /* drand48(): functions return non-negative, double-precision, 
                                        floating-point values, uniformly distributed over the interval [0.0 , 1.0].  */
  	particles[i].x_m = .015 * Cos(d);
  	particles[i].z_m = .015 * Sin(d);
    particles[i].r = 128.0/256.0;
    particles[i].g = 143.0/256.0;
    particles[i].b = 232.0/256.0;
    particles[i].z_sc = .045;
    particles[i].th = 0;
    particles[i].accel = (drand48()*0.025);
    particles[i].decel = 0.0025;
    }
  }
}


void init_particles(PARTICLES particles[500]) {
  int i;
  for (i=0;i<500;i++) {
  	// Start in the middle of the lotus
  	// of the fountain.
  	particles[i].x = 0;
  	particles[i].y = 3;
  	particles[i].z = 1;
    
    // Make the particles move in a random
    // direction outward 
  	double d = (drand48()*360);
  	particles[i].x_m = .015 * Cos(d);
  	particles[i].z_m = .015 * Sin(d);
    
    // Particles are blue
    particles[i].r = 128.0/256.0;
    particles[i].g = 143.0/256.0;
    particles[i].b = 232.0/256.0;

    // particle size
    particles[i].z_sc = .045;

    // Particles do not need to rotate
    particles[i].th = 0;

    // Accelerate and decelerate at different rates
    particles[i].accel = (drand48()*0.025);
    particles[i].decel = 0.0025;
  }
}


void draw_particles(PARTICLES particles[500],
	                unsigned int tex_01,
	                unsigned int tex) {
  int i;
  for (i=0;i<500;i++) {
    // Color the particle
  	glColor4f(particles[i].r,particles[i].g,particles[i].b,.6);

  	glPushMatrix();

  	// Transforms
  	glTranslated(particles[i].x,particles[i].y,particles[i].z);
  	glScaled(particles[i].z_sc,particles[i].z_sc,particles[i].z_sc);

        glEnable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
  	glEnable(GL_BLEND);
  	glBlendFunc(GL_DST_COLOR,GL_ZERO);
  	glBindTexture(GL_TEXTURE_2D,tex_01);
    
    // Draw the particle 
  	glBegin (GL_QUADS);
  	  glTexCoord2f(0,1);
          glVertex3f (0, 1, 0);
          glTexCoord2f(1,1);
          glVertex3f (1, 1, 0);
          glTexCoord2f(1,0);
          glVertex3f (1, 0, 0);
          glTexCoord2f(0,0);
          glVertex3f (0, 0, 0);
        glEnd();

    glBlendFunc(GL_ONE, GL_ONE);
    glBindTexture(GL_TEXTURE_2D,tex);

    // Draw the particle
    glBegin (GL_QUADS);
  	glTexCoord2f(0,1);
    glVertex3f (0, 1, 0);
    glTexCoord2f(1,1);
    glVertex3f (1, 1, 0);
    glTexCoord2f(1,0);
    glVertex3f (1, 0, 0);
    glTexCoord2f(0,0);
    glVertex3f (0, 0, 0);
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glPopMatrix();

  }
}

// ----------------




/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{

   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position

   if (fp) {
      Cx = +2*dim*Sin(rot); // Change camera dimensions 
      Cz = -2*dim*Cos(rot);

      gluLookAt(Ex,Ey,Ez, Cx+Ex,Ey,Cz+Ez, 0,1,0); //  Use gluLookAt
  }else{



      if (mode)
      {
         double Ex = -2*dim*Sin(th)*Cos(ph);
         double Ey = +2*dim        *Sin(ph);
         double Ez = +2*dim*Cos(th)*Cos(ph);
         gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
      }
      //  Orthogonal - set world orientation
      else
      {
         glRotatef(ph,1,0,0);
         glRotatef(th,0,1,0);
      }
   }



   if(light){   

      glShadeModel(GL_SMOOTH);
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};

      //  Spotlight color and direction
      float yellow[] = {1.0,1.0,0.0,1.0};
      //float Direction[] = {Cos(Th)*Sin(Ph),Sin(Th)*Sin(Ph),-Cos(Ph),0};

      //  Light position
      float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Location of viewer for specular calculations
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
      //  Two sided mode
      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,side);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Set specular colors
      glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
      glMaterialf(GL_FRONT,GL_SHININESS,shiny);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
      //  Set spotlight parameters
      //glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,Direction);
      //glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,sco);
      //glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,Exp);
      //  Set attenuation
      glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION ,at0/100.0);
      glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION   ,at1/100.0);
      glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,at2/100.0);

   }else{
      glDisable(GL_LIGHTING);
   }
   
   glRotated(30,1,0,0);
   
   ground   (-7.00, -1.00, -3.00, 1,1,1, 0,0,0,0, 14, 15, 10);
   fountain(0, -0.75, 1, 0.3,0.3,0.3, 45,0,1,0,13);
   table(6 , 0.25,  2, 0.4, 0.4, 0.4, 45, 0, 1,0);



   update_particles(particles);
   draw_particles(particles, texture[0], texture[1]);

   // ----- flowers around pund
   plant_flower(-1.5, 0.3,  2.3,    0.3,0.3,0.3,  0, 0,0,0, 5);
   plant_flower( 1.5, 0.3,  2.3,    0.3,0.3,0.3,  20, 0,1,0, 6); 
   plant_flower(-1.5, 0.3,  0  ,    0.3,0.3,0.3,  30, 0,1,0, 7);
   plant_flower( 1.5, 0.3,  0  ,    0.3,0.3,0.3,  45, 0,1,0, 5); 

  // ---------- fences 


   double k;
   for (k=-6.8; k < 7; k=k+2){
      fence(k , 0.1,  -4, 0.1, 0.1, 0.1, 0, 0, 0,0, 'c');
  }
  fence(6.8 , 0.1,  -4, 0.1, 0.1, 0.1, 0, 0, 0,0, 'r');           


  //pine_tree(0   ,  0, 0, 0.80 ,0.80 , 0.80, 0, 0, 0, 0);

   pine_tree(-5   ,  1.6, -3.00, 0.80 ,0.80 , 0.80, 30, 0, 1, 0);
   pine_tree(-5.50,  1.0,  4.00, 0.60 ,0.60 , 0.60, 45, 0, 1, 0);
   pine_tree( 5.50,  1.0, -2.00, 0.60 ,0.60 , 0.60, 70, 0, 1, 0); 


 
   glDisable(GL_TEXTURE_2D);
   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
   glWindowPos2i(5,25);
   Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d ",ambient,diffuse,specular,emission);
   //Print("Angle=%d,%d  Dim=%.1f Projection=%s Light=%s", th,ph,dim,mode?"Perpective":"Orthogonal",light?"On":"Off");
   if (light)
   {
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d ",ambient,diffuse,specular,emission);
   }
   //  Render the scene and make it visible
   ErrCheck("display");   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}

//------------- end of my code 


/*
 *  GLUT calls this routine when the window is resized
 *  Adopted from ex13
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   if(!fp) {
      //  Right arrow key - increase angle by 5 degrees
      if (key == GLUT_KEY_RIGHT)
         th += 5;
      //  Left arrow key - decrease angle by 5 degrees
      else if (key == GLUT_KEY_LEFT)
         th -= 5;
      //  Up arrow key - increase elevation by 5 degrees
      else if (key == GLUT_KEY_UP)
         ph += 5;
      //  Down arrow key - decrease elevation by 5 degrees
      else if (key == GLUT_KEY_DOWN)
         ph -= 5;
      //  PageUp key - increase dim
      else if (key == GLUT_KEY_PAGE_UP && dim>1)
         dim -= 0.1;
      //  PageDown key - decrease dim
      else if (key == GLUT_KEY_PAGE_DOWN)
         dim += 0.1;
     //  Smooth color model


      //  Keep angles to +/-360 degrees
      th %= 360;
      ph %= 360;
   }
   //  Update projection
   myProject();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      X = Y = th = ph = 0;
   else if (ch == 'l' || ch == 'L')
      light = 1-light;

   //  Pause Light
   else if (ch == ' ') {
      move = 1-move;
   }
   //  Move light
   else if (ch == 'k')
      zh += 3;
   else if (ch == 'K')
      zh -= 3;
   else if (ch == 'y')
      ylight += 1;
   else if (ch == 'Y')
      ylight -= 1;

   //  Toggle first person
   else if (ch == 'f' || ch == 'F')
   {
      fp = 1-fp;
   }
   //  Increase/decrease spot azimuth
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   Th = Th%360;
   Ph = Ph%360;
   //  Reproject



   if (fp) {
      double dt = 0.05;
      if (ch == 'w' || ch == 'W'){
         Ex += Cx*dt; //Update the eye vector based on the camera vector
         Ez += Cz*dt;
      }
      else if (ch == 'a' || ch == 'A'){
         rot -= 3;
      }
      else if (ch == 's' || ch == 'S'){
         Ex -= Cx*dt;
         Ez -= Cz*dt;
      }
      else if (ch == 'd' || ch == 'D'){
         rot += 3;
      }

      //  Keep angles to +/-360 degrees
      rot %= 360;
   }
   else {
      //  Reset view angle
      if (ch == '0')
         th = ph = 0;
      //  Switch display mode
      else if (ch == 'm' || ch == 'M')
         mode = 1-mode;
      //  Change field of view angle
      else if (ch == '1')
         fov--;
      else if (ch == '2')
         fov++;
     //  z key - increase dim
      else if (ch == 'z')
         dim += 0.1;
      //  x - decrease dim
      else if (ch == 'x' && dim>1)
         dim -= 0.1;
   }
   
   //  Reproject
   myProject();
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   myProject();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(1000,750);
   glutCreateWindow("Amir Kashipazha, project");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);

   init_particles(particles);

   //Load Textures
   texture[0] = LoadTexBMP("textures/pine_tree.bmp");
   texture[1] = LoadTexBMP("textures/tree.bmp");
   texture[2] = LoadTexBMP("textures/pine_leaf.bmp");
   //texture[3] = LoadTexBMP("textures/lawn.bmp");
   texture[4] = LoadTexBMP("textures/stem_2.bmp");
   texture[5] = LoadTexBMP("textures/petal_r_y.bmp");
   texture[6] = LoadTexBMP("textures/petal_g_y.bmp");
   texture[7] = LoadTexBMP("textures/petal_y.bmp");
   //texture[8] = LoadTexBMP("textures/leaf_flower.bmp");
   texture[9] = LoadTexBMP("textures/water.bmp");
   texture[10] = LoadTexBMP("textures/brick.bmp");
   texture[11] = LoadTexBMP("textures/wood_1.bmp");
   texture[12] = LoadTexBMP("textures/rock_1.bmp");
   texture[13] = LoadTexBMP("textures/rock_2.bmp");
   texture[14] = LoadTexBMP("textures/lawn_2.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
