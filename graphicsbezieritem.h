#ifndef GRAPHICSBEZIERITEM_H
#define GRAPHICSBEZIERITEM_H

#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QLinkedList>
#include <QPen>
#include <QVector>

class ControlPointItem;

class GraphicsBezierItem : public QGraphicsItem {
 public:
  explicit GraphicsBezierItem(QGraphicsItem* parent = 0);
  GraphicsBezierItem(const QPointF& c1,
                     const QPointF& c2,
                     QGraphicsItem* parent = 0);
  GraphicsBezierItem(const QPointF& c1,
                     const QPointF& c2,
                     qreal _precision,
                     QGraphicsItem* parent = 0);

  void addControl(const QPointF& c);
  void addControls(const QVector<QPointF>& c);
  void setControl(int num, const QPointF& c);
  void setControls(QVector<QPointF>& c);
  void removeControl(int num);
  void popControl();
  void popControls(int num);

  void turn(int angle);
  void scale(int factor);
  const QVector<QPointF> getControlPoints() const;
  const QVector<QPointF>& getCurve() const;

  qreal getPrecision() const;
  void setPrecision(qreal value);

  QPen getPen() const;
  void setPen(const QPen& value);

  void update();
  QRectF boundingRect() const;
  void paint(QPainter* painter,
             const QStyleOptionGraphicsItem* option,
             QWidget* widget);

  QPointF singleCurvePoint(const qreal& parameter_t);

  qreal getAngle() const;
  void setAngle(qreal angle);

  qreal getScale() const;
  void setScale(qreal scale);

 private:
  QVector<ControlPointItem> _control_points;
  qreal _precision;
  QVector<QPointF> _curve_points;
  QRectF _curve_boundaries;
  QPen _curve_pen, _lines_pen;
  qreal _angle, _angle_delta;
  qreal _scale, _scale_delta;
  QPointF singleCurvePointAux(const QVector<QPointF>& points,
                              const qreal& parameter_t);
  void updateRect();
  void updateCurveScale();
  void updateCurveAngle();
};

class ControlPointItem : public QGraphicsItem {
 public:
  explicit ControlPointItem(GraphicsBezierItem* parent = 0);
  explicit ControlPointItem(int size, GraphicsBezierItem* parent = 0);
  explicit ControlPointItem(const QPointF& pos, GraphicsBezierItem* parent = 0);
  ControlPointItem(const QPointF& pos,
                   const int& size,
                   GraphicsBezierItem* parent = 0);

  ControlPointItem(const ControlPointItem& to_copy);
  ControlPointItem& operator=(const ControlPointItem& to_copy);

  QRectF boundingRect() const;
  void paint(QPainter* painter,
             const QStyleOptionGraphicsItem* option,
             QWidget* widget);
  void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
  void mousePressEvent(QGraphicsSceneMouseEvent* event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

  QVariant itemChange(GraphicsItemChange change, const QVariant& value);

  int getSize() const;
  void setSize(int value);

  QPen getPen() const;
  void setPen(const QPen& pen);

  GraphicsBezierItem* getBezierCurve() const;

 private:
  int _size = 5;
  QPen _pen;
  GraphicsBezierItem* _bezier_curve;
};

#endif  // GRAPHICSBEZIERITEM_H
