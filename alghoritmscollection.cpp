#include "alghoritmscollection.h"
#include <QtCore/qmath.h>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QtAlgorithms>

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
    int y_incr = 1 - 2 * static_cast<int>(_radius);
    int x_incr = 1;
    int radius_error = 0;
    _bresenhamPathAux(img, x, y);
    while (x <= y) {
        _bresenhamPathAux(img, x, y);
        ++x;
        radius_error += x_incr;
        x_incr += 2;
        if ((2 * radius_error + y_incr) >= 0) {
            --y;
            radius_error += y_incr;
            y_incr += 2;
        }
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
    draw_canvas->setPixel(_x + x, _y + y, _color.rgba());
    draw_canvas->setPixel(_x - x, _y + y, _color.rgba());
    draw_canvas->setPixel(_x + x, _y - y, _color.rgba());
    draw_canvas->setPixel(_x - x, _y - y, _color.rgba());
    draw_canvas->setPixel(_x + y, _y + x, _color.rgba());
    draw_canvas->setPixel(_x - y, _y + x, _color.rgba());
    draw_canvas->setPixel(_x + y, _y - x, _color.rgba());
    draw_canvas->setPixel(_x - y, _y - x, _color.rgba());
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
        img->setPixel(_start_x, _start_y, _color.rgba());
        return *img;
    }
    uint x1 = _start_x, y1 = _start_y, x2 = _end_x, y2 = _end_y;

    // Bresenham's line algorithm
    int delta_x(x2 - x1);
    // if x1 == x2, then it does not matter what we set here
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;

    int delta_y(y2 - y1);
    // if y1 == y2, then it does not matter what we set here
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;

    img->setPixel(x1, y1, _color.rgba());

    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));

        while (x1 != x2)
        {
            if ((error >= 0) && (error || (ix > 0)))
            {
                error -= delta_x;
                y1 += iy;
            }
            // else do nothing

            error += delta_y;
            x1 += ix;

            img->setPixel(x1, y1, _color.rgba());
        }
    }
    else
    {
        // error may go below zero
        int error(delta_x - (delta_y >> 1));

        while (y1 != y2)
        {
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                x1 += ix;
            }
            // else do nothing

            error += delta_x;
            y1 += iy;

            img->setPixel(x1, y1, _color.rgba());
        }
    }
    return *img;
}

QImage &alg::Line::ddaPath(QImage *img)
{
    if ((_start_x == _end_x) && (_start_y == _end_y)) {
        img->setPixel(_start_x, _start_y, _color.rgba());
        return *img;
    }
    int delta_x =_end_x - _start_x;
    int delta_y =_end_y-_start_y;
    qreal x=_start_x, y=_start_y;
    int steps;
    if(qAbs(delta_x) > qAbs(delta_y)) {
        steps=qAbs(delta_x);
    } else {
        steps=qAbs(delta_y);
    }
    qreal x_increment= delta_x/static_cast<float>(steps);
    qreal y_increment= delta_y/static_cast<float>(steps);

    img->setPixel(qRound(x),qRound(y),_color.rgba());
    for(int k=0; k<steps; k++) {
        x += x_increment;
        y += y_increment;

        img->setPixel(qRound(x),qRound(y),_color.rgba());
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
            img->setPixel(x0, y0, _color.rgba());
        }
        return *img;
    }
    if (dx == 0) {
        /* Vertical line */
        do {
            ++y0;
            img->setPixel(x0, y0, _color.rgba());
        } while (--dy != 0);
        return *img;
    }
    if (dx == dy) {
        /* Diagonal line */
        do {
            ++x0;
            ++y0;
            img->setPixel(x0, y0, _color.rgba());
        } while (--dy != 0);
        return *img;
    }
    if (dx > dy)
    {
        /* x major */
        if (x0 > x1)
        {
            qSwap(x0, x1);
            qSwap(y0, y1);
        }
        img->setPixel(_start_x, _start_y, _color.rgba());
        img->setPixel(_end_x, _end_y, _color.rgba());
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
        /* y major */
        if (y0 > y1)
        {
            qSwap(x0, x1);
            qSwap(y0, y1);
        }
        img->setPixel(_start_x, _start_y, _color.rgba());
        img->setPixel(_end_x, _end_y, _color.rgba());
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
