QT       += core gui opengl widgets
#LIBS+= -lglut -lGLU -lGL
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += openglwidgets
isEqual(QT_MAJOR_VERSION, 5): LIBS += -lGLU
include(./QtGifImage/src/gifimage/qtgifimage.pri)
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    glwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    backend/3d_v.c \
    backend/affine_transformations.c

HEADERS += \
    glwidget.h \
    mainwindow.h \
    backend/3d_v.h \
    backend/affine_transformations.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    3dviewer_en_AS.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
