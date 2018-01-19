#ifndef ALGHORITMSCOLLECTION_H
#define ALGHORITMSCOLLECTION_H

#include <QObject>
#include <QPainterPath>
#include <QRect>
#include <QLine>
#include <QColor>
#include <QImage>
#include <QList>

/* Collection of needed alghoritms
 * QLine is used as a container to describe lines
 * QPainterPath will contain pseudo-pixel data
 * utilized by QPainter
 */
namespace alg
{
class PseudoPixel: public QRect {};     //TODO: parametrize size
//TODO: make it a square
//virtual base class of an shape
class Shape
{
public:
    static uint getScaleFactor();
    QColor color() const;
    void setColor(const QColor &color);
    virtual ~Shape() = default;
    virtual QImage &bresenhamPath(QImage *img) = 0;

protected:
    static const uint scaleFactor = 5;
    QColor _color = qRgb(0xff, 0xff, 0xff);
};

class Circle: public Shape
{
public:
    Circle() = default;
    Circle(const uint x, const uint y, const uint radius);
    QImage &bresenhamPath(QImage *img);

private:
    uint _x, _y, _radius;

    void _bresenhamPathAux(QImage *draw_canvas, const uint x, const uint y);
};

class Line: public Shape
{
public:
    //Line() = default;
    Line(uint start_x, uint start_y, uint end_x, uint end_y);
    QImage &bresenhamPath(QImage *img);
    QImage &ddaPath(QImage *img);
    QImage &wuPath(QImage *img);

protected:
    uint _start_x, _start_y, _end_x, _end_y;
};

//demo
const QList<Line> demo_surname
{
    //s
    Line(11, 18, 18, 14),
    Line(11, 18, 18, 22),
    Line(11, 25, 18, 22),
    //h
    Line(21, 14, 21, 25),
    Line(21, 18, 26, 17),
    Line(26, 14, 26, 25),
    //a
    Line(29, 25, 33, 14),
    Line(33, 14, 37, 25),
    Line(30, 21, 35, 20),
    //p
    Line(40, 14, 40, 25),
    Line(40, 14, 40, 25),
    Line(45, 16, 40, 14),
    Line(45, 16, 40, 18),
    //o
    Line(50, 14, 47, 25),
    Line(47, 25, 54, 24),
    Line(50, 14, 54, 24),
    //s
    Line(11, 32, 18, 28),
    Line(11, 32, 18, 36),
    Line(11, 39, 18, 36),
    //h
    Line(21, 28, 21, 39),
    Line(21, 32, 26, 31),
    Line(26, 28, 26, 39),
    //n
    Line(37, 28, 37, 39),
    Line(29, 28, 29, 39),
    Line(29, 28, 37, 39),
    //i
    Line(40, 28, 45, 28),
    Line(40, 39, 45, 39),
    Line(42, 28, 43, 39),
    //k
    Line(47, 28, 47, 39),
    Line(47, 31, 54, 28),
    Line(49, 31, 54, 39),
    //o
    Line(57, 39, 60, 28),
    Line(64, 38, 60, 28),
    Line(57, 39, 64, 38),
    //v
    Line(69, 39, 66, 28),
    Line(69, 39, 72, 28),

};

const QList<Circle> demo_circles
{
    Circle(5, 5, 5),
            Circle(50, 50, 45),
            Circle(30, 14, 10)
};
}

#endif // ALGHORITMSCOLLECTION_H
