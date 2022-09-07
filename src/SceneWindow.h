#ifndef __GL_POLYGON_WINDOW_H__
#define __GL_POLYGON_WINDOW_H__ 1

#include <QGLWidget>
#include <QMenuBar>
#include <QSlider>
#include <QTimer>
#include <QBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QComboBox>


#include "SolarSystemWidget.h"

class SceneWindow: public QWidget
	{
	Q_OBJECT

	public slots:
		void resetAllSliders();

	public:
	
	// constructor / destructor
	SceneWindow(QWidget *parent);
	~SceneWindow();

	// visual hierarchy
	// menu bar
	enum { NumGridRows = 1, NumButtons = 1 };
	QGroupBox *horizontalGroupBox;
    // QPushButton *buttons[NumButtons];
	// void createMenu();
	void createHorizontalGroupBox();
	// void moveMouseEvent(QMouseEvent* event);


	// window layout
	QBoxLayout *windowLayout;

	// beneath that, the main widget
	SolarSystemWidget* solarSystemWidget;
	// and a slider for the number of vertices
	QSlider *Sunlight;
	QSlider* OrbitSpeed;
	QSlider *EarthSlider;
	QSlider *mercurySlider;
	QSlider *venusSlider;
	QSlider *earthSlider;
	QSlider *marsSlider;
	QSlider *jupyterSlider;
	QSlider *saturnSlider;
	QSlider *uranusSlider;
	QSlider *neptuneSlider;
	QSlider *spaceship;
	QSlider *right_engine;
	QSlider *left_engine;
	QComboBox *spaceship_panetOrbit;

	QPushButton* buttonR;
	QPushButton* activateBasis;
	QPushButton* deactivateBasis;

	QTimer   *pTimer;
	  
	// resets all the interface elements
	void ResetInterface();
	
	};
	
#endif
