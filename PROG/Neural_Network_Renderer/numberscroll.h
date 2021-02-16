#ifndef NUMBERSCROLL_H
#define NUMBERSCROLL_H

#include "numberwidget.h"
#include "MatrixNN/MatrixNN_global.h"

#include <QGroupBox>

#include <QScrollArea>

#include <QVector>


#include <iostream>

#include <vector>



#include <QVBoxLayout>



class NumberScroll : public QScrollArea {

    friend class NumberWidget;
    friend class ResultsWidget;

             Q_OBJECT

public:

    explicit NumberScroll(QWidget *parent);
    ~NumberScroll();

protected:

    void addNumber(const long long &value, const MNN::scalar &probability, const QString &label = "");

    std::vector<int> toVector() const;
    void reset();


private:

    QGroupBox *contents;

    std::vector<NumberWidget*> numbers;


};

#endif // NUMBERSCROLL_H
