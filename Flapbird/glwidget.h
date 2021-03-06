/*
 * This code was created by Jeff Molofee 1999
 * And ported to C++/Qt4 by Wesley Stessens 2009
 *
 * Contact Jeff through his website: http://nehe.gamedev.net/
 * Contact Wesley for port-specific comments: wesley@ubuntu.com
 */

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTime>
#include <QHash>
#include <GL/glut.h>

#include <stdlib.h>

class QTimer;

class GLWidget : public QGLWidget {
    Q_OBJECT

public:
    GLWidget();
    ~GLWidget();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *event);
    void changeEvent(QEvent *event);

private:
    void drawCube();
    void drawEnemy();
    void drawRoof();
    void drawDoor();
    void drawWindow();
    void textureRoof();
    bool frontalCollision(GLfloat birdX1, GLfloat birdX2, GLfloat birdY2, GLfloat canoX1, GLfloat canoX2, GLfloat canoY1, GLfloat canoY2);
    bool verticalCollision(GLfloat birdY1, GLfloat birdY2, GLfloat canoY1, GLfloat canoY2);
    void drawPipe(GLfloat distancia, GLfloat altura);
    void drawCenario();
    void drawFish();
    QTimer *timer;
    QTime time;
    GLuint filter, texture[7];
    GLfloat xrot, yrot, xspeed, yspeed, z, xinimigo, ytran;
    bool lightChanged, filterChanged, perdeu;
    int changeAmount;
};

#endif // GLWIDGET_H
