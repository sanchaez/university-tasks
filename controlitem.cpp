#include "graphicsbezieritem.h"

#include <QCursor>
#include <QPainter>
#include <QPoint>

#define DEFAULT_SIZE 5

ControlPointItem::ControlPointItem(const ControlPointItem& to_copy)
    : ControlPointItem(to_copy.pos(),
                       to_copy.getSize(),
                       to_copy.getBezierCurve()) {
  setPen(to_copy.getPen());
}

ControlPointItem::ControlPointItem(GraphicsBezierItem* parent)
    : ControlPointItem(QPointF(0, 0), DEFAULT_SIZE, parent) {}

ControlPointItem::ControlPointItem(int size, GraphicsBezierItem* parent)
    : ControlPointItem(QPointF(0, 0), size, parent) {}

ControlPointItem::ControlPointItem(const QPointF& pos,
                                   GraphicsBezierItem* parent)
    : ControlPointItem(pos, DEFAULT_SIZE, parent) {}

ControlPointItem::ControlPointItem(const QPointF& pos,
                                   const int& size,
                                   GraphicsBezierItem* parent)
    : QGraphicsItem(dynamic_cast<QGraphicsItem*>(parent)),
      _size(size),
      _pen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap),
      _bezier_curve(parent) {
  setPos(pos);
  setAcceptHoverEvents(true);
  setAcceptedMouseButtons(Qt::MouseButton::LeftButton);
  setCursor(QCursor(Qt::PointingHandCursor));
  setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable |
           QGraphicsItem::ItemSendsGeometryChanges);
}

ControlPointItem& ControlPointItem::operator=(const ControlPointItem& to_copy) {
  setParentItem(to_copy.parentItem());
  setSize(to_copy.getSize());
  setPos(to_copy.pos());
  setPen(to_copy.getPen());
  setAcceptHoverEvents(true);
  setAcceptedMouseButtons(Qt::MouseButton::LeftButton);
  setCursor(QCursor(Qt::PointingHandCursor));
  return *this;
}

QRectF ControlPointItem::boundingRect() const {
  int realSize = _size + _pen.width();
  return QRectF(-realSize, -realSize, 2 * realSize, 2 * realSize);
}

void ControlPointItem::paint(QPainter* painter,
                             const QStyleOptionGraphicsItem* option,
                             QWidget* widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(_pen);
  painter->drawEllipse(QPoint(0, 0), _size, _size);
}

void ControlPointItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
  Q_UNUSED(event)
  _pen.setColor(Qt::red);
}

void ControlPointItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
  Q_UNUSED(event)
  _pen.setColor(Qt::gray);
}

void ControlPointItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
  setPos(event->scenePos());
}

void ControlPointItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
  Q_UNUSED(event)
  setCursor(Qt::SizeAllCursor);
}

void ControlPointItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
  Q_UNUSED(event)
  setCursor(Qt::PointingHandCursor);
}

QVariant ControlPointItem::itemChange(QGraphicsItem::GraphicsItemChange change,
                                      const QVariant& value) {
  if (change == ItemPositionHasChanged) {
    // update parent
    _bezier_curve->update();
  }
  return QGraphicsItem::itemChange(change, value);
}

int ControlPointItem::getSize() const {
  return _size;
}

void ControlPointItem::setSize(int value) {
  _size = value;
}

QPen ControlPointItem::getPen() const {
  return _pen;
}

void ControlPointItem::setPen(const QPen& pen) {
  _pen = pen;
}
