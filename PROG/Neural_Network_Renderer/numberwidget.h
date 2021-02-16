#ifndef NUMBERWIDGET_H
#define NUMBERWIDGET_H

#include "MatrixNN/MatrixNN_global.h"

#include <stdint.h>

#include <QGroupBox>

#include <QLabel>
#include <QLCDNumber>

#include <QSlider>

#include <QHBoxLayout>

class NumberWidget : public QGroupBox {

    friend class NumberScroll;
    friend class ResultsWidget;

         Q_OBJECT

public:

    NumberWidget(QWidget *parent , const long long &num, const MNN::scalar &probability, const QString &label = "");
    ~NumberWidget();

    int value() const;

private:

    QLCDNumber *number;



};

#endif // NUMBERWIDGET_H
