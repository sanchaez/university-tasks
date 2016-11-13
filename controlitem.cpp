#include "controlitem.h"

#include <QPainter>
#include <QPoint>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>

ControlItem::ControlItem() :
    ControlItem(0) {
}

ControlItem::ControlItem(QGraphicsItem *parent) :
    QGraphicsItem(parent) {
}

ControlItem::ControlItem(QGraphicsItem *parent, int size) :
    QGraphicsItem(parent),
    _size(size),
    _pen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap)
{
    setPos(QPointF(0,0));
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::MouseButton::LeftButton);
    setCursor(QCursor(Qt::PointingHandCursor));
}

ControlItem::ControlItem(QGraphicsItem *parent, QPointF &pos) :
    QGraphicsItem(parent)
{
    setPos(pos);
}

QRectF ControlItem::boundingRect() const {
    int realSize = _size + _pen.width();
    return QRectF(-realSize, -realSize, 2*realSize, 2*realSize);
}

void ControlItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(_pen);
    painter->drawEllipse(QPoint(0,0), _size, _size);
}

void ControlItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    Q_UNUSED(event)
    _pen.setColor(Qt::red);
    update();
}

void ControlItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    Q_UNUSED(event)
    _pen.setColor(Qt::gray);
    update();
}

void ControlItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    setPos(event->scenePos());
    update();
}

void ControlItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event)
    setCursor(Qt::SizeAllCursor);
}

void ControlItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    setCursor(Qt::PointingHandCursor);
}

int ControlItem::getSize() const {
    return _size;
}

void ControlItem::setSize(int value) {
    _size = value;
}

