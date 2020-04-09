/*
 * This code was created by Jeff Molofee 1999
 * And ported to C++/Qt4 by Wesley Stessens 2009
 *
 * Contact Jeff through his website: http://nehe.gamedev.net/
 * Contact Wesley for port-specific comments: wesley@ubuntu.com
 */
#include <GL/glut.h>
#include "glwidget.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <QKeyEvent>
#include <QTimer>
#include <string>

#include <stdlib.h>

#define gravidade 0.01f

using std::string;
// Constructor
GLWidget::GLWidget() {
    setWindowTitle("Flapbird 3D");
    time = QTime::currentTime();
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    filter = 2;
    lightChanged = filterChanged = false;
    changeAmount = 0;
    xrot = yrot = xspeed = yspeed = 0.0f;
    z = -5.0f;

    xinimigo = 20.0f; //distancia
    ytran = 4.0f;

    perdeu = false;
}

// Destructor
GLWidget::~GLWidget() {
    glDeleteTextures(3, texture);
}

// Initialize OpenGL
void GLWidget::initializeGL() {
    glShadeModel(GL_SMOOTH); // Enable smooth shading
    qglClearColor(Qt::black); // Set the clear color to a black background

    glClearDepth(1.0f); // Depth buffer setup
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glDepthFunc(GL_LEQUAL); // Set type of depth test

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really nice perspective calculations

    // Set up textures
    // Note: Because we need different texture filters, we cannot use Qt's bindTexture()
    //       Qt automatically generates the most appropriate textures using available features in your setup
    //       which means that Qt always generates mipmaps if your system supports them
    //       We will use normal glBindTexture() etc. functions instead

    glEnable(GL_TEXTURE_2D);
    QImage imgParede = convertToGLFormat(QImage("../Flapbird/bricks.bmp"));
    QImage imgTelhado = convertToGLFormat(QImage("../Flapbird/roof4.jpg"));
    QImage imgJanela = convertToGLFormat(QImage("../Flapbird/window.bmp"));
    QImage imgPorta = convertToGLFormat(QImage("../Flapbird/door.bmp"));
    QImage imgCano = convertToGLFormat(QImage("../Flapbird/verde_abacate.jpg"));

    glGenTextures(5, texture);

    // Texture da Parede
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imgParede.width(), imgParede.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgParede.bits());

    // Texture da Telhado
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imgTelhado.width(), imgTelhado.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgTelhado.bits());

    // Texture da Janela
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgJanela.width(), imgJanela.height(), GL_RGBA, GL_UNSIGNED_BYTE, imgJanela.bits());

    // Textura da Porta
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgPorta.width(), imgPorta.height(), GL_RGBA, GL_UNSIGNED_BYTE, imgPorta.bits());

    //Textura do Cano
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgCano.width(), imgCano.height(), GL_RGBA, GL_UNSIGNED_BYTE, imgCano.bits());

    // Set up lighting
    GLfloat ambLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat diffLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightPos[] = {0.0f, 0.0f, 2.0f, 1.0f};
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffLight);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
}



// This is called when the OpenGL window is resized
void GLWidget::resizeGL(int width, int height) {
    // Prevent divide by zero (in the gluPerspective call)
    if (height == 0)
        height = 1;

    glViewport(0, 0, width, height); // Reset current viewport

    glMatrixMode(GL_PROJECTION); // Select projection matrix
    glLoadIdentity(); // Reset projection matrix

    gluPerspective(45.0f, static_cast<GLfloat>(width)/height, 0.1f, 100.0f); // Calculate aspect ratio

    glMatrixMode(GL_MODELVIEW); // Select modelview matrix
    glLoadIdentity(); // Reset modelview matrix
}

void GLWidget::drawEnemy()
{
    //glBindTexture(GL_TEXTURE_2D, 0);
   // glDisable(GL_TEXTURE_2D);
   // glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glBegin(GL_QUADS);


        // Front Face
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 4.0f, 1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 4.0f, 1.0f); // Top Left Of The Texture and Quad

        // Back Face
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 4.0f, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 4.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad

        // Top Face
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad

        // Bottom Face
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad

        // Right face
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 4.0f, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 4.0f, 1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad

        // Left Face
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 4.0f, 1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 4.0f, -1.0f); // Top Left Of The Texture and Quad


    glEnd();

}


void GLWidget::drawPipe(GLfloat distancia, GLfloat altura)
{

    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glBegin(GL_QUADS);


        // Front Face
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f + distancia, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f + distancia, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f + distancia, altura, 1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f + distancia, altura, 1.0f); // Top Left Of The Texture and Quad

        // Back Face
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f + distancia, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f + distancia, altura, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f + distancia, altura, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f + distancia, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad

        // Top Face
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f + distancia, 1.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f + distancia, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f + distancia, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f + distancia, 1.0f, -1.0f); // Top Right Of The Texture and Quad

        // Bottom Face
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f + distancia, -1.0f, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f + distancia, -1.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f + distancia, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f + distancia, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad

        // Right face
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f + distancia, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f + distancia, altura, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f + distancia, altura, 1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f + distancia, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad

        // Left Face
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f + distancia, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f + distancia, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f + distancia, altura, 1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f + distancia, altura, -1.0f); // Top Left Of The Texture and Quad


    glEnd();

    if(frontalCollision(-1.0f, 1.0f, -1.0f + ytran, distancia, 1.0f + distancia, altura, -1.0f))
    {
        xinimigo = 20.0f;//fazer condição de perda
    }

}

bool GLWidget::frontalCollision(GLfloat birdX1, GLfloat birdX2, GLfloat birdY2, GLfloat canoX1, GLfloat canoX2, GLfloat canoY1, GLfloat canoY2)
{
//    GLfloat GLbirdX1, GLbirdy1, GLbirdX2, GLbirdY2, GLcanoX1, GLcanoY1, GLcanoX2, GLcanoY2;

//    GLbirdX1 = birdX1;
//    GLbirdy1 = birdY1;

//    GLbirdX2 = birdX2;
//    GLbirdY2 = bird

    if(birdX2 >= canoX1 && birdX2<=canoX2)
    {
        if(birdY2 <= canoY1 && birdY2 >= canoY2)
        {
            return true;
        }
    }

    if(birdX1 >= canoX1 && birdX1 <= canoX2)
    {
        if(birdY2 <= canoY1 && birdY2 >= canoY2)
        {
            return true;
        }
    }


    return false;
}

bool GLWidget::verticalCollision(GLfloat birdY1, GLfloat birdY2, GLfloat canoY1, GLfloat canoY2)
{
    if(birdY1 >= canoY2 && birdY1 <= canoY1) return true;
    if(birdY2 >= canoY2 && birdY2 <= canoY1) return true;

    return false;

}


// Draw a cube using OpenGL
void GLWidget::drawCube() {
    //qglColor(Qt::white);
    //glColor3f(1.0f,1.0f,0.0f);


    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glBegin(GL_QUADS);

   glColor3f(1.0f,0.0f,0.0f);
        // Front Face
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
        // Back Face
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
        // Top Face
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
        // Bottom Face
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        // Right face
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        // Left Face
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad


    glEnd();

    //parede frontal
    glBegin(GL_POLYGON);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(0.5f, 1.0f); glVertex3f( 0.1f, 2.0f, -1.0f); //bottom contral
        glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);// Bottom of Right
    glEnd();
    //parede traseira
    glBegin(GL_POLYGON);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(0.5f, 1.0f); glVertex3f( 0.1f, 2.0f, 1.0f); //bottom left
        glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);// Bottom of Right
    glEnd();
}

//void GLWidget::drawRoof(){}


void GLWidget::drawRoof()
{
    glBindTexture(GL_TEXTURE_2D, texture[1]);

    //qglColor(Qt::white);
    // Telha esquerda
    glBegin(GL_POLYGON);

        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.1f, 2.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.1f, 2.0f, -1.0f); // Top Right Of The Texture and Quad
    glEnd();
    //telha direita
    glBegin(GL_POLYGON);
        //glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.1f, 2.0f, -1.0f);  //Top of Left
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.1f, 2.0f, 1.0f); //bottom left
        glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);// Bottom of Right
        glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);//telha direita Top of Right
    glEnd();

}

void GLWidget::drawDoor()
{
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.25f, -0.95f, 1.01f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.2f, -0.95f, 1.01f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.2f, 0.1f, 1.01f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.25f, 0.1f, 1.01f); // Top Left Of The Texture and Quad
    glEnd();

}

void GLWidget::drawWindow()
{
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    //Janela frontal direita
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 1.01f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.9f, -0.5f, 1.01f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.9f, 0.1f, 1.01f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.1f, 1.01f); // Top Left Of The Texture and Quad
    glEnd();

    //Janela frontal esquerda
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 1.01f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.9f, -0.5f, 1.01f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.9f, 0.1f, 1.01f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.1f, 1.01f); // Top Left Of The Texture and Quad
    glEnd();

    //Janela lado direito perto da frente
    glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(1.01f, -0.5f, 0.8f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(1.01f, -0.5f, 0.2f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(1.01f, 0.1f, 0.2f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(1.01f, 0.1f, 0.8f); // Top Left Of The Texture and Quad
    glEnd();

    //Janela lado direito perto de trás
    glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(1.01f, -0.5f, -0.1f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(1.01f, -0.5f, -0.8f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(1.01f, 0.1f, -0.8f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(1.01f, 0.1f,  -0.1f); // Top Left Of The Texture and Quad
    glEnd();

}

// OpenGL painting code goes here
void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen and depth buffer



    // Draw cube
    glLoadIdentity(); // Reset current modelview matrix
    //glTranslatef(0.0f, 0.0f, z); // Move into the screen faz a camera ficar fora e não dentro do objeto
    //glTranslatef(0.0f, 0.0f, -10.0f); // Move into the screen faz a camera ficar fora e não dentro do objeto
    glTranslatef(0.0f, ytran, -20.0f); // -20.0f faz ele andar para a direita
    glRotatef(xrot, 1.0f, 0.0f, 0.0f); // Rotate on X-axis
    glRotatef(yrot, 0.0f, 1.0f, 0.0f); // Rotate on Y-axis







    glClearColor (0.0f, 0.0f, 0.0f, 1.0f);




    xrot += xspeed; // X-axis rotation
    yrot += yspeed; // Y-axis rotation



    drawCube();

    drawRoof();

    drawDoor();

    drawWindow();

    // Show message when an enabled OpenGL feature has changed
    ytran = ytran - gravidade;
    //Outro objeto
    glLoadIdentity();



    glTranslatef(xinimigo, 0.0f, -20.0f);
    //drawRoof();
    //drawEnemy();

    //Fase--------
     drawPipe(xinimigo, 4);
     drawPipe(xinimigo+5, 5);
     drawPipe(xinimigo+8, 5);
     drawPipe(xinimigo+13, 3);
     drawPipe(xinimigo+21, 5);
     drawPipe(xinimigo+25, 6);
    //-----------


    xinimigo -= 0.02f;
    glLoadIdentity();



//    if(frontalCollision(-1.0f, 1.0f, -1.0f + ytran, xinimigo, 1.0f + xinimigo, 4.0f, -1.0f))
//    {
//        xinimigo = 40.0f;
//    }



    if ((lightChanged || filterChanged) && changeAmount > 0) {
        QString str;
        if (filterChanged)
            str = QString("Now filtering using ") ;
        else // lightChanged
            str = QString("Lighting is now ") + (glIsEnabled(GL_LIGHTING) ? "on" : "off");
        QFont font;
        QFontMetrics metrics(font);
        font.setPixelSize(20);
        font.setBold(true);
        if (changeAmount < 100)
            glColor4f(1.0, 1.0, 1.0, static_cast<GLfloat>(changeAmount) / 100);
        else
            glColor3f(1.0, 1.0, 1.0);
        this->renderText(10, 10 + metrics.height(), str, font);
        changeAmount--;
    }

    // Framerate control
    int delay = time.msecsTo(QTime::currentTime());
    if (delay == 0)
        delay = 1;
    time = QTime::currentTime();
    timer->start(qMax(0, 20 - delay));
}

// Key handler
void GLWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Escape:
        close(); // Quit on Escape
        break;
    case Qt::Key_F1:
        setWindowState(windowState() ^ Qt::WindowFullScreen); // Toggle fullscreen on F1
        break;
    case Qt::Key_L:
        if (glIsEnabled(GL_LIGHTING))
            glDisable(GL_LIGHTING);
        else
            glEnable(GL_LIGHTING);
        filterChanged = false;
        lightChanged = true;
        changeAmount = 140;
        break;
    case Qt::Key_F:
        ++filter;
        if (filter > 2)
            filter = 0;
        lightChanged = false;
        filterChanged = true;
        changeAmount = 140;
        break;
    case Qt::Key_PageUp:
        z -= 0.05f;
        break;
    case Qt::Key_PageDown:
        z += 0.05f;
        break;
    case Qt::Key_Up:
        xspeed -= 0.02f;
        break;
    case Qt::Key_Down:
        xspeed += 0.02f;
        break;
    case Qt::Key_Left:
        yspeed -= 0.02f;
        break;
    case Qt::Key_Right:
        yspeed += 0.02f;
        break;
    case Qt::Key_Space:
        ytran += 0.15f;
        break;
    default:
        QGLWidget::keyPressEvent(event); // Let base class handle the other keys
    }
}

void GLWidget::changeEvent(QEvent *event) {
    switch (event->type()) {
        case QEvent::WindowStateChange:
            // Hide cursor if the window is fullscreen, otherwise show it
            if (windowState() == Qt::WindowFullScreen)
                setCursor(Qt::BlankCursor);
            else
                unsetCursor();
            break;
        default:
            break;
    }
}
