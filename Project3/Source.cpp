#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "imageloader.h"

#define PI 3.14

static int shoulder = 0, elbow = 0, shoulder1 = 0, fingerBase = 0, fingerUp = 0, fingerBase1 = 0, fingerUp1 = 0, fingerBase2 = 0, fingerUp2 = 0, fingerBase3 = 0, fingerUp3 = 0, fingerBase4 = 0, fingerUp4 = 0, fingerBase5 = 0, fingerUp5 = 0;
static int shoulder_1 = 0, elbow_1 = 0, shoulder1_1 = 0, fingerBase_1 = 0, fingerUp_1 = 0, fingerBase1_1 = 0, fingerUp1_1 = 0, fingerBase2_1 = 0, fingerUp2_1 = 0, fingerBase3_1 = 0, fingerUp3_1 = 0, fingerBase4_1 = 0, fingerUp4_1 = 0, fingerBase5_1 = 0, fingerUp5_1 = 0;
static int legf[] = { 0, 0 };
static int legs[] = { 0, 0 };
static int knee[] = { 0, 0 };

int moving, startx, starty;

GLfloat angle = 0.0;  /* in degrees */
GLfloat angle2 = 0.0; /* in degrees */

double eye[] = { 0.0, 0.0, -25.0 };
double center[] = { 0.0, 0.0, 0.0 };
double up[] = { 0.0, 1.0, 0.0 };
float speed = 0.1;
float rotationAngle = 0.1;

// RGBA
//GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
//GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
//// x , y, z, w
//GLfloat light_position[] = { 2.0, 2.0, 3.0, 1.0 };
//GLfloat lightPos1[] = { -2.0, -2.0, 3.0, 1.0 };


GLuint _textureId0; //The id of the texture carpet 1
GLuint _textureId1; //The id of the texture carpet 2
GLuint _textureId2; //The id of the texture grass
GLuint _textureId3; //The id of the texture concrete


//Makes the image into a texture, and returns the id of the texture

GLuint loadTexture(Image* image)
{
    GLuint textureId;
    glGenTextures(1, &textureId);			 //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,				  //Always GL_TEXTURE_2D
        0,							  //0 for now
        GL_RGB,					  //Format OpenGL uses for image
        image->width, image->height, //Width and height
        0,							  //The border of the image
        GL_RGB,					  //GL_RGB, because pixels are stored in RGB format
        GL_UNSIGNED_BYTE,			  //GL_UNSIGNED_BYTE, because pixels are stored
                                     //as unsigned numbers
        image->pixels);			  //The actual pixel data
    return textureId;						  //Returns the id of the texture
}




void rotatePoint(double a[], double theta, double p[])
{

    double temp[3];
    temp[0] = p[0];
    temp[1] = p[1];
    temp[2] = p[2];

    temp[0] = -a[2] * p[1] + a[1] * p[2];
    temp[1] = a[2] * p[0] - a[0] * p[2];
    temp[2] = -a[1] * p[0] + a[0] * p[1];

    temp[0] *= sin(theta);
    temp[1] *= sin(theta);
    temp[2] *= sin(theta);

    temp[0] += (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
    temp[1] += (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
    temp[2] += (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

    temp[0] += cos(theta) * p[0];
    temp[1] += cos(theta) * p[1];
    temp[2] += cos(theta) * p[2];

    p[0] = temp[0];
    p[1] = temp[1];
    p[2] = temp[2];
}

void crossProduct(double a[], double b[], double c[])
{
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(double a[])
{
    double norm;
    norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
    norm = sqrt(norm);
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}

// Rotation about vertical direction
void rotateRight()
{
    rotatePoint(up, rotationAngle, eye);
}

void rotateLeft()
{
    rotatePoint(up, -rotationAngle, eye);
}

// Rotation about horizontal direction

void rotateUp()
{
    double horizontal[3];
    double direction[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
    crossProduct(up, direction, horizontal);
    normalize(horizontal);
    rotatePoint(horizontal, rotationAngle, eye);
    rotatePoint(horizontal, rotationAngle, up);
}

void rotateDown()
{
    double horizontal[3];
    double direction[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
    crossProduct(up, direction, horizontal);
    normalize(horizontal);
    rotatePoint(horizontal, -rotationAngle, eye);
    rotatePoint(horizontal, -rotationAngle, up);
}

// Forward and Backward
void moveForward()
{
    double direction[3];
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];

    eye[0] += direction[0] * speed;
    eye[1] += direction[1] * speed;
    eye[2] += direction[2] * speed;

    center[0] += direction[0] * speed;
    center[1] += direction[1] * speed;
    center[2] += direction[2] * speed;
}

void moveBackword()
{
    double direction[3];
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];

    eye[0] -= direction[0] * speed;
    eye[1] -= direction[1] * speed;
    eye[2] -= direction[2] * speed;
}

void myMenu(int id) {

    switch (id) {

    case 0:
        glBindTexture(GL_TEXTURE_2D, _textureId3);
        glutPostRedisplay();

        break;

    case 1:
        glBindTexture(GL_TEXTURE_2D, _textureId2);
        glutPostRedisplay();

        break;

    case 2:
        glBindTexture(GL_TEXTURE_2D, _textureId1);
        glutPostRedisplay();

        break;

    case 3:
        glBindTexture(GL_TEXTURE_2D, _textureId0);
        glutPostRedisplay();

        break;

    default:
        break;

    }

}

void init(void)
{
    Image* image0 = loadBMP("images/concrete.bmp");
    _textureId0 = loadTexture(image0);
    delete image0;

    Image* image1 = loadBMP("images/grass.bmp");
    _textureId1 = loadTexture(image1);
    delete image1;

    Image* image2 = loadBMP("images/carpet2.bmp");
    _textureId2 = loadTexture(image2);
    delete image2;

    Image* image3 = loadBMP("images/carpet1.bmp");
    _textureId3 = loadTexture(image3);
    delete image3;

    //// Turn on the power
    //glEnable(GL_LIGHTING);
    //// Flip light switch
    //glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);
	
    //// assign light parameters
    //glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	
    //glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    //glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);

  
    //glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    //glEnable(GL_NORMALIZE);
    ////Enable smooth shading
    //glShadeModel(GL_SMOOTH);

    // Enable Depth buffer
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glFrustum(-1.5, 1.5, -1.0, 1.0, 1.0, 100.0);


    glMatrixMode(GL_MODELVIEW);
}


void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    gluLookAt(eye[0], eye[1], eye[2],
        center[0], center[1], center[2],
        up[0], up[1], up[2]);


    glRotatef(angle, 1.0, 0.0, 0.0);
    glRotatef(angle2, 0.0, 1.0, 0.0);
    glTranslatef(2.0, 0.0, 0.0);

    


    //for the body moheb 1

    glPushMatrix();
    glTranslatef(-2, -2, 0.0);
    glScalef(3.0, 7.0, 1.5);
    glutWireCube(1.0);
    glPopMatrix();

    //for the face moheb 2

    glPushMatrix();
    glTranslatef(-2, 2.5, 0.0);
    glutWireSphere(1.0, 15, 15);
    glPopMatrix();

    //for legs
    // START OF RIGHT LEG
    glPushMatrix();

    glTranslatef(-1.0, -6.75, 0.0);
    glRotatef((GLfloat)legs[0], 0.0, 0.0, 1.0);
    glTranslatef(-0.5, 0.0, 0.0);

    glRotatef((GLfloat)legf[0], 1.0, 0.0, 0.0);
    glTranslatef(0.5, -0.5, 0.0);

    glPushMatrix();
    glScalef(0.75, 3.5, 1.5);
    glutWireCube(1.0);
    glPopMatrix();


    glTranslatef(0.0, -3, 0.0);
    glRotatef(GLfloat(knee[0]), 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.5, 0.0);
    glPushMatrix();
    glScalef(0.75, 3.5, 1.5);
    glutWireCube(1.0);
    glPopMatrix();

    glTranslatef(0.0, -1.5, 0.0);
    glPushMatrix();
    glScalef(0.75, 0.5, 1.75);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
    // END OF RIGHT LEG

    // START OF LEFT LEG
    glPushMatrix();

    glTranslatef(-3.0, -6.75, 0.0);
    glRotatef((GLfloat)legs[1], 0.0, 0.0, 1.0);
    glTranslatef(+0.5, 0.0, 0.0);

    glRotatef((GLfloat)legf[1], 1.0, 0.0, 0.0);
    glTranslatef(-0.5, -0.5, 0.0);

    glPushMatrix();
    glScalef(0.75, 3.5, 1.5);
    glutWireCube(1.0);
    glPopMatrix();


    glTranslatef(0.0, -3, 0.0);
    glRotatef(GLfloat(knee[1]), 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.5, 0.0);
    glPushMatrix();
    glScalef(0.75, 3.5, 1.5);
    glutWireCube(1.0);
    glPopMatrix();

    glTranslatef(0.0, -1.5, 0.0);
    glPushMatrix();
    glScalef(0.75, 0.5, 1.75);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
    // END OF LEFT LEG


    //left Arm fatma

    glPushMatrix();
    glTranslatef(0.5, -0.1, 0.0);
    glRotatef(-90.0, 0.0, 0.0, 1.0);

    glRotatef((GLfloat)shoulder, 0.0, 0.0, 1.0);
    glRotatef((GLfloat)shoulder1, 1.0, 0.0, 0.0);

    glPushMatrix();
    glScalef(2.0, 0.6, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
    glTranslatef(1.0, 0.0, 0.0);

    glRotatef((GLfloat)elbow, 0.0, 1.0, 0.0);
    glTranslatef(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(2.0, 0.6, 1.0);
    glutWireCube(1.0);
    glPopMatrix();

    //Draw finger flang 1 
    glPushMatrix();
    glTranslatef(1.0, -0.1, 0.0);
    glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
    glTranslatef(0.15, -0.1, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();


    //Draw finger flang 1 
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();
    glPopMatrix();

    //Draw finger flang 2 
    glPushMatrix();
    glTranslatef(1.0, 0.1, -0.075);
    glRotatef((GLfloat)fingerBase1, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.1, -0.075);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();


    //Draw finger flang 2

    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)fingerUp1, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();
    glPopMatrix();

    //Draw finger flang 3
    glPushMatrix();
    glTranslatef(1.0, 0.1, 0.2);
    glRotatef((GLfloat)fingerBase2, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.1, 0.2);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();

    //Draw finger flang 3 
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)fingerUp2, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();
    glPopMatrix();

    //Draw finger flang 4
    glPushMatrix();
    glTranslatef(1.0, 0.1, 0.075);
    glRotatef((GLfloat)fingerBase3, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.1, 0.075);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();


    //Draw finger flang 4
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)fingerUp3, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();
    glPopMatrix();

    //Draw finger flang 5 
    glPushMatrix();
    glTranslatef(1.0, 0.1, -0.2);
    glRotatef((GLfloat)fingerBase4, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.1, -0.2);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();

    //Draw finger flang 5
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)fingerUp4, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();

    //RIGHT Arm fatma

    glPushMatrix();
    glTranslatef(-4.5, -0.1, 0.0);
    glRotatef(-90.0, 0.0, 0.0, 1.0);
    glRotatef(-180.0, 1.0, 0.0, 0.0);

    glRotatef((GLfloat)shoulder_1, 0.0, 0.0, 1.0);
    glRotatef((GLfloat)shoulder1_1, 1.0, 0.0, 0.0);

    glPushMatrix();
    glScalef(2.0, 0.6, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
    glTranslatef(1.0, 0.0, 0.0);

    glRotatef((GLfloat)elbow_1, 0.0, 1.0, 0.0);
    glTranslatef(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(2.0, 0.6, 1.0);
    glutWireCube(1.0);
    glPopMatrix();

    //Draw finger flang 1 
    glPushMatrix();
    glTranslatef(1.0, -0.1, 0.0);
    glRotatef((GLfloat)fingerBase_1, 0.0, 0.0, 1.0);
    glTranslatef(0.15, -0.1, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();


    //Draw finger flang 1 
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)fingerUp_1, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();
    glPopMatrix();

    //Draw finger flang 2 
    glPushMatrix();
    glTranslatef(1.0, 0.1, -0.075);
    glRotatef((GLfloat)fingerBase1_1, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.1, -0.075);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();


    //Draw finger flang 2

    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)fingerUp1_1, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();
    glPopMatrix();

    //Draw finger flang 3
    glPushMatrix();
    glTranslatef(1.0, 0.1, 0.2);
    glRotatef((GLfloat)fingerBase2_1, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.1, 0.2);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();

    //Draw finger flang 3 
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)fingerUp2_1, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();
    glPopMatrix();

    //Draw finger flang 4
    glPushMatrix();
    glTranslatef(1.0, 0.1, 0.075);
    glRotatef((GLfloat)fingerBase3_1, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.1, 0.075);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();


    //Draw finger flang 4
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)fingerUp3_1, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();
    glPopMatrix();

    //Draw finger flang 5 
    glPushMatrix();
    glTranslatef(1.0, 0.1, -0.2);
    glRotatef((GLfloat)fingerBase4_1, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.1, -0.2);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();

    //Draw finger flang 5
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)fingerUp4_1, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.15, 0.15);
    glutWireCube(1);
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();

	/*glPushMatrix();
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glPopMatrix();*/


    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30, -12.65, 15);
    glTexCoord2f(3.0f, 0.0f); glVertex3f(30, -12.65, 15);
    glTexCoord2f(3.0f, 3.0f); glVertex3f(30, -12.65, -15);
    glTexCoord2f(0.0f, 3.0f); glVertex3f(-30, -12.65, -15);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glutSwapBuffers();
    glPopMatrix();
}

void reset()
{
     shoulder = 0, elbow = 0, shoulder1 = 0, fingerBase = 0, fingerUp = 0, fingerBase1 = 0, fingerUp1 = 0, fingerBase2 = 0, fingerUp2 = 0, fingerBase3 = 0, fingerUp3 = 0, fingerBase4 = 0, fingerUp4 = 0, fingerBase5 = 0, fingerUp5 = 0;
     shoulder_1 = 0, elbow_1 = 0, shoulder1_1 = 0, fingerBase_1 = 0, fingerUp_1 = 0, fingerBase1_1 = 0, fingerUp1_1 = 0, fingerBase2_1 = 0, fingerUp2_1 = 0, fingerBase3_1 = 0, fingerUp3_1 = 0, fingerBase4_1 = 0, fingerUp4_1 = 0, fingerBase5_1 = 0, fingerUp5_1 = 0;
     legf[0] = 0;
     legs[0] = 0;
     knee[0] = 0;
     legf[1] = 0;
     legs[1] = 0;
     knee[1] = 0;

    double eyeReset[] = { 0.0, 0.0, -25.0 };
    double centerReset[] = { 0.0, 0.0, 0.0 };
    double upReset[] = { 0.0, 1.0, 0.0 };
    for (int i = 0; i < 3; i++)
    {
        eye[i] = eyeReset[i];
        center[i] = centerReset[i];
        up[i] = upReset[i];
    }
}


void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        // shoulder movment
    case 's':
        if (shoulder < 180)
        {
            shoulder = (shoulder + 5) % 360;
            glutPostRedisplay();
        }

        if (shoulder_1 < 180)
        {
            shoulder_1 = (shoulder_1 + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'S':
        if (shoulder > 0)
        {
            shoulder = (shoulder - 5) % 360;
            glutPostRedisplay();
        }

        if (shoulder_1 > 0)
        {
            shoulder_1 = (shoulder_1 - 5) % 360;
            glutPostRedisplay();
        }

        break;

    case 'a':
        if (shoulder1 > -180)
        {
            shoulder1 = (shoulder1 - 5) % 360;
            glutPostRedisplay();
        }

        if (shoulder1_1 < 180)
        {
            shoulder1_1 = (shoulder1_1 + 5) % 360;
            glutPostRedisplay();
        }

        break;
    case 'A':
        if (shoulder1 < 0)
        {
            shoulder1 = (shoulder1 + 5) % 360;
            glutPostRedisplay();
        }

        if (shoulder1_1 > 0)
        {
            shoulder1_1 = (shoulder1_1 - 5) % 360;
            glutPostRedisplay();
        }

        break;

        // elbow movement
    case 'd':
        if (elbow > -130)
        {
            elbow = (elbow - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'D':
        if (elbow < 0)
        {
            elbow = (elbow + 5) % 360;
            glutPostRedisplay();
        }
        break;
        // finger 1 movement
    case 'q':
        if (fingerBase < 60) {
            fingerBase = (fingerBase + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'Q':
        if (fingerBase > -30) {
            fingerBase = (fingerBase - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'w':
        if (fingerUp < 90) {
            fingerUp = (fingerUp + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'W':
        if (fingerUp > 0) {
            fingerUp = (fingerUp - 5) % 360;
            glutPostRedisplay();
        }
        break;
        // finger 2 movement
    case 'e':
        if (fingerBase1 > -90) {
            fingerBase1 = (fingerBase1 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'E':
        if (fingerBase1 < 20) {
            fingerBase1 = (fingerBase1 + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'r':
        if (fingerUp1 > -140) {
            fingerUp1 = (fingerUp1 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'R':
        if (fingerUp1 < 0) {
            fingerUp1 = (fingerUp1 + 5) % 360;
            glutPostRedisplay();
        }
        break;
        // finger 3 movement
    case 't':
        if (fingerBase2 > -90) {
            fingerBase2 = (fingerBase2 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'T':
        if (fingerBase2 < 20) {
            fingerBase2 = (fingerBase2 + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'y':
        if (fingerUp2 > -140) {
            fingerUp2 = (fingerUp2 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'Y':
        if (fingerUp2 < 0) {
            fingerUp2 = (fingerUp2 + 5) % 360;
            glutPostRedisplay();
        }
        break;

        //finger 4 movement
    case 'u':
        if (fingerBase3 > -90) {
            fingerBase3 = (fingerBase3 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'U':
        if (fingerBase3 < 20) {
            fingerBase3 = (fingerBase3 + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'i':
        if (fingerUp3 > -140) {
            fingerUp3 = (fingerUp3 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'I':
        if (fingerUp3 < 0) {
            fingerUp3 = (fingerUp3 + 5) % 360;
            glutPostRedisplay();
        }
        break;
        //finger 5 movement
    case 'o':
        if (fingerBase4 > -90) {
            fingerBase4 = (fingerBase4 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'O':
        if (fingerBase4 < 20) {
            fingerBase4 = (fingerBase4 + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'p':
        if (fingerUp4 > -140) {
            fingerUp4 = (fingerUp4 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'P':
        if (fingerUp4 < 0) {
            fingerUp4 = (fingerUp4 + 5) % 360;
            glutPostRedisplay();
        }
        break;

        // elbow movement
    case 'c':
        if (elbow_1 < 130)
        {
            elbow_1 = (elbow_1 + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'C':
        if (elbow_1 > 0)
        {
            elbow_1 = (elbow_1 - 5) % 360;
            glutPostRedisplay();
        }
        break;
        // finger 1 movement
    case 'v':
        if (fingerBase_1 < 60) {
            fingerBase_1 = (fingerBase_1 + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'V':
        if (fingerBase_1 > -30) {
            fingerBase_1 = (fingerBase_1 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'b':
        if (fingerUp_1 < 90) {
            fingerUp_1 = (fingerUp_1 + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'B':
        if (fingerUp_1 > 0) {
            fingerUp_1 = (fingerUp_1 - 5) % 360;
            glutPostRedisplay();
        }
        break;
        // finger 2 movement
    case 'n':
        if (fingerBase1_1 > -90) {
            fingerBase1_1 = (fingerBase1_1 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'N':
        if (fingerBase1_1 < 20) {
            fingerBase1_1 = (fingerBase1_1 + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'm':
        if (fingerUp1_1 > -140) {
            fingerUp1_1 = (fingerUp1_1 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'M':
        if (fingerUp1_1 < 0) {
            fingerUp1_1 = (fingerUp1_1 + 5) % 360;
            glutPostRedisplay();
        }
        break;
        // finger 3 movement
    case ',':
        if (fingerBase2_1 > -90) {
            fingerBase2_1 = (fingerBase2_1 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case '<':
        if (fingerBase2_1 < 20) {
            fingerBase2_1 = (fingerBase2_1 + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case '.':
        if (fingerUp2_1 > -140) {
            fingerUp2_1 = (fingerUp2_1 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case '>':
        if (fingerUp2_1 < 0) {
            fingerUp2_1 = (fingerUp2_1 + 5) % 360;
            glutPostRedisplay();
        }
        break;

        //finger 4 movement
    case 'f':
        if (fingerBase3_1 > -90) {
            fingerBase3_1 = (fingerBase3_1 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'F':
        if (fingerBase3_1 < 20) {
            fingerBase3_1 = (fingerBase3_1 + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'g':
        if (fingerUp3_1 > -140) {
            fingerUp3_1 = (fingerUp3_1 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'G':
        if (fingerUp3_1 < 0) {
            fingerUp3_1 = (fingerUp3_1 + 5) % 360;
            glutPostRedisplay();
        }
        break;
        //finger 5 movement
    case 'h':
        if (fingerBase4_1 > -90) {
            fingerBase4_1 = (fingerBase4_1 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'H':
        if (fingerBase4_1 < 20) {
            fingerBase4_1 = (fingerBase4_1 + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'j':
        if (fingerUp4_1 > -140) {
            fingerUp4_1 = (fingerUp4_1 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'J':
        if (fingerUp4_1 < 0) {
            fingerUp4_1 = (fingerUp4_1 + 5) % 360;
            glutPostRedisplay();
        }
        break;
        // LEGS MOVEMENT
    case '1':
        if (legf[1] >= -70) {
            legf[1] = (legf[1] - 5) % 360;
            glutPostRedisplay();


        }
        break;

    case '2':
        if (legf[1] <= 70) {
            legf[1] = (legf[1] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case '9':
        if (legs[1] >= -30) {
            legs[1] = (legs[1] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case '/':
        if (legs[1] <= 30) {
            legs[1] = (legs[1] + 5) % 360;
            glutPostRedisplay();
        }

        break;


    case '3':
        if (legf[0] >= -70) {
            legf[0] = (legf[0] - 5) % 360;
            glutPostRedisplay();


        }
        break;

    case '4':
        if (legf[0] <= 70) {
            legf[0] = (legf[0] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'k':
        if (legs[0] >= -30) {
            legs[0] = (legs[0] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'K':
        if (legs[0] <= 30) {
            legs[0] = (legs[0] + 5) % 360;
            glutPostRedisplay();
        }

        break;

        // KNEES MOVEMENT
    case '7':
        if (knee[0] >= -30) {
            knee[0] = (knee[0] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case '8':
        if (knee[0] <= 30) {
            knee[0] = (knee[0] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case '5':
        if (knee[1] >= -30) {
            knee[1] = (knee[1] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case '6':
        if (knee[1] <= 30) {
            knee[1] = (knee[1] + 5) % 360;
            glutPostRedisplay();
        }
        break;


    case '+':
        moveForward();
        glutPostRedisplay();
        break;
    case '-':
        moveBackword();
        glutPostRedisplay();
        break;
    case '*':
        reset();
        glutPostRedisplay();
        break;

    case 27:
        exit(0);
        break;
    default:
        break;
    }

}

void specialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        rotateLeft();
        break;
    case GLUT_KEY_RIGHT:
        rotateRight();
        break;
    case GLUT_KEY_UP:
        rotateUp();
        break;
    case GLUT_KEY_DOWN:
        rotateDown();
        break;
    }
    glutPostRedisplay();
}


void mainMenu(int i) {

    switch (i) {

    case 1:
        reset();
        glutPostRedisplay();

        break;

    case 2:
        exit(0);
        break;
    }
}


int main(int argc, char** argv)
{


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
	init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    int subMenu = glutCreateMenu(myMenu);
    glutAddMenuEntry("Carpet 1", 0);
    glutAddMenuEntry("Carpet 2", 1);
    glutAddMenuEntry("Grass", 2);
    glutAddMenuEntry("Concrete", 3);
    glutCreateMenu(mainMenu);
    glutAddSubMenu("Change Floor", subMenu);
    glutAddMenuEntry("Reset", 1);
    glutAddMenuEntry("Exit", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}