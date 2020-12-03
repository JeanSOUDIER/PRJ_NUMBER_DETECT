#include "camerawidget.h"

#include "mainwindow.h"

CameraWidget::CameraWidget(MainWindow *parent) : QWidget(parent){

    mainWindow = parent;

    QVBoxLayout *main_layout = new QVBoxLayout(this);

        /*--------------------------------------*/

    QGroupBox *image_box = new QGroupBox(this);
        QVBoxLayout *image_box_layout = new QVBoxLayout(image_box);

        image_label = new QLabel("",image_box);
        image_label->setAlignment(Qt::AlignCenter);

        image_box_layout->setAlignment(Qt::AlignCenter);
        image_box_layout->addWidget(image_label);

        /*--------------------------------------*/

    QGroupBox *bot_box = new QGroupBox(this);
        QHBoxLayout *bot_box_layout = new QHBoxLayout(bot_box);

        photo_button = new QPushButton(tr("Take a photo") , bot_box);
        photo_button->setShortcut(QKeySequence(Qt::Key_Return));
        connect(photo_button , &QPushButton::clicked , this , &CameraWidget::on_photo_button);

        autosend_cb = new QCheckBox(tr("Automatically send to the robot") , bot_box);
        autosend_cb->setChecked(true);
        connect(autosend_cb , &QCheckBox::stateChanged , this , &CameraWidget::on_autosend_cb);


        //bot_box_layout->setAlignment(Qt::AlignCenter);
        bot_box_layout->addWidget(new QLabel(this));
        bot_box_layout->addWidget(photo_button);
        bot_box_layout->addWidget(autosend_cb);
        //bot_box_layout->setAlignment(bot_box_layout->chi, Qt::AlignLeft);
        bot_box_layout->setAlignment(photo_button , Qt::AlignCenter);
        bot_box_layout->setAlignment(autosend_cb , Qt::AlignLeft);

        /*--------------------------------------*/

    main_layout->addWidget(image_box);
    main_layout->addWidget(bot_box);

    camera = new CameraHandler();

    refreshTimer = new QTimer(this);
    refreshTimer->setInterval(15); //ms
    refreshTimer->setSingleShot(false);
    refreshTimer->setTimerType(Qt::TimerType::PreciseTimer);
    connect(refreshTimer , &QTimer::timeout , this , [this]{takePhoto();});
}

CameraWidget::~CameraWidget(){

    refreshTimer->stop();
    delete refreshTimer;

    delete camera;
    delete layout();

}

/************************************************************************************/

void CameraWidget::setPhoto(const QImage &photo){
    setPhoto(QPixmap::fromImage(photo));
}

void CameraWidget::setPhoto(const QPixmap &photo){
    image_label->setPixmap(photo);
}

cv::Mat CameraWidget::takePhoto(){

    const cv::Mat result = camera->takePhoto();

    const int old_width = mainWindow->width();
    const int old_height = mainWindow->height();

    setPhoto(
    UtilityOCV::toQPixmap(result , QImage::Format_RGB888).scaled(int(0.95*width()) , int(0.8*height()) , Qt::KeepAspectRatio)
    );

    mainWindow->resize(old_width , old_height);

//    setPhoto(
//    UtilityOCV::toQPixmap(result, QImage::Format_RGB888)
//    );

    return result;
}
/************************************************************************************/


QTimer* CameraWidget::timer(){return refreshTimer;}

QLabel* CameraWidget::photo(){return image_label;}

void CameraWidget::on_photo_button(){

    refreshTimer->stop();
    mainWindow->setState(MainWindow::STATE::RESULTS_DISPLAY);
    mainWindow->process(takePhoto());

//    if(autosend_cb->isChecked()){mainWindow->resultsWidget()->send();}






}

void CameraWidget::on_autosend_cb(){

    if(autosend_cb->isChecked()){
        mainWindow->resultsWidget()->setState(ResultsWidget::STATE::RETURN_ST);
    }
    else{
        mainWindow->resultsWidget()->setState(ResultsWidget::STATE::SEND_ST);
    }


}
