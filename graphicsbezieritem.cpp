#include "graphicsbezieritem.h"
#include <QPainter>
#include <QTime>
#include <QtConcurrent/QtConcurrent>
#include <cassert>
#include <functional>

#define DEFAULT_PRECISION 0.001

#ifdef QT_DEBUG
#include <QDebug>
#endif

GraphicsBezierItem::GraphicsBezierItem(QGraphicsItem* parent)
    : GraphicsBezierItem(QPointF(0, 0),
                         QPointF(0, 0),
                         DEFAULT_PRECISION,
                         parent) {}

GraphicsBezierItem::GraphicsBezierItem(const QPointF& c1,
                                       const QPointF& c2,
                                       QGraphicsItem* parent)
    : GraphicsBezierItem(c1, c2, DEFAULT_PRECISION, parent) {}

GraphicsBezierItem::GraphicsBezierItem(const QPointF& c1,
                                       const QPointF& c2,
                                       qreal precision,
                                       QGraphicsItem* parent)
    : QGraphicsItem(parent),
      _precision(precision),
      _curve_pen(Qt::SolidLine),
      _lines_pen(Qt::DashLine),
      _angle(0),
      _scale(1) {
  _curve_pen.setJoinStyle(Qt::RoundJoin);
  _curve_pen.setWidth(3);
  _lines_pen.setColor(Qt::gray);
  _lines_pen.setWidth(1);
  _control_points.reserve(2);
  _control_points.append(ControlPointItem(c1, this));
  _control_points.last().setColor(Qt::magenta);
  _control_points.append(ControlPointItem(c2, this));
  _control_points.last().setColor(Qt::magenta);
  update();
}

void GraphicsBezierItem::addControl(const QPointF& c) {
  _control_points.last().setColor(Qt::gray);
  _control_points.append(ControlPointItem(c, this));
  _control_points.last().setColor(Qt::magenta);
  update();
}

void GraphicsBezierItem::addControls(const QVector<QPointF>& c) {
  _control_points.last().setColor(Qt::gray);
  foreach (QPointF point, c) {
    _control_points.append(ControlPointItem(point, this));
  }
  _control_points.last().setColor(Qt::magenta);
  update();
}

void GraphicsBezierItem::removeControl(int num) {
  int current_size = _control_points.size();
  if (current_size > 2) {  // Bezier curve must contain at least 2 control
                           // points
    assert(num <= current_size - 2);
    _control_points.erase(_control_points.begin() + num);
    update();
  }
}

void GraphicsBezierItem::popControl() {
  _control_points.pop_back();
  update();
}

void GraphicsBezierItem::popControls(int num) {
  for (int i = 0; i < num; ++i) {
    _control_points.pop_back();
  }
  update();
}

void GraphicsBezierItem::setControl(int num, const QPointF& c) {
  assert(num < _control_points.size());
  _control_points[num].setPos(c);
  update();
}

const QVector<QPointF> GraphicsBezierItem::getControlPoints() const {
  QVector<QPointF> converted_points(0);
  converted_points.reserve(_control_points.size());
  foreach (const ControlPointItem& item, _control_points) {
    converted_points.append(item.pos());
  }
  return converted_points;
}

const QVector<QPointF>& GraphicsBezierItem::getCurve() const {
  return _curve_points;
}

qreal GraphicsBezierItem::getPrecision() const {
  return _precision;
}

void GraphicsBezierItem::setPrecision(qreal value) {
  _precision = value;
  update();
}

QPen GraphicsBezierItem::getPen() const {
  return _curve_pen;
}

void GraphicsBezierItem::setPen(const QPen& value) {
  _curve_pen = value;
  update();
}

void GraphicsBezierItem::update() {
  QVector<qreal> precision_points;
  updateRect();

  QVector<QPointF> control_qpoints(getControlPoints());
  _curve_points.clear();
  // alloc space
  {
    int new_vector_size = 1 / _precision;
    _curve_points.reserve(new_vector_size);
    precision_points.reserve(new_vector_size);
  }
  // calculate points

  for (qreal t = _precision; t <= 1 - _precision; t += _precision) {
    precision_points.append(t);
  }
#ifdef QT_DEBUG
  int start_timer = QTime::currentTime().msecsSinceStartOfDay();
#endif
  // kids, do not try this at home
  _curve_points.append(control_qpoints.first());
  _curve_points.append(QtConcurrent::blockingMapped(
      precision_points,
      std::bind(&GraphicsBezierItem::singleCurvePointAux, this, control_qpoints,
                std::placeholders::_1)));
  _curve_points.append(control_qpoints.last());

#ifdef QT_DEBUG
  qDebug() << "Elapsed: "
           << QTime::currentTime().msecsSinceStartOfDay() - start_timer << "\n";
#endif
}

QRectF GraphicsBezierItem::boundingRect() const {
  return _curve_boundaries;
}

void GraphicsBezierItem::paint(QPainter* painter,
                               const QStyleOptionGraphicsItem* option,
                               QWidget* widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(_curve_pen);
  painter->drawPolyline(_curve_points.data(), _curve_points.size());
  painter->setPen(_lines_pen);
  painter->drawPolyline(getControlPoints());
  painter->drawRect(_curve_boundaries);
}

void GraphicsBezierItem::updateRect() {
  prepareGeometryChange();
  _curve_boundaries.setLeft(_control_points.first().x());
  _curve_boundaries.setRight(_control_points.first().x());
  _curve_boundaries.setTop(_control_points.first().y());
  _curve_boundaries.setBottom(_control_points.first().y());

  for (ControlPointItem point : _control_points) {
    if (point.x() < _curve_boundaries.left())
      _curve_boundaries.setLeft(point.x());
    if (point.x() > _curve_boundaries.right())
      _curve_boundaries.setRight(point.x());
    if (point.y() < _curve_boundaries.top())
      _curve_boundaries.setTop(point.y());
    if (point.y() > _curve_boundaries.bottom())
      _curve_boundaries.setBottom(point.y());
  }
}

void GraphicsBezierItem::updateCurveAngle() {
  QMutableVectorIterator<ControlPointItem> control_points_iterator(
      _control_points);
  qreal angle_radians = (M_PI * _angle_delta) / 180;
  QPointF rect_center = _curve_boundaries.center();
  while (control_points_iterator.hasNext()) {
    QPointF point = control_points_iterator.next().pos();
    control_points_iterator.setValue(ControlPointItem(QPointF(
        (rect_center.x() + (point.x() - rect_center.x()) * qCos(angle_radians) -
         (point.y() - rect_center.y()) * qSin(angle_radians)),
        (rect_center.y() + (point.y() - rect_center.y()) * qCos(angle_radians) +
         (point.x() - rect_center.x()) * qSin(angle_radians)))));
  }
}

void GraphicsBezierItem::updateCurveScale() {
  QMutableVectorIterator<ControlPointItem> control_points_iterator(
      _control_points);
  while (control_points_iterator.hasNext()) {
    QPointF point = control_points_iterator.next().pos();
    if ((_scale_delta < 0)) {
      if (_scale > 1) {
        control_points_iterator.setValue(ControlPointItem(point * _scale));
      } else
        control_points_iterator.setValue(ControlPointItem(point / _scale));
    } else if (_scale_delta > 0) {
      if (_scale > 1) {
        control_points_iterator.setValue(ControlPointItem(point / _scale));
      } else
        control_points_iterator.setValue(ControlPointItem(point / _scale));
    }
  }
}

QPointF GraphicsBezierItem::singleCurvePointAux(QVector<QPointF> buffer,
                                                const qreal& parameter_t) {
  for (int point_list_size = buffer.size(); point_list_size > 1;
       --point_list_size) {
    for (int i = 0; i < point_list_size - 1; ++i) {
      buffer[i] = (buffer[i + 1] - buffer[i]) * parameter_t + buffer[i];
    }
  }
  return buffer.first();
}

QPointF GraphicsBezierItem::singleCurvePoint(const qreal& parameter_t) {
  return singleCurvePointAux(getControlPoints(), parameter_t);
}

qreal GraphicsBezierItem::getAngle() const {
  return _angle;
}

void GraphicsBezierItem::setAngle(qreal angle) {
  _angle_delta = _angle - angle;
  _angle = angle;
  updateCurveAngle();
  update();
}

qreal GraphicsBezierItem::getScale() const {
  return _scale;
}

void GraphicsBezierItem::setScale(qreal scale) {
  _scale_delta = _scale - scale;
  _scale = scale;
  updateCurveScale();
  update();
}
