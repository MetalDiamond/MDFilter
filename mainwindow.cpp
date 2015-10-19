#include "mainwindow.h"
#include "pgmp5.h"
#include "ui_mainwindow.h"
#include "slidedialog.h"

#include <QBuffer>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <iostream>

# define PI 3.14159265358979323846

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpen_Image, SIGNAL(triggered()), this, SLOT(openImage()));
    connect(ui->actionAdd_Image_To_Stack, SIGNAL(triggered()), this, SLOT(openSecondImage()));
    connect(ui->actionSave_Image, SIGNAL(triggered()), this, SLOT(saveImage()));
    connect(ui->actionGenerate_Rainbow, SIGNAL(triggered()), this, SLOT(generateRainbow()));
    connect(ui->actionAverage, SIGNAL(triggered()), this, SLOT(moyFilter()));
    connect(ui->actionDCT, SIGNAL(triggered()), this, SLOT(dct_compute()));
    connect(ui->actionFnS, SIGNAL(triggered()), this, SLOT(floydNSteinberg()));
    connect(ui->actionMPEG, SIGNAL(triggered()), this, SLOT(optic_stream()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printImage(QImage img)
{
    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(img));

    scene->addItem(item);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(item, Qt::KeepAspectRatio);
}

void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/metaldiamond/M2/TP/Compression/tp3/", tr("Image Files (*.png *.jpg *.bmp *.ppm *.pgm *.gif *.tif)"));
    QPixmap imgPixmap(fileName);
    QImage img = imgPixmap.toImage();

    //PGMP5 pgmImg(fileName);

    //Display image
    printImage(img);

    //Abstract test
    AbstractImage aImg(img);

    //Set filename
    m_img = aImg.toQImage();
    m_initImg = aImg.toQImage();

    //ui->imagePath->setText(fileName);
}

void MainWindow::openSecondImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/metaldiamond/M2/TP/Compression/tp3/", tr("Image Files (*.png *.jpg *.bmp *.ppm *.pgm *.gif *.tif)"));
    QPixmap imgPixmap(fileName);
    QImage img = imgPixmap.toImage();

    //Abstract test
    AbstractImage aImg(img);

    m_imgStack.push_back(aImg.toQImage());
}

void MainWindow::saveImage()
{
    //QFileDialog::getSaveFileName(this);
    m_img.save("/home/metaldiamond/lawl.png");
}

void MainWindow::floydNSteinberg()
{
    int nbColor = QInputDialog::getInt(this, "Generate LUT", "Color amount", 8, 2, 256, 8);

    AbstractImage img(m_img);
    AbstractImage qimg(img.width(), img.height());
    vector<RGB> lut;

    int pas = 256/(nbColor/8);

    for (int r=0; r<=256; r+=pas) {
        for (int g=0; g<=256; g+=pas) {
            for (int b=0; b<=256; b+=pas) {

                lut.push_back(RGB(r,g,b));
            }
        }
    }

    /*for (int k=0; k<lut.size(); k++){
        RGB kolor = lut[k];
        cout << kolor.r << ", " << kolor.g << ", " << kolor.b << endl;
    }*/

    int index = 0;

    for (int x=0; x < img.width(); x++) {
        for (int y=0; y < img.height(); y++) {

            double delta = 12000;

            for (int i=0; i<lut.size(); i++) {

                RGB color = img.pixel(x,y);
                RGB lutColor = lut[i];
                double newDelta = abs(color.r - lutColor.r) + abs(color.g - lutColor.g) + abs(color.b - lutColor.b);

                if (newDelta < delta) {
                    delta = newDelta;
                    index = i;
                }
            }

            qimg.setPixel(x, y, lut[index]);

            RGB c = img.pixel(x,y);
            RGB qc = qimg.pixel(x,y);

            double err_r = c.r - qc.r;
            double err_g = c.g - qc.g;
            double err_b = c.b - qc.b;

            if (x < img.width()) {
                c = img.pixel(x+1, y);
                img.setPixel(x+1, y, RGB(c.r+(7.0/16.0)*err_r, c.g+(7.0/16.0)*err_g, c.b+(7.0/16.0)*err_b));
            }

            if (y < img.height() && x>1) {
                c = img.pixel(x-1, y+1);
                img.setPixel(x-1, y+1, RGB(c.r+(3.0/16.0)*err_r, c.g+(3.0/16.0)*err_g, c.b+(3.0/16.0)*err_b));
            }

            if (y < img.height()) {
                c = img.pixel(x, y+1);
                img.setPixel(x, y+1, RGB(c.r+(5.0/16.0)*err_r, c.g+(5.0/16.0)*err_g, c.b+(5.0/16.0)*err_b));
            }

            if (x < img.width() && y < img.width()) {
                c = img.pixel(x+1, y+1);
                img.setPixel(x+1, y+1, RGB(c.r+(1.0/16.0)*err_r, c.g+(1.0/16.0)*err_g, c.b+(1.0/16.0)*err_b));
            }
        }
    }

    m_img = qimg.toQImage();
    printImage(m_img);
}

void MainWindow::generateRainbow()
{
    QImage img(QSize(256, 256), QImage::Format_RGB32);

    for (int x=0; x<256; x++) {
        for (int y=0; y<256; y++) {
            img.setPixel(QPoint(x, y), qRgb(255-floor((x+y)/2), x, y));
        }
    }

    //Display image
    printImage(img);

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

void print_bloc(int* bloc) {

    cout << "\n\n";

    for (int j=0; j<8; j++) {
        for (int i=0; i<8; i++)
            cout << bloc[j*8+i] << " ";

        cout << "\n";
    }
}

double c(int a) {
    if (a == 0)
        return 1./sqrt(2.);
    else
        return 1.;
}

double dct(int u, int v, int* bloc) {

    double sum = 0;

    for (int y=0; y<8; y++) {
        for (int x=0; x<8; x++)
            sum += cos(((2*x+1)*u*PI) / 16.) * cos(((2*y+1)*v*PI) / 16.) * bloc[y*8+x];
    }

    return round(sum * ((c(u)*c(v)) / 4.));
}

double dct_reverse(int x, int y, int *dct_bloc) {

    double sum = 0;

    for (int v=0; v<8; v++) {
        for (int u=0; u<8; u++)
            sum += c(u) * c(v) * cos(((2*x+1)*u*PI) / 16.) * cos(((2*y+1)*v*PI) / 16.) * dct_bloc[v*8+u];
    }

    return sum * (1./4.);
}

int * q_matrix(int k) {
    int *qMatrix = (int*) malloc(64*sizeof(int));

    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++){
            qMatrix[j*8+i] = (1+k*(1+i+j));
        }
    }

    return qMatrix;
}

int* quantify_bloc(int* qm, int* bloc) {
    int *qBloc = (int*) malloc(64*sizeof(int));

    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++){
            qBloc[j*8+i] = bloc[j*8+i] / qm[j*8+i];
            //cout << bloc[j*8+i] << " / " << qm[j*8+i] << " = " << bloc[j*8+i] / qm[j*8+i] << endl;
        }
    }

    return qBloc;
}

int* unquantify_bloc(int* qm, int* bloc) {
    int *uqBloc = (int*) malloc(64*sizeof(int));

    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++){
            uqBloc[j*8+i] = bloc[j*8+i] * qm[j*8+i];
            //cout << bloc[j*8+i] << " / " << qm[j*8+i] << " = " << bloc[j*8+i] / qm[j*8+i] << endl;
        }
    }

    return uqBloc;
}

QImage rebuild_image(int blocs[][64], int w, int h) {

    QImage img(w,h,QImage::Format_RGB32);

    int indexBloc = 0;

    for (int y=0; y<h; y+=8) {
        for (int x=0; x<w; x+=8) {

            for (int j=0; j<8; j++) {
                for (int i=0; i<8; i++) {

                    int newColor = blocs[indexBloc][j*8+i] + 128;

                    if (newColor > 255)
                        newColor = 255;
                    if (newColor < 0)
                        newColor = 0;

                    int color = newColor;
                    img.setPixel(QPoint(x+i,y+j), qRgb(color, color, color));
                }
            }

            indexBloc++;
        }
    }

    return img;
}

void MainWindow::dct_compute() {

    const int width = m_img.width();
    const int height = m_img.height();
    const int nbBlocs = (width/8) * (height/8);
    const int sizeBloc = 64;

    //Découpage des blocs
    int blocs[nbBlocs][sizeBloc];
    int indexBloc = 0;

    for (int y=0; y<height; y+=8) {
        for (int x=0; x<width; x+=8) {

            for (int j=0; j<8; j++) {
                for (int i=0; i<8; i++) {
                    QRgb pixel = m_img.pixel(x+i,y+j);
                    blocs[indexBloc][j*8+i] = qGray(pixel) - 128;
                }
            }

            indexBloc++;
        }
    }

    /*printImage(rebuild_image(blocs, width, height));
    cout << "\nBloc 0" << endl;
    print_bloc(blocs[0]);*/

    //Compute dct's blocs
    int dct_blocs[nbBlocs][sizeBloc];

    for (int i=0; i<nbBlocs; i++) {

        for (int v=0; v<8; v++) {
            for (int u=0; u<8; u++) {
                dct_blocs[i][v*8+u] = dct(u,v,blocs[i]);
            }
        }
    }

    //printImage(rebuild_image(dct_blocs, width, height));
    /*cout << "\nDCT" << endl;
    print_bloc(dct_blocs[0]);*/

    int taux = SlideDialog::getInt(this, QString("Taux de compression"), 0, 25);

    //Matrice de quantification
    int* qm = q_matrix(taux);
    /*cout << "\nMatrice de Q" << endl;
    print_bloc(qm);*/

    //Quantification des bloc DCT
    int** quantified_blocs = (int**) malloc(nbBlocs*sizeof(sizeBloc*sizeof(int)));//[nbBlocs][sizeBloc];

    for (int i=0; i<nbBlocs; i++) {
        quantified_blocs[i] = quantify_bloc(qm, dct_blocs[i]);
    }

    /*cout << "\nQuantified Bloc" << endl;
    print_bloc(quantified_blocs[0]);*/

    //Déquantification des bloc
    int** unquantified_blocs = (int**) malloc(nbBlocs*sizeof(sizeBloc*sizeof(int)));//[nbBlocs][sizeBloc];

    for (int i=0; i<nbBlocs; i++) {
        unquantified_blocs[i] = unquantify_bloc(qm, quantified_blocs[i]);
    }

    /*cout << "\nUnquantified Bloc" << endl;
    print_bloc(unquantified_blocs[0]);*/

    //Reverse DCT
    int reverse_blocs[nbBlocs][sizeBloc];

    for (int i=0; i<nbBlocs; i++) {

        for (int y=0; y<8; y++) {
            for (int x=0; x<8; x++) {
                reverse_blocs[i][y*8+x] = dct_reverse(x, y, unquantified_blocs[i]);
            }
        }
    }

    printImage(rebuild_image(reverse_blocs, width, height));
    cout << "\nReversed" << endl;
    print_bloc(reverse_blocs[0]);

    m_img = rebuild_image(reverse_blocs, width, height);
}

// MPEG ======================================================

void MainWindow::optic_stream() {

    AbstractImage seq_1(m_img);
    AbstractImage seq_2(m_imgStack[0]);

    const int width = seq_1.width();
    const int height = seq_1.height();

    vector<QPoint> blocVectors;

    for (int y=0; y<height; y+=8) {
        for (int x=0; x<width; x+=8) {

            AbstractImage bloc = seq_2.bloc(x, y);

            int pas = 8/2 + 1;
            int newX = x;
            int newY = y;

            while (pas >= 1) {

                AbstractImage bloc_m = seq_1.bloc(newX, newY);
                double mse = AbstractImage::mse(bloc, bloc_m);

                //Bloc H
                if (newY-pas > 0) {
                    AbstractImage bloc_h = seq_1.bloc(newX, newY-pas);
                    double tmp_mse = AbstractImage::mse(bloc, bloc_h);
                    if (mse > tmp_mse) {
                        mse = tmp_mse;
                        newY = newY-pas;
                    }
                }

                //Bloc D
                if (newX-pas > 0) {
                    AbstractImage bloc_d = seq_1.bloc(newX-pas, newY);
                    double tmp_mse = AbstractImage::mse(bloc, bloc_d);

                    if (mse > tmp_mse) {
                        mse = tmp_mse;
                        newX = newX-pas;
                    }
                }

                //Bloc B
                if (newY+pas < height) {
                    AbstractImage bloc_b = seq_1.bloc(newX, newY+pas);
                    double tmp_mse = AbstractImage::mse(bloc, bloc_b);

                    if (mse > tmp_mse) {
                        mse = tmp_mse;
                        newY = newY+pas;
                    }
                }

                //Bloc G
                if (newX+pas < width) {
                    AbstractImage bloc_g = seq_1.bloc(newX+pas, newY);
                    double tmp_mse = AbstractImage::mse(bloc, bloc_g);

                    if (mse > tmp_mse) {
                        mse = tmp_mse;
                        newX = newX+pas;
                    }
                }

                pas /= 2;
            }

            //List de vecteur dans l'ordre de parcours des blocs
            blocVectors.push_back(QPoint(newX - x, newY - y));
        }
    }

    //Reconstruction à partir du flot optique
    AbstractImage predImg(seq_1.width(), seq_1.height());

    int i = 0;

    for (int y=0; y<height; y+=8) {
        for (int x=0; x<width; x+=8) {

            QPoint dec = blocVectors[i];
            cout << "(" << dec.x() << "," << dec.y() << ")" << endl;
            predImg.setBloc(x, y, seq_1.bloc(x+dec.x(), y+dec.y()));
            i++;
        }
    }

    printImage(predImg.toQImage());
}

void MainWindow::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    ui->xPos->setText(QString::number(event->scenePos().x()));
    ui->yPos->setText(QString::number(event->scenePos().y()));
}
