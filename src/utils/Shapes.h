#ifndef A25831C8_51A5_403F_B582_7BD4DC518B2A
#define A25831C8_51A5_403F_B582_7BD4DC518B2A

#include <GL/gl.h>

static const float PI = 3.1415926535;

/*
  materialStruct

  Lighting values of the materials

*/
typedef struct materialStruct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess;
} materialStruct;

/*
  cube

  PARAMS
    p_front     the material properties of the object
    flag        determines whether to apply markus.ppm or Marc_Dekamps.ppm texture
    texID       texture ID to bind

*/
void cube(const materialStruct* p_front, int flag, GLuint* texID); 

/*
  cylinder
  
  PARAMS
    p_front     the material properties of the object

*/
void cylinder(const materialStruct* p_front);

/*
  openedCone
  
  PARAMS
    p_front     the material properties of the object
    r           radius of one of the sides of the cylinder. 
                If radius is large, then it will be a deformed cylinder, 
                else it becomes a cone

*/
void openedCone(const materialStruct* p_front, float r);

/*
  gear
  complex shape that uses the cylinder implentation together with a 
  pyramid polygon to create a gear-like object.

  PARAMS
    p_front     the material properties of the object
    n_spikes    determines the number of pyramids (spikes) that the gear will have.

*/
void gear(const materialStruct* p_front, int n_spikes);

#endif /* A25831C8_51A5_403F_B582_7BD4DC518B2A */


