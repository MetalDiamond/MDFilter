#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>
#include <math.h>
#include "ker.h"
#include "abstractimage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void printImage(QImage img);

private:
    Ui::MainWindow *ui;
    QImage m_img;
    std::vector<QImage> m_imgStack;
    QImage m_initImg;

private slots:
    void openImage();
    void openSecondImage();
    void saveImage();

    void floydNSteinberg();
    void generateRainbow();
    void moyFilter();

    void dct_compute();

    void optic_stream();

public slots:

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MAINWINDOW_H
