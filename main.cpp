
#include "mainwindow.h"

#ifdef _DEBUG
#include <QDebug>
#include <QThreadPool>
#endif

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  MainWindow w;
  w.show();
#ifdef _DEBUG
  qDebug() << QThreadPool::globalInstance()->maxThreadCount();
#endif
  return app.exec();
}
