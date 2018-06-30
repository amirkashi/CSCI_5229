#ifndef CSCIx229
#define CSCIx229

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

#ifdef __cplusplus
extern "C" {
#endif
// ------------------------------------
// PARTICLE TYPE
typedef struct {
  // Location
  double x;
  double y;
  double z;
  // horizontal motion
  double x_m;
  double z_m;
  // Color
  double r;
  double g;
  double b;
  // Rotation angle
  double th;
  // vertical acceleration
  double accel;
  // vertical deceleration
  double decel;
  // Scaling
  double z_sc;
}PARTICLES;



// ------------------------------------

void Print(const char* format , ...);
void Fatal(const char* format , ...);
unsigned int LoadTexBMP(const char* file);
void Project(double fov,double asp,double dim);
void ErrCheck(const char* where);
int  LoadOBJ(const char* file);

#ifdef __cplusplus
}
#endif

#endif
