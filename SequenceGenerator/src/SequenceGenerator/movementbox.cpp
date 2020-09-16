#include "movementbox.h"

#include "mainwindow.h"

//#include <QDebug>

MovementBox::MovementBox(SequenceBox *_sequenceBox , Movement* _movement, bool fromSequence){

    movement = _movement;
    sequenceBox = _sequenceBox;
    setup();

    if(!fromSequence){sequenceBox->addMovement(this);}

    if(movement->getMode() == MovementMode::COORDINATES){toCoordinates(false , true); }
    else{toDirection(false , true);}


}

MovementBox::MovementBox(SequenceBox *_sequenceBox){

    movement = new Movement();
    sequenceBox = _sequenceBox;

    setup();

    sequenceBox->addMovement(this);

    if(movement->getMode() == MovementMode::COORDINATES){toCoordinates(false, false);}
    else{toDirection(false, false);}

}

MovementBox::~MovementBox(){
    qDeleteAll(children());
    delete movement;
}



/**************************************************************/

void MovementBox::setup(){

    QFormLayout *main_layout = new QFormLayout(this);

    delete_me_button = new QToolButton(this);
    delete_me_button->setToolTip(tr("Delete this movement"));
    delete_me_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    delete_me_button->setIcon(QIcon(QPixmap(":/buttons/rsc/buttons/remove_button.png")));
    connect(delete_me_button , &QToolButton::clicked , this , &MovementBox::kill_me);

    duration_spinBox = new QSpinBox(this);
    duration_spinBox->setRange(0 , std::numeric_limits<int>::max());
    duration_spinBox->setSuffix(" ms");
    duration_spinBox->setValue(100);
    connect(duration_spinBox , QOverload<int>::of(&QSpinBox::valueChanged), this , &MovementBox::updateMovement);

    direction_comboBox = new QComboBox(this);
    direction_comboBox->addItem(tr("None") , static_cast<int>(Direction::NONE));
    direction_comboBox->addItem(tr("Forward") , static_cast<int>(Direction::FORWARD));
    direction_comboBox->addItem(tr("Backwards") , static_cast<int>(Direction::BARCKWARDS));
    direction_comboBox->addItem(tr("Right") , static_cast<int>(Direction::RIGHT));
    direction_comboBox->addItem(tr("Left") , static_cast<int>(Direction::LEFT));
    connect(direction_comboBox , QOverload<int>::of(&QComboBox::currentIndexChanged), this , &MovementBox::updateMovement);

    x_spinBox = new QSpinBox(this);
    x_spinBox->setRange(std::numeric_limits<int>::min() , std::numeric_limits<int>::max());
    x_spinBox->setSingleStep(1);
    x_spinBox->setSuffix(" mm");
    connect(x_spinBox , QOverload<int>::of(&QSpinBox::valueChanged), this , &MovementBox::updateMovement);

    y_spinBox = new QSpinBox(this);
    y_spinBox->setRange(std::numeric_limits<int>::min() , std::numeric_limits<int>::max());
    y_spinBox->setSingleStep(1);
    y_spinBox->setSuffix(" mm");
    connect(y_spinBox , QOverload<int>::of(&QSpinBox::valueChanged), this , &MovementBox::updateMovement);

    z_spinBox = new QSpinBox(this);
    z_spinBox->setRange(std::numeric_limits<int>::min() , std::numeric_limits<int>::max());
    z_spinBox->setSingleStep(1);
    z_spinBox->setSuffix(" mm");
    connect(z_spinBox , QOverload<int>::of(&QSpinBox::valueChanged), this , &MovementBox::updateMovement);


    main_layout->addRow(new QLabel(tr("Direction:") , this) , direction_comboBox);
    main_layout->addRow(new QLabel(tr("X:") , this) , x_spinBox);
    main_layout->addRow(new QLabel(tr("Y:") , this) , y_spinBox);
    main_layout->addRow(new QLabel(tr("Z:") , this) , z_spinBox);
    main_layout->addRow(new QLabel(tr("Duration:") , this) , duration_spinBox);
    main_layout->addRow(new QLabel(tr("Delete movement:") , this) , delete_me_button);


    setLayout(main_layout);



}

/**************************************************************/

Movement *MovementBox::getMovement(){return movement;}

void MovementBox::setMovement(Movement* _movement){movement = _movement; updateFields();}

void MovementBox::toDirection(bool with_reset, bool movementisPrioritary){

    QFormLayout *main_layout = dynamic_cast<QFormLayout*>(layout());

    main_layout->itemAt(0, QFormLayout::LabelRole)->widget()->show();
    main_layout->itemAt(0 , QFormLayout::FieldRole)->widget()->show();
    direction_comboBox->setEnabled(true);

    main_layout->itemAt(1, QFormLayout::LabelRole)->widget()->hide();
    main_layout->itemAt(1 , QFormLayout::FieldRole)->widget()->hide();
    x_spinBox->setEnabled(false); 

    main_layout->itemAt(2 , QFormLayout::LabelRole)->widget()->hide();
    main_layout->itemAt(2, QFormLayout::FieldRole)->widget()->hide();
    y_spinBox->setEnabled(false);

    main_layout->itemAt(3 , QFormLayout::LabelRole)->widget()->hide();
    main_layout->itemAt(3 , QFormLayout::FieldRole)->widget()->hide();
    z_spinBox->setEnabled(false);


    movement->setMode(MovementMode::DIRECTION , false);

    if(with_reset){
        direction_comboBox->setCurrentIndex(direction_comboBox->findData(static_cast<int>(Direction::NONE)));
        x_spinBox->setValue(0);
        y_spinBox->setValue(0);
        z_spinBox->setValue(0);
    }

    if(movementisPrioritary){
        updateFields();
    }
    else{
        updateMovement();
        updateFields();

    }

}

void MovementBox::toCoordinates(bool with_reset, bool updateMov){

    QFormLayout *main_layout = dynamic_cast<QFormLayout*>(layout());

    main_layout->itemAt(0 , QFormLayout::LabelRole)->widget()->hide();
    main_layout->itemAt(0 , QFormLayout::FieldRole)->widget()->hide();
    direction_comboBox->setEnabled(false);

    main_layout->itemAt(1 , QFormLayout::LabelRole)->widget()->show();
    main_layout->itemAt(1 , QFormLayout::FieldRole)->widget()->show();
    x_spinBox->setEnabled(true);

    main_layout->itemAt(2 , QFormLayout::LabelRole)->widget()->show();
    main_layout->itemAt(2 , QFormLayout::FieldRole)->widget()->show();
    y_spinBox->setEnabled(true);

    main_layout->itemAt(3 , QFormLayout::LabelRole)->widget()->show();
    main_layout->itemAt(3 , QFormLayout::FieldRole)->widget()->show();
    z_spinBox->setEnabled(true);

    movement->setMode(MovementMode::COORDINATES , false);

    if(with_reset){
        direction_comboBox->setCurrentIndex(direction_comboBox->findData(static_cast<int>(Direction::NONE)));
        x_spinBox->setValue(0);
        y_spinBox->setValue(0);
        z_spinBox->setValue(0);
    }

    if(updateMov){updateFields();}
    else{
        updateMovement();
        updateFields();
    }



}

/**************************************************************/

void MovementBox::updateMovement(){

    const int index = sequenceBox->getSequence()->getMovements().indexOf(movement);
    movement->setDuration(duration_spinBox->value());

    movement->setDirection(static_cast<Direction>(direction_comboBox->currentData().toInt()));

    movement->setX(x_spinBox->value());
    movement->setY(y_spinBox->value());
    movement->setZ(z_spinBox->value());

    sequenceBox->getSequence()->replaceMovement(index, movement);
    sequenceBox->getMainWindow()->updateInfo();

    sequenceBox->getMainWindow()->updateInfo();
}

void MovementBox::updateFields(){

    //Disable movement update while updating fields from movement data
    disconnect(duration_spinBox , QOverload<int>::of(&QSpinBox::valueChanged), this , &MovementBox::updateMovement);
    disconnect(direction_comboBox , QOverload<int>::of(&QComboBox::currentIndexChanged), this , &MovementBox::updateMovement);
    disconnect(x_spinBox , QOverload<int>::of(&QSpinBox::valueChanged), this , &MovementBox::updateMovement);
    disconnect(y_spinBox , QOverload<int>::of(&QSpinBox::valueChanged), this , &MovementBox::updateMovement);
    disconnect(z_spinBox , QOverload<int>::of(&QSpinBox::valueChanged), this , &MovementBox::updateMovement);

    duration_spinBox->setValue(movement->getDuration());

    direction_comboBox->setCurrentIndex(direction_comboBox->findData(static_cast<int>(movement->getDuration())));

    x_spinBox->setValue(movement->getX());
    y_spinBox->setValue(movement->getY());
    z_spinBox->setValue(movement->getZ());

    sequenceBox->getMainWindow()->updateInfo();

    connect(duration_spinBox , QOverload<int>::of(&QSpinBox::valueChanged), this , &MovementBox::updateMovement);
    connect(direction_comboBox , QOverload<int>::of(&QComboBox::currentIndexChanged), this , &MovementBox::updateMovement);
    connect(x_spinBox , QOverload<int>::of(&QSpinBox::valueChanged), this , &MovementBox::updateMovement);
    connect(y_spinBox , QOverload<int>::of(&QSpinBox::valueChanged), this , &MovementBox::updateMovement);
    connect(z_spinBox , QOverload<int>::of(&QSpinBox::valueChanged), this , &MovementBox::updateMovement);

}

void MovementBox::kill_me(){

    sequenceBox->removeMovement(this);
    delete this;

}
