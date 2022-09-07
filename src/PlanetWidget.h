#ifndef __GL_POLYGON_WIDGET_H__
#define __GL_POLYGON_WIDGET_H__

#include <QGLWidget>

struct materialStruct;

class PlanetWidget: public QGLWidget
	{ // class PlanetWidget

	Q_OBJECT

	public:
		PlanetWidget(QWidget *parent, GLuint texID, double mass, double radius, double* pose, double* momentum, int i);
		GLuint _texID;
		double _radius;
		double _mass;
		double _pose[3];
		double _forceVector[3];
		double _momentum[3];
		int index;

	  
	}; // class PlanetWidget
	
#endif
