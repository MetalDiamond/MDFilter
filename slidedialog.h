#ifndef SLIDEDIALOG_H
#define SLIDEDIALOG_H

#include <QDialog>

namespace Ui {
class SlideDialog;
}

class SlideDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SlideDialog(QWidget *parent = 0);
    ~SlideDialog();

    void setSliderBounds(int min, int max);
    void setTitle(QString title);

    int getSlideValue();

    static int getInt(QWidget *parent, QString name, int minValue, int maxValue);

private:
    Ui::SlideDialog *ui;

private slots:
    void setSlideLabel();
};

#endif // SLIDEDIALOG_H
