#include "prj.h"

void grass_block(double x,double y,double z,double dx_mat[5][5],double th_mat[5][5],
                 unsigned int tex,unsigned int tex_b) {
  glPushMatrix();
  // Translate as desired
  glTranslated(x,y,z);
  glScaled(.2,.4,.2);
  
  int i;
  int j;
  float dx;
  float th;
  // Make 16 blades of grass inside a unit box.
  // Each blade is rotated a random amount and
  // scaled within a random amount (a reasonable scale)
/*
  for (i=0;i<5;i++) {
    for (j=0;j<5;j++) {
      dx = dx_mat[i][j];
      th = th_mat[i][j];
      grass_blade((float)i/5,0,(float)j/5 , dx,7.5*dx,.1 , th,0,0 , tex);
    }
  }
*/
  float white[] = {221.0/255.0,213.0/255.0,242.0/255.0,1};
  float shinyvec[1];
  shinyvec[0] = 2.0;
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,tex_b);

  glColor3f(.0025,.0025,.0025);

  glBegin(GL_QUADS);
  glNormal3f(0,1,0);
  glTexCoord2f(0,1);
  glVertex3f(0,0,0);

  glNormal3f(0,1,0);
  glTexCoord2f(1,1);
  glVertex3f(1,0,0);

  glNormal3f(0,1,0);
  glTexCoord2f(1,0);
  glVertex3f(1,0,1);

  glNormal3f(0,1,0);
  glTexCoord2f(0,0);
  glVertex3f(0,0,1);

  glEnd();

  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
}
