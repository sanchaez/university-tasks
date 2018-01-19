#include "mainwindow.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>

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
  toolsGroup->addAction(actionEllipse);
  QObject::connect(toolsGroup, &QActionGroup::triggered, this,
                   &MainWindow::setNewTool);
  actionPen->setData(static_cast<int>(ScribbleArea::ToolType::Pen));
  actionRect->setData(static_cast<int>(ScribbleArea::ToolType::Rectangle));
  actionFill->setData(static_cast<int>(ScribbleArea::ToolType::Bucket));
  actionPolyline->setData(static_cast<int>(ScribbleArea::ToolType::Polygon));
  actionEllipse->setData(static_cast<int>(ScribbleArea::ToolType::Ellipse));
  actionPen->setChecked(true);
  actionUndo->setShortcut(QKeySequence::Undo);
  actionRedo->setShortcut(QKeySequence::Redo);
  actionExit->setShortcut(QKeySequence::Quit);
  actionSave->setShortcut(QKeySequence::Save);
  actionSave_as->setShortcut(QKeySequence::SaveAs);
  actionOpen->setShortcut(QKeySequence::Open);
  actionNew->setShortcut(QKeySequence::New);

  QObject::connect(actionExit, &QAction::triggered, this, &MainWindow::close);
  QObject::connect(actionClearAll, &QAction::triggered, drawingArea,
                   &ScribbleArea::clearImage);
  QObject::connect(actionUndo, &QAction::triggered, drawingArea,
                   &ScribbleArea::undo);
  QObject::connect(actionRedo, &QAction::triggered, drawingArea,
                   &ScribbleArea::redo);
  QObject::connect(actionFilled, &QAction::triggered, drawingArea,
                   &ScribbleArea::setFilled);
  setWindowTitle(tr("New"));
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

void MainWindow::on_actionAbout_triggered() {
  QMessageBox::information(this, tr("About"),
                           tr("SimplePaint 0.1b by Alexander Shaposhnikov"));
}

void MainWindow::on_actionSave_triggered() {
  if (lastSavePath.isEmpty()) {
    on_actionSave_as_triggered();
    return;
  } else if (!drawingArea->saveImage(lastSavePath, Q_NULLPTR)) {
    QMessageBox::critical(this, tr("Error!"), tr("File failed to save."));
  }
}

void MainWindow::on_actionSave_as_triggered() {
  QString buf_name = QFileDialog::getSaveFileName(
      this, "Save file", "~/", tr("Images (*.png *.bmp *.jpg)"));
  if (!drawingArea->saveImage(buf_name, Q_NULLPTR)) {
    QMessageBox::critical(this, tr("Error!"), tr("File failed to save."));
  } else {
    setWindowTitle(buf_name);
    lastSavePath = buf_name;
  }
}

void MainWindow::on_actionOpen_triggered() {
  if (!drawingArea->openImage(QFileDialog::getOpenFileName(
          this, "Open file", "~/", tr("Images (*.png *.bmp *.jpg)")))) {
    QMessageBox::critical(this, tr("Error!"), tr("File failed to save."));
  }
}

void MainWindow::on_actionNew_triggered() {
  setWindowTitle(tr("New"));
  drawingArea->newImage();
}
