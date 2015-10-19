#ifndef ABSTRACTIMAGE_H
#define ABSTRACTIMAGE_H

#include <iostream>
#include <QImage>
#include <QColor>

struct RGB{
    float r, g, b;
    RGB(float _r, float _g, float _b) : r(_r), g(_g), b(_b){}
    RGB() : r(0), g(0), b(0) {}
};

class AbstractImage
{
public:

    AbstractImage(int width, int height, RGB color);
    AbstractImage(int width, int height);
    AbstractImage(const AbstractImage &img);
    AbstractImage(const QImage &img);

    void setPixel(int x, int y, RGB color);
    void setPixel(int x, int y, float grey);

    void setBlue(int x, int y, float b);
    void setGreen(int x, int y, float g);
    void setRed(int x, int y, float r);

    RGB pixel(int x, int y) const;

    int width() const;
    int height() const;

    QImage toQImage();

    AbstractImage bloc(int x, int y);
    void setBloc(int x, int y, AbstractImage bloc);
    std::vector<AbstractImage> getBlocs();

    static double mse(const AbstractImage &i1, const AbstractImage &i2);

private:

    int pos(int x, int y) const;
    RGB toRGB(QColor color) const;

    int m_width;
    int m_height;

    RGB* m_matrix;
};

#endif // ABSTRACTIMAGE_H
