#ifndef STANDARTMOVABLEITEM_H
#define STANDARTMOVABLEITEM_H
#include <QGraphicsItem>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QPainter>
#include <QPen>

class EditRectTop : public QAbstractGraphicsShapeItem {
 public:
  explicit EditRectTop(int size, QGraphicsItem* parent);
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
  QRectF boundingRect() const;
  void paint(QPainter* painter,
             const QStyleOptionGraphicsItem* option,
             QWidget* widget);
  QVector<EditRectTop*> boundTops() const;
  void bindTops(const QVector<EditRectTop*>& boundTops);
  void bindTop(EditRectTop* boundTop);
  void unbindTop(int);
  void unbindTop(EditRectTop* boundTop);

  qreal size() const;
  void setSize(qreal size);

 private:
  QVector<EditRectTop*> _boundTops;
  qreal _size;
};

class EditRect : public QGraphicsItem {
 public:
  explicit EditRect();
  explicit EditRect(QRectF rect,
                    int topSize = 3,
                    QGraphicsItem* parent = Q_NULLPTR);
  virtual ~EditRect();
  virtual void paintShape(QPainter* painter);
  QRectF boundingRect() const;
  void paint(QPainter* painter,
             const QStyleOptionGraphicsItem* option,
             QWidget* widget);

 private:
  qreal _topSize;
  EditRectTop *_topLeft, *_topRight, *_botLeft, *_botRight;
  QRectF genDrawRect() const;
};

#endif  // STANDARTMOVABLEITEM_H
