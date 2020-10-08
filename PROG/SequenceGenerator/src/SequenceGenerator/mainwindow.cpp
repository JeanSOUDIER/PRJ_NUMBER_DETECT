#include "mainwindow.h"

//#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){

     QScreen *screen = QGuiApplication::primaryScreen();
     QRect  screenGeometry = screen->geometry();
     const int height = int(screenGeometry.height()*0.6);
     const int width = int(screenGeometry.width()*0.6);

    setup_menu();
    setup_splitter();
    setup_tray();
    setup_windows();

    last_path = QDir::currentPath();

    setGeometry(int(screenGeometry.width()*0.2),int(screenGeometry.height()*0.2),width, height);
    setWindowTitle(QString::fromUtf8("Sequence generator"));
    QWidget::setWindowFlags(Qt::Window);


}

MainWindow::~MainWindow(){

    delete trayIcon;
    delete aboutWindow;
    delete menuBar();
    delete layout();

}
/**************************************************************/

void MainWindow::resizeEvent(QResizeEvent *event){

    QMainWindow::resizeEvent(event);
    sequence_box->scaleScroll();
    event->accept();

}

/**************************************************************/

void MainWindow::setup_menu(){


    file_menu = menuBar()->addMenu(tr("&File"));

        open_action = new QAction(tr("&Open"), file_menu);
        open_action->setShortcut(QKeySequence("Ctrl+O"));
        connect(open_action, &QAction::triggered, this, &MainWindow::open);

        save_action = new QAction(tr("&Save"), file_menu);
        save_action->setShortcut(QKeySequence("Ctrl+S"));
        connect(save_action, &QAction::triggered, this, &MainWindow::save);

        close_action = new QAction(tr("&Close"), file_menu);
        close_action->setShortcut(QKeySequence("Ctrl+K"));
        connect(close_action, &QAction::triggered, this, &MainWindow::close);

        file_menu->addAction(open_action);
        file_menu->addSeparator();
        file_menu->addAction(save_action);
        file_menu->addSeparator();
        file_menu->addAction(close_action);



    help_menu = menuBar()->addMenu(tr("&Help"));

        about_action = new QAction(tr("&About") , help_menu);
        connect(about_action, &QAction::triggered, this, &MainWindow::about);


        help_menu->addAction(about_action);

}

void MainWindow::setup_splitter(){


    mainSplitter = new QSplitter(this);
    mainSplitter->setOrientation(Qt::Horizontal);

    QGroupBox *input_box = new QGroupBox(mainSplitter);
        QHBoxLayout *input_box_layout = new QHBoxLayout(input_box);

        QTabWidget *tabs_input = new QTabWidget(input_box);

            QGroupBox *sequence_input_box = new QGroupBox(tabs_input);
                QFormLayout *sequence_input_box_layout = new QFormLayout(sequence_input_box);

                direction_mode_rb = new QRadioButton(tr("Direction mode") , sequence_input_box);
                connect(direction_mode_rb, &QRadioButton::clicked , this , &MainWindow::updateSequenceMode);

                coordinates_mode_rb = new QRadioButton(tr("Coordinate mode") , sequence_input_box);
                connect(coordinates_mode_rb, &QRadioButton::clicked , this , &MainWindow::updateSequenceMode);

                coordinates_mode_rb->setChecked(true);

                QToolButton *addMovement_button = new QToolButton(sequence_input_box);
                addMovement_button->setToolTip(tr("Add a new movement to the sequence"));
                addMovement_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
                addMovement_button->setIcon(QIcon(QPixmap(":/buttons/rsc/buttons/add_button.png")));
                connect(addMovement_button , &QToolButton::clicked , this , &MainWindow::addMovement);

                sequence_box = new SequenceBox(this);

                QPushButton *generate_button = new QPushButton(tr("Generate"),tabs_input);
                connect(generate_button , &QPushButton::clicked , this , &MainWindow::generate);

                sequence_input_box_layout->addRow(direction_mode_rb, coordinates_mode_rb);
                sequence_input_box_layout->addRow(new QLabel(tr("Add a new movement"),sequence_input_box) , addMovement_button);
                sequence_input_box_layout->addRow(sequence_box);
                sequence_input_box_layout->addRow(generate_button);


                                /*+++++++++++++*/

                preview_parameter_box = new QGroupBox(tabs_input);
                QFormLayout *preview_parameter_box_layout = new QFormLayout(preview_parameter_box);

                   speed_spinbox = new QSpinBox(preview_parameter_box);
                   speed_spinbox->setRange(0 , std::numeric_limits<int>::max());
                   speed_spinbox->setSingleStep(1);
                   speed_spinbox->setSuffix(" m.s^-1");
                   connect(speed_spinbox , QOverload<int>::of(&QSpinBox::valueChanged) , this , &MainWindow::updateInfo);

                   begin_x_spinbox = new QSpinBox(preview_parameter_box);
                   begin_x_spinbox->setRange(std::numeric_limits<int>::min() , std::numeric_limits<int>::max());
                   begin_x_spinbox->setSingleStep(1);
                   begin_x_spinbox->setSuffix(" mm");
                   connect(begin_x_spinbox , QOverload<int>::of(&QSpinBox::valueChanged) , this , &MainWindow::updateInfo);

                   begin_y_spinbox = new QSpinBox(preview_parameter_box);
                   begin_y_spinbox->setRange(std::numeric_limits<int>::min() , std::numeric_limits<int>::max());
                   begin_y_spinbox->setSingleStep(1);
                   begin_y_spinbox->setSuffix(" mm");
                   connect(begin_y_spinbox , QOverload<int>::of(&QSpinBox::valueChanged) , this , &MainWindow::updateInfo);

                   begin_z_spinbox = new QSpinBox(preview_parameter_box);
                   begin_z_spinbox->setRange(std::numeric_limits<int>::min() , std::numeric_limits<int>::max());
                   begin_z_spinbox->setSingleStep(1);
                   begin_z_spinbox->setSuffix(" mm");
                   connect(begin_z_spinbox , QOverload<int>::of(&QSpinBox::valueChanged) , this , &MainWindow::updateInfo);

                   item_size_slider = new QSlider(preview_parameter_box);
                   item_size_slider->setMaximum(100);
                   item_size_slider->setMinimum(0);
                   item_size_slider->setOrientation(Qt::Horizontal);
                   item_size_slider->setValue(100);
                   connect(item_size_slider , &QSlider::valueChanged , this , &MainWindow::on_slider_update);

                   QLabel *parameters_explain_label = new QLabel(preview_parameter_box);
                   parameters_explain_label->setTextFormat(Qt::RichText);
                   parameters_explain_label->setText("<b>"+ tr("N.B.:") + "</b>" + tr(" This data is not saved in the exported file.\n"));

                   preview_parameter_box_layout->addRow(new QLabel(tr("Speed (absolute): "), preview_parameter_box) , speed_spinbox);
                   preview_parameter_box_layout->addRow(new QLabel(tr("X: "), preview_parameter_box) , begin_x_spinbox);
                   preview_parameter_box_layout->addRow(new QLabel(tr("Y: "), preview_parameter_box) , begin_y_spinbox);
                   preview_parameter_box_layout->addRow(new QLabel(tr("Z: "), preview_parameter_box) , begin_z_spinbox);
                   preview_parameter_box_layout->addRow(parameters_explain_label);
                   preview_parameter_box_layout->addRow(new QLabel(tr("Item size: "), preview_parameter_box) , item_size_slider);

                preview_parameter_box->setLayout(preview_parameter_box_layout);



                                        /*+++++++++++++*/

            QGroupBox *file_box = new QGroupBox(tabs_input);
                QFormLayout *file_box_layout = new QFormLayout(file_box);

            format_combox = new QComboBox(file_box);
            format_combox->addItem("XML" , static_cast<int>(Format::XML));
            format_combox->addItem("CSV" , static_cast<int>(Format::CSV));
            format_combox->addItem(tr("Both") , static_cast<int>(Format::ALL));

            file_name_field = new FileNameField(file_box);
            file_name_field->setPlaceholderText(tr("Enter file name here."));
            file_name_field->setText("sequence");

            path_field = new QLineEdit(file_box);
            path_field->setText("C:/");
            path_field->setPlaceholderText(tr("Path to the save folder"));
            path_field->setEnabled(false);

            QPushButton *browse_button = new QPushButton(tr("Browse") , file_box);
            connect(browse_button, &QPushButton::clicked , this , &MainWindow::browse);

            file_box_layout->addRow(new QLabel(tr("File name: "), file_box) , file_name_field);
            file_box_layout->addRow(new QLabel(tr("Format: "), file_box) , format_combox);

            file_box_layout->addRow(path_field , browse_button);


                                        /*+++++++++++++*/


            tabs_input->addTab(sequence_input_box , tr("Sequence"));
            tabs_input->addTab(preview_parameter_box , tr("Preview parameters"));
            tabs_input->addTab(file_box , tr("File"));


    input_box_layout->addWidget(tabs_input);
    input_box->setLayout(input_box_layout);


                            /*-------------------------------------*/

    QGroupBox *preview_box = new QGroupBox(mainSplitter);
        QVBoxLayout *preview_box_layout = new QVBoxLayout(preview_box);

        QTabWidget *tabs_preview = new QTabWidget(preview_box);


                             /*+++++++++++++*/

            QGroupBox *render_box = new QGroupBox(tabs_preview);
            QVBoxLayout *render_box_layout = new QVBoxLayout(render_box);


            view = new View(this);

            view->addEntity(begin_x_spinbox->value() , begin_y_spinbox->value() , begin_z_spinbox->value() , 0);
            view->editEntityColour(0x9D1C00 , 0);

            preview_disabled_label = new QLabel(tr("Preview is disabled for direction mode"),render_box);
            preview_disabled_label->hide();

            render_box_layout->addWidget(view , 1);
            render_box_layout->addWidget(preview_disabled_label);
            render_box_layout->setAlignment(Qt::AlignCenter);
            render_box->setLayout(render_box_layout);




                             /*+++++++++++++*/

            QGroupBox *preview_info_box = new QGroupBox(tabs_preview);
            QFormLayout *preview_info_box_layout = new QFormLayout(preview_info_box);

            total_time_label = new QLabel(preview_info_box);
            total_time_label->setTextFormat(Qt::RichText);
            total_time_label->setText("0<b> ms</b>");

            total_distance_label = new QLabel(preview_info_box);
            total_distance_label->setTextFormat(Qt::RichText);
            total_distance_label->setText("0<b> mm</b>");

            preview_info_box_layout->addRow(new QLabel(tr("Total time: ") , preview_info_box) , total_time_label);
            preview_info_box_layout->addRow(new QLabel(tr("Total distance: ") , preview_info_box) , total_distance_label);
            preview_info_box->setLayout(preview_info_box_layout);

                             /*+++++++++++++*/
            tabs_preview->addTab(render_box , tr("Preview"));
            tabs_preview->addTab(preview_info_box , tr("Drawing information"));


            QPushButton *goToBeginning_button = new QPushButton(tr("Go to beginning") , preview_box);
            connect(goToBeginning_button , &QPushButton::clicked , this , &MainWindow::goToBeginning);

            preview_button = new QPushButton(tr("Toggle display") , preview_box);
            preview_button->setCheckable(true);
            preview_button->setChecked(true);
            connect(preview_button , &QPushButton::clicked , this , &MainWindow::preview);

            preview_box_layout->addWidget(tabs_preview);
            preview_box_layout->addWidget(goToBeginning_button);
            preview_box_layout->addWidget(preview_button);

                            /*-------------------------------------*/


    mainSplitter->addWidget(input_box);
    mainSplitter->addWidget(preview_box);

    setCentralWidget(mainSplitter);
    updateSequenceMode();


    sequence_input_box->setMinimumHeight(500);

}

void MainWindow::setup_tray(){

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icon/rsc/icon/icon.png"));
    trayIcon->setVisible(true);
}

void MainWindow::setup_windows(){


    aboutWindow = new AboutWindow(this);
    aboutWindow->hide();

    connect(aboutWindow , &QDialog::accepted , this , [this]{aboutWindow->hide();});

}

/**************************************************************/

void MainWindow::updateInfo(){

    uint64_t total_time = 0;
    double total_distance = 0;

    if(coordinates_mode_rb->isChecked()){

        if(!sequence_box->getSequence()->getMovements().isEmpty()){
            Movement *mov = sequence_box->getSequence()->getMovements().at(0);

            total_time += mov->getDuration();
            total_distance += qSqrt(
                                qPow(begin_x_spinbox->value() - mov->getX() , 2) +
                                qPow(begin_y_spinbox->value() - mov->getY() , 2) +
                                qPow(begin_z_spinbox->value() - mov->getZ() , 2)
                        );

            for(int index = 1 ; index < sequence_box->getSequence()->getMovements().size()-1 ; index ++){

                Movement *mov_1 = sequence_box->getSequence()->getMovements().at(index);
                Movement *mov_2 =  sequence_box->getSequence()->getMovements().at(index+1);

                total_time += mov_1->getDuration() + mov_2->getDuration();
                total_distance += qSqrt(
                                    qPow(mov_2->getX() - mov_1->getX() , 2) +
                                    qPow( mov_2->getY() - mov_1->getY() , 2) +
                                    qPow( mov_2->getZ() - mov_1->getZ() , 2)
                            );



            }

        }


        view->moveEntity(-begin_x_spinbox->value() , begin_z_spinbox->value() , -begin_y_spinbox->value() , 0); //Axis of the robot and of the view are not the same

    }else{
        if(!sequence_box->getSequence()->getMovements().isEmpty()){

            for(int index = 0 ; index < sequence_box->getSequence()->getMovements().size() ; index ++){

                Movement *mov = sequence_box->getSequence()->getMovements().at(index);

                total_time += mov->getDuration();
                total_distance += mov->getDuration()*speed_spinbox->value();

            }

        }

        view->moveEntity(0 , 0, 0 , 0);




    }




    total_time_label->setText(QString::number(total_time) + "<b> ms</b>");
    total_distance_label->setText(QString::number(total_distance) + "<b> mm</b>");


}


void MainWindow::moveEntity(Movement *mov , const unsigned int index){view->moveEntity(mov , index);}

void MainWindow::deleteEntity(const unsigned index){view->deleteEntity(index);}

/**************************************************************/

void MainWindow::open(){

    const QString filepath = QFileDialog::getOpenFileName(this,tr("Select a file"), last_path, tr("XML Files , CSV files (*.xml , *.csv)"));

    QFileInfo f (filepath);
    last_path = f.path();

    SequenceReader reader(filepath);
    sequence_box->setSequence(reader.readSequence());

    view->clear();
    for(int index = 0 ; index < sequence_box->getBoxes().size() ; index++){
        view->addEntity(sequence_box->getSequence()->getMovements().at(index) , index);

    }

    sequence_box->scaleScroll();

    on_slider_update();


}

void MainWindow::close(){

    if(sequence_box->getBoxes().size()>0){

        QMessageBox::StandardButton ask = QMessageBox::question(this , tr("Closing the current sequence") ,tr("Closing the current sequence.\nWould you like to save it ?") , QMessageBox::Yes | QMessageBox::No);
        if(ask == QMessageBox::Yes){save();}

    }

}

void MainWindow::save(){

    if(sequence_box->getBoxes().size()>0){
        if(generate()){trayIcon->showMessage("Sequence Generator" , tr("Save completed.") , trayIcon->icon() , 3000);}

    }

}

void MainWindow::browse(){
     const QString folder = QFileDialog::getExistingDirectory(this, tr("Select a directory"),path_field->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks).toUtf8();
     if(!folder.isEmpty()){path_field->setText(folder);}
}


void MainWindow::about(){aboutWindow->show();}

void MainWindow::addMovement(){

    MovementBox *mov_box = new MovementBox(sequence_box);

    view->addEntity(mov_box->getMovement() , sequence_box->getBoxes().indexOf(mov_box));

    if(direction_mode_rb->isChecked()){mov_box->toDirection(true , false);}
    else{mov_box->toCoordinates(true , false);}

    sequence_box->scaleScroll();
    on_slider_update();


}

void MainWindow::updateSequenceMode(){

    QFormLayout *parameters_layout = dynamic_cast<QFormLayout*>(preview_parameter_box->layout());


    if(direction_mode_rb->isChecked()){

        view->hide();
        view->hideAll();
        preview_button->setChecked(false);
        preview_button->hide();

        preview_disabled_label->show();

        parameters_layout->itemAt(0, QFormLayout::LabelRole)->widget()->show();
        parameters_layout->itemAt(0 , QFormLayout::FieldRole)->widget()->show();
        speed_spinbox->setEnabled(true);

        parameters_layout->itemAt(1, QFormLayout::LabelRole)->widget()->hide();
        parameters_layout->itemAt(1 , QFormLayout::FieldRole)->widget()->hide();
        begin_x_spinbox->setEnabled(false);

        parameters_layout->itemAt(2, QFormLayout::LabelRole)->widget()->hide();
        parameters_layout->itemAt(2 , QFormLayout::FieldRole)->widget()->hide();
        begin_y_spinbox->setEnabled(false);

        parameters_layout->itemAt(3, QFormLayout::LabelRole)->widget()->hide();
        parameters_layout->itemAt(3 , QFormLayout::FieldRole)->widget()->hide();
        begin_z_spinbox->setEnabled(false);

        for(int index = 0 ; index < sequence_box->getSequence()->getMovements().size() ; index ++){sequence_box->getBoxes()[index]->toDirection(true , false);}


    }
    else{


        view->show();
        view->showAll();
        preview_button->setChecked(true);
        preview_button->show();

        preview_disabled_label->hide();

        parameters_layout->itemAt(0, QFormLayout::LabelRole)->widget()->hide();
        parameters_layout->itemAt(0 , QFormLayout::FieldRole)->widget()->hide();
        speed_spinbox->setEnabled(false);

        parameters_layout->itemAt(1, QFormLayout::LabelRole)->widget()->show();
        parameters_layout->itemAt(1 , QFormLayout::FieldRole)->widget()->show();
        begin_x_spinbox->setEnabled(true);

        parameters_layout->itemAt(2, QFormLayout::LabelRole)->widget()->show();
        parameters_layout->itemAt(2 , QFormLayout::FieldRole)->widget()->show();
        begin_y_spinbox->setEnabled(true);

        parameters_layout->itemAt(3, QFormLayout::LabelRole)->widget()->show();
        parameters_layout->itemAt(3 , QFormLayout::FieldRole)->widget()->show();
        begin_z_spinbox->setEnabled(true);

        for(int index = 0 ; index < sequence_box->getSequence()->getMovements().size() ; index ++){sequence_box->getBoxes()[index]->toCoordinates(true , false);}

    }

}

bool MainWindow::generate(){

    bool withOverride = false;

    const QString filename = path_field->text() + "/" + file_name_field->text();

    QFile file_xml(filename + ".xml");
    QFile file_csv(filename + ".csv");

    if(file_xml.exists() || file_csv.exists()){

        QMessageBox::StandardButton ask = QMessageBox::question(this , tr("Old file found") ,tr("An old version of the file was found.\nWould you like to replace it ?") , QMessageBox::Yes | QMessageBox::No);

        if(ask == QMessageBox::Yes){
            file_xml.remove();
            file_csv.remove();
            withOverride = true;
        }

    }
    else{withOverride = true;}

    if(withOverride){

        SequenceWriter writer(sequence_box->getSequence()->getMovements() , filename);
        if(writer.generate(static_cast<Format>(format_combox->currentData().toInt()))){

            last_path = path_field->text();

            QMessageBox::StandardButton info = QMessageBox::information(this , tr("Process ended successfully") ,tr("File successfully generated in\n") + path_field->text() + "/" + file_name_field->text() , QMessageBox::Ok);
            Q_UNUSED(info);
            return true;


        }
        else{

            QMessageBox::StandardButton info = QMessageBox::information(this , tr("Process failed") ,tr("Could not generate file in\n") + path_field->text() + "/" + file_name_field->text() , QMessageBox::Ok);
            Q_UNUSED(info);
            return false;
        }

    }

    return false;



}

void MainWindow::preview(){

    if(preview_button->isChecked()){view->showAll();}
    else{view->hideAll();}


}

void MainWindow::goToEntity(const unsigned int index){view->goToEntity(index);}

void MainWindow::goToBeginning(){view->goToEntity(0);}

void MainWindow::on_slider_update(){view->resizeAll(item_size_slider->value());}
