#include "mainwindow.h"

#include <QSettings>

#include "glwidget.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  sincSettings();

}

void MainWindow::move() {
  if (ui->GLwidget->object) {
    move_object(ui->GLwidget->object, ui->xMoveSpinBox->text().toDouble(), 0, 0);
    ui->GLwidget->update();
  }
}

void MainWindow::sincSettings() {
  if (ui->GLwidget->settings.value("line_type", "solid").toString() == "solid")
    ui->solidRadioButton->setChecked(true);
  else if (ui->GLwidget->settings.value("line_type", "solid").toString() ==
           "dotted")
    ui->dottedRadioButton->setChecked(true);
  ui->thicknessEdgeSpinBox->setValue(
      ui->GLwidget->settings.value("line_width", 1.0).toDouble());
  ui->sizeVertexSpinBox->setValue(
      ui->GLwidget->settings.value("vertex_size", 1.0).toDouble());

  if (ui->GLwidget->settings.value("vertex_type", "circle").toString() ==
      "circle")
    ui->circleDisplayRadioButton->setChecked(true);
  else if (ui->GLwidget->settings.value("vertex_type").toString() == "square")
    ui->squareDisplayRadioButton->setChecked(true);
  else if (ui->GLwidget->settings.value("vertex_type").toString() == "none")
    ui->noneDisplayRadioButton->setChecked(true);

  if (ui->GLwidget->settings.value("proection_type", "parallel").toString() ==
      "parallel")
    ui->parallelRadioButton->setChecked(true);
  else
    ui->centralRadioButton->setChecked(true);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_solidRadioButton_clicked() {
  ui->GLwidget->line_type = "solid";
  ui->GLwidget->update();
}

void MainWindow::on_dottedRadioButton_clicked() {
  ui->GLwidget->line_type = "dotted";
  ui->GLwidget->update();
}

void MainWindow::on_thicknessEdgeSpinBox_valueChanged(double arg1) {
  ui->GLwidget->line_width = arg1;
  ui->GLwidget->update();
}

void MainWindow::on_chooseColorBackgroundLabel_clicked() {
  QColor color = QColorDialog::getColor(Qt::yellow, this);
  if (color.isValid()) {
    ui->GLwidget->background_red = color.redF();
    ui->GLwidget->background_green = color.greenF();
    ui->GLwidget->background_blue = color.blueF();
    ui->GLwidget->background_alpha = color.alphaF();
    ui->GLwidget->update();
  }
}

void MainWindow::on_chooseColorEdgeButton_clicked() {
  QColor color = QColorDialog::getColor(Qt::yellow, this);
  if (color.isValid()) {
    ui->GLwidget->edge_red = color.redF();
    ui->GLwidget->edge_green = color.greenF();
    ui->GLwidget->edge_blue = color.blueF();
    ui->GLwidget->edge_alpha = color.alphaF();
    ui->GLwidget->update();
  }
}

void MainWindow::on_chooseColorVertexButton_clicked() {
  QColor color = QColorDialog::getColor(Qt::yellow, this);
  if (color.isValid()) {
    ui->GLwidget->vertex_red = color.redF();
    ui->GLwidget->vertex_green = color.greenF();
    ui->GLwidget->vertex_blue = color.blueF();
    ui->GLwidget->vertex_alpha = color.alphaF();
    ui->GLwidget->update();
  }
}

void MainWindow::on_sizeVertexSpinBox_valueChanged(double arg1) {
  ui->GLwidget->vertex_size = arg1;
  ui->GLwidget->update();
}

void MainWindow::on_noneDisplayRadioButton_clicked() {
  ui->GLwidget->vertex_type = "none";
  ui->GLwidget->update();
}

void MainWindow::on_squareDisplayRadioButton_clicked() {
  ui->GLwidget->vertex_type = "square";
  ui->GLwidget->update();
}

void MainWindow::on_circleDisplayRadioButton_clicked() {
  ui->GLwidget->vertex_type = "circle";
  ui->GLwidget->update();
}

void MainWindow::on_parallelRadioButton_clicked() {
  ui->GLwidget->proection_type = "parallel";
  ui->GLwidget->update();
}

void MainWindow::on_centralRadioButton_clicked() {
  ui->GLwidget->proection_type = "central";
  ui->GLwidget->update();
}

void MainWindow::on_takeSnapshotButton_clicked() {
  QString file = QFileDialog::getSaveFileName(this, "Save as...", "screen.bmp",
                                              "BMP (*.bmp);; JPEF (*.jpeg)");
  if (file != "") {
    ui->GLwidget->grabFramebuffer().save(file, NULL, 100);
  }
}

void MainWindow::on_uploadFileButton_clicked() {
  QString file =
      QFileDialog::getOpenFileName(this, "Upload file ", "", "OBJ (*.obj)");
  if (file != "") {
    ui->filenameValue->setText(file);
    QFileInfo fileInfo(file);
    QString filename(fileInfo.fileName());
    ui->filenameValue->setText(filename);
    if (ui->GLwidget->object) {
      //            free(obj);
      //очищать масссивы, а потом структуру
    }
    ui->GLwidget->upload_file(file);
    ui->vertexCountValue->setText(
        QString::number(ui->GLwidget->object->count_of_vertexes));
    ui->edgeCountValue->setText(
        QString::number(ui->GLwidget->object->count_of_facets));
  }
}

void MainWindow::on_takeGifButton_clicked() {
  filepath = QFileDialog::getSaveFileName(this, "Save as...", "animation.gif",
                                          "GIF (*.gif)");

  if (filepath != "") {
    if (timer_ready) {
      ui->GLwidget->gif = new QGifImage();
      ui->GLwidget->timer = new QTimer();
    }
    wtimer();
    timer_ready = false;
  }
}

void MainWindow::take_gif() {
  ui->GLwidget->time++;
  QImage image = ui->GLwidget->grabFramebuffer().scaled(640, 480);

  ui->GLwidget->gif->setDefaultDelay(10);
  ui->GLwidget->gif->addFrame(image);

  if (ui->GLwidget->time == 50) {
    ui->GLwidget->time = 0;
    ui->GLwidget->timer->stop();
    ui->GLwidget->gif->save(filepath);
    image.QImage::bits();
    timer_ready = true;
    delete ui->GLwidget->timer;
    delete ui->GLwidget->gif;
  }
}

void MainWindow::wtimer() {
  ui->GLwidget->timer->start(100);
  connect(ui->GLwidget->timer, SIGNAL(timeout()), this, SLOT(take_gif()));
}

void MainWindow::on_xMoveSpinBox_valueChanged(double arg1) {
  if (ui->GLwidget->object) {
    move_object(ui->GLwidget->object, arg1 - ui->GLwidget->x_last_move, 0, 0);
    ui->GLwidget->x_last_move = arg1;
    ui->GLwidget->update();
  }
}

void MainWindow::on_yMoveSpinBox_valueChanged(double arg1) {
  if (ui->GLwidget->object) {
    move_object(ui->GLwidget->object, 0, arg1 - ui->GLwidget->y_last_move, 0);
    ui->GLwidget->y_last_move = arg1;
    ui->GLwidget->update();
  }
}

void MainWindow::on_zMoveSpinBox_valueChanged(double arg1) {
  if (ui->GLwidget->object) {
    move_object(ui->GLwidget->object, 0, 0, arg1 - ui->GLwidget->z_last_move);
    ui->GLwidget->z_last_move = arg1;
    ui->GLwidget->update();
  }
}

void MainWindow::on_scaleSpinBox_valueChanged(double arg1) {
  if (ui->GLwidget->object) {
    scale_object(ui->GLwidget->object, arg1 / ui->GLwidget->scale_last);
    ui->GLwidget->scale_last = arg1;
    ui->GLwidget->update();
  }
}

void MainWindow::on_xRotateSpinBox_valueChanged(double arg1) {
  if (ui->GLwidget->object) {
    rotate_object(ui->GLwidget->object, arg1 - ui->GLwidget->angle_x_last, 0,
                  0);
    ui->GLwidget->angle_x_last = arg1;
    ui->GLwidget->update();
  }
}

void MainWindow::on_yRotateSpinBox_valueChanged(double arg1) {
  if (ui->GLwidget->object) {
    rotate_object(ui->GLwidget->object, 0, arg1 - ui->GLwidget->angle_y_last,
                  0);
    ui->GLwidget->angle_y_last = arg1;
    ui->GLwidget->update();
  }
}

void MainWindow::on_zRotateSpinBox_valueChanged(double arg1) {
  if (ui->GLwidget->object) {
    rotate_object(ui->GLwidget->object, 0, 0,
                  arg1 - ui->GLwidget->angle_z_last);
    ui->GLwidget->angle_z_last = arg1;
    ui->GLwidget->update();
  }
}
