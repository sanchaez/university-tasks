#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QKeyEvent>
#include <QSize>
#include <QStack>
#include <QVector>
#include <QWidget>
class ScribbleArea : public QWidget {
  Q_OBJECT

 public:
  ScribbleArea(QWidget* parent = 0);
  enum class ToolType { Pen, Bucket, Rectangle, Polygon, Ellipse };
  bool openImage(const QString& fileName);
  bool saveImage(const QString& fileName, const char* fileFormat);
  ToolType getTool() const;

  QColor getForegroundToolColor() const;
  void setForegroundToolColor(const QColor& value);

  int getPenWidth() const;
  void setPenWidth(int value);
  qreal getScale() const;
  void setScale(const qreal& value);

  QColor getBackgroundToolColor() const;
  void setBackgroundToolColor(const QColor& value);

  bool getFilled() const;
  void setFilled(bool value);

  bool isModified() const;

  bool getDrawingState() const;
  void setDrawingState(bool value);

 public slots:
  void undo();
  void redo();
  void setTool(const ToolType& value);
  void newImage();
  void clearImage();
  void print();

 protected:
  void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
  void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
  void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
  void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
  void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

  void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
  void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

  QSize minimumSizeHint() const Q_DECL_OVERRIDE;
  QSize sizeHint() const Q_DECL_OVERRIDE;

 private:
  void drawLineTo(const QPoint& endPoint);
  void drawRectTo(const QPoint& endPoint);
  void drawEllipseTo(const QPoint& endPoint);
  void floodFillQueueOptimized(const QPoint& from);
  void scanlineFillQueue(const QPoint& from);
  void scanlineFillStack(const QPoint& from);
  void resizeImage(QImage* image, const QSize& newSize);

  ToolType tool;

  bool modified, scribbling, filled, useBackgroundColor, equalMode,
      allowDrawing;

  int penWidth;
  qreal scale;
  QColor foregroundToolColor, backgroundToolColor;
  QImage image;
  QStack<QImage> undoStack, redoStack;
  QVector<QPoint> polyline;
  QPoint lastPoint;
};

#endif  // SCRIBBLEAREA_H
