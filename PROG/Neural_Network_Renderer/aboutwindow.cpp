#include "aboutwindow.h"

#include <QGroupBox>
#include <QGuiApplication>

#include <QImage>

#include <QLabel>

#include <QScreen>
#include <QScrollArea>

#include <QTextEdit>


#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>

#include <QDebug>


AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent){


    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    const int h = int(screenGeometry.height()*0.75);
    const int w = int(screenGeometry.width()*0.3);


    QFont bold_font;
    bold_font.setBold(true);

    QVBoxLayout *main_layout = new QVBoxLayout(this);

    /*-------------------------------------------------*/

    QGroupBox *topBox = new QGroupBox(this);
        QHBoxLayout *topBox_layout = new QHBoxLayout(topBox);

         /*++++++++++++++*/

        QLabel *logo_label = new QLabel(topBox);
        logo_label->setMaximumSize(int(w*0.5) , int(h*0.3));
        logo_label->setPixmap(QPixmap(":/img/rsc/img/logo.png").scaled(int(w*0.5), int(h*0.3) , Qt::KeepAspectRatio));

        /*++++++++++++++*/

        QGroupBox *dev_box = new QGroupBox(topBox);
            QFormLayout *dev_box_layout = new QFormLayout(dev_box);

            QLabel *dev_title = new QLabel(tr("Developpers:") , dev_box);
            dev_title->setFont(bold_font);

            QLabel *dev_content = new QLabel("DUHAMEL Erwan / SOUDIER Jean" , topBox);

            QLabel *dev_contact_title = new QLabel(tr("Contact:") , dev_box);
            dev_contact_title->setFont(bold_font);

            QLabel *dev_contact_content = new QLabel("" , topBox);
            dev_contact_content->setText("<a href=\"mailto:erwanduhamel@outlook.com\">erwanduhamel@outlook.com</a> <br>"
                                         "<a href=\"mailto:jean.soudier@insa-strasbourg.fr\">jean.soudier@insa-strasbourg.fr</a>");
            dev_contact_content->setTextFormat(Qt::RichText);
            dev_contact_content->setTextInteractionFlags(Qt::TextBrowserInteraction);
            dev_contact_content->setOpenExternalLinks(true);

            QLabel *copyright_title = new QLabel("Organism:" , topBox);
            copyright_title->setFont(bold_font);

            QLabel *copyright_content = new QLabel("INSA Strasbourg 2020-2021" , topBox);


            dev_box_layout->addRow(dev_title , dev_content);
            dev_box_layout->addRow(dev_contact_title , dev_contact_content);
            dev_box_layout->addRow(copyright_title , copyright_content);


        dev_box->setLayout(dev_box_layout);

        /*++++++++++++++*/

    topBox_layout->addWidget(logo_label);
    topBox_layout->addWidget(dev_box);

    topBox->setLayout(topBox_layout);

    /*-------------------------------------------------*/

    QScrollArea *licenses_scroll = new QScrollArea(this);
        QGroupBox *licenses_box = new QGroupBox (licenses_scroll);
            QFormLayout *licenses_box_layout = new QFormLayout(licenses_box);

            QLabel *about_title = new QLabel(tr("Using this software:") , licenses_box);
            about_title->setFont(bold_font);

            QLabel *about_desc = new QLabel(licenses_box);
            about_desc->setTextFormat(Qt::RichText);
            about_desc->setText(tr("This software is developped according to the Qt Open Source license and is thus completely open-source.\n"
                                   "Source code is available on demand.\n"));
            about_desc->setWordWrap(true);


            QLabel *desc_title = new QLabel(tr("Software description:") , licenses_box);
            desc_title->setFont(bold_font);

            QLabel *desc_contents = new QLabel(tr("This software is a GUI meant to interface a Nvidia Jetson Nano board equipped with a camera and a TurtleBot with an arm on it.\n"
                                                  "When a photo is taken, any number between 0 and 9 (inclusive) are detected, recognized by a neural network and sent to the robot, which will write them on a board.") , licenses_box);

            QLabel *licenses_title = new QLabel(tr("Licenses:") , licenses_box);
            licenses_title->setFont(bold_font);

            QTextEdit *GNU_license_field = new QTextEdit(licenses_box);
            GNU_license_field->setText(license_GNU_LGPL);
            GNU_license_field->setEnabled(true);
            GNU_license_field->setReadOnly(true);

            QTextEdit *BSD_3_license_field = new QTextEdit(licenses_box);
            BSD_3_license_field->setText(license_Qt_BSD_3);
            BSD_3_license_field->setEnabled(true);
            BSD_3_license_field->setReadOnly(true);



            licenses_box_layout->addRow(about_title);
            licenses_box_layout->addRow(about_desc);
            licenses_box_layout->addRow(new QLabel("" , licenses_box)); //Décalage
            licenses_box_layout->addRow(desc_title);
            licenses_box_layout->addRow(desc_contents);
            licenses_box_layout->addRow(new QLabel("" , licenses_box)); //Décalage
            licenses_box_layout->addRow(licenses_title);
            licenses_box_layout->addRow(new QLabel("" , licenses_box)); //Décalage
            licenses_box_layout->addRow(new QLabel("GNU - LGPL:" , licenses_box));
            licenses_box_layout->addRow(GNU_license_field);
            licenses_box_layout->addRow(new QLabel("Qt - BSD-3:" , licenses_box));
            licenses_box_layout->addRow(BSD_3_license_field);


        licenses_box->setLayout(licenses_box_layout);
        licenses_box->setObjectName("fieldsAndButtonsBox");


        licenses_scroll->setWidget(licenses_box);
        licenses_scroll->setWidgetResizable(true);



    main_layout->addWidget(topBox);
    main_layout->addWidget(licenses_scroll);


    //topBox->setFixedWidth(int(0.95*w));
    licenses_scroll->widget()->setFixedWidth(int(0.98*w));

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("About this software"));
    //setFixedWidth(w);
    resize(w , h);
    setLayout(main_layout);


}

AboutWindow::~AboutWindow(){qDeleteAll(children());}



void AboutWindow::closeEvent(QCloseEvent *event){

    hide();
    event->ignore();

}
