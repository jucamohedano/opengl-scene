#ifndef __GL_POLYGON_WINDOW_G__
#define __GL_POLYGON_WINDOW_G__ 1

#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QVector3D>
#include <QMatrix4x4>

#include "include/FreeImage/Dist/FreeImagePlus.h"
#include "GL/gl.h"



class PlanetWidget;
struct materialStruct;


class SolarSystemWidget: public QGLWidget
{ 
    Q_OBJECT
    
    public:
    SolarSystemWidget(QWidget *parent);

    // planets
    PlanetWidget* sun;
    PlanetWidget* mercury;
    PlanetWidget* venus;
    PlanetWidget* earth;
    PlanetWidget* mars;
    PlanetWidget* jupyter;
    PlanetWidget* saturn;
    PlanetWidget* uranus;
    PlanetWidget* neptune;

    public slots:
    void updateAngle();
    void updateSunLight(int);
    void activateStandardBasis();
    void deactivateStandardBasis();
    void resetOrbitValues();
    void updateOrbitSpeed(int n);
    void updatePlanetEarthMass(int n);
    void updatePlanetMercuryMass(int n);
    void updatePlanetVenusMass(int n);
    void updatePlanetMarsMass(int n);
    void updatePlanetJupyterMass(int n);
    void updatePlanetSaturnMass(int n);
    void updatePlanetUranusMass(int n);
    void updatePlanetNeptuneMass(int n);
    void updateSpaceshipSpeed(int);
    void updateSpaceship_rightEngine(int);
    void updateSpaceship_leftEngine(int);
    void setPlanetIndex(int);
    

    protected:
	// called when OpenGL context is set up
	void initializeGL();
	// called every time the widget is resized
	void resizeGL(int w, int h);
	// called every time the widget needs painting
	void paintGL();
    void drawPlanet(PlanetWidget* planet);
    void drawAxis();
    void renderCubemap();

    private:
    void planet(int texID);
    void normalize(GLfloat*);
    void loadTextures();
    void loadCubemap();
    void createSolarSystem();
    double* gforce(PlanetWidget* p1, PlanetWidget* p2); // calculates gravitational force
    double* addVectors(double* vectors[], int size);
    void updatePlanetOrbit(PlanetWidget* p1);

    // user interactive mouse functions
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    

    void updateModelViewMatrix();
    QVector3D calculateArcBallVector(int x, int y);

    QMatrix4x4 mModelMatrix;
    QMatrix4x4 mRotate;
    QMatrix4x4 mViewMatrix;
    QVector3D rotAxis;
    
    float speedInc;
    float sunlightIntensity;
    double _time;
    double _time_sinewave;
    int scribble, rotate;
    float x, y, z;
    float xStart, yStart, zStart, xEnd, yEnd, zEnd; // old mouse pose
    float scrollDelta;
    int drawStandardBasis;
    float spaceshipSpeed;
    float left_engine_deg;
    float right_engine_deg;
    int planetIndex;

    QVector3D objSpaceRotAxis;
    float angle;

    int mWidth;
    int mHeight;


    constexpr static int nTex = 11; // number of textures
    constexpr static int nTexCube = 6;
    GLuint texID[nTex];  // Texture ID's for the three textures.
    GLuint texIDcube;

    int width[nTex];
    int height[nTex];

    int widthCube[nTexCube];
    int heightCube[nTexCube];

    BYTE* iData[nTex];
    BYTE* iDataCube[nTexCube];

    const char* textures[11] = {
    "textures/markus.ppm",
    "textures/Marc_Dekamps.ppm",
    "textures/sun.jpg",
    "textures/mercury.jpg",
    "textures/venus_surface.jpg",
    "textures/Mercator-projection.ppm",
    "textures/mars.jpg",
    "textures/jupyter.jpg",
    "textures/saturn.jpg",
    "textures/uranus.jpg",
    "textures/neptune.jpg"
};

const char* cubemapTextures[6] = {
    "textures/skybox/right.png",
    "textures/skybox/left.png",
    "textures/skybox/top.png",
    "textures/skybox/bottom.png",
    "textures/skybox/back.png",
    "textures/skybox/front.png"
};

};
	
#endif
