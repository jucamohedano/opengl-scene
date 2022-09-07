#include <math.h>
#include <stdio.h>
#include "utils/Spaceship.h"

void drawSpaceship(GLuint* texID, float _time_sinewave, float left, float right){
    // spaceship

    /*

    BOTTOM OF SPACESHIP
    gluDisk and GL_POINTS

    */
    glPushMatrix();
      glPushAttrib(GL_LIGHTING_BIT);
        glTranslatef(0.,0.,-1.);
        // materials of gludisk bottom of spaceship
        materialStruct* p_front = &ruby;
        glMaterialfv(GL_FRONT, GL_AMBIENT,    p_front->ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_front->diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,   p_front->specular);
        glMaterialf(GL_FRONT, GL_SHININESS,   p_front->shininess);

        GLUquadric *_qobj = gluNewQuadric();
        gluQuadricNormals(_qobj, GLU_SMOOTH); // One normal is generated for every vertex of a quadric. This is the initial value.
        gluQuadricDrawStyle(_qobj, GLU_FILL);
        gluDisk(_qobj, 0, 1.0, 200, 200);
        gluDeleteQuadric(_qobj);

        // fire behind spaceship defined by sine wave function
        glPushMatrix();
          glTranslatef(0.,0.,-1.);
            float k, x, y;
            float wave_length = 2;
            float amplitude = 0.5;
            float inc = 0.05;
            glPointSize(26);
            glTranslatef(0.,0,_time_sinewave);
            glBegin(GL_POINTS);
                for(x=-1+_time_sinewave;x<=1+_time_sinewave;x+=inc)
                {
                  k = 2 * M_PI / wave_length;
                  y = amplitude * sin(k * x);
                  glVertex3f(0, y, -x);
                  glVertex3f(0.2, y, -x);
                  glVertex3f(-0.2, y, -x);
                  glVertex3f(0, y+0.2, -x);
                  glVertex3f(0.2, y+0.2, -x);
                  glVertex3f(-0.2, y+0.2, -x);
                }
            glEnd();
        glPopMatrix();
      glPopAttrib();
    glPopMatrix();

    /* 

    MAIN BODY OF SPACESHIP 
    cylinder, cone and sphere on the top

    */

    //cylinder
    glPushMatrix();
      glPushAttrib(GL_LIGHTING_BIT);
        cylinder(&yellowPlastic);
      glPopAttrib();
      glPopMatrix();

      //cone
      glPushMatrix();
        glPushAttrib(GL_LIGHTING_BIT);
          glTranslatef(0.,0.,1.0);
          openedCone(&cyanPlastic, 0.2);
        glPopAttrib();
      glPopMatrix();

      //sphere, pointing tip
      glPushMatrix();
        glPushAttrib(GL_LIGHTING_BIT);
        glDisable(GL_TEXTURE_2D);
        // define sphere materials for sphere
        glMaterialfv(GL_FRONT, GL_AMBIENT,    brassMaterials.ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,    brassMaterials.diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,   brassMaterials.specular);
        glMaterialf(GL_FRONT, GL_SHININESS,   brassMaterials.shininess);

        glTranslatef(0.0,0.0,2.96);
        GLUquadric *qobj = gluNewQuadric();
        gluQuadricTexture(qobj, GL_TRUE);
        gluQuadricNormals(qobj, GLU_SMOOTH); // One normal is generated for every vertex of a quadric. This is the initial value.
        gluQuadricDrawStyle(qobj, GLU_FILL);
        gluSphere(qobj, 0.2, 200, 200);
        gluDeleteQuadric(qobj);
        glPopAttrib();
      glPopMatrix();
    

    /* 

     ENGINES OF SPACESHIP
     gluDisk and cone

    */

    // left engine
    glPushMatrix();
      glPushAttrib(GL_LIGHTING_BIT);
        glTranslatef(1.5,0.0,0.0);
        glRotatef(-45, 0.,1.,0.);
        glScalef(0.2, 0.1, 1.0);
        // draw transformed cube
        cube(&ruby, 3, texID);
      glPopAttrib();
    glPopMatrix();

    left_engine(left);

    // right engine
    glPushMatrix();
      glPushAttrib(GL_LIGHTING_BIT);
      glTranslatef(-1.5,0.0,0.0);
      glRotatef(45, 0.,1.,0.);
      glScalef(0.2, 0.1, 1.0);
      cube(&ruby, 3, texID);
      glPopAttrib();
    glPopMatrix();

    right_engine(right);
    
    /*

      WINDOWS OF SPACESHIP

    */
    
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(1.0,1.0,1.0);
    glPushMatrix();
        glTranslatef(-0.25, 0.1, 2.);
        glScalef(0.5,0.5,0.5);
        glRotatef(-63, 1.,0.,0.);
        // draw window
        cube(&ruby, 0, texID);
    glPopMatrix();
    glRotatef(180, 0., 0., 1.0); // rotate spaceship and place the other window simetrically in the same place
    glPushMatrix();
        glTranslatef(-0.25, 0.1, 2.);
        glScalef(0.5,0.5,0.5);
        glRotatef(-63, 1.,0.,0.);
        // draw window
        cube(&ruby, 1, texID);
    glPopMatrix();
    glDisable(GL_COLOR_MATERIAL);
}


void left_engine(float deg)
{
  // gluDisk bottom of engine
  glPushMatrix();
  glRotatef(deg, 1.0, 0, 0);
  glPushMatrix();
  // materials of gludisk right engine bottom
    glPushAttrib(GL_LIGHTING_BIT);
      materialStruct* leftEngine = &polishedCopper;
      glMaterialfv(GL_FRONT, GL_AMBIENT,    leftEngine->ambient);
      glMaterialfv(GL_FRONT, GL_DIFFUSE,    leftEngine->diffuse);
      glMaterialfv(GL_FRONT, GL_SPECULAR,   leftEngine->specular);
      glMaterialf(GL_FRONT, GL_SHININESS,   leftEngine->shininess);
      glTranslatef(1.8, 0., -0.6);
      GLUquadric *qobjL = gluNewQuadric();
      gluQuadricNormals(qobjL, GLU_SMOOTH); // One normal is generated for every vertex of a quadric. This is the initial value.
      gluQuadricDrawStyle(qobjL, GLU_FILL);
      gluDisk(qobjL, 0, 0.4, 200, 200);
      gluDeleteQuadric(qobjL);
    glPopAttrib();
  glPopMatrix();
  glPushMatrix();
    glPushAttrib(GL_LIGHTING_BIT);
      glTranslatef(1.8, 0., -0.2);
      glScalef(0.4,0.4,0.4);
      // cylinder of left engine
      cylinder(&brassMaterials);
      glTranslatef(0.,0.,1.0);
      // draw cone at top of the cylinder that forms the engine
      openedCone(&cyanPlastic, 0.01);
    glPopAttrib();
  glPopMatrix();
  glPopMatrix();
}

void right_engine(float deg)
{
  // gluDisk bottom of engine
  glPushMatrix();
  glRotatef(deg, 1.0, 0, 0);
  glPushMatrix();
    // materials of gludisk right engine bottom
    materialStruct* rightEngine = &polishedCopper;
    glMaterialfv(GL_FRONT, GL_AMBIENT,    rightEngine->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    rightEngine->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   rightEngine->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   rightEngine->shininess);
    glTranslatef(-1.8, 0., -0.6);
    GLUquadric *qobjR = gluNewQuadric();
    // One normal is generated for every vertex of a quadric. This is the initial value.
    gluQuadricNormals(qobjR, GLU_SMOOTH); 
    gluQuadricDrawStyle(qobjR, GLU_FILL);
    gluDisk(qobjR, 0, 0.4, 200, 200);
    gluDeleteQuadric(qobjR);
  glPopMatrix();
  glPushMatrix();
    glPushAttrib(GL_LIGHTING_BIT);
      glTranslatef(-1.8, 0., -0.2);
      glScalef(0.4,0.4,0.4);
      // cylinder of right engine
      cylinder(&brassMaterials);
      glTranslatef(0.,0.,1.0);
      // draw cone at top of the cylinder that forms the engine
      openedCone(&cyanPlastic, 0.01);
    glPopAttrib();
  glPopMatrix();
  glPopMatrix();
}