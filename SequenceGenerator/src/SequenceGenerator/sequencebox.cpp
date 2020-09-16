#include "mainwindow.h"
#include "sequencebox.h"

//#include <QDebug>

SequenceBox::SequenceBox(MainWindow *_mainWindow) : QGroupBox(_mainWindow){

    mainWindow =_mainWindow;

    sequence = new Sequence();

     QVBoxLayout *main_layout = new QVBoxLayout(this);

     scroll = new QScrollArea(this);

    scroll_contents = new QGroupBox(scroll);
          QVBoxLayout *scroll_contents_layout = new QVBoxLayout(scroll_contents);
    scroll_contents->setLayout(scroll_contents_layout);

    scroll->setWidget(scroll_contents);
    scroll->setWidgetResizable(true);
    scroll->setMinimumHeight(int(mainWindow->height()*0.8));


     main_layout->addWidget(scroll);

}


SequenceBox::~SequenceBox(){qDeleteAll(children());}

/**************************************************************/



void SequenceBox::addMovement(MovementBox *mov_box){

    QVBoxLayout *scroll_contents_layout = dynamic_cast<QVBoxLayout*>(scroll_contents->layout());

     sequence->addMovement(mov_box->getMovement());
     boxes.append(mov_box);

     scroll_contents_layout->addWidget(mov_box);

}

void SequenceBox::addBox(MovementBox *mov_box){

    QVBoxLayout *scroll_contents_layout = dynamic_cast<QVBoxLayout*>(scroll_contents->layout());

     boxes.append(mov_box);

     scroll_contents_layout->addWidget(mov_box);


}

void SequenceBox::removeMovement(MovementBox *mov_box){

    const int index = boxes.indexOf(mov_box);
    boxes.remove(index);
    sequence->removeMovement(index);
}

void SequenceBox::load_sequence(bool withReset){

    if(withReset){reset();}

    QVBoxLayout *scroll_contents_layout = dynamic_cast<QVBoxLayout*>(scroll_contents->layout());

    for(int index = 0 ; index < sequence->getMovements().size() ; index++){
       MovementBox *mov_box = new MovementBox(this , sequence->getMovements().at(index) , true);
       addBox(mov_box);
       scroll_contents_layout->addWidget(mov_box);
    }

}

void SequenceBox::reset(){

    sequence->clear(false);
    qDeleteAll(boxes);
    boxes.clear();


}

/**************************************************************/

MainWindow* SequenceBox::getMainWindow(){return mainWindow;}

QScrollArea* SequenceBox::getScroll(){return scroll;}

Sequence *SequenceBox::getSequence(){return sequence;}

void SequenceBox::setSequence(Sequence* _seq){

    reset();
    sequence = _seq;
    load_sequence(false);

}

QVector<MovementBox*> SequenceBox::getBoxes(){return boxes;}
