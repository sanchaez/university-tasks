#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "graphicsbezieritem.h"

class MainWindow : public QMainWindow, private Ui::MainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = 0);

 private slots:
  void on_pointNumberSpinBox_valueChanged(int arg1);

  void on_precisionSpinBox_valueChanged(double arg1);

  void on_scaleDoubleSpinBox_valueChanged(double arg1);

  void on_angleDoubleSpinBox_valueChanged(double arg1);

 private:
  QGraphicsScene* scene;
  GraphicsBezierItem gbi;
};

#endif  // MAINWINDOW_H
