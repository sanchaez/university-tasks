#include <QApplication>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>
//#include <QDebug>
//#include <QThreadPool>
#include <ctime>
#include "graphicsbezieritem.h"

#define NUM_OF_POINTS 30
int gen_num_floored() {
    return qrand() % 400;
}
#define GEN_POINTF QPointF(gen_num_floored(), gen_num_floored())
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //qDebug() << QThreadPool::globalInstance()->maxThreadCount();

    QGraphicsScene scene;
    QGraphicsView view(&scene);
    scene.setSceneRect(0, 0, 400, 400);
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view.setRenderHints(QPainter::Antialiasing);

    qsrand(time(0));

    GraphicsBezierItem *gbi = new GraphicsBezierItem(GEN_POINTF,GEN_POINTF, 0.004);\
    QVector<QPointF> *control_points = new QVector<QPointF>();
    control_points->reserve(NUM_OF_POINTS);
    for(int i = 0; i < NUM_OF_POINTS; ++i) {
        control_points->append(GEN_POINTF);
    }
    gbi->addControls(*control_points);
    delete control_points;
    scene.addItem(gbi);

    view.show();

    return app.exec();
}
