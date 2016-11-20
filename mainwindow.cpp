#include "mainwindow.h"
#include <ctime>

int gen_num_floored() {
  return qrand() % 395;
}

#define INITIAL_NUM_OF_POINTS 30
#define INITIAL_PRECISION_VALUE 0.005
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

  QVector<QPointF>* control_points = new QVector<QPointF>();
  control_points->reserve(INITIAL_NUM_OF_POINTS);
  for (int i = 0; i < INITIAL_NUM_OF_POINTS; ++i) {
    control_points->append(GEN_POINTF);
  }
  gbi.addControls(*control_points);
  delete control_points;

  scene->addItem(&gbi);
  graphicsView->update();
}
