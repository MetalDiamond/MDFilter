#include "slidedialog.h"
#include "ui_slidedialog.h"

SlideDialog::SlideDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SlideDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(setSlideLabel()));
    //QObject::connect(ui->buttonBox->button(), SIGNAL(clicked()), this, SLOT(accept()));
}

SlideDialog::~SlideDialog()
{
    delete ui;
}

void SlideDialog::setSliderBounds(int min, int max)
{
    ui->horizontalSlider->setMinimum(min);
    ui->horizontalSlider->setMaximum(max);
}

void SlideDialog::setTitle(QString title)
{
    ui->label->setText(title);
}

int SlideDialog::getSlideValue()
{
    return ui->horizontalSlider->value();
}

int SlideDialog::getInt(QWidget *parent, QString title, int minValue, int maxValue)
{
    SlideDialog *sd = new SlideDialog(parent);

    sd->setSliderBounds(minValue, maxValue);
    sd->setTitle(title);

    if (sd->exec()) {
        return sd->getSlideValue();
    } else {
        return -1;
    }

    //sd.show();
}

void SlideDialog::setSlideLabel()
{
    ui->label_2->setText(QString::number(ui->horizontalSlider->value()));
}
