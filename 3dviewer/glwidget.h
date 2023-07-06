#ifndef GLWIDGET_H
#define GLWIDGET_H

#define GL_SILENCE_DEPRECATION
#include <QtWidgets>
#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QSettings>
#include <GLUT/glut.h>
#include <QSettings>
#include "qgifimage.h"

extern "C" {
#include "backend/3d_v.h"
#include "backend/affine_transformations.h"
}

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent = nullptr);
    double rot_x, rot_y;
    QSettings settings;
    QString line_type;
    QString vertex_type;
    QString proection_type;
    double line_width;
    double vertex_size;
    void saveSettings();
    ~GLWidget();
    float background_red, background_green, background_blue, background_alpha;
    float edge_red, edge_green, edge_blue, edge_alpha;
    float vertex_red, vertex_green, vertex_blue, vertex_alpha;
    void drawObject();
    QTimer *timer = NULL;
    int time = 0;
    QGifImage *gif = NULL;
    void upload_file(QString filepath);
    data_t* object = NULL;
    data_t* object_temp = NULL;
    double x_last_move = 0, y_last_move = 0, z_last_move = 0;
    double scale_last = 1;
    double angle_x_last = 0, angle_y_last = 0, angle_z_last = 0;

private:
    char* file = NULL;
    float xRot = 0, yRot = 0, zRot = 0;
    float xRot_last = 0, yRot_last = 0, zRot_last = 0;
    QPoint mPos;
    int start_x = 0, start_y = 0;
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void loadSettings();
};


#endif // GLWIDGET_H
