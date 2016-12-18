#include "scribblescrollarea.h"
#include <QWheelEvent>
#include <QScrollBar>
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

    QPointF scrollbarPos =
        QPointF(horizontalScrollBar()->value(), verticalScrollBar()->value());
    QPointF delta = (event->posF() / oldScale - widget()->pos() / oldScale) *
                    (newScale - oldScale);
    horizontalScrollBar()->setValue(scrollbarPos.x() + delta.x());
    verticalScrollBar()->setValue(scrollbarPos.y() + delta.y());
  } else {
    QScrollArea::wheelEvent(event);
  }
}

void ScribbleScrollArea::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Shift:
      scalingMode = true;
      break;
    default:
      event->ignore();
      break;
  }
}

void ScribbleScrollArea::keyReleaseEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Shift:
      scalingMode = false;
      break;
    default:
      event->ignore();
      break;
  }
}

ScribbleArea* ScribbleScrollArea::getScribbleArea() const {
  return scribbleArea;
}

void ScribbleScrollArea::setScribbleArea(ScribbleArea* value) {
  scribbleArea = value;
  setWidget(value);
}
