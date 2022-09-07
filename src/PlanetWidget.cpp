#include <GL/glu.h>
#include "PlanetWidget.h"

// constructor
PlanetWidget::PlanetWidget(QWidget *parent, GLuint texID, double mass, double radius, 
                           double* pose, double* momentum, int i)
  : QGLWidget(parent),
    _texID(texID),
    _radius(radius),
    _mass(mass),
    index(i)
{ // constructor

  _pose[0] = pose[0];
  _pose[1] = pose[1];
  _pose[2] = pose[2];

  _momentum[0] = momentum[0];
  _momentum[1] = momentum[1];
  _momentum[2] = momentum[2];

} // constructor

