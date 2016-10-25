#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainterPath>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <alghoritmscollection.h>
#include <QElapsedTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum Algorithm { DDA, Bresenham_lines, Bresenham_circles, Wu };

    Q_ENUM(Algorithm)

    void setAlgorithm( Algorithm algorithm );
    Algorithm alghoritm() const;

public slots:
    void update_image();
    void update_on_index_change(int index);
    void update_1000_test_current_index(bool state);
private:
    Ui::MainWindow *ui;
    QImage *image;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *pixmap_ptr;
};


#endif // MAINWINDOW_H
