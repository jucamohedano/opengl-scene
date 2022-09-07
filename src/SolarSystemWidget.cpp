#include <GL/gl.h>
#include <GL/glu.h>

#include <QGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QEvent>
#include <QVector3D>

#include <QtMath>

#include <iostream>

#include "utils/Materials.h"
#include "utils/Shapes.h"
#include "utils/Spaceship.h"
#include "PlanetWidget.h"
#include "SolarSystemWidget.h"


// constructor
SolarSystemWidget::SolarSystemWidget(QWidget *parent)
    : QGLWidget(parent)
    {
        speedInc = 0.000001;
        scribble = 0;
        rotate = 0;
        _time_sinewave = 0;
        spaceshipSpeed = 1;
        left_engine_deg = 1.;
        right_engine_deg = 1.;
        planetIndex = 0;

        xStart = yStart = zStart = xEnd = yEnd = zEnd = 0;
        drawStandardBasis = 0;
        sunlightIntensity = 1;

        angle = 0;
        objSpaceRotAxis = {0.,0.,0.};


        /*
        For all bodies in the solar system,
            momentum is static in the x direction
            constant momentum in the y direction
            increasing momentum in the z direction
        */
        double sunPose[3] = {0.0, 0.0, 0.0};
        double sunMomentum[3] = {0.0, 0.0, 0.0};
        sun = new PlanetWidget(this, 2, 1000.0, 1.8, sunPose, sunMomentum, -1);

        double mercuryPose[3] = {3.0,0.0,0.0};
        double mercuryMomentum[3] = {0.0, 1.0, 35.0};
        mercury = new PlanetWidget(this, 3, 2.0, 0.1, mercuryPose, mercuryMomentum, 0);

        double venusPose[3] = {3.7,0.0,0.0};  
        double venusMomentum[3] = {0.0, 1.0, 15.0};
        venus = new PlanetWidget(this, 4, 1., 0.17, venusPose, venusMomentum, 1);

        double earthPose[3] = {4.2,0.0,0.0};
        double earthMomentum[3] = {0.0, 1.0, 30.0};
        earth = new PlanetWidget(this, 5, 2.0, 0.17, earthPose, earthMomentum, 2);

        double marsPose[3] = {4.7,0.0,0.0};
        double marsMomentum[3] = {0.0, 1.0, 32.0};
        mars = new PlanetWidget(this, 6, 2., 0.12, marsPose, marsMomentum, 3);
        
        double jupyterPose[3] = {5.3,0.0,0.0};
        double jupyterMomentum[3] = {0.0, 1.0, 33.0};
        jupyter = new PlanetWidget(this, 7, 2.0, 0.3, jupyterPose, jupyterMomentum, 4);
        
        double saturnPose[3] = {6.5,0.0,0.0};
        double saturnMomentum[3] = {0.0, 1.0, 30.0};
        saturn = new PlanetWidget(this, 8, 2.0, 0.27, saturnPose, saturnMomentum, 5);
        
        double uranusPose[3] = {7.9,0.0,0.0};
        double uranusMomentum[3] = {0.0, 1.0, 14.0};
        uranus = new PlanetWidget(this, 9, 1., 0.18, uranusPose, uranusMomentum, 6);

        double neptunePose[3] = {8.9,0.0,0.0};
        double neptuneMomentum[3] = {0.0, 1.0, 13.5};
        neptune = new PlanetWidget(this, 10, 1., 0.18, neptunePose, neptuneMomentum, 7);
    }

void SolarSystemWidget::wheelEvent(QWheelEvent *event){
    scrollDelta +=  event->delta() / 120;
}

void SolarSystemWidget::mousePressEvent(QMouseEvent* event)
{
  rotate = 0;
  if(event->button() == Qt::LeftButton)
  {
    // set mouse starting and ending positions
    xStart = event->x(); 
    yStart = event->y(); 

    xEnd = event->x();
    yEnd = event->y();

    // set true to use arcball camera
    scribble = 1; 
    rotate = 1;
  }
}

void SolarSystemWidget::mouseReleaseEvent(QMouseEvent* event)
{
  // stop moving and do not use arcball camera
  if (event->button() == Qt::LeftButton) {
    scribble = 0;
    rotate = 0;
  }
}

void SolarSystemWidget::mouseMoveEvent(QMouseEvent *event){
  if(event->buttons() && Qt::LeftButton)
  {
    if(rotate)
    { // update mouse positions
      xEnd = event->x();
      yEnd = event->y();
      updateModelViewMatrix(); // refresh modelview matrix, lookAt pose
    }
    xStart = event->x();
    yStart = event->y();
  }

}

void SolarSystemWidget::updateModelViewMatrix()
{
  QVector3D v = calculateArcBallVector(xStart, yStart); // from the mouse
  QVector3D u = calculateArcBallVector(xEnd, yEnd);

  // angle to rotate
  angle = std::acos(std::min(1.0f, QVector3D::dotProduct(v, u))); // min to avoid acos to give us values > 1

  rotAxis = QVector3D::crossProduct(v,u); // axis that we will rotate about

  float current_matrix[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, current_matrix); // get current MODELVIEW matrix
  mRotate = QMatrix4x4(current_matrix);
  QMatrix4x4 eye2ObjSpaceMat = mRotate;

  // QMatrix4x4 * QVector3D
  // Returns the result of transforming point according to matrix, with the matrix applied pre-point
  objSpaceRotAxis = eye2ObjSpaceMat * rotAxis;

  xStart = xEnd;
  yStart = yEnd;
}

QVector3D SolarSystemWidget::calculateArcBallVector(int x, int y)
{
  // convert x and y screen coordinates to normalised device coordinates (NDC), ignoring z component
  QVector3D pt_ndc = QVector3D(2.0 * x / mWidth - 1.0, 2.0 * y / mHeight  - 1.0 , 0);
  pt_ndc.setY(pt_ndc.y() * -1);
  
  /*
  Computes z-coordinates for (x',y') NDC by trying to map them to points on the surface of a sphere
  of radius 1, centerted at the origin of our NDC system
  compute z-coordinates by executing pythagoras theorem with two conditions:
        z <= 1.0 or z > 1
  */
  float z = pt_ndc.x() * pt_ndc.x() + pt_ndc.y() * pt_ndc.y();

  if(z <= 1.0)
      pt_ndc.setZ(std::sqrt(1.0 - z));
  else
      pt_ndc.normalize();

  return pt_ndc;
}

void SolarSystemWidget::activateStandardBasis()
{
  printf("Ativate\n");
  drawStandardBasis = 1;
  this->repaint();
}

void SolarSystemWidget::deactivateStandardBasis()
{
  printf("Deactivate\n");
  drawStandardBasis = 0;
  this->repaint();
}

// called when OpenGL context is set up
void SolarSystemWidget::initializeGL()
{ // initializeGL()
     // Model matrix : an identity matrix (model will be at the origin)
    mModelMatrix.setToIdentity();
    mRotate.setToIdentity();
    mViewMatrix.setToIdentity();



    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(atan(tan(100.0 * 3.14159 / 360.0) / 1.0) * 360.0 / 3.141593, 1.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1., 1., 1.,
             0., 0., 0.,
             0.0, 1.0, 0.0);



    loadTextures();
    this->loadCubemap();


} // initializeGL()

// called every time the widget is resized
void SolarSystemWidget::resizeGL(int w, int h)
{ // resizeGL()
  // set the viewport to the entire widget - no
    int side = qMin(w, h);
    glViewport((w - side) / 2, (h - side) / 2, side, side);

    mWidth = side;
    mHeight = side;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(atan(tan(100.0 * 3.14159 / 360.0) / 1.0) * 360.0 / 3.141593, 1.0, 0.1, 100.0);
    // glOrtho(-1.,1.,-1.,1.,-1.,1);

} // resizeGL()

void SolarSystemWidget::normalize(GLfloat* vector)
{
  GLfloat norm = sqrt(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
  vector[0] /= norm;
  vector[1] /= norm;
  vector[2] /= norm;
}

// called every time the widget needs painting
void SolarSystemWidget::paintGL()
{ // paintGL()
    // clear the widget


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_NORMALIZE);
      glShadeModel(GL_FLAT);

    // You must set the matrix mode to model view directly before enabling the depth test

    glMatrixMode(GL_MODELVIEW);
      glEnable(GL_DEPTH_TEST);
      
      if (drawStandardBasis)
        drawAxis();


    glColor3f(1.0, 1.0, 1.0); // set color white so that textures are not colored on top
    glEnable(GL_TEXTURE_2D);
	  glEnable(GL_TEXTURE_CUBE_MAP);
    glDisable(GL_LIGHTING);
    this->renderCubemap();

    createSolarSystem();

    // flush to screen
    glFlush();

} // paintGL()

void SolarSystemWidget::drawAxis()
{
  glPushAttrib(GL_LIGHTING_BIT);
  glEnable(GL_COLOR_MATERIAL);
    glPushMatrix();
    glLineWidth(10.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(100.0, 0.0, 0.0);
    glEnd();
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 100.0, 0.0);
    glEnd();
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 100.0);
    glEnd();
    glPopMatrix();
  glDisable(GL_COLOR_MATERIAL);
  glPopAttrib();
}

void SolarSystemWidget::createSolarSystem()
{ 
  glDisable(GL_TEXTURE_CUBE_MAP);
  // rotate around the scene using the arcball camera rotation - drag mouse
  if (rotate)
  {
    glRotatef(2 * qRadiansToDegrees(angle), objSpaceRotAxis[0], objSpaceRotAxis[1], objSpaceRotAxis[2]);
  }
  // Zoom in and out of the scene
  glScalef(1 - scrollDelta/160, 1 - scrollDelta/160, 1 - scrollDelta/160);

  // diffuse lighting set by the user
  GLfloat dif[] = {sunlightIntensity, sunlightIntensity, sunlightIntensity, 1.};
  GLfloat spec[] = {0.8, 0.8, 0.8, 1.};
  // global ambient light values
  float ambientLevel[] = { 0.15, 0.15, 0.15, 1. };    
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambientLevel );

  glEnable(GL_LIGHTING);

  glPushMatrix();
    glScalef(0.1, 0.1, 0.1); // make solar system very small to fit inside cubemap

    glPushMatrix();
      GLfloat centerLight[] = {0., 0., 0.};
      glLightfv(GL_LIGHT0, GL_POSITION, centerLight);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
      glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
      glEnable(GL_LIGHT0); // set lighting conditions for the scene
      drawPlanet(sun);
    glPopMatrix();

    glPushMatrix();
      drawPlanet(mercury);
    glPopMatrix();

    glPushMatrix();
      drawPlanet(venus);
    glPopMatrix();

    glPushMatrix();
      drawPlanet(earth);
    glPopMatrix();
    
    glPushMatrix();
      drawPlanet(mars);
    glPopMatrix();

    glPushMatrix();
      drawPlanet(jupyter);
    glPopMatrix();

    glPushMatrix();
      drawPlanet(saturn);
    glPopMatrix();

    glPushMatrix();
      drawPlanet(uranus);
    glPopMatrix();

    glPushMatrix();
      drawPlanet(neptune);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    // saturn's asteroid ring
    glPushAttrib(GL_LIGHTING_BIT);
      glPushMatrix();
        glTranslatef(saturn->_pose[0],saturn->_pose[1],saturn->_pose[2]);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glRotatef(0.5*_time, 0.5f, 1.f, 0.5f);
        glScalef(0.35,0.35,0.1);
        gear(&polishedCopper, 60);
      glPopMatrix();
    glPopAttrib();

    // uranus' asteroid ring
    glPushAttrib(GL_LIGHTING_BIT);
      glPushMatrix();
        glTranslatef(uranus->_pose[0],uranus->_pose[1],uranus->_pose[2]);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glRotatef(0.5*_time, 0.5f, 1.f, 0.f);
        glScalef(0.25,0.25,0.05);
        gear(&yellowPlastic, 20);
      glPopMatrix();
    glPopAttrib();

  glPopMatrix();
}


double* SolarSystemWidget::gforce(PlanetWidget* p1, PlanetWidget* p2)
{
    // calculate distance between p1 (star) and p2 (planet)
    double r[3] = {p1->_pose[0] - p2->_pose[0],
                   p1->_pose[1] - p2->_pose[1],
                   p1->_pose[2] - p2->_pose[2]};
    
    double dist_r = sqrt(pow(r[0], 2)+pow(r[1], 2)+pow(r[2], 2));

    // unit vector in the direction from p1 to p2
    double r_hat[3] = {r[0]/dist_r, 
                    r[1]/dist_r, 
                    r[2]/dist_r};

    double G = 1.0; // gravitational constant. In real world is 6.67e-11

    // apply Newton's law of universal gravitation F=(G*m1*m2)/r^2
    double forceM = (G*p1->_mass*p2->_mass)/(pow(dist_r, 2));
    double* forceV = new double[3];

    // magnitud and direction into one vector
    forceV[0] = -forceM*r_hat[0];
    forceV[1] = -forceM*r_hat[1];
    forceV[2] = -forceM*r_hat[2];

    return forceV;
}

void SolarSystemWidget::drawPlanet(PlanetWidget* planet)
{
  // sun's glowing effect
  GLfloat mat_emission[] = {0.8, 0.8, 0.8, 0.0};
  glPushAttrib(GL_LIGHTING_BIT);
  if (planet == sun)
  {
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
  }

  PlanetWidget* planet_to_orbit;     // spaceship'a planet to orbit
  if (planetIndex == 0)
    planet_to_orbit = mercury;
  else if (planetIndex == 1)
    planet_to_orbit = venus;
  else if (planetIndex == 2)
    planet_to_orbit = earth;
  else if (planetIndex == 3)
    planet_to_orbit = mars;
  else if (planetIndex == 4)
    planet_to_orbit = jupyter;
  else if (planetIndex == 5)
    planet_to_orbit = saturn;
  else if (planetIndex == 6)
    planet_to_orbit = uranus;
  else if (planetIndex == 7)
    planet_to_orbit = neptune;

  if (planet == earth)    // reder spaceship once per frame, that's why we make this check
  {
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(planet_to_orbit->_pose[0],planet_to_orbit->_pose[1],planet_to_orbit->_pose[2]);
    glPushMatrix();
    glRotatef(spaceshipSpeed*_time, 1.,0.,0.);
    glTranslatef(0., 1.5, 0.);
    glRotatef(1*_time, 0.,0.,1.);
    glScalef(0.2, 0.2, 0.2);
    drawSpaceship(texID, _time_sinewave, left_engine_deg, right_engine_deg);
    glPopMatrix();
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
  }

  // update planet's orbit according to their gravitational force to the rest of the bodies
  updatePlanetOrbit(planet);
  glTranslatef(planet->_pose[0],planet->_pose[1],planet->_pose[2]);
  
  // rotation as a function of time
  glRotatef(0.5*_time, 0.f, 1.f, 0.f);

    // create sphere and bind texture to it
  GLUquadric *qobj = gluNewQuadric();
  gluQuadricTexture(qobj, GL_TRUE);
  gluQuadricNormals(qobj, GLU_SMOOTH); // One normal is generated for every vertex of a quadric
  gluQuadricDrawStyle(qobj, GLU_FILL);
  glRotatef(-90, 1.0, 0.0, 0.0); // correct orientation for planet's textures
  glBindTexture( GL_TEXTURE_2D, texID[planet->_texID] );
  gluSphere(qobj, planet->_radius, 200, 200);
  gluDeleteQuadric(qobj);
  glPopAttrib();
}

void SolarSystemWidget::renderCubemap()
{
  glPushMatrix();
  glScalef(20.,20.,20.);   // scale it to make it the background of the scene
  // Render the right quad
    glBindTexture(GL_TEXTURE_CUBE_MAP, texIDcube);
    glBegin(GL_QUADS);
    glTexCoord3f(  0.5f, -0.5f,  0.5f );  glVertex3f(  0.5f, -0.5f,  0.5f );  
    glTexCoord3f(  0.5f, -0.5f, -0.5f );  glVertex3f(  0.5f, -0.5f, -0.5f );  
    glTexCoord3f(  0.5f,  0.5f, -0.5f );  glVertex3f(  0.5f,  0.5f, -0.5f );  
    glTexCoord3f(  0.5f,  0.5f,  0.5f );  glVertex3f(  0.5f,  0.5f,  0.5f ); 
    glEnd();
  // Render the left quad
  glBegin(GL_QUADS);
    glTexCoord3f(  -0.5f, -0.5f,  0.5f ); glVertex3f(  -0.5f, -0.5f,  0.5f ); 
    glTexCoord3f(  -0.5f, -0.5f, -0.5f ); glVertex3f(  -0.5f, -0.5f, -0.5f ); 
    glTexCoord3f(  -0.5f,  0.5f, -0.5f ); glVertex3f(  -0.5f,  0.5f, -0.5f ); 
    glTexCoord3f(  -0.5f,  0.5f,  0.5f ); glVertex3f(  -0.5f,  0.5f,  0.5f );
  glEnd();
  // Render the top quad
  glBegin(GL_QUADS);
    glTexCoord3f( -0.5f,  0.5f, -0.5f ); glVertex3f( -0.5f,  0.5f, -0.5f );
    glTexCoord3f( -0.5f,  0.5f,  0.5f ); glVertex3f( -0.5f,  0.5f,  0.5f );
    glTexCoord3f(  0.5f,  0.5f,  0.5f ); glVertex3f(  0.5f,  0.5f,  0.5f );
    glTexCoord3f(  0.5f,  0.5f, -0.5f ); glVertex3f(  0.5f,  0.5f, -0.5f );
  glEnd();
  // Render the bottom quad
  glBegin(GL_QUADS);
    glTexCoord3f( -0.5f, -0.5f, -0.5f ); glVertex3f( -0.5f, -0.5f, -0.5f );  
    glTexCoord3f( -0.5f, -0.5f,  0.5f ); glVertex3f( -0.5f, -0.5f,  0.5f );  
    glTexCoord3f(  0.5f, -0.5f,  0.5f ); glVertex3f(  0.5f, -0.5f,  0.5f );  
    glTexCoord3f(  0.5f, -0.5f, -0.5f ); glVertex3f(  0.5f, -0.5f, -0.5f ); 
  glEnd();
  // Render the back quad
  glBegin(GL_QUADS);
    glTexCoord3f(  0.5f,  -0.5f,   -0.5f ); glVertex3f(  0.5f, -0.5f, -0.5f );  
    glTexCoord3f( -0.5f,  -0.5f,   -0.5f ); glVertex3f( -0.5f, -0.5f, -0.5f );  
    glTexCoord3f( -0.5f,   0.5f,   -0.5f ); glVertex3f( -0.5f,  0.5f, -0.5f );  
    glTexCoord3f(  0.5f,   0.5f,   -0.5f ); glVertex3f(  0.5f,  0.5f, -0.5f ); 
  glEnd();
  // Render the front quad
    glBegin(GL_QUADS);
    glTexCoord3f(  0.5f,  -0.5f,   0.5f ); glVertex3f(  0.5f, -0.5f,  0.5f );  
    glTexCoord3f( -0.5f,  -0.5f,   0.5f ); glVertex3f( -0.5f, -0.5f,  0.5f );  
    glTexCoord3f( -0.5f,   0.5f,   0.5f ); glVertex3f( -0.5f,  0.5f,  0.5f );  
    glTexCoord3f(  0.5f,   0.5f,   0.5f ); glVertex3f(  0.5f,  0.5f,  0.5f ); 
    glEnd();
  glPopMatrix();
}

void SolarSystemWidget::updatePlanetOrbit(PlanetWidget* p1)
{

    float dt = speedInc;    // simulation speed controller
    std::vector<PlanetWidget*> planets{sun, mercury, venus, earth, mars, jupyter, saturn, uranus, neptune};

    // find planet p1 in the planets vector and remove it
    PlanetWidget* p2;
    for (uint i=0; i<planets.size(); i++){
        p2 = planets.at(i);
        if (p1 == p2){
            planets.erase(planets.begin()+i);
        }
    }

    // calculate gravitational force between planet p1 and the rest of the planet in the system
    double** _gforce = new double* [(int)planets.size()];
    _gforce[0] = gforce(p1, planets.at(0));
    _gforce[1] = gforce(p1, planets.at(1));
    _gforce[2] = gforce(p1, planets.at(2));
    _gforce[3] = gforce(p1, planets.at(3));
    _gforce[4] = gforce(p1, planets.at(4));
    _gforce[5] = gforce(p1, planets.at(5));
    _gforce[6] = gforce(p1, planets.at(6));
    _gforce[7] = gforce(p1, planets.at(7));

    // sum the resuls from the gforce calls
    double* gforce_sum = addVectors(_gforce, sizeof(_gforce));

    p1->_forceVector[0] = gforce_sum[0];
    p1->_forceVector[1] = gforce_sum[1];
    p1->_forceVector[2] = gforce_sum[2];

    // calculate momentum
    p1->_momentum[0] = p1->_momentum[0] + p1->_forceVector[0] * dt;
    p1->_momentum[1] = p1->_momentum[1] + p1->_forceVector[1] * dt;
    p1->_momentum[2] = p1->_momentum[2] + p1->_forceVector[2] * dt;
    // calculate posision
    p1->_pose[0] = p1->_pose[0] + p1->_momentum[0] / p1->_mass*dt;
    p1->_pose[1] = p1->_pose[1] + p1->_momentum[1] / p1->_mass*dt;
    p1->_pose[2] = p1->_pose[2] + p1->_momentum[2] / p1->_mass*dt;
}

double* SolarSystemWidget::addVectors(double* vectors[], int size)
{
    double* resultV = new double[3]();
    int bound = size/sizeof(vectors[0]);
    for(int i=0; i<bound; i++)
    {
        for(int j=0; j<3; j++)
        {
            resultV[j] += vectors[i][j];
            // printf("%f\n", resultV[j]);
        }
    }
    return resultV;
}

void SolarSystemWidget::updateAngle(){
  _time += 1.0;
  // if (_time_sinewave == 361)
  //   _time_sinewave = -1;
  _time_sinewave +=0.1;
  this->repaint();
}




void SolarSystemWidget::resetOrbitValues()
{
  speedInc = 0.0001;

  delete sun;
  delete earth;
  delete mercury;
  delete venus;
  delete mars;
  delete jupyter;
  delete neptune;
  delete uranus;
  delete saturn;

  double sunPose[3] = {0.0, 0.0, 0.0};
  double sunMomentum[3] = {0.0, 0.0, 0.0};
  sun = new PlanetWidget(this, 2, 1000.0, 1.8, sunPose, sunMomentum, -1);

  double mercuryPose[3] = {3.0,0.0,0.0};
  double mercuryMomentum[3] = {0.0, 1.0, 35.0};
  mercury = new PlanetWidget(this, 3, 2.0, 0.1, mercuryPose, mercuryMomentum, 0);

  double venusPose[3] = {3.7,0.0,0.0};  
  double venusMomentum[3] = {0.0, 1.0, 15.0};
  venus = new PlanetWidget(this, 4, 1., 0.17, venusPose, venusMomentum, 1);

  double earthPose[3] = {4.2,0.0,0.0};
  double earthMomentum[3] = {0.0, 1.0, 30.0};
  earth = new PlanetWidget(this, 5, 2.0, 0.17, earthPose, earthMomentum, 2);

  double marsPose[3] = {4.7,0.0,0.0};
  double marsMomentum[3] = {0.0, 1.0, 32.0};
  mars = new PlanetWidget(this, 6, 2., 0.12, marsPose, marsMomentum, 3);
  
  double jupyterPose[3] = {5.3,0.0,0.0};
  double jupyterMomentum[3] = {0.0, 1.0, 33.0};
  jupyter = new PlanetWidget(this, 7, 2.0, 0.3, jupyterPose, jupyterMomentum, 4);
  
  double saturnPose[3] = {6.5,0.0,0.0};
  double saturnMomentum[3] = {0.0, 1.0, 30.0};
  saturn = new PlanetWidget(this, 8, 2.0, 0.27, saturnPose, saturnMomentum, 5);
  
  double uranusPose[3] = {7.9,0.0,0.0};
  double uranusMomentum[3] = {0.0, 1.0, 14.0};
  uranus = new PlanetWidget(this, 9, 1., 0.18, uranusPose, uranusMomentum, 6);

  double neptunePose[3] = {8.9,0.0,0.0};
  double neptuneMomentum[3] = {0.0, 1.0, 13.5};
  neptune = new PlanetWidget(this, 10, 1., 0.18, neptunePose, neptuneMomentum, 7);

  this->repaint();
}

void SolarSystemWidget::updateSunLight(int val)
{
  sunlightIntensity = val/10.;
  // printf("%f\n", sunlightIntensity);
  this->repaint();
}

void SolarSystemWidget::updateOrbitSpeed(int n){
  float inc = (float)n / 10000.;
  // printf("%f\n", inc);
  speedInc = inc;
  this->repaint();
}

void SolarSystemWidget::updatePlanetEarthMass(int n){
  float inc = (float)n / 10.;
  // printf("%f\n", inc);
  earth->_mass = inc;
  this->repaint();
}

void SolarSystemWidget::updatePlanetMercuryMass(int n){
  float inc = (float)n / 10.;
  // printf("%f\n", inc);
  mercury->_mass = inc;
  this->repaint();
}

void SolarSystemWidget::updatePlanetVenusMass(int n){
  float inc = (float)n / 10.;
  // printf("%f\n", inc);
  venus->_mass = inc;
  this->repaint();
}

void SolarSystemWidget::updatePlanetMarsMass(int n){
  float inc = (float)n / 10.;
  // printf("%f\n", inc);
  mars->_mass = inc;
  this->repaint();
}

void SolarSystemWidget::updatePlanetJupyterMass(int n){
  float inc = (float)n / 10.;
  // printf("%f\n", inc);
  jupyter->_mass = inc;
  this->repaint();
}

void SolarSystemWidget::updatePlanetSaturnMass(int n){
  float inc = (float)n / 10.;
  // printf("%f\n", inc);
  saturn->_mass = inc;
  this->repaint();
}

void SolarSystemWidget::updatePlanetUranusMass(int n){
  float inc = (float)n / 10.;
  // printf("%f\n", inc);
  uranus->_mass = inc;
  this->repaint();
}

void SolarSystemWidget::updatePlanetNeptuneMass(int n){
  float inc = (float)n / 10.;
  // printf("%f\n", inc);
  neptune->_mass = inc;
  this->repaint();
}

void SolarSystemWidget::updateSpaceshipSpeed(int n){
  float inc = (float)n / 10.;
  // printf("%f\n", inc);
  spaceshipSpeed = inc;
  this->repaint();
}

void SolarSystemWidget::updateSpaceship_rightEngine(int n)
{
  float inc = (float)n / 10.;
  // printf("%f\n", inc);
  right_engine_deg = inc;
  this->repaint();
}

void SolarSystemWidget::updateSpaceship_leftEngine(int n)
{
  float inc = (float)n / 10.;
  // printf("%f\n", inc);
  left_engine_deg = inc;
  this->repaint();
}

void SolarSystemWidget::setPlanetIndex(int i)
{
  planetIndex = i;
  this->repaint();
}


// Texture loading

void SolarSystemWidget::loadCubemap()
{
  // Loading Pictures to get width, height and Color Channel Information
    glGenTextures( 1, &texIDcube );  // Get the texture object IDs.
    glBindTexture(GL_TEXTURE_CUBE_MAP, texIDcube);
    for (int i=0; i<nTexCube; i++){
        iDataCube[i] = 0;
        FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(cubemapTextures[i]);
        if (format == FIF_UNKNOWN) {
            printf("Unknown file type for texture image file %s\n", cubemapTextures[i]);
            continue;
        }
        FIBITMAP* bitmap = FreeImage_Load(format, cubemapTextures[i], 0);

        if (!bitmap) {
            printf("Cannot load file image %s. \n", cubemapTextures[i]);
            continue;
        }
        else{
            printf("Textures %s successfully loaded. \n", cubemapTextures[i]);
        }

        FIBITMAP* bitmap2 = FreeImage_ConvertTo24Bits(bitmap); // convert to RGB format

        FreeImage_Unload(bitmap);
        iDataCube[i] = FreeImage_GetBits(bitmap2);
        widthCube[i] = FreeImage_GetWidth(bitmap2);
        heightCube[i] = FreeImage_GetHeight(bitmap2);
        if (iDataCube[i]) {
            printf("Texture image loaded from file %s, size %dx%d\n", 
                                cubemapTextures[i], widthCube[i], heightCube[i]);
            int format; // The format of the color data in memory, depends on platform.
            if ( FI_RGBA_RED == 0 )
                format = GL_RGB;
            else
                format = GL_BGR;
            
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, widthCube[i], heightCube[i], 0, GL_RGB, GL_UNSIGNED_BYTE, iDataCube[i]
            );
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        }
        else {
            printf("Failed to get texture data from %s\n", textures[i]);
        }
    }
}

void SolarSystemWidget::loadTextures()
{
    // Loading Pictures to get width, height and Color Channel Information
    glGenTextures( nTex, texID );  // Get the texture object IDs.
    for (int i=0; i<nTex; i++){
        iData[i] = 0;
        FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(textures[i]);
        if (format == FIF_UNKNOWN) {
            printf("Unknown file type for texture image file %s\n", textures[i]);
            continue;
        }
        FIBITMAP* bitmap = FreeImage_Load(format, textures[i], 0);

        if (!bitmap) {
            // printf("Cannot load file image %s\nSTB Reason: %s\n", textures[i], stbi_failure_reason());
            printf("Cannot load file image %s. \n", textures[i]);
            continue;
        }
        else{
            printf("Textures %s successfully loaded. \n", textures[i]);
        }

        FIBITMAP* bitmap2 = FreeImage_ConvertTo24Bits(bitmap); // convert to RGB format
        if (i == 0 || i == 1)
        {
          bitmap2 = FreeImage_Rotate(bitmap2, 180);
        }
        FreeImage_Unload(bitmap);
        iData[i] = FreeImage_GetBits(bitmap2);
        width[i] = FreeImage_GetWidth(bitmap2);
        height[i] = FreeImage_GetHeight(bitmap2);
        if (iData[i]) {
            printf("Texture image loaded from file %s, size %dx%d\n", 
                                textures[i], width[i], height[i]);
            int format; // The format of the color data in memory, depends on platform.
            if ( FI_RGBA_RED == 0 )
                format = GL_RGB;
            else
                format = GL_BGR;
            glBindTexture( GL_TEXTURE_2D, texID[i] );  // Will load image data into texture object #i
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width[i], height[i], 0, format,
                                GL_UNSIGNED_BYTE, iData[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Required since there are no mipmaps.
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        }
        else {
            printf("Failed to get texture data from %s\n", textures[i]);
        }
    }
}