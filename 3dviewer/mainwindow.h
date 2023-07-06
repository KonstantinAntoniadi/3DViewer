#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QMainWindow>
#include <QOpenGLWidget>

extern "C" {
#include "backend/3d_v.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void wtimer();
  QString filepath;

 private slots:
  void on_dottedRadioButton_clicked();
  void on_solidRadioButton_clicked();

  void on_thicknessEdgeSpinBox_valueChanged(double arg1);
  void on_chooseColorBackgroundLabel_clicked();
  void on_chooseColorEdgeButton_clicked();
  void on_sizeVertexSpinBox_valueChanged(double arg1);
  void on_squareDisplayRadioButton_clicked();
  void on_circleDisplayRadioButton_clicked();
  void on_noneDisplayRadioButton_clicked();
  void on_chooseColorVertexButton_clicked();
  void on_parallelRadioButton_clicked();
  void on_centralRadioButton_clicked();
  void on_takeSnapshotButton_clicked();
  void on_uploadFileButton_clicked();
  void on_takeGifButton_clicked();
  void move();

  void on_xMoveSpinBox_valueChanged(double arg1);

  void on_yMoveSpinBox_valueChanged(double arg1);

  void on_zMoveSpinBox_valueChanged(double arg1);

  void on_scaleSpinBox_valueChanged(double arg1);

  void on_xRotateSpinBox_valueChanged(double arg1);

  void on_yRotateSpinBox_valueChanged(double arg1);

  void on_zRotateSpinBox_valueChanged(double arg1);

 private:
  void take_and_save_gif();
  void sincSettings();
  Ui::MainWindow *ui;
  QOpenGLWidget *m_glWidget;
  bool timer_ready = true;

 public slots:
  void take_gif();
};
#endif  // MAINWINDOW_H
