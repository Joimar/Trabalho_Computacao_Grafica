#ifndef MODELS_H
#define MODELS_H
//#include <QGLWidget>
//#include <QTime>
//#include <QHash>


class Models
{
public:
    Models();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    //void keyPressEvent(QKeyEvent *event);
    //void changeEvent(QEvent *event);

private:
      void drawCube();
};

#endif // MODELS_H
