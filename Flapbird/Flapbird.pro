QT += opengl

TARGET = Flapbird
TEMPLATE = app

RESOURCES += res.qrc

SOURCES += \
        fase1.cpp \
        glwidget.cpp \
        loadobj.cpp \
        main.cpp \
        models.cpp

HEADERS += \
    fase1.h \
    glwidget.h \
    loadobj.h \
    models.h



    LIBS += /usr/lib/x86_64-linux-gnu/libglut.so \
        -lGL \
        -lGLU \
        -lglfw

