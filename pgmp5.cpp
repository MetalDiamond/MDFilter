#include "pgmp5.h"

#include <iostream>
#include <fstream>
#include <QPixmap>

using namespace std;

PGMP5::PGMP5(QString path) {

    QPixmap imgPixmap(path);
}
