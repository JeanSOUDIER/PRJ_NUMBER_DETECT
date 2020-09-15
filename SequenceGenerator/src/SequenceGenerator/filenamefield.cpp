#include "filenamefield.h"

FileNameField::FileNameField(QWidget *parent) : QLineEdit(parent){}


void FileNameField::keyPressEvent(QKeyEvent *event){


    if(event->key() == Qt::Key::Key_Backslash ||
            event->key() == Qt::Key::Key_Slash ||
            event->key() == Qt::Key::Key_Colon ||
            event->key() == Qt::Key::Key_Asterisk ||
            event->key() == Qt::Key::Key_Greater ||
            event->key() == Qt::Key::Key_Less ||
            event->key() == Qt::Key::Key_Bar ||
            event->key() == Qt::Key::Key_Question ||
            event->key() == Qt::Key::Key_QuoteDbl){event->ignore();}
    else{
        QLineEdit::keyPressEvent(event);
        event->accept();
    }

}
