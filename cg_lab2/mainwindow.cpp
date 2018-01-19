#include "mainwindow.h"
#include <ctime>

int gen_num_floored() {
  return qrand() % 395;
}

#define INITIAL_NUM_OF_POINTS 20
#define INITIAL_PRECISION_VALUE 0.001
#define GEN_POINTF QPointF(gen_num_floored(), gen_num_floored())

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      gbi(GEN_POINTF, GEN_POINTF, INITIAL_PRECISION_VALUE) {
  setupUi(this);
  precisionSpinBox->setValue(INITIAL_PRECISION_VALUE);
  pointNumberSpinBox->setValue(INITIAL_NUM_OF_POINTS);

  scene = new QGraphicsScene();
  graphicsView->setScene(scene);
  scene->setSceneRect(0, 0, 400, 400);
  graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  graphicsView->setRenderHints(QPainter::Antialiasing);

  qsrand(time(0));

  scene->addItem(&gbi);
  graphicsView->update();
}

void MainWindow::on_pointNumberSpinBox_valueChanged(int arg1) {
  int size_diff = arg1 - gbi.getControlPoints().size();
  if (size_diff > 0) {
    QVector<QPointF> new_points;
    new_points.reserve(qAbs(size_diff));
    for (int i = 0; i < qAbs(size_diff); ++i) {
      new_points.append(GEN_POINTF);
    }
    gbi.addControls(new_points);
  } else {
    gbi.popControls(qAbs(size_diff));
  }
}

void MainWindow::on_precisionSpinBox_valueChanged(double arg1) {
  gbi.setPrecision(arg1);
}

void MainWindow::on_angleDoubleSpinBox_valueChanged(double arg1) {
  gbi.setAngle(arg1);
}

void MainWindow::on_horizontalSlider_valueChanged(int value) {
  gbi.setScale(value / 100.);
}
