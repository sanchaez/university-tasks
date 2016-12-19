#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QSize>
#include <QWidget>

class ScribbleArea : public QWidget {
  Q_OBJECT

 public:
  ScribbleArea(QWidget* parent = 0);
  enum class ToolType { Pen, Bucket, Rectangle, Polygon };
  bool openImage(const QString& fileName);
  bool saveImage(const QString& fileName, const char* fileFormat);
  ToolType getTool() const;

  QColor getForegroundToolColor() const;
  void setForegroundToolColor(const QColor& value);

  int getPenWidth() const;
  void setPenWidth(int value);

  bool isModified() const;

  qreal getScale() const;
  void setScale(const qreal& value);

  QColor getBackgroundToolColor() const;
  void setBackgroundToolColor(const QColor& value);

 public slots:
  void setTool(const ToolType& value);
  void clearImage();
  void print();

 protected:
  void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
  void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
  void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

  void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
  void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

  QSize minimumSizeHint() const Q_DECL_OVERRIDE;
  QSize sizeHint() const Q_DECL_OVERRIDE;

 private:
  void drawLineTo(const QPoint& endPoint);
  void floodFillQueueOptimized(const QPoint& from);
  void scanlineFillQueue(const QPoint& from);
  void scanlineFillStack(const QPoint& from);
  void resizeImage(QImage* image, const QSize& newSize);

  ToolType tool;

  bool modified, scribbling, useBackgroundColor;

  int penWidth;
  qreal scale;
  QColor foregroundToolColor, backgroundToolColor;
  QImage image;
  QPoint lastPoint;
};

#endif  // SCRIBBLEAREA_H
