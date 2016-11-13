#include <QApplication>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "graphicsbezieritem.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QGraphicsScene scene;
    QGraphicsView view(&scene);
    scene.setSceneRect(0, 0, 400, 400);
    view.setRenderHints(QPainter::Antialiasing);
    view.show();

    GraphicsBezierItem *gbi = new GraphicsBezierItem(QPointF(50,50), QPointF(350,50));
    gbi->addControl(QPointF(170,70));
    gbi->addControl(QPointF(300,300));
    scene.addItem(gbi);

    return app.exec();
}
