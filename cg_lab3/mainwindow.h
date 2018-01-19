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

  void on_actionAbout_triggered();

  void on_actionSave_triggered();

  void on_actionSave_as_triggered();

  void on_actionOpen_triggered();

  void on_actionNew_triggered();

 private:
  ScribbleArea* drawingArea;
  ScribbleScrollArea* scrollArea;
  QActionGroup* toolsGroup;
  QString lastSavePath, lastFileFormat;
};

#endif  // MAINWINDOW_H
