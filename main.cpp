#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "imageloader.h"
#include "glm.h"


#define PI 3.14

static int shoulder = 0, elbow = 0, shoulder1 = 0, fingerBase = 0, fingerUp = 0, fingerBase1 = 0, fingerUp1 = 0, fingerBase2 = 0, fingerUp2 = 0, fingerBase3 = 0, fingerUp3 = 0, fingerBase4 = 0, fingerUp4 = 0, fingerBase5 = 0, fingerUp5 = 0;
static int shoulder_1 = 0, elbow_1 = 0, shoulder1_1 = 0, fingerBase_1 = 0, fingerUp_1 = 0, fingerBase1_1 = 0, fingerUp1_1 = 0, fingerBase2_1 = 0, fingerUp2_1 = 0, fingerBase3_1 = 0, fingerUp3_1 = 0, fingerBase4_1 = 0, fingerUp4_1 = 0, fingerBase5_1 = 0, fingerUp5_1 = 0;

static int legf[] = { 0, 0 };
static int legs[] = { 0, 0 };
static int knee[] = { 0, 0 };

int moving, startx, starty;

GLfloat angle = 0.0;  /* in degrees */
GLfloat angle2 = 0.0; /* in degrees */

double eye[] = { -30.0, 5.0, -10.0 };
double center[] = { 0.0, 0.0, 0.0 };
double up[] = { 0.0, 1.0, 0.0 };
float speed = 0.1;
float rotationAngle = 0.1;

//loading objects
const char* ball = "data/soccerball.obj";
const char* goal = "data/football-goal.obj";

//ball location
float ballLocationZ = 0.0;
float ballLocationY = 0.0;

// light

float light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
float light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };


GLfloat light_position[] = { 2.0, 2.0, 3.0, 1.0 };
GLfloat lightPos0[] = { -2.0, 2.0, -3.0, 1.0 };

//texture IDs
GLuint _textureId0; //The id of the texture Grass 1
GLuint _textureId1; //The id of the texture Grass 2
GLuint _textureId2; //The id of the texture Concrete


//frame number for both animations 
static int poseNo = 0;
static int celebNo = 0;

static int animation = 0; //to know what animation

//poses for shooting
int shootingPoses[10][7]{ {-55, -50, 5, 0, 0,ballLocationZ,ballLocationY},
                 {-45, -40, 5, 0, 0,ballLocationZ,ballLocationY},
                 {-30, -25, 5, 0, 0,ballLocationZ,ballLocationY},
                 {-15, -20, 5, 0, 0,ballLocationZ,ballLocationY},
                 {0, -10, 5, 0, 0,ballLocationZ,ballLocationY},
                 {15, -5, 5, 0, 0,ballLocationZ,ballLocationY},
                {20, -5, 5, 0, 0, ballLocationZ - 5, ballLocationY + 2},
                {10, 0, 5, 0, 0, ballLocationZ - 15, ballLocationY + 4},
                {0, 0, 5, 0, 0, ballLocationZ - 20, ballLocationY + 6},
                {0, 0, 5, 0, 0, ballLocationZ - 22, ballLocationY + 6} };


//poses for clebrating
int celebratingPoses[8][5]{ {-5, 0, -5, 0, 0} ,
    {-10,-10,-10,-10,80},
    {-20,-40,-20,-40,100},
    { -20,-55,-20,-55,120 },
    {-20 , -40 , -20 ,-40 , 135},
    {-10, -10, -10, -10, 100} ,
    {-5, -5, -5, -5, 80},
    {0, 0, -5, 0, 0} };


// to set poses according to the chosen animation anf frame number
void setPose(int animation, int frame) {

    switch (animation) {
    case 0:                         //shooting

        legf[1] = shootingPoses[frame][0];
        knee[1] = shootingPoses[frame][1];
        legf[0] = shootingPoses[frame][2];
        knee[0] = shootingPoses[frame][3];
        shoulder = shootingPoses[frame][4];
        shoulder_1 = shootingPoses[frame][4];
        ballLocationZ = shootingPoses[frame][5];
        ballLocationY = shootingPoses[frame][6];

        break;

    case 1:                       //celebrating

        legf[1] = celebratingPoses[frame][0];
        knee[1] = celebratingPoses[frame][1];
        legf[0] = celebratingPoses[frame][2];
        knee[0] = celebratingPoses[frame][3];
        shoulder = celebratingPoses[frame][4];
        shoulder_1 = celebratingPoses[frame][4];
        break;
    }
}

//to update pose number
void poseTimer(int value) {

    switch (value) {

    case 0:

        if (animation == 1) {

            poseNo = poseNo % 10;
            setPose(0, poseNo);
            poseNo++;
            glutPostRedisplay();
            glutTimerFunc(150, poseTimer, 0);
        }
        break;

    case 1:

        if (animation == 2) {

            celebNo = celebNo % 8;
            setPose(1, celebNo);
            celebNo++;
            glutPostRedisplay();
            glutTimerFunc(150, poseTimer, 1);
        }
        break;
    }
}




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


//Draw Model
void drawmodel(char* filename)
{
    GLMmodel* model = glmReadOBJ(filename);
    glmUnitize(model);
    glmFacetNormals(model);
    glmVertexNormals(model, 90.0);
    glmScale(model, .15);
    glmDraw(model, GLM_SMOOTH | GLM_MATERIAL);
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



void init(void)
{
    Image* image2 = loadBMP("images/concrete.bmp");
    _textureId2 = loadTexture(image2);
    delete image2;

    Image* image1 = loadBMP("images/grass2.bmp");
    _textureId1 = loadTexture(image1);
    delete image1;

    Image* image0 = loadBMP("images/grass1.bmp");
    _textureId0 = loadTexture(image0);
    delete image0;

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glLightfv(GL_LIGHT1, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    glEnable(GL_NORMALIZE);
    //Enable smooth shading
    glShadeModel(GL_SMOOTH);

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
    glScalef(3.0, 7.0, 1);
    glutWireCube(1.0);
    glPopMatrix();

    //for the face moheb 2

    glPushMatrix();
    glTranslatef(0.0, 4.75, 0.0);
    glutWireSphere(1.0, 15, 15);
    glPopMatrix();

    //for legs
    // START OF RIGHT LEG
    glPushMatrix();


    glTranslatef(1.0, -3.5, 0.0);
    glRotatef((GLfloat)legf[0], 1.0, 0.0, 0.0);
    glRotatef((GLfloat)legs[0], 0.0, 0.0, 1.0);

    glTranslatef(-1.0, 3.5, 0.0);
    glTranslatef(1.0, -5.25, 0.0);

    glPushMatrix();
    glScalef(1.0, 3.5, 1);
    glutWireCube(1.0);
    glPopMatrix();

    glTranslatef(0.0, -1.75, 0.0);

    glRotatef(GLfloat(knee[0]), 1.0, 0.0, 0.0);

    glTranslatef(0.0, 1.75, 0.0);
    glTranslatef(0.0, -3.5, 0.0);

    glPushMatrix();
    glScalef(1.0, 3.5, 1.0);
    glutWireCube(1.0);
    glPopMatrix();

    glTranslatef(0.0, -1.5, 0.0);
    glPushMatrix();
    glScalef(1.0, 0.5, 1.75);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
    // END OF RIGHT LEG

    // START OF LEFT LEG
    glPushMatrix();

    glTranslatef(-1.0, -3.5, 0.0);
    glRotatef((GLfloat)legf[1], 1.0, 0.0, 0.0);
    glRotatef((GLfloat)legs[1], 0.0, 0.0, 1.0);

    glTranslatef(1.0, 3.5, 0.0);
    glTranslatef(-1.0, -5.25, 0.0);

    glPushMatrix();

    glScalef(1.0, 3.5, 1.0);
    glutWireCube(1.0);
    glPopMatrix();

    glTranslatef(0.0, -1.75, 0.0);

    glRotatef(GLfloat(knee[1]), 1.0, 0.0, 0.0);

    glTranslatef(0.0, 1.75, 0.0);
    glTranslatef(0.0, -3.5, 0.0);

    glPushMatrix();
    glScalef(1.0, 3.5, 1);
    glutWireCube(1.0);
    glPopMatrix();

    glTranslatef(0.0, -1.5, 0.0);
    glPushMatrix();
    glScalef(1.0, 0.5, 1.75);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
    // END OF LEFT LEG


    //left Arm fatma

    glPushMatrix();
    glTranslatef(2.0, 2.0, 0.0);
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
    glRotatef(-90.0, 0.0, 0.0, 1.0);
    glRotatef(-180.0, 1.0, 0.0, 0.0);

    glTranslatef(-2.0, 2.0, 0.0);


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



    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30, -10.65, 35);
    glTexCoord2f(3.0f, 0.0f); glVertex3f(30, -10.65, 35);
    glTexCoord2f(3.0f, 3.0f); glVertex3f(30, -10.65, -35);
    glTexCoord2f(0.0f, 3.0f); glVertex3f(-30, -10.65, -35);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    if (animation == 0 || animation == 1) {
        glPushMatrix();
        glTranslatef(-1, (-9 + ballLocationY), (-3.0 + ballLocationZ));
        glScalef(10, 10, 10);
        drawmodel((char*)ball);
        glPopMatrix();
    }
    else if (animation == 2) {
        glPushMatrix();
        glTranslatef(-1, -9 , -22.0 );
        glScalef(10, 10, 10);
        drawmodel((char*)ball);
        glPopMatrix();
    }
    glPushMatrix();
    glTranslatef(0, -4.5, -25);
    glScalef(50, 50, 50);
    drawmodel((char*)goal);
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

     ballLocationZ = 0.0;
     ballLocationY = 0.0;

    double eyeReset[] = { -30.0, 5.0, -10.0 };
    double centerReset[] = { 0.0, 0.0, 0.0 };
    double upReset[] = { 0.0, 1.0, 0.0 };
    for (int i = 0; i < 3; i++)
    {
        eye[i] = eyeReset[i];
        center[i] = centerReset[i];
        up[i] = upReset[i];
    }

    glBindTexture(GL_TEXTURE_2D, _textureId0);
    animation = 0;
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
            elbow = (elbow + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'D':
        if (elbow < 0)
        {
            elbow = (elbow - 5) % 360;
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
    case '-':
        if (fingerUp1 > -140) {
            fingerUp1 = (fingerUp1 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case '+':
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
    case '}':
        if (fingerUp3 > -140) {
            fingerUp3 = (fingerUp3 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case ']':
        if (fingerUp3 < 0) {
            fingerUp3 = (fingerUp3 + 5) % 360;
            glutPostRedisplay();
        }
        break;
        //finger 5 movement
    case '[':
        if (fingerBase4 > -90) {
            fingerBase4 = (fingerBase4 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case '{':
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
            elbow_1 = (elbow_1 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'C':
        if (elbow_1 > 0)
        {
            elbow_1 = (elbow_1 + 5) % 360;
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
        if (knee[0] >= -50) {
            knee[0] = (knee[0] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case '8':
        if (knee[0] < 0) {
            knee[0] = (knee[0] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case '5':
        if (knee[1] >= -50) {
            knee[1] = (knee[1] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case '6':
        if (knee[1] < 0) {
            knee[1] = (knee[1] + 5) % 360;
            glutPostRedisplay();
        }
        break;


    case 'i':
        moveForward();
        glutPostRedisplay();
        break;
    case 'o':
        moveBackword();
        glutPostRedisplay();
        break;
    case 'r':
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




void animationMenu(int id) {

    switch (id) {

    case 0:
        animation = 1;
        glutTimerFunc(0, poseTimer, 0);
        glutPostRedisplay();
        break;

    case 1:
        animation = 2;
        glutTimerFunc(0, poseTimer, 1);
        glutPostRedisplay();
        break;

    }
}


void floorMenu(int id) {

    switch (id) {

    case 0:
        glBindTexture(GL_TEXTURE_2D, _textureId0);
        glutPostRedisplay();

        break;

    case 1:
        glBindTexture(GL_TEXTURE_2D, _textureId1);
        glutPostRedisplay();

        break;

    case 2:
        glBindTexture(GL_TEXTURE_2D, _textureId2);
        glutPostRedisplay();

        break;

    default:
        break;

    }

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




void createPopupMenus() {

    int subMenu = glutCreateMenu(floorMenu);
    glutAddMenuEntry("Grass 1", 0);
    glutAddMenuEntry("Grass 2", 1);
    glutAddMenuEntry(" Concrete", 2);

    int subMenu2 = glutCreateMenu(animationMenu);
    glutAddMenuEntry("Shooting", 0);
    glutAddMenuEntry("Celebrating", 1);

    glutCreateMenu(mainMenu);
    glutAddSubMenu("Change Floor", subMenu);
    glutAddSubMenu("Animation", subMenu2);
    glutAddMenuEntry("Reset", 1);
    glutAddMenuEntry("Exit", 2);



    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


int main(int argc, char** argv)
{


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
	init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys); 
    createPopupMenus();

    glutMainLoop();

    return 0;
}