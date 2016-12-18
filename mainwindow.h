#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "scribblearea.h"
#include "scribblescrollarea.h"
#include <QActionGroup>
#include <QScrollArea>

class MainWindow : public QMainWindow, private Ui::MainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = 0);
 public slots:
  void setNewTool(QAction*);

 private:
  ScribbleArea* drawingArea;
  ScribbleScrollArea* scrollArea;
  QActionGroup* toolsGroup;
};

#endif  // MAINWINDOW_H
