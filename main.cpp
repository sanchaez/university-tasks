
#include "mainwindow.h"

#ifdef QT_DEBUG
#include <QDebug>
#include <QThreadPool>
#endif

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  MainWindow w;
  w.show();
#ifdef QT_DEBUG
  qDebug() << QThreadPool::globalInstance()->maxThreadCount();
#endif
  return app.exec();
}
