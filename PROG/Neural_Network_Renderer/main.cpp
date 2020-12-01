#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]){

    QApplication app(argc, argv);
    app.setStyle("fusion");

        MainWindow window;
        window.show();

    return app.exec();

}
