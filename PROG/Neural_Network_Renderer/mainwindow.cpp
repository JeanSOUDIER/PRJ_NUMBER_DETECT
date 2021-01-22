#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){


    _threshold = 127;
    _min_form_size = 3000;
    _max_form_size = 800*1000;
    _padding = 11;

    /************************************/

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    const int height = int(screenGeometry.height()*0.6);
    const int width = int(screenGeometry.width()*0.6);

    /************************************/

    setup_widgets();
    setup_menu();
    setup_windows();

    /************************************/

    setup_neuralnetwork();

    /************************************/

    constexpr int ttyTHS1 = 40;

    _bluetooth_handler = new Bluetooth(ttyTHS1 , 9600);

    /************************************/

    setGeometry(int(screenGeometry.width()*0.2),int(screenGeometry.height()*0.2),width, height);
    setWindowTitle(QString::fromUtf8("Number detection projet [INSA]"));
    QWidget::setWindowFlags(Qt::Window);

    cameraWidget()->photo()->setMaximumHeight(int(0.6*screenGeometry.height()));
    cameraWidget()->timer()->start();


}

MainWindow::~MainWindow(){

    delete _bluetooth_handler;

    delete neural_network;

    delete about_window;
    delete settings_window;

    delete menuBar();
    delete layout();


}



/**********************************************/

void MainWindow::closeEvent(QCloseEvent *event){

    setState(MainWindow::STATE::CAMERA_DISPLAY);
    QMainWindow::closeEvent(event);
    event->accept();

}

/*********************************************/

void MainWindow::setup_widgets(){

    camera_widget = new CameraWidget(this);

    results_widget = new ResultsWidget(this);
    results_widget->hide();

    setCentralWidget(camera_widget);


}
void MainWindow::setup_windows(){

    about_window = new AboutWindow(this);
    about_window->hide();
    connect(about_window , &QDialog::accepted , this , [this]{about_window->hide();});

    settings_window = new SettingsWindow(this);
    settings_window->hide();
    connect(settings_window , &QDialog::accepted , this , [this]{settings_window->hide();});


}
void MainWindow::setup_menu(){

    system_menu = menuBar()->addMenu(tr("&System"));
        load_nn_ac = new QAction(tr("&Load neural network structure...") , system_menu);
        connect(load_nn_ac , &QAction::triggered , this , [this]{

            const std::string file_path = QFileDialog::getOpenFileName(this,tr("Select a file"), QDir::currentPath(), tr("XML Files (*.xml)")).toStdString();

            if(file_path.empty()){return;}

            if(neuralNetwork()->load(file_path)){
                QMessageBox::StandardButton info = QMessageBox::information(this , tr("Process ended successfully") , tr("Neural network was correctly initialized with the file.") , QMessageBox::Ok);
                Q_UNUSED(info);

            }else{
                QMessageBox::StandardButton info = QMessageBox::information(this , tr("Process failed") , tr("Could not initialize neural network with this file.\nPlease make sure it is accessible and in the correct format.") , QMessageBox::Ok);
                Q_UNUSED(info);
            }



        });

        exit_action = new QAction(tr("&Quit") , system_menu);
        exit_action->setShortcut(QKeySequence("Ctrl+W"));
        connect(exit_action , &QAction::triggered , this , &QMainWindow::close);

     system_menu->addAction(load_nn_ac);
     system_menu->addSeparator();
     system_menu->addAction(exit_action);



    /***********************/

    settings_menu = menuBar()->addMenu(tr("S&ettings"));
        open_settings_action = new QAction(tr("Open the settings &menu...") , settings_menu);
        connect(open_settings_action , &QAction::triggered , this , [this]{settings_window->show();});

    settings_menu->addAction(open_settings_action);


    /***********************/

    help_menu = menuBar()->addMenu(tr("&Help"));
        about_action = new QAction(tr("&About") , help_menu);

    help_menu->addAction(about_action);

    connect(about_action , &QAction::triggered , this , [this]{about_window->show();});


    menuBar()->setNativeMenuBar(false);

}
void MainWindow::setup_neuralnetwork(){


    neural_network = new Project_network(4);
    QMessageBox::StandardButton info = QMessageBox::information(this , tr("Neural network initialization") , tr("Please choose a MNN XML file to initiate the neural network.") , QMessageBox::Ok);
    Q_UNUSED(info)
    std::string file = QFileDialog::getOpenFileName(this,tr("Select a file"), QDir::currentPath(), tr("XML Files (*.xml)")).toStdString();
;
    while(!neural_network->load(file)){

        QMessageBox::StandardButton warning = QMessageBox::warning(this , tr("Neural network initialization") , tr("Invalid File. Please select another one.") , QMessageBox::Ok | QMessageBox::Cancel);

        switch (warning) {

            case(QMessageBox::Ok):{
            file = QFileDialog::getOpenFileName(this,tr("Select a file"), QDir::currentPath(), tr("XML Files (*.xml)")).toStdString();
            break;
            }

            case(QMessageBox::Cancel):{
            file = QString(QDir::currentPath() + "/default_nn/mnist_4_layers_v5_rough.xml").toStdString();
            QMessageBox::StandardButton info = QMessageBox::information(this ,
                                                                        tr("Neural network initialization") ,
                                                                        tr("Neural network was initialized with sample file ") + QString::fromStdString(file) + "." , QMessageBox::Ok);
            Q_UNUSED(info);
            break;
            }
            default:{break;}

        }
    }



    QMessageBox::StandardButton validation = QMessageBox::information(this , tr("Neural network initialization") , tr("Neural network was successfully initialized from the selected file.") , QMessageBox::Ok);
    Q_UNUSED(validation);

}

/*********************************************/

void MainWindow::setState(const MainWindow::STATE &state){


    switch(state){

        case(MainWindow::STATE::CAMERA_DISPLAY):{
            takeCentralWidget();
            setCentralWidget(camera_widget);
            results_widget->hide();
            camera_widget->show();
            exit_action->setEnabled(true);
            QWidget::setWindowFlags(Qt::Window);
            cameraWidget()->timer()->start();
            break;
        }

        case(MainWindow::STATE::RESULTS_DISPLAY):{
            takeCentralWidget();
            setCentralWidget(results_widget);
            camera_widget->hide();
            results_widget->show();
            exit_action->setEnabled(false);
            QWidget::setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
            break;
        }

        default:{break;}

    }

    show();
  //  menuBar()->show();
    QWidget::activateWindow();
}

CameraWidget* MainWindow::cameraWidget(){return camera_widget;}
ResultsWidget* MainWindow::resultsWidget(){return results_widget;}
Project_network* MainWindow::neuralNetwork(){return neural_network;}
Bluetooth* MainWindow::bluetooth_handler(){return _bluetooth_handler;}


void MainWindow::setPadding(const unsigned &padding){_padding = padding;}
void MainWindow::setMaximumFormSize(const uint64_t &max){_max_form_size = max;}
void MainWindow::setMinimumFormSize(const uint64_t &min){_min_form_size = min;}
void MainWindow::setThreshold(const unsigned &threshold){_threshold = threshold;}



/*********************************************/

void MainWindow::process(const cv::Mat &photo){

    if(photo.empty()){return;} //Safety



    const cv::Mat top_padding = cv::Mat::zeros(_padding , 28 , CV_8UC1);
    const cv::Mat side_padding = cv::Mat::zeros(28+_padding , _padding , CV_8UC1);

    const cv::Mat bot_padding = cv::Mat::zeros(_padding , 28+2*_padding , CV_8UC1);


    const cv::Mat gray_image = UtilityOCV::RGBtoGray(photo);
    const cv::Mat bw_image = UtilityOCV::threshold(gray_image , _threshold);

    cv::Mat detection_results_mat;

    std::vector<ste::Matrix<MNN::scalar>> pictures;
    pictures.reserve(9); //Arbitrary pre-allocation

    cv::Mat concatenate(28 , 28 , CV_8UC1);

    for(cv::Mat &image :  UtilityOCV::regionprops(photo , gray_image , bw_image ,
                                                        detection_results_mat ,
                                                        _min_form_size , _max_form_size,
                                                        UtilityOCV::SOURCE::ORIGINAL , UtilityOCV::SOURCE::GRAY ,
                                                        UtilityOCV::SORT::X ,
                                                        false)){



        cv::Mat cvMat = UtilityOCV::resize(UtilityOCV::invert(image) , 28 , 28).clone()/255;

        cv::vconcat(cvMat , top_padding , cvMat);
        cv::hconcat(cvMat , side_padding , cvMat);
        cv::hconcat(side_padding , cvMat , cvMat);
        cv::vconcat(bot_padding , cvMat , cvMat);

        const cv::Mat cv_data = UtilityOCV::resize(cvMat , 28 , 28);

#ifdef DOUBLE_PRECISION
        const ste::Matrix<MNN::scalar> data =  UtilityOCV::toSteMatrix_D(cv_data).reshape(784 , 1);
#else
        const ste::Matrix<MNN::scalar> data =  UtilityOCV::toSteMatrix_F(cv_data).reshape(784 , 1);
#endif

     pictures.push_back(data);

     cv::hconcat(concatenate , cv_data*255, concatenate);

    }

    concatenate = concatenate.colRange(28 , concatenate.cols); //Remove initial pre-allocation
                /*----------------------------------------------*/

    resultsWidget()->setSplitPhotos(
                UtilityOCV::toQPixmap(concatenate , QImage::Format_Grayscale8).scaled(int(0.3*width()) , int(0.45*height()) , Qt::KeepAspectRatio)
                );



    resultsWidget()->setBWImage(
                UtilityOCV::toQPixmap(
                    UtilityOCV::invert(bw_image), QImage::Format_Grayscale8).scaled(int(0.3*width()) , int(0.45*height()) , Qt::KeepAspectRatio)
                );


    resultsWidget()->setDetection(
                UtilityOCV::toQPixmap(detection_results_mat , QImage::Format_RGB888).scaled(int(0.6*width()) , int(0.45*height()) , Qt::KeepAspectRatio)
                );

                /*----------------------------------------------*/


    std::vector<long long> predictions;
    predictions.reserve(pictures.size());

    std::vector<MNN::scalar> probabilities;
    probabilities.reserve(pictures.size());

    for(const ste::Matrix<MNN::scalar> &image : pictures){

        const ste::Matrix<MNN::scalar> out = neuralNetwork()->output(image);

        const MNN::scalar prob = out.max();
        probabilities.push_back(prob);

        predictions.push_back(Utility::indexOf(out.toVector1D() , prob));
    }


    resultsWidget()->numbers() = predictions;
    resultsWidget()->probabilities() = probabilities;
    resultsWidget()->updateNumbers();

}


