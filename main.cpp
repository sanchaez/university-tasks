#include <QApplication>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>
//#include <QDebug>
//#include <QThreadPool>
#include <ctime>
#include "graphicsbezieritem.h"

int gen_num_floored() {
    return qrand() % 399;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //qDebug() << QThreadPool::globalInstance()->maxThreadCount();
    QGraphicsScene scene;
    QGraphicsView view(&scene);
    scene.setSceneRect(0, 0, 400, 400);
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view.setRenderHints(QPainter::Antialiasing);
    view.show();
    qsrand(time(0));
    GraphicsBezierItem *gbi = new GraphicsBezierItem(QPointF(gen_num_floored(), gen_num_floored()),
                                                     QPointF(gen_num_floored(),gen_num_floored()), 0.005);
    for(int i = 0; i < 20; ++i) {
        gbi->addControl(QPointF(gen_num_floored(), gen_num_floored()));
    }
    scene.addItem(gbi);

    return app.exec();
}
