#ifndef SCRIBBLESCROLLAREA_H
#define SCRIBBLESCROLLAREA_H

#include <QScrollArea>
#include "scribblearea.h"

class ScribbleScrollArea : public QScrollArea {
 public:
  ScribbleScrollArea(ScribbleArea* a = Q_NULLPTR);

  ScribbleArea* getScribbleArea() const;
  void setScribbleArea(ScribbleArea* value);

 protected:
  void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;
  void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
  void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

 private:
  ScribbleArea* scribbleArea;
  bool scalingMode;
};

#endif  // SCRIBBLESCROLLAREA_H
