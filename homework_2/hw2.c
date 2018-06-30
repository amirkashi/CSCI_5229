
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
double zh=0;      //  Rotation
int fp = 0;       //  First Peson Toggle
int rot = 0.0;    //  Rotation for first person
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world
int axes=0;       //  Display axes
int mode=0;       //  Projection mode

double Ex = 0;
double Ey = 0;
double Ez = 5;
double Cx = 0; 
double Cz = 0;


//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))
#define PI 3.1416
/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   
   if(fp) {
      gluPerspective(fov,asp,dim/4,4*dim);
   }
   else {
   
	//  Perspective transformation
	if (mode)
      gluPerspective(fov,asp,dim/10,10*dim);
	//  Orthogonal projection
	else
		glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
	}
	//  Switch to manipulating the model matrix
	glMatrixMode(GL_MODELVIEW);
	//  Undo previous transformations
	glLoadIdentity();
}




/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */


// draw a cube with random color 
static void cube_random_color(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double rx, double ry, double rz)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,rx,ry,rz); //glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(
             ((double) rand() / (RAND_MAX)),
             ((double) rand() / (RAND_MAX)),
             ((double) rand() / (RAND_MAX))
            );
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(
             ((double) rand() / (RAND_MAX)),
             ((double) rand() / (RAND_MAX)),
             ((double) rand() / (RAND_MAX))
            );
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(
             ((double) rand() / (RAND_MAX)),
             ((double) rand() / (RAND_MAX)),
             ((double) rand() / (RAND_MAX))
            );
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(
             ((double) rand() / (RAND_MAX)),
             ((double) rand() / (RAND_MAX)),
             ((double) rand() / (RAND_MAX))
            );
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(
             ((double) rand() / (RAND_MAX)),
             ((double) rand() / (RAND_MAX)),
             ((double) rand() / (RAND_MAX))
            );
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(
             ((double) rand() / (RAND_MAX)),
             ((double) rand() / (RAND_MAX)),
             ((double) rand() / (RAND_MAX))
            );
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}




void drawTree(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double xx, double yy, double zz) {
   double i;
   double radius = 0.3, height = 0.2;

   glPushMatrix();
   //  Transform cube
   glTranslated(x,y,z);
   glRotated(th,xx,yy,zz); //   glRotated(th,0,0,1);
   glScaled(dx,dy,dz);

   /* tree trunk */
   //glColor3f(0.647059, 0.164706, 0.164706);
   glColor3f(0, 0.5, 0);
   glBegin(GL_QUAD_STRIP);
      for(i = 0.0; i <= 2.1*PI; i+=.05){
         glVertex3d(radius * cos(i), height, radius * sin(i));
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }
   glEnd();

   //glColor3f(0.647059, 0.164706, 0.164706);
   glColor3f(0, 1, 0);
   /* Top of Cylinder */
   glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, height, 0.0);

      for(i = 0.0; i < 2*PI*radius*4; i+=.125) {
         glVertex3d(radius * cos(i), height, radius * sin(i));
      }
   glEnd();
   /* Bottom of Cylinder */
   glColor3f(0, 1, 0);
   glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, -height, 0.0);

      for(i = 0.0; i < 2*PI*radius*4; i+=.125) {
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }
   
   glEnd();

   glPopMatrix();
}



static void vase_01(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_POLYGON);
   //  Bottom
   glColor3f(1,0,1);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   glEnd();

   // front 
   glBegin(GL_POLYGON);
   glColor3f(1,0,0);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1.75,+1, 1);
   glVertex3f(-1.75,+1, 1);
   glEnd();

   //  Right
   glBegin(GL_POLYGON);
   glColor3f(1,1,0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1.75,+1,-1);
   glVertex3f(+1.75,+1,+1);
   glEnd();
   //  Left
   glBegin(GL_POLYGON);
   glColor3f(0,1,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1.75,+1,+1);
   glVertex3f(-1.75,+1,-1);
   glEnd();
   //  Back
   glBegin(GL_POLYGON);
   glColor3f(0,0,1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1.75,+1,-1);
   glVertex3f(+1.75,+1,-1);
   glEnd();

   //  Top - soil surface 
   glBegin(GL_POLYGON);
   //glColor3f(0.5,0.35,0.05); // light brouwn
   glColor3f(0.35,0.25,0.2);
   glVertex3f(-1.65,+0.8,+1);
   glVertex3f(+1.65,+0.8,+1);
   glVertex3f(+1.65,+0.8,-1);
   glVertex3f(-1.65,+0.8,-1);
   glEnd();
   // tree trunk 
   drawTree(0   , 3   , 0    , 1.5, 18, 1.5,   0,  0, 0, 1);
   // branches 
   // left 
   drawTree(-2  , 2   , 0    , 0.4, 9 , 0.4, 105, 0, 0, 1);
   drawTree(-1.6, 3.25, 0    , 0.3, 7 , 0.3, 105, 0, 0, 1);
   drawTree(-1.4, 4.25, 0    , 0.2, 5 , 0.2, 105, 0, 0, 1);
   drawTree(-1  , 5.25, 0    , 0.1, 3 , 0.1, 105, 0, 0, 1);
   // right
   drawTree(2   , 2   , 0    , 0.4, 9 , 0.4, 75, 0, 0, 1);
   drawTree(1.6 , 3.25, 0    , 0.3, 7 , 0.3, 75, 0, 0, 1);
   drawTree(1.4 , 4.25, 0    , 0.2, 5 , 0.0, 75, 0, 0, 1);
   drawTree(1   , 5.25, 0    , 0.1, 3 , 0.1, 75, 0, 0, 1);
   // back
   drawTree(0   , 2.5 , -2    , 0.5, 9 , 0.5, 75, 10, 0, 0);  
   drawTree(0   , 3.5 , -1.75 , 0.3, 7 , 0.3, 75, 10, 0, 0);
   drawTree(0   , 4.5 , -1.25 , 0.2, 5, 0.2, 75, 10, 0, 0);
   drawTree(0   , 5.5 , -1    , 0.1, 3 , 0.1, 75, 10, 0, 0);
   // front 
   drawTree(0   , 2.5 , 2   , 0.5, 9 , 0.5, 105, 10, 0, 0);
   drawTree(0   , 3.5 , 1.2 , 0.3, 7 , 0.3, 105, 10, 0, 0);
   drawTree(0   , 4.5 , 1.1 , 0.2, 5 , 0.2, 105, 10, 0, 0);
   drawTree(0   , 5.5 , 1   , 0.1, 3 , 0.1, 105, 10, 0, 0);

   cube_random_color(0, 7.8, 0, 0.75, 0.75, 0.75, 45, 45, 0,45);

   // first branch 
   cube_random_color(3.5 , 1,   0, 0.4, 0.4, 0.4, 45, 45, 0,45);   
   cube_random_color(-3.5, 1,   0, 0.4, 0.4, 0.4, 45, 45, 0,45);   
   cube_random_color(0 , 1.5,  3.5, 0.4, 0.4, 0.4, 45, 45, 0,45);   
   cube_random_color(0 , 1.5, -3.5, 0.4, 0.4, 0.4, 45, 45, 0,45);   


   // second branch 
   cube_random_color(2, 2.8, 0, 0.2, 0.2, 0.2, 45, 45, 0,45);   
   cube_random_color(-2, 2.8, 0, 0.2, 0.2, 0.2, 45, 45, 0,45);   
   cube_random_color(0, 2.9, 2, 0.2, 0.2, 0.2, 45, 45, 0,45);   
   cube_random_color(0, 3, -2, 0.2, 0.2, 0.2, 45, 45, 0,45);   
   //  Undo transofrmations
   glPopMatrix();
}





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
   
   vase_01(-4 , -4 , 3, 0.1, 0.1, 0.1, 0);   
   vase_01(-1 , -4 , 2, 0.4, 0.4, 0.4, 45);   
   vase_01(2.5, -4 , 1, 0.15, 0.15, 0.15, 145);

   vase_01(-3.5 ,  -2 , 1, 0.3, 0.3, 0.3, 90);   
    
   vase_01(2.5  ,  -1 , 1, 0.6, 0.6, 0.6, 270);

   vase_01(-1 , 1 , -5, 0.3, 0.3, 0.3, 200);
   vase_01(-1 , 1 , 0, 0.3, 0.3, 0.3, 200);
   vase_01(-1 , 1 ,  5, 0.3, 0.3, 0.3, 200);

   vase_01(2.5, -4 , -3, 0.15, 0.15, 0.15, 145);
   vase_01(-3.5, 2.5 , -3, 0.15, 0.15, 0.15, 250);
   vase_01(-3.5, 2 , 0, 0.15, 0.15, 0.15, 250);
   vase_01(-3.5, 1.5 , 3, 0.15, 0.15, 0.15, 250);
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
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
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project();
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
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
      
   //  Toggle first person
   else if (ch == 'f' || ch == 'F')
   {
      fp = 1-fp;
   }

   if (fp) {
      double dt = 0.05;
      if (ch == 'w' || ch == 'W'){
         Ex += Cx*dt;
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
      else if (ch == '-' && ch>1)
         fov--;
      else if (ch == '+' && ch<179)
         fov++;
     //  z key - increase dim
      else if (ch == 'z')
         dim += 0.1;
      //  x - decrease dim
      else if (ch == 'x' && dim>1)
         dim -= 0.1;

   }
   
   //  Reproject
   Project();
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
   Project();
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
   glutInitWindowSize(600,600);
   glutCreateWindow("Amir Kashipazha: HW2, Drawing in 3D");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
