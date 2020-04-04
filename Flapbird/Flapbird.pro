
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        fase1.cpp \
        loadobj.cpp \
        main.cpp \
        models.cpp

HEADERS += \
    fase1.h \
    loadobj.h \
    models.h

    LIBS += /usr/lib/x86_64-linux-gnu/libglut.so \
        -lGL \
        -lGLU \
        -lglfw

