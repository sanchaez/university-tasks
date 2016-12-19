#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QScrollArea>
#include "scribblearea.h"
#include "scribblescrollarea.h"
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = 0);
 public slots:
  void setNewTool(QAction*);

 private slots:
  void on_actionForegroundColor_triggered();

  void on_actionBackgroundColor_triggered();

  void on_actionStrokeSize_triggered();

 private:
  ScribbleArea* drawingArea;
  ScribbleScrollArea* scrollArea;
  QActionGroup* toolsGroup;
};

#endif  // MAINWINDOW_H
