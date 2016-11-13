#ifndef GRAPHICSBEZIERITEM_H
#define GRAPHICSBEZIERITEM_H

#include <QGraphicsItem>
#include <QPen>
#include <QVector>
#include <QLinkedList>
#include "controlitem.h"

class GraphicsBezierItem : public QGraphicsItem
{
public:
    explicit GraphicsBezierItem(QGraphicsItem *parent = 0);
    GraphicsBezierItem(const QPointF &c1, const QPointF &c2, QGraphicsItem *parent = 0);
    GraphicsBezierItem(const QPointF &c1, const QPointF &c2, float _precision, QGraphicsItem *parent = 0);

    void addControl(const QPointF &c);
    void setControl(int num, const QPointF &c);
    void removeControl(int num);

    const QVector<QPointF> &getControls() const;
    const QVector<QPointF> &getCurve() const;

    float getPrecision() const;
    void setPrecision(float value);

    QPen getPen() const;
    void setPen(const QPen &value);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:

    QVector<QPointF> _control_points;
    qreal _precision = 0.05;
    QVector<QPointF> _curve_points;
    QRectF _curve_boundaries;
    QPen _pen;

    void update();
    void updateRect();
    QPointF singleCurvePoint(const QVector<QPointF> &points, const qreal &parameter_t);
};

#endif // GRAPHICSBEZIERITEM_H
