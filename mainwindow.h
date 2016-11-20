#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "graphicsbezieritem.h"

class MainWindow : public QMainWindow, private Ui::MainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = 0);

 private:
  QGraphicsScene* scene;
  GraphicsBezierItem gbi;
};

#endif  // MAINWINDOW_H
