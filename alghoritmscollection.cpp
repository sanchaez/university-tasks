#include "alghoritmscollection.h"
#include <QtCore/qmath.h>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QtAlgorithms>

//delay is used to show practical difference between algorithms
void delay()
{
    QTime dieTime= QTime::currentTime().addMSecs(5);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

alg::Circle::Circle(const uint x, const uint y, const uint radius)
{
    _x = x;
    _y = y;
    _radius = radius;
    _color = qRgb(0xff, 0xff, 0xff);
}

QImage &alg::Circle::bresenhamPath(QImage *img)
{
    uint x = 0;
    uint y = _radius;
    int parameter = 3 - 2 * _radius;
    while (x < y) {
        _bresenhamPathAux(img, x, y);
        ++x;
        if (parameter < 0) {
            parameter +=  4 * x ;
        } else {
            --y;
            parameter += 4 * (x - y);
        }
        _bresenhamPathAux(img, x, y);
    }
    return *img;
}

QColor alg::Shape::color() const
{
    return _color;
}

void alg::Shape::setColor(const QColor &color)
{
    _color = color;
}

uint alg::Shape::getScaleFactor()
{
    return scaleFactor;
}

void alg::Circle::_bresenhamPathAux(QImage *draw_canvas, const uint x, const uint y)
{
    //draw 8 parts at once
    delay();
    draw_canvas->setPixel(_x + x, _y + y, _color.rgb());
    draw_canvas->setPixel(_x - x, _y + y, _color.rgb());
    draw_canvas->setPixel(_x + x, _y - y, _color.rgb());
    draw_canvas->setPixel(_x - x, _y - y, _color.rgb());
    draw_canvas->setPixel(_x + y, _y + x, _color.rgb());
    draw_canvas->setPixel(_x - y, _y + x, _color.rgb());
    draw_canvas->setPixel(_x + y, _y - x, _color.rgb());
    draw_canvas->setPixel(_x - y, _y - x, _color.rgb());
}



alg::Line::Line(uint start_x, uint start_y, uint end_x, uint end_y)
{
    _start_x = start_x;
    _start_y = start_y;
    _end_x = end_x;
    _end_y = end_y;
}

QImage &alg::Line::bresenhamPath(QImage *img)
{
    //Bresenham alghoritm works asssuming coordinates nonequal
    if ((_start_x == _end_x) && (_start_y == _end_y)) {
        img->setPixel(_start_x, _start_y, _color.rgb());
        return *img;
    }
    int delta_x = _end_x - _start_x;
    int delta_y = _end_y - _start_y;
    int j = _start_y;
    int epsilon = delta_y - delta_x;
    if (delta_x > 0){
        for (uint i = _start_x; i < _end_x; ++i) {
            delay();
            img->setPixel(i, j, _color.rgb());
            if (epsilon >= 0) {
                ++j;
                epsilon -= delta_x;
            }
            //++i;
            epsilon += delta_y;
        }
    } else {
        for (uint i = _end_x; i < _start_x; ++i) {
            delay();
            img->setPixel(i, j, _color.rgb());
            if (epsilon >= 0) {
                ++j;
                epsilon -= delta_x;
            }
            //++i;
            epsilon += delta_y;
        }
    }
    return *img;
}

QImage &alg::Line::ddaPath(QImage *img)
{
    if ((_start_x == _end_x) && (_start_y == _end_y)) {
        img->setPixel(_start_x, _start_y, _color.rgb());
        return *img;
    }
    int delta_x=_end_x - _start_x;
    int delta_y=_end_y-_start_y;
    qreal x=_start_x, y=_start_y;
    int steps;
    if(qAbs(delta_x) > qAbs(delta_y)) {
        steps=qAbs(delta_x);
    } else {
        steps=qAbs(delta_y);
    }
    qreal x_increment=delta_x/static_cast<float>(steps);
    qreal y_increment= delta_y/static_cast<float>(steps);

    img->setPixel(qRound(x),qRound(y),_color.rgb());
    for(int k=0; k<steps; k++) {
        x += x_increment;
        y += y_increment;
        delay();
        img->setPixel(qRound(x),qRound(y),_color.rgb());
    }
    return *img;
}

QImage &alg::Line::wuPath(QImage *img)
{
    //draw initial pixels

    uint x0 = _start_x;
    uint x1 = _end_x;
    uint y0 = _start_y;
    uint y1 = _end_y;
    qreal dx = qAbs(static_cast<qreal>(x1) - static_cast<qreal>(x0));
    qreal dy = qAbs(static_cast<qreal>(y1) - static_cast<qreal>(y0));
    //exceptional cases
    if (dy == 0) {
        /* Horizontal line */
        while (dx-- != 0) {
            ++x0;
            img->setPixel(x0, y0, _color.rgb());
        }
        return *img;
    }
    if (dx == 0) {
        /* Vertical line */
        do {
            ++y0;
            img->setPixel(x0, y0, _color.rgb());
        } while (--dy != 0);
        return *img;
    }
    if (dx == dy) {
        /* Diagonal line */
        do {
            ++x0;
            ++y0;
            img->setPixel(x0, y0, _color.rgb());
        } while (--dy != 0);
        return *img;
    }

    if (dx > dy)
    {
        if (x0 > x1)
        {
            qSwap(x0, x1);
            qSwap(y0, y1);
        }
        //delay();
        img->setPixel(_start_x, _start_y, _color.rgb());
        //delay();
        img->setPixel(_end_x, _end_y, _color.rgb());
        int slope = y1 > y0 ? 1: -1;
        qreal gradient = slope * (dy / dx);
        qreal y = y0 + gradient;
        for (auto x = x0 + 1; x < x1; ++x)
        {
            QColor color_top = _color;
            QColor color_bottom = _color;
            color_top.setAlphaF(1 - y + qFloor(y));
            color_bottom.setAlphaF(y - qFloor(y));

            img->setPixel(x, qFloor(y), color_bottom.rgba());
            img->setPixel(x, qFloor(y) + 1, color_top.rgba());

            y += gradient;
        }
    } else {
        if (y0 > y1)
        {
            qSwap(x0, x1);
            qSwap(y0, y1);
        }
        //delay();
        img->setPixel(_start_x, _start_y, _color.rgb());
        //delay();
        img->setPixel(_end_x, _end_y, _color.rgb());
        int slope = x1 > x0 ? 1: -1;
        qreal gradient = slope * (dx / dy);
        qreal x = x0 + gradient;
        for (auto y = y0 + 1; y < y1; ++y)
        {
            QColor color_top = _color;
            QColor color_bottom = _color;
            color_top.setAlphaF(1 - x + qFloor(x));
            color_bottom.setAlphaF(x - qFloor(x));

            img->setPixel(qFloor(x), y, color_bottom.rgba());
            img->setPixel(qFloor(x) + 1, y, color_top.rgba());

            x += gradient;
        }
    }
    return *img;
}
