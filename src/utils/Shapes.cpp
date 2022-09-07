#include <cmath>
#include "utils/Shapes.h"

void cube(const materialStruct* p_front, int flag, GLuint* texID){
  
  // Here we give every face the same normal
  GLfloat normals[][3] = { {0.333, 0.333, 0.333 }, {0.333, 0.333, 0.333}, {0.333, 0.333, 0.333}, {0.3333, 0.3333, 0.333}};

  // Here we have permuted the first normal array
  // GLfloat normals[][3] = {  {-1., 0., 0.}, {0., 0., 1.}, {1., 0. ,0.}, {0., 0., -1.} };

  if (flag == 3)
  {
    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_front->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_front->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   p_front->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   p_front->shininess);
  }

  glNormal3fv(normals[0]);
  glBegin(GL_POLYGON);
    glVertex3f( 1.0, 0.,  1.0);
    glVertex3f( 1.0, 0., 0.);
    glVertex3f( 1.0,  1.0, 0.);
    glVertex3f( 1.0,  1.0,  1.0);
  glEnd();

  glNormal3fv(normals[3]);
  glBegin(GL_POLYGON);
    glVertex3f(0., 0., 0.);
    glVertex3f( 1.0, 0., 0.);
    glVertex3f( 1.0,  1.0, 0.);
    glVertex3f(0.,  1.0, 0.);
  glEnd();

  // window of the spaceship
  glNormal3fv(normals[2]);
  glEnable(GL_TEXTURE_2D);

  // set white color and enable markus.ppm or Marc_Dekamps.ppm texture
  glColor3f(1,1,1);
  if (flag == 0)
    glBindTexture(GL_TEXTURE_2D, texID[0]);
  else if (flag == 1)
    glBindTexture(GL_TEXTURE_2D, texID[1]);
  glBegin(GL_POLYGON);
    glTexCoord3f(0., 0., 1.0);        glVertex3f(0., 0., 1.0);
    glTexCoord3f(1.0, 0., 1.0);       glVertex3f( 1.0, 0., 1.0);
    glTexCoord3f(1.0,  1.0, 1.0);     glVertex3f( 1.0,  1.0, 1.0);
    glTexCoord3f(0.,  1.0, 1.0);      glVertex3f(0.,  1.0, 1.0);
  glEnd();
  if (flag == 0 || flag == 1)
    glDisable(GL_TEXTURE_2D);

  glNormal3fv(normals[1]);
  glBegin(GL_POLYGON);
    glVertex3f( 0., 0.,  1.0);
    glVertex3f( 0., 0., 0.);
    glVertex3f( 0.,  1.0, 0.);
    glVertex3f( 0.,  1.0,  1.0);
  glEnd();

  glNormal3fv(normals[1]);
  glBegin(GL_POLYGON);
    glVertex3f( 1.0, 0.,  1.0);
    glVertex3f( 0., 0., 1.0);
    glVertex3f( 0.,  0., 0.);
    glVertex3f( 1.0,  0.,  0.);
  glEnd();

  glNormal3fv(normals[1]);
  glBegin(GL_POLYGON);
    glVertex3f( 1.0, 1.0,  1.0);
    glVertex3f( 0., 1.0, 1.0);
    glVertex3f( 0.,  1.0, 0.);
    glVertex3f( 1.0,  1.0,  0.);
  glEnd();
}



void openedCone(const materialStruct* p_front, float r){

    int N        = 100;       // number of faces
    int n_div   =  1;         // number of height divisions
	
   glMaterialfv(GL_FRONT, GL_AMBIENT,    p_front->ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_front->diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR,   p_front->specular);
   glMaterialf(GL_FRONT, GL_SHININESS,   p_front->shininess);

  float x0, x1, y0, y1;

  float z_min = -1;
  float z_max = 1;

  float delta_z = (z_max - z_min)/n_div;
  
  for (int i = 0; i < N; i++){
    for(int i_z = 0; i_z < n_div; i_z++){
      x0 = cos(2*i*PI/N);
      x1 = cos(2*(i+1)*PI/N);
      y0 = sin(2*i*PI/N);
      y1 = sin(2*(i+1)*PI/N);

      float z = 0;
      glBegin(GL_POLYGON);
      glVertex3f(x0,y0,z);
        glNormal3f(x0,y0,0);
      glVertex3f(x1,y1,z);
        glNormal3f(x1,y1,0);
      glVertex3f(r*x1,r*y1,z+delta_z);  // apply radius to opposite side of cylinder
        glNormal3f(x1,y1,0);
      glVertex3f(r*x0,r*y0,z+delta_z);  // apply radius to opposite side of cylinder
        glNormal3f(x0,y0,0);
      glEnd();
    }
  }
}


void gear(const materialStruct* p_front, int n_spikes){
  
	int N        = n_spikes;    // This determines the number of faces of the cylinder
  int n_div   =  1;           // number of height divisions
   glMaterialfv(GL_FRONT, GL_AMBIENT,    p_front->ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_front->diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR,   p_front->specular);
   glMaterialf(GL_FRONT, GL_SHININESS,   p_front->shininess);
  
  float x0, x1, x2, x3, y0, y1, y2, y3;

  float z_min = 0.;
  float z_max =  1.;  // depth 1.0 max
  float r = 1.5;       // radius
  float delta_z = (z_max - z_min)/n_div;
  
  for (int i = 0; i < N; i++){
glBegin(GL_POLYGON);
    for(int i_z = 0; i_z < n_div; i_z++){
      x0 = cos(2*i*PI/N);
      x1 = cos(2*(i+1)*PI/N);
      x2 = r*cos(2*i*PI/N);
      x3 = r*cos(2*(i+1)*PI/N);
      y0 = sin(2*i*PI/N);
      y1 = sin(2*(i+1)*PI/N);
      y2 = r*sin(2*i*PI/N);
      y3 = r*sin(2*(i+1)*PI/N);

      // cylinder starts
      glVertex3f(x0,y0,0);    glNormal3f(x0,y0,0);
      glVertex3f(x2,y2,0);    glNormal3f(x2,y2,0);
      glVertex3f(x2,y2,0.5);  glNormal3f(x2,y2,0.5);
      glVertex3f(x0,y0,0.5);  glNormal3f(x0,y0,0.5);

      glVertex3f(x2,y2,0);    glNormal3f(x2,y2,0);
      glVertex3f(x3,y3,0);    glNormal3f(x3,y3,0);
      glVertex3f(x3,y3,0.5);  glNormal3f(x3,y3,0.5);
      glVertex3f(x2,y2,0.5);  glNormal3f(x2,y2,0.5);

      glVertex3f(x3,y3,0);    glNormal3f(x3,y3,0);
      glVertex3f(x1,y1,0);    glNormal3f(x1,y1,0);
      glVertex3f(x1,y1,0.5);  glNormal3f(x1,y1,0.5);
      glVertex3f(x3,y3,0.5);  glNormal3f(x3,y3,0.5);

      glVertex3f(x1,y1,0);    glNormal3f(x1,y1,0);  
      glVertex3f(x0,y0,0);    glNormal3f(x0,y0,0);  
      glVertex3f(x1,y1,0.5);  glNormal3f(x1,y1,0.5);
      glVertex3f(x0,y0,0.5);  glNormal3f(x0,y0,0.5);
      // cylinder ends

      // keep building on top of cylinder
      if (i%2==0)
      { // pyramid starts
        x0 = x2; // old x2
        x1 = x3; // old x3
        x2 = 2.*cos(2*i*PI/N);
        x3 = 2.*cos(2*(i+0.5)*PI/N);
        y0 = y2; 
        y1 = y3;
        y2 = 2.*sin(2*i*PI/N); 
        y3 = 2.*sin(2*(i+0.5)*PI/N);

        // join the two points int he middle to make a triangle
        x2 = x3;
        y2 = y3;
        float z = 0.25; // where the pyramid's top common vertex meets

        glVertex3f(x0,y0,0);    glNormal3f(x0,y0,0);
        glVertex3f(x2,y2,z);    glNormal3f(x2,y2,0);
        glVertex3f(x2,y2,z);  glNormal3f(x2,y2,0.5);
        glVertex3f(x0,y0,0.5);  glNormal3f(x0,y0,0.5);

        glVertex3f(x2,y2,z);    glNormal3f(x2,y2,0);
        glVertex3f(x3,y3,z);    glNormal3f(x3,y3,0);
        glVertex3f(x3,y3,z);  glNormal3f(x3,y3,0.5);
        glVertex3f(x2,y2,z);  glNormal3f(x2,y2,0.5);

        glVertex3f(x3,y3,z);    glNormal3f(x3,y3,0);
        glVertex3f(x1,y1,0);    glNormal3f(x1,y1,0);
        glVertex3f(x1,y1,0.5);  glNormal3f(x1,y1,0.5);
        glVertex3f(x3,y3,z);  glNormal3f(x3,y3,0.5);

        glVertex3f(x1,y1,0);    glNormal3f(x1,y1,0);
        glVertex3f(x0,y0,0);    glNormal3f(x0,y0,0);
        glVertex3f(x1,y1,0.5);  glNormal3f(x1,y1,0.5);
        glVertex3f(x0,y0,0.5);  glNormal3f(x0,y0,0.5);
      } // pyramid ends
    }
glEnd();
  }

}

void cylinder(const materialStruct* p_front){
   glMaterialfv(GL_FRONT, GL_AMBIENT,    p_front->ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_front->diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR,   p_front->specular);
   glMaterialf(GL_FRONT, GL_SHININESS,   p_front->shininess);
    int N        = 100; // This determines the number of faces of the cylinder
    int n_div   =  10;  // This determines the fineness of the cylinder along its length
  
  float x0, x1, y0, y1;

  float z_min = -1;
  float z_max =  1;

  float delta_z = (z_max - z_min)/n_div;
  
  for (int i = 0; i < N; i++){
    for(int i_z = 0; i_z < n_div; i_z++){
      x0 = cos(2*i*PI/N);
      x1 = cos(2*(i+1)*PI/N);
      y0 = sin(2*i*PI/N);
      y1 = sin(2*(i+1)*PI/N);

      float z = z_min + i_z*delta_z;
      glBegin(GL_POLYGON);
      glVertex3f(x0,y0,z);
      glNormal3f(x0,y0,0);
      glVertex3f(x1,y1,z);
      glNormal3f(x1,y1,0);
      glVertex3f(x1,y1,z+delta_z);
      glNormal3f(x1,y1,0);
      glVertex3f(x0,y0,z+delta_z);
      glNormal3f(x0,y0,0);
      glEnd();
    }
  }
}