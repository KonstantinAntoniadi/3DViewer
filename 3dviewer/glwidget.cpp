#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
}

GLWidget::~GLWidget()
{
    saveSettings();
    free_struct(object);
}

void GLWidget::saveSettings(){
    if (line_type == "solid") this->settings.setValue("line_type", "solid");
    else this->settings.setValue("line_type", "dotted");
    this->settings.setValue("line_width", this->line_width);
    this->settings.setValue("background_red", this->background_red);
    this->settings.setValue("background_green", this->background_green);
    this->settings.setValue("background_blue", this->background_blue);
    this->settings.setValue("background_alpha", this->background_alpha);

    this->settings.setValue("edge_red", this->edge_red);
    this->settings.setValue("edge_green", this->edge_green);
    this->settings.setValue("edge_blue", this->edge_blue);
    this->settings.setValue("edge_alpha", this->edge_alpha);

    this->settings.setValue("vertex_red", this->vertex_red);
    this->settings.setValue("vertex_green", this->vertex_green);
    this->settings.setValue("vertex_blue", this->vertex_blue);
    this->settings.setValue("vertex_alpha", this->vertex_alpha);

    this->settings.setValue("vertex_size", this->vertex_size);
    this->settings.setValue("vertex_type", this->vertex_type);

    this->settings.setValue("proection_type", this->proection_type);
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    loadSettings();
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void GLWidget::loadSettings()
{
    this->line_type = settings.value("line_type", "solid").toString();
    this->line_width = settings.value("line_width", 1.0).toDouble();

    this->background_red = settings.value("background_red", 0.0).toFloat();
    this->background_green = settings.value("background_green", 0.0).toFloat();
    this->background_blue = settings.value("background_blue", 1.0).toFloat();
    this->background_alpha = settings.value("background_alpha", 1.0).toFloat();

    this->edge_red = settings.value("edge_red", 0.0).toFloat();
    this->edge_green = settings.value("edge_green", 0.0).toFloat();
    this->edge_blue = settings.value("edge_blue", 0.0).toFloat();
    this->edge_alpha = settings.value("edge_alpha", 1.0).toFloat();

    this->vertex_red = settings.value("vertex_red", 0.0).toFloat();
    this->vertex_green = settings.value("vertex_green", 0.0).toFloat();
    this->vertex_blue = settings.value("vertex_blue", 0.0).toFloat();
    this->vertex_alpha = settings.value("vertex_alpha", 1.0).toFloat();

    this->vertex_size = settings.value("vertex_size", 1.0).toDouble();
    this->vertex_type = settings.value("vertex_type", "circle").toString();

    this->proection_type = settings.value("proection_type", "parallel").toString();
}

void GLWidget::paintGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int size = 3;
    float far = 150;
    float fov = 60.0 * M_PI / 180;  // 60 угол в градусах
    float heapHeight = size / (2 * tan(fov / 2));
    if (proection_type == "parallel") {
        glOrtho(-size, size, -size, size, -far, far);
    } else {
        glFrustum(-size, size, -size, size, heapHeight, far);
    }
    glClearColor(background_red, background_green, background_blue, background_alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -3 * heapHeight);
    drawObject();
}

void GLWidget::upload_file(QString filepath) {
    free_struct(object);
    object = (data_t* )calloc(1, sizeof(data_t));
    if (object) {
        file = filepath.toLocal8Bit().data();
        if (parse_obj_file(file, object) && allocate_memory_for_array_of_vertexes(file, object)) {
            object->numbers_of_vertexes_in_facets = 0;
            if (allocate_memory_for_array_vertexes_in_facets(file, object) && allcocate_memory_for_array_of_facets(file, object)) {
                centering(object->single_array_for_vertexes, object->count_of_vertexes);
                for (unsigned int i = 0; i < object->numbers_of_vertexes_in_facets; i++)
                    object->single_array_for_facets[i] = object->single_array_for_facets[i] - 1;
                update();
            }
        }
    }
    

}


void GLWidget::drawObject() {
    if (object){
        glPointSize(vertex_size);

        glVertexPointer(3, GL_DOUBLE, 0, object->single_array_for_vertexes);
        glEnableClientState(GL_VERTEX_ARRAY);
        glLineWidth(line_width);

        if (vertex_type == "square") {
            glDisable(GL_POINT_SMOOTH);
            glEnable(GL_POINT_SPRITE);
        }
        else if (vertex_type == "circle") {
            glDisable(GL_POINT_SPRITE);
            glEnable(GL_POINT_SMOOTH);
        }

        if (vertex_type != "none") {
             glColor4d(vertex_red, vertex_green, vertex_blue, vertex_alpha);
             glDrawArrays(GL_POINTS, 0, object->count_of_vertexes);
        }
        glColor4d(edge_red, edge_green, edge_blue, edge_alpha);

        if (line_type == "dotted") {
            glPushAttrib(GL_ENABLE_BIT);
            glLineStipple(7, 0xAAAA);
            glEnable(GL_LINE_STIPPLE);
        } else {
            glDisable(GL_LINE_STIPPLE);
        }
        glDrawElements(GL_LINES, object->numbers_of_vertexes_in_facets, GL_UNSIGNED_INT, object->single_array_for_facets);

        glDisableClientState(GL_VERTEX_ARRAY);
    }

}
