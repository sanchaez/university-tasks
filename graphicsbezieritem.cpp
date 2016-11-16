#include "graphicsbezieritem.h"

#include <cassert>
#include <functional>
#include <QPainter>
#include <QDebug>
#include <QTime>
#include <QtConcurrent/QtConcurrent>
#define DEFAULT_PRECISION 0.05

GraphicsBezierItem::GraphicsBezierItem(QGraphicsItem *parent) :
    GraphicsBezierItem(QPointF(0,0), QPointF(0,0), DEFAULT_PRECISION, parent) {}

GraphicsBezierItem::GraphicsBezierItem(const QPointF &c1, const QPointF &c2, QGraphicsItem *parent) :
    GraphicsBezierItem(c1, c2, DEFAULT_PRECISION, parent) {}

GraphicsBezierItem::GraphicsBezierItem(const QPointF &c1, const QPointF &c2, qreal precision, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    _precision(precision),
    _curve_pen(Qt::SolidLine),
    _lines_pen(Qt::DashLine)
{
    _curve_pen.setJoinStyle(Qt::RoundJoin);
    _curve_pen.setWidth(3);

    _lines_pen.setColor(Qt::gray);
    _lines_pen.setWidth(1);
    _control_points.reserve(2);
    _control_points.append(ControlPointItem(c1, this));
    _control_points.append(ControlPointItem(c2, this));
    update();
}

void GraphicsBezierItem::addControl(const QPointF &c) {
    _control_points.append(ControlPointItem(c, this));
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
    _control_points[num].setPos(c);
    update();
}

const QVector<QPointF> GraphicsBezierItem::getControlPoints() const {
    QVector<QPointF> converted_points(0);
    converted_points.reserve(_control_points.size());
    foreach(const ControlPointItem &item, _control_points) {
        converted_points.append(item.pos());
    }
    return converted_points;
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
    return _curve_pen;
}

void GraphicsBezierItem::setPen(const QPen &value) {
    _curve_pen = value;
    update();
}

void GraphicsBezierItem::update()
{
    int start_timer = QTime::currentTime().msecsSinceStartOfDay();
    QVector<QPointF> control_qpoints(getControlPoints());
    QVector<qreal> precision_points;
    _curve_points.clear();
    //alloc space
    {
        int new_vector_size = 0;
        for (qreal t = 0 ; t <= 1 ; t += _precision) {
            ++new_vector_size;
        }
        _curve_points.reserve(new_vector_size);
             precision_points.reserve(new_vector_size);
    }
    //calculate points

    _curve_points.append(control_qpoints.first());
    for (qreal t = _precision ; t <= 1-_precision; t += _precision) {
        precision_points.append(t);
    }
    //kids, do not try this at home
    _curve_points.append(QtConcurrent::blockingMapped(precision_points,
                                                 std::bind(&GraphicsBezierItem::singleCurvePointAux,
                                                           this, control_qpoints, std::placeholders::_1)));
    _curve_points.append(control_qpoints.last());
    updateRect();
    qDebug() << "Elapsed: " << QTime::currentTime().msecsSinceStartOfDay() - start_timer << "\n";

}

QRectF GraphicsBezierItem::boundingRect() const {
    return _curve_boundaries;
}

void GraphicsBezierItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(_curve_pen);
    painter->drawPolyline(_curve_points.data(), _curve_points.size());
    painter->setPen(_lines_pen);
    painter->drawPolyline(getControlPoints());
    //painter->drawRect(_curve_boundaries);
}

void GraphicsBezierItem::updateRect() {
    prepareGeometryChange();
    //_curve_boundaries.setRect(0,0,0,0);
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
QPointF GraphicsBezierItem::singleCurvePointAux(const QVector<QPointF> &points, const qreal &parameter_t){
        QVector<QPointF> recalculated_points;
        QVector<QPointF> temporary_buffer_points(points);
        int point_list_size = points.size();
        while (point_list_size > 1) {
            recalculated_points.clear();
            recalculated_points.reserve(point_list_size - 1);
            for (int i = 0 ; i < point_list_size - 1 ; ++i)
                recalculated_points.append((temporary_buffer_points[i+1] - temporary_buffer_points[i]) * parameter_t
                        + temporary_buffer_points[i]);
            --point_list_size;
            if(point_list_size == 1) return recalculated_points.first();
            temporary_buffer_points.clear();
            temporary_buffer_points.reserve(point_list_size);
            temporary_buffer_points = recalculated_points;
        }
        return points.first();
}


QPointF GraphicsBezierItem::singleCurvePoint(const qreal &parameter_t) {

    return singleCurvePointAux(getControlPoints(), parameter_t);
}


GraphicsBezierItem *ControlPointItem::getBezierCurve() const
{
    return _bezier_curve;
}

