#ifndef SEQUENCEBOX_H
#define SEQUENCEBOX_H

#include "sequence.h"
#include "movementbox.h"

#include <QGroupBox>
#include <QScrollArea>
class MainWindow;

class SequenceBox : public QGroupBox{

    friend class MainWindow;
    friend class MovementBox;

                Q_OBJECT

public:

    explicit SequenceBox(MainWindow* = nullptr);
    ~SequenceBox();

    void setSequence(Sequence *);

    QScrollArea* getScroll();

    MainWindow* getMainWindow();
    Sequence* getSequence();
    QVector<MovementBox*> getBoxes();

protected:

    void addMovement(MovementBox *);
    void addBox(MovementBox *);
    void removeMovement(MovementBox*);

private:

    void load_sequence(bool withReset);
    void reset();

    MainWindow *mainWindow;

    QScrollArea *scroll;
    QGroupBox *scroll_contents;

    Sequence* sequence;
    QVector<MovementBox*> boxes;
};

#endif // SEQUENCEBOX_H
