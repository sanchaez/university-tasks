#include "editrect.h"
#include <QCursor>

EditRectTop::EditRectTop(int size, QGraphicsItem* parent)
    : QAbstractGraphicsShapeItem(parent), _size(size) {}

void EditRectTop::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
  qreal xdelta = event->scenePos().x() - this->scenePos().x(),
        ydelta = event->scenePos().y() - this->scenePos().y();
  setPos(event->scenePos());
  for (int index = 0; index < _boundTops.size(); ++index) {
    _boundTops[index]->setPos(_boundTops[index]->x() + xdelta,
                              _boundTops[index]->y() + ydelta);
  }
}

QRectF EditRectTop::boundingRect() const {
  return QRectF(this->scenePos(), _size);
}

void EditRectTop::paint(QPainter* painter,
                        const QStyleOptionGraphicsItem* option,
                        QWidget* widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)
  painter->drawEllipse(boundingRect());
}

QVector<EditRectTop*> EditRectTop::boundTops() const {
  return _boundTops;
}

void EditRectTop::bindTops(const QVector<EditRectTop*>& boundTops) {
  _boundTops = boundTops;
}

void EditRectTop::bindTop(EditRectTop* boundTop) {
  _boundTops.append(boundTop);
}

void EditRectTop::unbindTop(EditRectTop* boundTop) {
  _boundTops.remove(_boundTops.indexOf(boundTop));
}

qreal EditRectTop::size() const {
  return _size;
}

void EditRectTop::setSize(qreal size) {
  _size = size;
}

EditRect::EditRect() : EditRect(QRectF(0, 0, 10, 10)) {}

EditRect::EditRect(QRectF rect, int topSize, QGraphicsItem* parent)
    : QGraphicsItem(parent) {
  _topLeft = new EditRectTop(topSize, dynamic_cast<QGraphicsItem*>(this));
  _topRight = new EditRectTop(topSize, dynamic_cast<QGraphicsItem*>(this));
  _botLeft = new EditRectTop(topSize, dynamic_cast<QGraphicsItem*>(this));
  _botRight = new EditRectTop(topSize, dynamic_cast<QGraphicsItem*>(this));
  _topLeft->setPos(rect.topLeft());
  _topRight->setPos(rect.topRight());
  _botLeft->setPos(rect.bottomLeft());
  _botRight->setPos(rect.bottomRight());
  _topLeft->setCursor(Qt::SizeFDiagCursor);
  _topRight->setCursor(Qt::SizeBDiagCursor);
  _botRight->setCursor(Qt::SizeFDiagCursor);
  _botLeft->setCursor(Qt::SizeBDiagCursor);
  // top edge
  _topLeft->bindTop(_topRight);
  _topRight->bindTop(_topLeft);
  // left edge
  _topLeft->bindTop(_botLeft);
  _botLeft->bindTop(_topLeft);
  // right edge
  _topRight->bindTop(_botRight);
  _botRight->bindTop(_topRight);
  // bottom edge
  _botLeft->bindTop(_botRight);
  _botRight->bindTop(_botLeft);
}

EditRect::~EditRect() {
  delete _topLeft;
  delete _topRight;
  delete _botLeft;
  delete _botRight;
}

void EditRect::paintShape(QPainter* painter) {}

QRectF EditRect::boundingRect() const {
  QRectF drawRect(genDrawRect());
  return QRectF(drawRect.left() + _topSize, drawRect.top() + _topSize,
                drawRect.width() + _topSize, drawRect.height() + _topSize);
}

void EditRect::paint(QPainter* painter,
                     const QStyleOptionGraphicsItem* option,
                     QWidget* widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)
  painter->drawRect(genDrawRect());
  paintShape(painter);
}

QRectF EditRect::genDrawRect() const {
  return QRectF(_topLeft->scenePos(), _botRight->scenePos());
}
