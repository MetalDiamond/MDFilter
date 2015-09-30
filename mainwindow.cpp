#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpen_Image, SIGNAL(triggered()), this, SLOT(openImage()));
    connect(ui->actionGenerate_Rainbow, SIGNAL(triggered()), this, SLOT(generateRainbow()));
    connect(ui->actionAverage, SIGNAL(triggered()), this, SLOT(moyFilter()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp *.ppm *.gif *.tif)"));
    QImage img(fileName);

    //Display image
    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(img));
    scene->addItem(item);

    //Set filename
    m_img = img;
    ui->imagePath->setText(fileName);
}

void MainWindow::generateRainbow()
{
    QImage img(QSize(256, 256), QImage::Format_RGB32);

    for (int x=0; x<256; x++) {
        for (int y=0; y<256; y++) {
            img.setPixel(QPoint(x, y), qRgb(256-floor((x+y)/2), x, y));
        }
    }

    //Display image
    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(img));
    scene->addItem(item);

    m_img = img;
}

void MainWindow::moyFilter()
{
    QImage newImg(m_img.size(), QImage::Format_RGB32);
    double moy = 0;

    ker_t *kernel = ker_init(5,5);
    for (int x=0;x<5;x++) {
        for (int y=0;y<5;y++)
            kernel->array[x][y] = 1;
    }

    ker_normalize(kernel);

    //Parcours de l'image avec une marge pour ne pas traiter les bords
    for (int x=2; x < newImg.width()-3; x++) {
        for (int y=2; y < newImg.height()-3; y++) {

            moy = 0;

            int half = kernel->width/2;

            //Calcul de la moyenne avec kernel
            for (int k=0; k < kernel->width; ++k) {
                for (int l=0; l < kernel->height; ++l) {

                    QColor pixel = m_img.pixel((x+k-half), (y+l-half));
                    moy += kernel->array[k][l] * pixel.red();
                }
            }

            newImg.setPixel(x, y, qRgb(moy, moy, moy));
        }
    }

    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(newImg));
    scene->addItem(item);
}
