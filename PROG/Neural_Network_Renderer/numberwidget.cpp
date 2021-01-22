#include "numberwidget.h"

NumberWidget::NumberWidget(QWidget *parent , const long long &num , const QString &label) : QGroupBox(label , parent){

    QHBoxLayout *main_layout = new QHBoxLayout(this);

    number = new QLCDNumber(this);
    number->setDigitCount(1);
    number->display(int(num));
    number->setMinimumHeight(100);
    number->setMaximumWidth(100);


//    number_selector = new QSlider(this);
//    number_selector->setMinimum(0);
//    number_selector->setMaximum(9);
//    number_selector->setTickInterval(1);
//    number_selector->setValue(int(num));
//    connect(number_selector , &QSlider::valueChanged , this , &NumberWidget::on_number_selector_update);


    main_layout->addWidget(number);
    //main_layout->addWidget(number_selector);


    setLayout(main_layout);

}

NumberWidget::~NumberWidget(){

    delete number;
    //delete number_selector;

    delete layout();

}

int NumberWidget::value() const{return number->intValue();}


//void NumberWidget::on_number_selector_update(){
//    number->display(number_selector->value());
//}
