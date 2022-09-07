#include<QLabel>
#include "SceneWindow.h"


// constructor / destructor
SceneWindow::SceneWindow(QWidget *parent)
	: QWidget(parent)
	{ // constructor

	// create the window layout
	createHorizontalGroupBox();
	windowLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);

	// create main widget
	solarSystemWidget = new SolarSystemWidget(this);
	windowLayout->addWidget(solarSystemWidget);

	
	windowLayout->addWidget(horizontalGroupBox);
	

	pTimer = new QTimer;
        pTimer->start(10);

    connect(pTimer, SIGNAL(timeout()),  solarSystemWidget, SLOT(updateAngle()));

	// orbit speed or simulation speed
	connect(OrbitSpeed, SIGNAL(valueChanged(int)), solarSystemWidget, SLOT(updateOrbitSpeed(int)));

	// sun light diffusive intensity
	connect(Sunlight, SIGNAL(valueChanged(int)), solarSystemWidget, SLOT(updateSunLight(int)));

	// reset button
	connect(buttonR, SIGNAL(released()),  solarSystemWidget, SLOT(resetOrbitValues()));
	connect(buttonR, SIGNAL(released()),  this, SLOT(resetAllSliders()));

	// standard basis activation for debuging purposes
	connect(activateBasis, SIGNAL(released()),  solarSystemWidget, SLOT(activateStandardBasis()));
	connect(deactivateBasis, SIGNAL(released()),  solarSystemWidget, SLOT(deactivateStandardBasis()));

	// planets mass sliders
	connect(EarthSlider, SIGNAL(valueChanged(int)), solarSystemWidget, SLOT(updatePlanetEarthMass(int)));
	connect(mercurySlider, SIGNAL(valueChanged(int)), solarSystemWidget, SLOT(updatePlanetMercuryMass(int)));
	connect(venusSlider, SIGNAL(valueChanged(int)), solarSystemWidget, SLOT(updatePlanetVenusMass(int)));
	connect(marsSlider, SIGNAL(valueChanged(int)), solarSystemWidget, SLOT(updatePlanetMarsMass(int)));
	connect(jupyterSlider, SIGNAL(valueChanged(int)), solarSystemWidget, SLOT(updatePlanetJupyterMass(int)));
	connect(saturnSlider, SIGNAL(valueChanged(int)), solarSystemWidget, SLOT(updatePlanetSaturnMass(int)));
	connect(uranusSlider, SIGNAL(valueChanged(int)), solarSystemWidget, SLOT(updatePlanetUranusMass(int)));
	connect(neptuneSlider, SIGNAL(valueChanged(int)), solarSystemWidget, SLOT(updatePlanetNeptuneMass(int)));
	connect(spaceship, SIGNAL(valueChanged(int)), solarSystemWidget, SLOT(updateSpaceshipSpeed(int)));

	// engine sliders
	connect(right_engine, SIGNAL(valueChanged(int)), solarSystemWidget, SLOT(updateSpaceship_rightEngine(int)));
	connect(left_engine, SIGNAL(valueChanged(int)), solarSystemWidget, SLOT(updateSpaceship_leftEngine(int)));

	// spaceship planet to orbit
	connect(spaceship_panetOrbit, SIGNAL(currentIndexChanged(int)), solarSystemWidget, SLOT(setPlanetIndex(int)));


	} // constructor

SceneWindow::~SceneWindow()
	{ // destructor
	delete solarSystemWidget;
	delete EarthSlider;
	delete mercurySlider;
	delete venusSlider;
	delete marsSlider;
	delete jupyterSlider;
	delete saturnSlider;
	delete uranusSlider;
	delete neptuneSlider;
	delete windowLayout;
	delete buttonR;
	delete Sunlight;
	delete OrbitSpeed;
	delete spaceship;
	delete right_engine;
	delete left_engine;
	} // destructor

// resets all the interface elements
void SceneWindow::ResetInterface()
	{ // ResetInterface()
	
	// now force refresh
	solarSystemWidget->update();
	update();
	} // ResetInterface()


void SceneWindow::createHorizontalGroupBox()
{
    horizontalGroupBox = new QGroupBox(tr(""),this);
	horizontalGroupBox->setMinimumSize(112, 612);
	horizontalGroupBox->setMaximumSize(212, 612);
    QVBoxLayout *layout = new QVBoxLayout(this);

    // buttons
	buttonR = new QPushButton(tr("Reset scene"));
	layout->addWidget(buttonR);
	activateBasis = new QPushButton(tr("Activate standard basis"));
	layout->addWidget(activateBasis);
	deactivateBasis = new QPushButton(tr("Deactivate standard basis"));
	layout->addWidget(deactivateBasis);

	// sliders
	Sunlight = new QSlider(Qt::Horizontal);
	Sunlight->setMinimum(-10);
	Sunlight->setMaximum(10);
	Sunlight->setValue(10);

	OrbitSpeed = new QSlider(Qt::Horizontal);
	OrbitSpeed->setMinimum(0);
	OrbitSpeed->setMaximum(100);
	OrbitSpeed->setValue(0); 

	EarthSlider = new QSlider(Qt::Horizontal);
	EarthSlider->setMinimum(10);
	EarthSlider->setMaximum(30);
	EarthSlider->setValue(20);

	mercurySlider = new QSlider(Qt::Horizontal);
	mercurySlider->setMinimum(10);
	mercurySlider->setMaximum(30);
	mercurySlider->setValue(20);

	venusSlider = new QSlider(Qt::Horizontal);
	venusSlider->setMinimum(10);
	venusSlider->setMaximum(30);
	venusSlider->setValue(10);

	marsSlider = new QSlider(Qt::Horizontal);
	marsSlider->setMinimum(10);
	marsSlider->setMaximum(30);
	marsSlider->setValue(20);

	jupyterSlider = new QSlider(Qt::Horizontal);
	jupyterSlider->setMinimum(10);
	jupyterSlider->setMaximum(30);
	jupyterSlider->setValue(20);

	saturnSlider = new QSlider(Qt::Horizontal);
	saturnSlider->setMinimum(10);
	saturnSlider->setMaximum(30);
	saturnSlider->setValue(20);

	uranusSlider = new QSlider(Qt::Horizontal);
	uranusSlider->setMinimum(10);
	uranusSlider->setMaximum(30);
	uranusSlider->setValue(10);

	neptuneSlider = new QSlider(Qt::Horizontal);
	neptuneSlider->setMinimum(10);
	neptuneSlider->setMaximum(30);
	neptuneSlider->setValue(10);

	spaceship = new QSlider(Qt::Horizontal);
	spaceship->setMinimum(10);
	spaceship->setMaximum(100);
	spaceship->setValue(10);

	right_engine = new QSlider(Qt::Horizontal);
	right_engine->setMinimum(10);
	right_engine->setMaximum(3600);
	right_engine->setValue(10);

	left_engine = new QSlider(Qt::Horizontal);
	left_engine->setMinimum(10);
	left_engine->setMaximum(3600);
	left_engine->setValue(10);

	// Planet that the spaceship orbits around
    spaceship_panetOrbit = new QComboBox();
    spaceship_panetOrbit->addItem("Mercury");
    spaceship_panetOrbit->addItem("Venus");
    spaceship_panetOrbit->addItem("Earth");
	spaceship_panetOrbit->addItem("Mars");
	spaceship_panetOrbit->addItem("Jupyter");
	spaceship_panetOrbit->addItem("Saturn");
	spaceship_panetOrbit->addItem("Uranus");
	spaceship_panetOrbit->addItem("Neptune");
    
	layout->addWidget(new QLabel(tr("Sun light intensity")));
	layout->addWidget(Sunlight);

	layout->addWidget(new QLabel(tr("Simulation Speed")));
	layout->addWidget(OrbitSpeed);
	
	layout->addWidget(new QLabel(tr("Earth Mass")));
	layout->addWidget(EarthSlider);

	layout->addWidget(new QLabel(tr("Mercury Mass")));
	layout->addWidget(mercurySlider);
	
	layout->addWidget(new QLabel(tr("Venus Mass")));
	layout->addWidget(venusSlider);
	
	layout->addWidget(new QLabel(tr("Mars Mass")));
	layout->addWidget(marsSlider);

	layout->addWidget(new QLabel(tr("Jupiter Mass")));
	layout->addWidget(jupyterSlider);

	layout->addWidget(new QLabel(tr("Saturn Mass")));
	layout->addWidget(saturnSlider);

	layout->addWidget(new QLabel(tr("Uranus Mass")));
	layout->addWidget(uranusSlider);

	layout->addWidget(new QLabel(tr("Neptune Mass")));
	layout->addWidget(neptuneSlider);

	layout->addWidget(new QLabel(tr("Spaceship speed")));
	layout->addWidget(spaceship);

	layout->addWidget(new QLabel(tr("Spaceship right engine")));
	layout->addWidget(right_engine);

	layout->addWidget(new QLabel(tr("Spaceship left engine")));
	layout->addWidget(left_engine);

	layout->addWidget(new QLabel(tr("Spaceship orbiting planet")));
	layout->addWidget(spaceship_panetOrbit);

    horizontalGroupBox->setLayout(layout);
}

void SceneWindow::resetAllSliders()
{
	Sunlight->setValue(10);
	OrbitSpeed->setValue(0); 
	EarthSlider->setValue(20); 
	mercurySlider->setValue(20);
	venusSlider->setValue(10);
	marsSlider->setValue(20);
	jupyterSlider->setValue(20);
	saturnSlider->setValue(20);
	uranusSlider->setValue(10);
	neptuneSlider->setValue(10);
	spaceship->setValue(10);
	right_engine->setValue(10);
	left_engine->setValue(10);
	
	solarSystemWidget->update();
	update();
}