#include "numberscroll.h"

NumberScroll::NumberScroll(QWidget *parent) : QScrollArea(parent){

    contents = new QGroupBox(this);

    QVBoxLayout *main_layout = new QVBoxLayout(contents);

    contents->setLayout(main_layout);

    setWidget(contents);
    setWidgetResizable(true);

}

NumberScroll::~NumberScroll(){

    reset();
    delete contents;

}


void NumberScroll::addNumber(const long long &value, const QString &label){

    numbers.push_back(new NumberWidget(this , value , label));

    contents->layout()->addWidget(numbers.back());

}


void NumberScroll::reset(){

    qDeleteAll(numbers);
    numbers.clear();
}


std::vector<int> NumberScroll::toVector() const{

    std::vector<int> result;
    result.reserve(numbers.size());

    for(NumberWidget *number : numbers){result.push_back(number->value());}

    return result;
}
