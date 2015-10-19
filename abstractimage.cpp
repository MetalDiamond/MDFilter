#include "abstractimage.h"



AbstractImage::AbstractImage(int width, int height, RGB color)
{
    m_width = width;
    m_height = height;

    m_matrix = (RGB*) malloc(m_width * m_height * sizeof(RGB));

    for (int x=0; x<m_width; x++) {
        for (int y=0; y<m_height; y++) {

            setPixel(x,y,color);
        }
    }
}

AbstractImage::AbstractImage(int width, int height)
{
    m_width = width;
    m_height = height;

    m_matrix = (RGB*) malloc(m_width * m_height * sizeof(RGB));

    for (int x=0; x<m_width; x++) {
        for (int y=0; y<m_height; y++) {

            setPixel(x,y,RGB(0,0,0));
        }
    }
}

AbstractImage::AbstractImage(const AbstractImage &img)
{
    m_width = img.width();
    m_height = img.height();

    m_matrix = (RGB*) malloc(m_width * m_height * sizeof(RGB));

    for (int x=0; x<m_width; x++) {
        for (int y=0; y<m_height; y++) {
            setPixel(x, y, img.pixel(x,y));
        }
    }
}

AbstractImage::AbstractImage(const QImage &img)
{
    m_width = img.width();
    m_height = img.height();

    m_matrix = (RGB*) malloc(m_width * m_height * sizeof(RGB));

    for (int x=0; x<m_width; x++) {
        for (int y=0; y<m_height; y++) {
            RGB color = toRGB(img.pixel(x,y));
            setPixel(x, y, color);
        }
    }
}

void AbstractImage::setPixel(int x, int y, RGB color)
{
    if(x < m_width && y < m_height) {
        m_matrix[pos(x,y)] = color;
    }
}

void AbstractImage::setPixel(int x, int y, float grey)
{
    if(x < m_width && y < m_height) {
        m_matrix[pos(x,y)] = RGB(grey, grey, grey);
    }
}

void AbstractImage::setBlue(int x, int y, float b)
{
    m_matrix[pos(x,y)].b = b;
}

void AbstractImage::setGreen(int x, int y, float g)
{
    m_matrix[pos(x,y)].g = g;
}

void AbstractImage::setRed(int x, int y, float r)
{
    m_matrix[pos(x,y)].r = r;
}

RGB AbstractImage::pixel(int x, int y) const
{
    return m_matrix[pos(x,y)];
}

int AbstractImage::width() const
{
    return m_width;
}

int AbstractImage::height() const
{
    return m_height;
}

QImage AbstractImage::toQImage()
{
    QImage img(QSize(m_width, m_height), QImage::Format_RGB32);

    for (int x=0; x<m_width; x++) {
        for (int y=0; y<m_height; y++) {
            RGB color = m_matrix[pos(x,y)];

            if (color.r > 255) color.r = 255;
            if (color.g > 255) color.g = 255;
            if (color.b > 255) color.b = 255;

            img.setPixel(x,y,qRgb(color.r, color.g, color.b));
        }
    }

    return img;
}

AbstractImage AbstractImage::bloc(int x, int y)
{
    AbstractImage bloc(8, 8);

    if (x+8 < m_width || y+8 < m_height) {

        for (int i=0; i<8; i++) {
            for (int j=0; j<8; j++) {
                bloc.setPixel(i, j, pixel(x+i, y+j));
            }
        }
    }

    return bloc;
}

void AbstractImage::setBloc(int x, int y, AbstractImage bloc)
{
    if (x+8 < m_width || y+8 < m_height) {

        for (int i=0; i<8; i++) {
            for (int j=0; j<8; j++) {
                setPixel(x+i, y+j, bloc.pixel(i, j));
            }
        }
    }
}

std::vector<AbstractImage> AbstractImage::getBlocs()
{
    std::vector<AbstractImage> blocs;

    for(int x=0; x<m_width; x+=8) {
        for(int y=0; y<m_height; y+=8) {
            blocs.push_back(bloc(x, y));
        }
    }
}

double AbstractImage::mse(const AbstractImage &i1, const AbstractImage &i2)
{
    double sum = 0;
    int w = i1.width();
    int h = i1.height();

    for (int x=0; x<w; x++) {
        for (int y=0; y<h; y++) {

            RGB color1 = i1.pixel(x,y);
            RGB color2 = i2.pixel(x,y);

            double dr = abs(color1.r - color2.r);
            double dg = abs(color1.g - color2.g);
            double db = abs(color1.b - color2.b);

            dr *= dr; dg *= dg; db *=db;

            sum += dr + dg + db;
        }
    }

    return sum / (w*h);
}

int AbstractImage::pos(int x, int y) const
{
    return y*m_width+x;
}

RGB AbstractImage::toRGB(QColor color) const
{
    return RGB(color.red(), color.green(), color.blue());
}
