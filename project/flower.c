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




void flower_1(double x,double y,double z,
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





