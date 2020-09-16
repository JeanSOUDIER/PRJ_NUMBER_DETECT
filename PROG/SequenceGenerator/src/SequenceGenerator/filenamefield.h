#ifndef FILENAMEFIELD_H
#define FILENAMEFIELD_H

#include <QKeyEvent>

#include <QLineEdit>


class FileNameField : public QLineEdit{

public:
    FileNameField(QWidget *parent = nullptr);
protected:

    void keyPressEvent(QKeyEvent *) override; //Prevents some characters to be chosen for a file name

};

#endif // FILENAMEFIELD_H
