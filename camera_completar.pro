TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11

INCLUDEPATH += bib
INCLUDEPATH += gui_glut
INCLUDEPATH += user

LIBS += -lGL -lGLU -lglut

HEADERS += \
    bib/Camera.h \
    bib/CameraDistante.h \
    bib/CameraJogo.h \
    bib/Desenha.h \
    bib/Vetor3D.h \
    modelo/bule.h \
    modelo/casa.h \
    modelo/cubo.h \
    modelo/modelo.h

SOURCES += \
    bib/Camera.cpp \
    bib/CameraDistante.cpp \
    bib/CameraJogo.cpp \
    bib/Desenha.cpp \
    bib/Vetor3D.cpp \
    main.cpp \
    modelo/bule.cpp \
    modelo/casa.cpp \
    modelo/cubo.cpp \
    modelo/modelo.cpp

