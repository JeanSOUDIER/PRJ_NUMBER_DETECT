#ifndef NUMBERWIDGET_H
#define NUMBERWIDGET_H

#include <stdint.h>

#include <QGroupBox>

#include <QLCDNumber>

#include <QSlider>

#include <QHBoxLayout>

class NumberWidget : public QGroupBox {

    friend class NumberScroll;
    friend class ResultsWidget;

         Q_OBJECT

public:

    NumberWidget(QWidget *parent , const long long &num, const QString &label = "");
    ~NumberWidget();

    int value() const;

private:

    QLCDNumber *number;

//    QSlider *number_selector;

//private slots:

//    void on_number_selector_update();

};

#endif // NUMBERWIDGET_H
