#ifndef A9E36041_4130_4D53_8670_2278B8F39A6E
#define A9E36041_4130_4D53_8670_2278B8F39A6E

#include "utils/Materials.h"
#include "utils/Shapes.h"
#include "GL/gl.h"
#include "GL/glu.h"

/*
  drawSpaceship

  PARAMS
    GLuint* texID               array of texture IDs
    float _time_sinewave        input value to the sine wave function determined by _time increments
    float left                  angle of rotation for the left engine
    float right                 angle of rotation for the right engine
*/
void drawSpaceship(GLuint* texID, float _time_sinewave, float left, float right);

/*
  left_engine

  PARAMS
    float                       angle of rotation for the left engine
*/
void left_engine(float);

/*
  right_engine

  PARAMS
    float                       angle of rotation for the right engine
*/
void right_engine(float);

#endif /* A9E36041_4130_4D53_8670_2278B8F39A6E */

