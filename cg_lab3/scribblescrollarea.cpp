#include "scribblescrollarea.h"

#include <QScrollBar>
#include <QWheelEvent>
ScribbleScrollArea::ScribbleScrollArea(ScribbleArea* a)
    : QScrollArea(a), scribbleArea(a), scalingMode(false) {}

void ScribbleScrollArea::wheelEvent(QWheelEvent* event) {
  if (scalingMode) {
    qreal oldScale = scribbleArea->getScale();
    qreal newScale = scribbleArea->getScale();
    if (newScale > 1) {
      newScale += (event->delta() / 120);
    } else if (event->delta() < 0 && newScale <= 1) {
      newScale /= qAbs(event->delta() / 60);
    } else {
      newScale *= qAbs(event->delta() / 60);
    }
    scribbleArea->setScale(newScale);

    QPoint scrollbarPos =
        QPoint(horizontalScrollBar()->value(), verticalScrollBar()->value());
    QPoint delta = (event->pos() / oldScale - widget()->pos() / oldScale) *
                   (newScale - oldScale);
    horizontalScrollBar()->setValue(scrollbarPos.x() + delta.x());
    verticalScrollBar()->setValue(scrollbarPos.y() + delta.y());
  } else {
    QScrollArea::wheelEvent(event);
  }
  setMouseTracking(true);
}

void ScribbleScrollArea::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Control:
      scalingMode = true;
      scribbleArea->setDrawingState(false);
      setCursor(Qt::OpenHandCursor);
      areaCursor = scribbleArea->cursor();
      scribbleArea->setCursor(Qt::OpenHandCursor);
      break;
    default:
      QWidget::keyPressEvent(event);
  }
}

void ScribbleScrollArea::keyReleaseEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Control:
      scalingMode = false;
      scribbleArea->setDrawingState(true);
      setCursor(Qt::ArrowCursor);
      scribbleArea->setCursor(areaCursor);
      break;
    default:
      QWidget::keyReleaseEvent(event);
  }
}

void ScribbleScrollArea::mouseMoveEvent(QMouseEvent* event) {
  if (scalingMode) {
    QPoint scrollbarPos =
        QPoint(horizontalScrollBar()->value(), verticalScrollBar()->value());
    QPoint delta = event->pos() - mousePos;
    mousePos = event->pos();
    horizontalScrollBar()->setValue(scrollbarPos.x() - delta.x());
    verticalScrollBar()->setValue(scrollbarPos.y() - delta.y());
  }
}

void ScribbleScrollArea::mousePressEvent(QMouseEvent* e) {
  if (scalingMode) {
    mousePos = e->pos();
    setCursor(Qt::ClosedHandCursor);
    scribbleArea->setCursor(Qt::ClosedHandCursor);
  }
}

void ScribbleScrollArea::mouseReleaseEvent(QMouseEvent*) {
  if (scalingMode) {
    setCursor(Qt::OpenHandCursor);
    scribbleArea->setCursor(Qt::OpenHandCursor);
  }
}

ScribbleArea* ScribbleScrollArea::getScribbleArea() const {
  return scribbleArea;
}

void ScribbleScrollArea::setScribbleArea(ScribbleArea* value) {
  scribbleArea = value;
  setWidget(value);
}
