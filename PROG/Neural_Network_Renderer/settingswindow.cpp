#include "settingswindow.h"

#include "mainwindow.h"

SettingsWindow::SettingsWindow(MainWindow *parent) : QDialog(parent){

    mainWindow = parent;


    QVBoxLayout *main_layout = new QVBoxLayout(this);

    /*--------------------------------------*/

    QGroupBox *widgets_box = new QGroupBox(this);
    QFormLayout *widgets_box_layout = new QFormLayout(widgets_box);


        padding_spin = new QSpinBox(widgets_box);
        padding_spin->setMinimum(0);
        padding_spin->setMaximum(std::numeric_limits<int>::max());
        padding_spin->setValue(11);
        padding_spin->setButtonSymbols(QAbstractSpinBox::PlusMinus);

                             /*++++++++++++++++++++*/

        min_size_spin = new QSpinBox(widgets_box);
        min_size_spin->setMinimum(0);
        min_size_spin->setMaximum(std::numeric_limits<int>::max()-1);
        min_size_spin->setValue(800);
        min_size_spin->setSuffix(" px");
        min_size_spin->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        connect(min_size_spin ,
                QOverload<int>::of(&QSpinBox::valueChanged) ,
                this ,
                [this]{max_size_spin->setMinimum(min_size_spin->value() + 1);});

                             /*++++++++++++++++++++*/

        max_size_spin = new QSpinBox(widgets_box);
        max_size_spin->setMinimum(min_size_spin->value() + 1);
        max_size_spin->setMaximum(std::numeric_limits<int>::max());
        max_size_spin->setValue(800*1000);
        max_size_spin->setSuffix(" px");
        max_size_spin->setButtonSymbols(QAbstractSpinBox::PlusMinus);

                             /*++++++++++++++++++++*/

        QGroupBox *threshold_box = new QGroupBox(widgets_box);
        QFormLayout *threshold_box_layout = new QFormLayout(threshold_box);

            threshold_slider = new QSlider(threshold_box);
            threshold_slider->setRange(0 , 255);
            threshold_slider->setValue(127);
            threshold_slider->setOrientation(Qt::Horizontal);
            connect(threshold_slider ,
                    QOverload<int>::of(&QSlider::valueChanged) ,
                    this ,
                    [this]{threshold_value->setText(QString::number(threshold_slider->value()));});

            threshold_value = new QLabel(threshold_slider);
            threshold_value->setText(QString::number(threshold_slider->value()));

            threshold_box_layout->addRow(threshold_slider , threshold_value);



        reset_button = new QPushButton(tr("Restore defaults") , widgets_box);
        connect(reset_button , &QPushButton::clicked , this , [this]{reset();});


        widgets_box_layout->addRow(new QLabel(tr("Padding: ") , widgets_box) , padding_spin);
        widgets_box_layout->addRow(new QLabel(tr("Minimum motif size: ") , widgets_box) , min_size_spin);
        widgets_box_layout->addRow(new QLabel(tr("Maximum motif size: ") , widgets_box) , max_size_spin);
        widgets_box_layout->addRow(new QLabel(tr("BW conversion threshold: ")), threshold_box);
        widgets_box_layout->addRow(reset_button);


    /*--------------------------------------*/

    QGroupBox *buttons_box = new QGroupBox(this);

    QHBoxLayout *button_box_layout = new QHBoxLayout(buttons_box);

    cancel_button = new QPushButton(tr("Cancel") , buttons_box);
    connect(cancel_button ,
            &QPushButton::clicked ,
            this ,
            [this]{reset(); hide();});

    ok_button = new QPushButton(tr("OK") , buttons_box);
    connect(ok_button ,
            &QPushButton::clicked ,
            this ,
            &SettingsWindow::on_ok_button);

    button_box_layout->addWidget(cancel_button);
    button_box_layout->addWidget(ok_button);
    button_box_layout->setAlignment(Qt::AlignRight);

/*--------------------------------------*/

    main_layout->addWidget(widgets_box);
    main_layout->addWidget(buttons_box);

    setLayout(main_layout);

}

SettingsWindow::~SettingsWindow(){
    delete layout();
}



void SettingsWindow::reset(){

    padding_spin->setValue(11);

    threshold_slider->setValue(127);

    min_size_spin->setValue(800);

    max_size_spin->setValue(1000*800);
    max_size_spin->setMinimum(min_size_spin->value() + 1);

}


void SettingsWindow::on_ok_button(){

    hide();

    mainWindow->setThreshold(threshold_slider->value());
    mainWindow->setMaximumFormSize(max_size_spin->value());
    mainWindow->setMinimumFormSize(min_size_spin->value());
    mainWindow->setPadding(padding_spin->value());

}




void SettingsWindow::closeEvent(QCloseEvent *event){
    reset();
    hide();
    event->ignore();
}


