
 
#include "CSCIx229.h"

#define PI 3.1416

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int fp=0;
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
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
int one       =   1;  // Unit value
int distance  =   10;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light

unsigned int texture[5]; // Texture names

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




// mye code here 
/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */

void front(){         //Builds bottom of the piece
    
   glEnable(GL_TEXTURE_2D);
//   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_REPLACE);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[3]);
    glBegin(GL_QUADS);
   
   glNormal3f( 0, 0, +1);
   glTexCoord2f(0,0);glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0);glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1);glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1);glVertex3f(-1,+1, 1);

    glEnd();

  
}

void back(){

   glEnable(GL_TEXTURE_2D);
//   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_REPLACE);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[3]);
    glBegin(GL_QUADS);
   
   glNormal3f( 0, 0, -1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);

    glEnd();

}

/*
void cube(){
   glPushMatrix();
   
   glNormal3f( 0, 0, +1);
   front();
   glNormal3f( 0, 0, -1);
   back();

   glNormal3f( +1, 0, 0);
   glRotated(90,0,1,0);  //rigth
   front();
   
   glNormal3f( -1, 0, 0);
   glRotated(180,0,1,0); //left
   front();
   //glTranslated(2,0,0);

   glNormal3f( 0, -1, 0);
   glRotated(90,1,0,0); // bt
   front();
   glNormal3f( 0, +1, 0);
   glRotated(180,1,0,0);  //top 
   front();


   glPopMatrix();

}
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

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D, texture[3]);

   glNormal3f( 0, 0, 1);
   front();

   back();

   //glTranslated(1,0,0);
   glRotated(90,0,1,0);  //rigth
   front();
   //glTranslated(1,0,0);
   glRotated(180,0,1,0); //left
   front();
   //glTranslated(2,0,0);
   glNormal3f( 0, -1, 0);
   glRotated(90,1,0,0); // bt
   front();
   //glTranslated(2,0,0);
   glNormal3f( 0, 2, 2);
   glRotated(180,1,0,0);  //top 
   front();

   //  Undo transofrmations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}




void drawTree(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double xx, double yy, double zz, unsigned int body, unsigned int top, unsigned int bt) {
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
   for (j = 0; j <= 360; j+=.125) {
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



static void plant(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);


   // draw vase 
   drawTree(0   , -0.1   , 0    , 5, 5, 5,   0,  0, 0, 1, 1, 1, 4);
   // tree trunk 
   drawTree(0   , 3   , 0    , 1.5, 18, 1.5,   0,  0, 0, 1, 0, 0, 0);
   // branches 
   // left 
   drawTree(-2  , 2   , 0    , 0.4, 9 , 0.4, 105, 0, 0, 1, 0, 0, 0);
   drawTree(-1.6, 3.25, 0    , 0.3, 7 , 0.3, 105, 0, 0, 1, 0, 0, 0);
   drawTree(-1.4, 4.25, 0    , 0.2, 5 , 0.2, 105, 0, 0, 1, 0, 0, 0);
   drawTree(-1  , 5.25, 0    , 0.1, 3 , 0.1, 105, 0, 0, 1, 0, 0, 0);
   // right
   drawTree(2   , 2   , 0    , 0.4, 9 , 0.4, 75, 0, 0, 1, 0, 0, 0);
   drawTree(1.6 , 3.25, 0    , 0.3, 7 , 0.3, 75, 0, 0, 1, 0, 0, 0);
   drawTree(1.4 , 4.25, 0    , 0.2, 5 , 0.0, 75, 0, 0, 1, 0, 0, 0);
   drawTree(1   , 5.25, 0    , 0.1, 3 , 0.1, 75, 0, 0, 1, 0, 0, 0);
   // back
   drawTree(0   , 2.5 , -2    , 0.5, 9 , 0.5, 75, 10, 0, 0, 0, 0, 0);  
   drawTree(0   , 3.5 , -1.75 , 0.3, 7 , 0.3, 75, 10, 0, 0, 0, 0, 0);
   drawTree(0   , 4.5 , -1.25 , 0.2, 5, 0.2, 75, 10, 0, 0, 0, 0, 0);
   drawTree(0   , 5.5 , -1    , 0.1, 3 , 0.1, 75, 10, 0, 0, 0, 0, 0);
   // front 
   drawTree(0   , 2.5 , 2   , 0.5, 9 , 0.5, 105, 10, 0, 0, 0, 0, 0);
   drawTree(0   , 3.5 , 1.2 , 0.3, 7 , 0.3, 105, 10, 0, 0, 0, 0, 0);
   drawTree(0   , 4.5 , 1.1 , 0.2, 5 , 0.2, 105, 10, 0, 0, 0, 0, 0);
   drawTree(0   , 5.5 , 1   , 0.1, 3 , 0.1, 105, 10, 0, 0, 0, 0, 0);

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
   glDisable(GL_TEXTURE_2D);
}



// Draw lengthy box that will serve as ground for objects
static void drawGround(double x, double y, double z, 
                       double dx,double dy,double dz)
{
   glPushMatrix();

   glTranslated(x, y, z);   
   glScaled(dx, dy, dz);

   glEnable(GL_TEXTURE_2D);
   //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D, texture[2]);

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
   glTexCoord2f(1,0); glVertex3f(+0,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+0,+1,0);
   glTexCoord2f(0,1); glVertex3f(-1,+1,0);

   glNormal3f(0,+1,0);
   glTexCoord2f(0,0); glVertex3f(0,+1,0);
   glTexCoord2f(1,0); glVertex3f(+0,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,0);

   glNormal3f(0,+1,0);
   glTexCoord2f(0,0); glVertex3f(0,+1,0);
   glTexCoord2f(1,0); glVertex3f(+1,+1,0);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(0,+1,-1);

   glNormal3f(0,+1,0);
   glTexCoord2f(0,0); glVertex3f(0,1,0);
   glTexCoord2f(1,0); glVertex3f(0,+1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,0);         
   //  Bottom
   glNormal3f(0, -1, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1); 
   glEnd();  

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
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
   

   glShadeModel(GL_SMOOTH);
   //  Translate intensity to color vectors
   float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
   float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
   float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
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
   //  glColor sets ambient and diffuse color materials
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);
   //  Enable light 0
   glEnable(GL_LIGHT0);
   //  Set ambient, diffuse, specular components and position of light 0
   glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
   glLightfv(GL_LIGHT0,GL_POSITION,Position);



   plant(-3.5 , -1.6 , 1, 0.3, 0.3, 0.3, 90);   
   plant(2.5  , -1.3 , 1, 0.6, 0.6, 0.6, 270);
   plant(-1   , -1.6 , 0, 0.3, 0.3, 0.3, 200);
   plant(-3.5 , -1.8 , -3, 0.15, 0.15, 0.15, 250);

   drawGround(0, -2, 0, 5, 0.1, 5); 

  //cube_random_color(2, -2, 0, 2, 2, 2, 45, 0, 0,0); 

  //cube();


   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
   //  Render the scene and make it visible
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
      th = ph = 0;
   //  Pause Light
   else if (ch == ' ') {
      move = 1-move;
   }
   //  Move light
   else if (ch == 'l')
      zh += 3;
   else if (ch == 'k')
      zh -= 3;
   //  Toggle first person
   else if (ch == 'f' || ch == 'F')
   {
      fp = 1-fp;
   }

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
   glutInitWindowSize(750,750);
   glutCreateWindow("Amir Kashipazha, HW 3");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //Load Textures
   texture[0] = LoadTexBMP("tree.bmp");
   texture[1] = LoadTexBMP("vase.bmp");
   texture[2] = LoadTexBMP("rock1.bmp");
   texture[3] = LoadTexBMP("block.bmp");
   texture[4] = LoadTexBMP("soil.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
