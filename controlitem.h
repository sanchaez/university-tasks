#ifndef CONTROLITEM_H
#define CONTROLITEM_H

#include <QGraphicsEllipseItem>
#include <QPen>

class ControlItem : public QGraphicsItem
{
public:
    ControlItem();
    ControlItem(QGraphicsItem *parent);
    ControlItem(QGraphicsItem *parent, int size);
    ControlItem(QGraphicsItem *parent, QPointF &pos);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    int getSize() const;
    void setSize(int value);

private:
    int _size = 5;
    QPen _pen;
};

#endif // CONTROLITEM_H
