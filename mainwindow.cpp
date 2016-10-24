#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMetaEnum>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    image(new QImage(100, 100, QImage::Format_ARGB32_Premultiplied))
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    //setup view
    auto scaled_image = image->scaled(100 * alg::Shape::getScaleFactor(), 100 * alg::Shape::getScaleFactor());
    pixmap_ptr = scene->addPixmap(QPixmap::fromImage(scaled_image));
    scene->setSceneRect(scaled_image.rect());
    ui->graphicsView->setScene(scene);
    //populate combo box
    //guaranteed safety for enum overflow
    for(int x = Algorithm::DDA; x <= Algorithm::Wu; ++x){
        //using MetaEnum for enum names
        //Qt MetaObjects woo!
        QMetaEnum e = QMetaEnum::fromType<MainWindow::Algorithm>();
        ui->algorithmSelectorComboBox->insertItem(x, e.valueToKey(static_cast<Algorithm>(x)));
    }
    update_on_index_change(0);
    //connect update signal
    connect(ui->algorithmSelectorComboBox, SIGNAL(activated(int)),
            this, SLOT(update_on_index_change(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete image;
}

void MainWindow::update_on_index_change(int index)
{
    QElapsedTimer timer;
    image->fill(Qt::black);
    switch(static_cast<MainWindow::Algorithm>(index)) {
    case MainWindow::DDA:
        timer.start();
        foreach(auto x, alg::demo_surname) {
            x.ddaPath(image);
        }
        break;
    case MainWindow::Bresenham_lines:
        timer.start();
        foreach(auto x, alg::demo_surname) {
            x.bresenhamPath(image);
        }
        break;
    case MainWindow::Bresenham_circles:
        timer.start();
        foreach(auto x, alg::demo_circles) {
            x.bresenhamPath(image);
        }
        break;
    case MainWindow::Wu:
        timer.start();
        foreach(auto x, alg::demo_surname) {
            x.wuPath(image);
        }
        break;
    }
    ui->timeTaken->setText(QString("Time taken: %1 msec").arg(timer.elapsed()));
    update_image();
}

void MainWindow::update_image()
{
    pixmap_ptr->setPixmap(QPixmap::fromImage(image->scaled(100 * alg::Shape::getScaleFactor(), 100 * alg::Shape::getScaleFactor())));
    ui->graphicsView->setScene(scene);
}

