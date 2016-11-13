#include "graphicsbezieritem.h"

#include <cassert>
#include <QPainter>
#include <QDebug>

GraphicsBezierItem::GraphicsBezierItem(QGraphicsItem *parent) :
    GraphicsBezierItem(QPointF(0,0), QPointF(0,0), parent) {
}

GraphicsBezierItem::GraphicsBezierItem(const QPointF &c1, const QPointF &c2, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    _control_points({c1,c2}){
    update();
}

GraphicsBezierItem::GraphicsBezierItem(const QPointF &c1, const QPointF &c2, float precision, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    _control_points({c1,c2}),
    _precision(precision) {
    update();
}

void GraphicsBezierItem::addControl(const QPointF &c) {
    _control_points.append(c);
    update();
}

void GraphicsBezierItem::removeControl(int num) {
    int current_size = _control_points.size();
    if (current_size > 2) { //Bezier curve must contain at least 2 control points
        assert(num <= current_size - 2);
        _control_points.erase(_control_points.begin() + num);
        update();
    }
}

void GraphicsBezierItem::setControl(int num, const QPointF &c) {
    assert(num < _control_points.size());
    _control_points[num] = c;
    update();
}

const QVector<QPointF>& GraphicsBezierItem::getControls() const {
    return _control_points;
}

const QVector<QPointF> &GraphicsBezierItem::getCurve() const {
    return _curve_points;
}

float GraphicsBezierItem::getPrecision() const {
    return _precision;
}

void GraphicsBezierItem::setPrecision(float value) {
    _precision = value;
}

QPen GraphicsBezierItem::getPen() const {
    return _pen;
}

void GraphicsBezierItem::setPen(const QPen &value) {
    _pen = value;
    updateRect();
}

QRectF GraphicsBezierItem::boundingRect() const {
    return _curve_boundaries;
}

void GraphicsBezierItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(_pen);
    painter->drawPolyline(_curve_points.data(), _curve_points.size());
}

void GraphicsBezierItem::update() {
    _curve_points.clear();

    for (qreal t = 0 ; t <= 1 ; t += _precision)
        _curve_points.push_back(singleCurvePoint(_control_points, t));

    updateRect();
}

void GraphicsBezierItem::updateRect() {
    _curve_boundaries.setRect(0,0,0,0);

    for (QPointF &p : _curve_points) {
        if (p.x() <= _curve_boundaries.left())
            _curve_boundaries.setLeft(p.x());
        else if (p.x() >= _curve_boundaries.right())
            _curve_boundaries.setRight(p.x());

        if (p.y() <= _curve_boundaries.top())
            _curve_boundaries.setTop(p.y());
        else if (p.y() >= _curve_boundaries.bottom())
            _curve_boundaries.setBottom(p.y());
    }
}

QPointF GraphicsBezierItem::singleCurvePoint(QVector<QPointF> points, qreal parameter_t) {
    QVector<QPointF> recalculated_points;
    int size = points.size();

    if (size == 1)
        return points[0];
    else {
        for (int i = 0 ; i < size - 1 ; i++)
            recalculated_points.push_back((points[i+1] - points[i]) * parameter_t + points[i]);

        return singleCurvePoint(recalculated_points, parameter_t);
    }
}




