#include "mainwindow.h"
#include <QColorDialog>
#include <QHBoxLayout>
#include <QInputDialog>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setupUi(this);
  drawingArea = new ScribbleArea;
  scrollArea = new ScribbleScrollArea;
  scrollArea->setBackgroundRole(QPalette::Dark);
  scrollArea->setScribbleArea(drawingArea);
  scrollArea->setAlignment(Qt::AlignCenter);
  // scrollArea->setWidgetResizable(true);
  setCentralWidget(scrollArea);
  toolsGroup = new QActionGroup(this);
  toolsGroup->addAction(actionPen);
  toolsGroup->addAction(actionRect);
  toolsGroup->addAction(actionPolyline);
  toolsGroup->addAction(actionFill);
  QObject::connect(toolsGroup, &QActionGroup::triggered, this,
                   &MainWindow::setNewTool);
  actionPen->setData(static_cast<int>(ScribbleArea::ToolType::Pen));
  actionRect->setData(static_cast<int>(ScribbleArea::ToolType::Rectangle));
  actionFill->setData(static_cast<int>(ScribbleArea::ToolType::Bucket));
  actionPolyline->setData(static_cast<int>(ScribbleArea::ToolType::Polygon));
  actionPen->setChecked(true);
  QObject::connect(actionExit, &QAction::triggered, this, &MainWindow::close);
}

void MainWindow::setNewTool(QAction* a) {
  drawingArea->setTool(static_cast<ScribbleArea::ToolType>(a->data().toInt()));
}

void MainWindow::on_actionForegroundColor_triggered() {
  drawingArea->setForegroundToolColor(QColorDialog::getColor(
      drawingArea->getForegroundToolColor(), this, tr("Foreground color")));
}

void MainWindow::on_actionBackgroundColor_triggered() {
  drawingArea->setBackgroundToolColor(QColorDialog::getColor(
      drawingArea->getBackgroundToolColor(), this, tr("Background color")));
}

void MainWindow::on_actionStrokeSize_triggered() {
  drawingArea->setPenWidth(
      QInputDialog::getInt(this, tr("Set pen stroke"), tr("&Value:"),
                           drawingArea->getPenWidth(), 1, 999, 1));
}
