#include "alghoritmscollection.h"
#include <QtCore/qmath.h>

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
    int decision = 3 - 2 * _radius;
    while (x < y) {
        _bresenhamPathAux(img, x, y);
        --x;
        if (decision < 0) {
            decision = decision + 4*x + 6;
        } else {
            --y;
            decision = 4 * (x - y) + 10;
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
    int delta_y = _end_y - _end_x;
    int j = _start_y;
    int epsilon = delta_y - delta_x;
    if (delta_x > 0){
        for (uint i = _start_x; i < _end_x; ++i) {
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
        img->setPixel(qRound(x),qRound(y),_color.rgb());
    }
    return *img;
}

QImage &alg::Line::wuPath(QImage *img)
{
    //draw initial pixel
    img->setPixel(_start_x, _end_x, _color.rgb());

    int delta_x = _end_x - _start_x;
    int delta_y = _end_y - _start_y;
    int direction_increment_x = 1;
    uint start_x = _start_x;
    uint start_y = _start_y;
    if (delta_x < 0) {
        direction_increment_x = -1;
        delta_x = qAbs(delta_x);
    }

    //special-case: horizontal, vertical, and diagonal lines
    if (delta_y == 0) {
        //horisontal
        while (delta_x-- != 0) {
            start_x += direction_increment_x;
            img->setPixel(start_x, start_y, _color.rgb());
        }
        return *img;
    }
    if (delta_x == 0) {
        //vertical
        do {
            start_y++;
            img->setPixel(start_x, start_y, _color.rgb());
        } while (--delta_y != 0);
        return *img;
    }
    if (delta_x == delta_y) {
        //diagonal
        do {
            start_x += direction_increment_x;
            start_y++;
            img->setPixel(start_x, start_y, _color.rgb());
        } while (--delta_y != 0);
        return *img;
    }

    uint error_accumulator = 0;
    ushort error_adj;
    ushort intensity_shift = 100; //percentage of shifting
    uint weighting = static_cast<uint>(_color.lightness());
    if (delta_y > delta_x) {
        /* Y-major line; calculate 16-bit fixed-point fractional part of a
             pixel that X advances each time Y advances 1 pixel, truncating the
             result so that we won't overrun the endpoint along the X axis */
        error_adj = static_cast<ulong>(delta_x << 16) / static_cast<ulong>(delta_y);
        /* Draw all pixels other than the first and last */
        while (--delta_y) {
            uint error_accumulator_temp = error_accumulator;
            error_accumulator += error_adj;
            if (error_accumulator <= error_accumulator_temp) {
                start_x += direction_increment_x;
            }
            ++start_y; // Y-major
            img->setPixel(start_x, start_y, _color.lighter(weighting + intensity_shift).rgb());
            img->setPixel(start_x + direction_increment_x, start_y, _color.darker(weighting + intensity_shift).rgb());
        }
    } else {
        error_adj = (delta_y << 2 )/ delta_x;
        while (--delta_x) {
            uint error_accumulator_temp = error_accumulator;
            error_accumulator += error_adj;
            if (error_accumulator <= error_accumulator_temp) {
                ++start_y;
            }
            start_x += direction_increment_x; // X-major
            img->setPixel(start_x, start_y, _color.lighter(weighting + intensity_shift).rgb());
            img->setPixel(start_x, start_y + 1, _color.darker(weighting + intensity_shift).rgb());
        }
    }
    // Draw the final pixel
    img->setPixel(_end_x, _end_y, _color.rgb());
    return *img;
}
