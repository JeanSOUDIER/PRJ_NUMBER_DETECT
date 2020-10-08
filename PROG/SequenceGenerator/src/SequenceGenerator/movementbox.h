#ifndef MOVEMENTBOX_H
#define MOVEMENTBOX_H

#include "global.h"

#include "movement.h"

#include <QComboBox>

#include <QGroupBox>

#include <QLabel>



#include <QSpinBox>

#include <QToolButton>

#include <QFormLayout>

class MainWindow;
class SequenceBox;

class MovementBox : public QGroupBox{

    friend class MainWindow;

                Q_OBJECT

public:

    MovementBox(SequenceBox *_sequenceBox, Movement *_movement, bool fromSequence);
    MovementBox(SequenceBox *_sequenceBox);
    ~MovementBox();

    void setMovement(Movement *_movement);

    Movement* getMovement();


protected:

    void updateFields();
    void toDirection(bool with_reset, bool movementisPrioritary);
    void toCoordinates(bool with_reset , bool movementisPrioritary);

private :

    bool isSetupCompleted;

    SequenceBox* sequenceBox;

    QComboBox *direction_comboBox;
    QSpinBox *x_spinBox;
    QSpinBox *y_spinBox;
    QSpinBox *z_spinBox;

    QSpinBox *duration_spinBox;

    QToolButton *delete_me_button;



    Movement *movement;




    void setup();

private slots:

    void updateMovement();
    void go_to_me();
    void kill_me();

};

#endif // MOVEMENTBOX_H
