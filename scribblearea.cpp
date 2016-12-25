#include "scribblearea.h"

#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrintDialog>
#include <QPrinter>
#endif

#include <QCursor>
#include <QQueue>
#include <QStack>
#include <QThread>

ScribbleArea::ScribbleArea(QWidget* parent)
    : QWidget(parent), image(500, 500, QImage::Format_ARGB32_Premultiplied) {
  // setAttribute(Qt::WA_StaticContents);
  // setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  clearImage();
  modified = false;
  scribbling = false;
  useBackgroundColor = false;
  equalMode = false;
  penWidth = 1;
  scale = 1;
  foregroundToolColor = Qt::black;
  backgroundToolColor = Qt::red;
  tool = ScribbleArea::ToolType::Pen;
  setCursor(Qt::CrossCursor);
}

bool ScribbleArea::openImage(const QString& fileName) {
  QImage loadedImage;
  if (!loadedImage.load(fileName)) return false;
  update();
  scale = 1;
  resize(loadedImage.size());
  undoStack.clear();
  redoStack.clear();

  return true;
}

bool ScribbleArea::saveImage(const QString& fileName, const char* fileFormat) {
  if (image.save(fileName, fileFormat)) {
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
  if (allowDrawing) {
    if (event->button() == Qt::RightButton) {
      useBackgroundColor = true;
    } else if (event->button() != Qt::LeftButton) {
      event->ignore();
    }
    undoStack.push(image);
    if (!redoStack.isEmpty()) redoStack.clear();
    switch (tool) {
      case ScribbleArea::ToolType::Pen:
      case ScribbleArea::ToolType::Rectangle:
      case ScribbleArea::ToolType::Ellipse:
        lastPoint = event->pos() / scale;
        scribbling = true;
        break;
      case ScribbleArea::ToolType::Bucket:
        floodFillQueueOptimized(event->pos() / scale);
        break;
      default:
        event->ignore();
    }
  } else {
    event->ignore();
  }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent* event) {
  if (allowDrawing) {
    if ((event->buttons() & (Qt::LeftButton | Qt::RightButton)) && scribbling)
      switch (tool) {
        case ScribbleArea::ToolType::Pen:
          drawLineTo(event->pos() / scale);
          break;
        case ScribbleArea::ToolType::Rectangle:
          image = undoStack.top();
          drawRectTo(event->pos() / scale);
          break;
        case ScribbleArea::ToolType::Ellipse:
          image = undoStack.top();
          drawEllipseTo(event->pos() / scale);
          break;
        default:
          event->ignore();
      }
    update();
  } else {
    event->ignore();
  }
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent* event) {
  if (allowDrawing) {
    if (event->button() == Qt::RightButton) {
      useBackgroundColor = false;
    }
    if ((event->buttons() & (Qt::LeftButton | Qt::RightButton)) && scribbling) {
      switch (tool) {
        case ScribbleArea::ToolType::Pen:
          drawLineTo(event->pos() / scale);
          scribbling = false;
          break;
        case ScribbleArea::ToolType::Rectangle:
          image = undoStack.top();
          drawRectTo(event->pos() / scale);
          scribbling = false;
          break;
        case ScribbleArea::ToolType::Ellipse:
          image = undoStack.top();
          drawEllipseTo(event->pos() / scale);
          scribbling = false;
          break;
        default:
          event->ignore();
      }

    } else {
      event->ignore();
    }
    update();
  } else {
    event->ignore();
  }
}

void ScribbleArea::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Shift:
      equalMode = true;
      break;
    default:
      event->ignore();
      break;
  }
}

void ScribbleArea::keyReleaseEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Shift:
      equalMode = false;
      break;
    default:
      event->ignore();
      break;
  }
}

void ScribbleArea::floodFillQueueOptimized(const QPoint& from) {
  QQueue<QPoint> pixelQueue;
  QColor targetColor = image.pixelColor(from), fillColor;
  if (useBackgroundColor) {
    fillColor = backgroundToolColor;
  } else {
    fillColor = foregroundToolColor;
  }

  if (targetColor == fillColor) {
    return;
  }
  pixelQueue.enqueue(from);
  while (!pixelQueue.isEmpty()) {
    QPoint pixelWest = pixelQueue.dequeue();
    QPoint pixelEast = pixelWest;
    while (pixelEast.x() < image.width() &&
           image.pixelColor(pixelEast) == targetColor) {
      pixelEast.setX(pixelEast.x() + 1);
    }
    while (pixelWest.x() >= 0 && image.pixelColor(pixelWest) == targetColor) {
      pixelWest.setX(pixelWest.x() - 1);
    }

    for (int xcoord = pixelEast.x() - 1, ycoord = pixelEast.y();
         xcoord > pixelWest.x(); --xcoord) {
      image.setPixelColor(xcoord, ycoord, fillColor);
      // QThread::msleep(1);

      // QCoreApplication::processEvents();
      if (ycoord > 0 && ycoord < image.height() - 1) {
        if (image.pixelColor(xcoord, ycoord - 1) == targetColor) {
          pixelQueue.enqueue(QPoint(xcoord, ycoord - 1));
        }
        if (image.pixelColor(xcoord, ycoord + 1) == targetColor) {
          pixelQueue.enqueue(QPoint(xcoord, ycoord + 1));
        }
      }
    }
  }
  update();
}

void ScribbleArea::scanlineFillQueue(const QPoint& from) {
  QColor targetColor = image.pixelColor(from), fillColor;
  if (useBackgroundColor) {
    fillColor = backgroundToolColor;
  } else {
    fillColor = foregroundToolColor;
  }
  if (targetColor == fillColor) return;
  QStack<QPoint> pixels;
  pixels.push(from);

  while (!pixels.empty()) {
    QPoint nextPoint = pixels.pop();
    while (nextPoint.y() >= 0 && image.pixelColor(nextPoint) == targetColor)
      nextPoint.setY(nextPoint.y() - 1);
    nextPoint.setY(nextPoint.y() + 1);
    bool left = false;
    bool right = false;
    while (nextPoint.y() < image.height() &&
           image.pixelColor(nextPoint) == targetColor) {
      image.setPixelColor(nextPoint, fillColor);
      if (!left && nextPoint.x() > 0 &&
          image.pixelColor(nextPoint.x() - 1, nextPoint.y()) == targetColor) {
        pixels.push(QPoint(nextPoint.x() - 1, nextPoint.y()));
        left = true;
      } else if (left && nextPoint.x() - 1 == 0 &&
                 image.pixelColor(nextPoint.x() - 1, nextPoint.y()) !=
                     targetColor)
        left = false;

      if (!right && nextPoint.x() < image.width() - 1 &&
          image.pixelColor(nextPoint.x() + 1, nextPoint.y()) == targetColor) {
        pixels.push(QPoint(nextPoint.x() + 1, nextPoint.y()));
        right = true;
      } else if (right && nextPoint.x() < image.width() - 1 &&
                 image.pixelColor(nextPoint.x() + 1, nextPoint.y()) !=
                     targetColor) {
        right = false;
      }
      nextPoint.setY(nextPoint.y() + 1);
    }
  }
  update();
}

void ScribbleArea::scanlineFillStack(const QPoint& from) {
  QColor targetColor = image.pixelColor(from), fillColor;
  if (useBackgroundColor) {
    fillColor = backgroundToolColor;
  } else {
    fillColor = foregroundToolColor;
  }
  if (targetColor == fillColor) return;
  QStack<QPoint> pixels;
  pixels.push(from);

  while (!pixels.empty()) {
    QPoint nextPoint = pixels.pop();
    while (nextPoint.y() >= 0 && image.pixelColor(nextPoint) == targetColor)
      nextPoint.setY(nextPoint.y() - 1);
    nextPoint.setY(nextPoint.y() + 1);
    bool left = false;
    bool right = false;
    while (nextPoint.y() < image.height() &&
           image.pixelColor(nextPoint) == targetColor) {
      image.setPixelColor(nextPoint, fillColor);
      if (!left && nextPoint.x() > 0 &&
          image.pixelColor(nextPoint.x() - 1, nextPoint.y()) == targetColor) {
        pixels.push(QPoint(nextPoint.x() - 1, nextPoint.y()));
        left = true;
      } else if (left && nextPoint.x() - 1 == 0 &&
                 image.pixelColor(nextPoint.x() - 1, nextPoint.y()) !=
                     targetColor)
        left = false;

      if (!right && nextPoint.x() < image.width() - 1 &&
          image.pixelColor(nextPoint.x() + 1, nextPoint.y()) == targetColor) {
        pixels.push(QPoint(nextPoint.x() + 1, nextPoint.y()));
        right = true;
      } else if (right && nextPoint.x() < image.width() - 1 &&
                 image.pixelColor(nextPoint.x() + 1, nextPoint.y()) !=
                     targetColor) {
        right = false;
      }
      nextPoint.setY(nextPoint.y() + 1);
    }
  }
  update();
}

void ScribbleArea::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.scale(scale, scale);
  QRect dirtyRect =
      painter.matrix().inverted().mapRect(event->rect()).adjusted(-1, -1, 1, 1);
  painter.drawImage(dirtyRect, image, dirtyRect);
}

void ScribbleArea::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
}

QSize ScribbleArea::minimumSizeHint() const {
  return image.rect().size() * scale;
}

QSize ScribbleArea::sizeHint() const { return image.rect().size() * scale; }

void ScribbleArea::drawLineTo(const QPoint& endPoint) {
  QPainter painter(&image);
  if (useBackgroundColor) {
    painter.setPen(QPen(backgroundToolColor, penWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
  } else {
    painter.setPen(QPen(foregroundToolColor, penWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
  }
  painter.drawLine(lastPoint, endPoint);
  modified = true;

  int rad = (penWidth / 2) + 2;
  update(
      QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
  lastPoint = endPoint;
}

void ScribbleArea::drawRectTo(const QPoint& endPoint) {
  QPainter painter(&image);
  if (useBackgroundColor) {
    painter.setPen(QPen(backgroundToolColor, penWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    if (filled && lastPoint != endPoint) {
      painter.setBrush(QBrush(foregroundToolColor));
    }

  } else {
    painter.setPen(QPen(foregroundToolColor, penWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    if (filled && lastPoint != endPoint) {
      painter.setBrush(QBrush(backgroundToolColor));
    }
  }
  QPainterPath path;
  if (equalMode) {
    path.addRect(QRect(
        lastPoint, QPoint(lastPoint.x() + qAbs(lastPoint.x() - endPoint.x()),
                          lastPoint.y() + qAbs(lastPoint.x() - endPoint.x()))));
  } else {
    path.addRect(QRect(lastPoint, endPoint));
  }
  painter.fillPath(path, painter.brush());
  painter.drawPath(path);
}

void ScribbleArea::drawEllipseTo(const QPoint& endPoint) {
  QPainter painter(&image);
  if (useBackgroundColor) {
    painter.setPen(QPen(backgroundToolColor, penWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    if (filled && lastPoint != endPoint) {
      painter.setBrush(QBrush(foregroundToolColor));
    }

  } else {
    painter.setPen(QPen(foregroundToolColor, penWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    if (filled && lastPoint != endPoint) {
      painter.setBrush(QBrush(backgroundToolColor));
    }
  }
  QPainterPath path;
  if (equalMode) {
    path.addEllipse(QRect(
        lastPoint, QPoint(lastPoint.x() + qAbs(lastPoint.x() - endPoint.x()),
                          lastPoint.y() + qAbs(lastPoint.x() - endPoint.x()))));
  } else {
    path.addEllipse(QRect(lastPoint, endPoint));
  }
  painter.fillPath(path, painter.brush());
  painter.drawPath(path);
}

void ScribbleArea::resizeImage(QImage* image, const QSize& newSize) {
  if (image->size() == newSize) return;

  QImage newImage(newSize, QImage::Format_RGB32);
  newImage.fill(qRgb(255, 255, 255));
  QPainter painter(&newImage);
  painter.drawImage(QPoint(0, 0), *image);
  *image = newImage;
}

bool ScribbleArea::getDrawingState() const { return allowDrawing; }

void ScribbleArea::setDrawingState(bool value) { allowDrawing = value; }

bool ScribbleArea::isModified() const { return modified; }

bool ScribbleArea::getFilled() const { return filled; }

void ScribbleArea::setFilled(bool value) { filled = value; }

void ScribbleArea::undo() {
  if (!undoStack.isEmpty()) {
    redoStack.push(image);
    image = undoStack.pop();
  }
  update();
}

void ScribbleArea::redo() {
  if (!redoStack.isEmpty()) {
    image = redoStack.pop();
    undoStack.push(image);
  }
  update();
}

QColor ScribbleArea::getBackgroundToolColor() const {
  return backgroundToolColor;
}

void ScribbleArea::setBackgroundToolColor(const QColor& value) {
  backgroundToolColor = value;
}

qreal ScribbleArea::getScale() const { return scale; }

void ScribbleArea::setScale(const qreal& value) {
  scale = value;
  resize(image.size() * scale);
  update();
}
int ScribbleArea::getPenWidth() const { return penWidth; }

void ScribbleArea::setPenWidth(int value) { penWidth = value; }

QColor ScribbleArea::getForegroundToolColor() const {
  return foregroundToolColor;
}

void ScribbleArea::setForegroundToolColor(const QColor& value) {
  foregroundToolColor = value;
}

ScribbleArea::ToolType ScribbleArea::getTool() const { return tool; }

void ScribbleArea::setTool(const ScribbleArea::ToolType& value) {
  tool = value;
}

void ScribbleArea::newImage() {
  image = QImage(500, 500, QImage::Format_ARGB32_Premultiplied);

  undoStack.clear();
  redoStack.clear();
  clearImage();
  scribbling = false;
  useBackgroundColor = false;
  equalMode = false;
  penWidth = 1;
  scale = 1;
  foregroundToolColor = Qt::black;
  backgroundToolColor = Qt::white;
  tool = ScribbleArea::ToolType::Pen;
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
