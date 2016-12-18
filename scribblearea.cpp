#include "scribblearea.h"

#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#include <QPrintDialog>
#endif

#include "scribblearea.h"

ScribbleArea::ScribbleArea(QWidget* parent)
    : QWidget(parent),
      scale(1),
      image(500, 500, QImage::Format_ARGB32_Premultiplied) {
  setAttribute(Qt::WA_StaticContents);
  // setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  clearImage();
  modified = false;
  scribbling = false;
  penWidth = 1;
  foregroundToolColor = Qt::blue;
  tool = ScribbleArea::ToolType::Pen;
}

bool ScribbleArea::openImage(const QString& fileName) {
  QImage loadedImage;
  if (!loadedImage.load(fileName))
    return false;

  QSize newSize = loadedImage.size().expandedTo(size());
  resizeImage(&loadedImage, newSize);
  image = loadedImage;
  modified = false;
  update();
  return true;
}

bool ScribbleArea::saveImage(const QString& fileName, const char* fileFormat) {
  QImage visibleImage = image;
  resizeImage(&visibleImage, size());

  if (visibleImage.save(fileName, fileFormat)) {
    modified = false;
    return true;
  } else {
    return false;
  }
}

void ScribbleArea::clearImage() {
  image.fill(qRgb(255, 255, 255));
  modified = true;
  update();
}

void ScribbleArea::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    switch (tool) {
      case ScribbleArea::ToolType::Pen:
        lastPoint = event->pos() / scale;
        scribbling = true;
    }
  }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent* event) {
  if ((event->buttons() & Qt::LeftButton) && scribbling)
    switch (tool) {
      case ScribbleArea::ToolType::Pen:
        drawLineTo(event->pos() / scale);
    }
  update();
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton && scribbling) {
    switch (tool) {
      case ScribbleArea::ToolType::Pen:
        drawLineTo(event->pos() / scale);
    }
    scribbling = false;
  }
}

void ScribbleArea::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.scale(scale, scale);
  QRect dirtyRect =
      painter.matrix().inverted().mapRect(event->rect()).adjusted(-1, -1, 1, 1);
  painter.drawImage(dirtyRect, image, dirtyRect);
}

void ScribbleArea::resizeEvent(QResizeEvent* event) {
  //  if (width() > image.width() || height() > image.height()) {
  //    int newWidth = qMax(width() + 128, image.width());
  //    int newHeight = qMax(height() + 128, image.height());
  //    resizeImage(&image, QSize(newWidth, newHeight));
  //    update();
  //  }
  QWidget::resizeEvent(event);
}

QSize ScribbleArea::minimumSizeHint() const {
  return size();
}

QSize ScribbleArea::sizeHint() const {
  return image.rect().size() * scale;
}

void ScribbleArea::drawLineTo(const QPoint& endPoint) {
  QPainter painter(&image);
  painter.setPen(QPen(foregroundToolColor, penWidth, Qt::SolidLine,
                      Qt::RoundCap, Qt::RoundJoin));
  painter.drawLine(lastPoint, endPoint);
  modified = true;

  int rad = (penWidth / 2) + 2;
  update(
      QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
  lastPoint = endPoint;
}

void ScribbleArea::resizeImage(QImage* image, const QSize& newSize) {
  if (image->size() == newSize)
    return;

  QImage newImage(newSize, QImage::Format_RGB32);
  newImage.fill(qRgb(255, 255, 255));
  QPainter painter(&newImage);
  painter.drawImage(QPoint(0, 0), *image);
  *image = newImage;
}

qreal ScribbleArea::getScale() const {
  return scale;
}

void ScribbleArea::setScale(const qreal& value) {
  scale = value;
  resize(image.size() * scale);
  update();
}

bool ScribbleArea::isModified() const {
  return modified;
}

int ScribbleArea::getPenWidth() const {
  return penWidth;
}

void ScribbleArea::setPenWidth(int value) {
  penWidth = value;
}

QColor ScribbleArea::getForegroundToolColor() const {
  return foregroundToolColor;
}

void ScribbleArea::setForegroundToolColor(const QColor& value) {
  foregroundToolColor = value;
}

ScribbleArea::ToolType ScribbleArea::getTool() const {
  return tool;
}

void ScribbleArea::setTool(const ScribbleArea::ToolType& value) {
  tool = value;
}

void ScribbleArea::print() {
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
  QPrinter printer(QPrinter::HighResolution);

  QPrintDialog printDialog(&printer, this);
  if (printDialog.exec() == QDialog::Accepted) {
    QPainter painter(&printer);
    QRect rect = painter.viewport();
    QSize size = image.size();
    size.scale(rect.size(), Qt::KeepAspectRatio);
    painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
    painter.setWindow(image.rect());
    painter.drawImage(0, 0, image);
  }
#endif  // QT_NO_PRINTER
}
