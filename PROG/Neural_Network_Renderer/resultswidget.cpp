#include "resultswidget.h"

#include "mainwindow.h"

ResultsWidget::ResultsWidget(MainWindow *parent) : QWidget(parent){

    mainWindow = parent;

    QHBoxLayout *main_layout = new QHBoxLayout(this);

    /*--------------------------------------*/


    QSplitter *main_splitter = new QSplitter(this);
    main_splitter->setOrientation(Qt::Horizontal);

    /*--------------------------------------*/

    QSplitter *images_splitter = new QSplitter(main_splitter);
    images_splitter->setOrientation(Qt::Vertical);

        QSplitter *original_histogram_splitter = new QSplitter(images_splitter);
            QGroupBox *original_gb = new QGroupBox(tr("Individual photos:") , original_histogram_splitter);
            QHBoxLayout *origial_gb_layout = new QHBoxLayout(original_gb);
                split_images = new QLabel("" , original_gb);
                split_images->setAlignment(Qt::AlignCenter);
            origial_gb_layout->addWidget(split_images);

            QGroupBox *histogram_gb = new QGroupBox(tr("Thresholded BW photo:") , original_histogram_splitter);
            QHBoxLayout *histogram_gb_layout = new QHBoxLayout(histogram_gb);
                bw_image = new QLabel("" , histogram_gb);
                bw_image->setAlignment(Qt::AlignCenter);
            histogram_gb_layout->addWidget(bw_image);

            original_histogram_splitter->addWidget(original_gb);
            original_histogram_splitter->addWidget(histogram_gb);


                /*+++++++++++++++++++++++++*/

        QGroupBox *detection_gb = new QGroupBox(tr("Detection results:") , images_splitter);
        QHBoxLayout *detection_gb_layout = new QHBoxLayout(detection_gb);
            detection_image = new QLabel("" , images_splitter);
            detection_image->setAlignment(Qt::AlignCenter);
        detection_gb_layout->addWidget(detection_image);

                /*+++++++++++++++++++++++++*/

        images_splitter->addWidget(original_histogram_splitter);
        images_splitter->addWidget(detection_gb);

    /*--------------------------------------*/

    QGroupBox *numbers_box = new QGroupBox(tr("Neural network decisions:") , main_splitter);
        QVBoxLayout *numbers_box_layout = new QVBoxLayout(numbers_box);

        number_scroll = new NumberScroll(main_splitter);

        cancel_button = new QPushButton(tr("Cancel") , numbers_box);
        connect(cancel_button , &QPushButton::clicked , this , &ResultsWidget::on_return);
        cancel_button->hide();

        send_return_button = new QPushButton(tr("Return") , numbers_box);
        send_return_button->setShortcut(QKeySequence(Qt::Key_Backspace));

        connect(send_return_button , &QPushButton::clicked , this , &ResultsWidget::on_return);

        numbers_box_layout->addWidget(number_scroll);
        numbers_box_layout->addWidget(cancel_button);
        numbers_box_layout->addWidget(send_return_button);

    /*--------------------------------------*/

    main_splitter->addWidget(images_splitter);
    main_splitter->addWidget(numbers_box);

    main_layout->addWidget(main_splitter);
    setLayout(main_layout);

}

ResultsWidget::~ResultsWidget(){

    delete layout();

}




void ResultsWidget::send(){send(numbers());}

void ResultsWidget::send(const std::vector<long long> &numbers){

    std::cout << numbers.size() << std::endl;

    if(numbers.empty()){return;}

    std::cout << "empty == false" << std::endl;

    QString q_result = "";

    for(const long long &value : numbers){
        q_result += QString::number(value);
    }

    mainWindow->bluetooth_handler()->SetTX(q_result.toStdString());

}

/***************************************************************************/

void ResultsWidget::setSplitPhotos(const QImage &photo){setSplitPhotos(QPixmap::fromImage(photo));}
void ResultsWidget::setBWImage(const QImage &photo){setBWImage(QPixmap::fromImage(photo));}
void ResultsWidget::setDetection(const QImage &photo){setDetection(QPixmap::fromImage(photo));}

void ResultsWidget::setSplitPhotos(const QPixmap &photo){split_images->setPixmap(photo);}
void ResultsWidget::setBWImage(const QPixmap &photo){bw_image->setPixmap(photo);}
void ResultsWidget::setDetection(const QPixmap &photo){detection_image->setPixmap(photo);}

void ResultsWidget::setState(const ResultsWidget::STATE &state){


    switch(state){

        case(STATE::SEND_ST):{
            disconnect(send_return_button , &QPushButton::clicked , this , &ResultsWidget::on_return);
            connect(send_return_button , &QPushButton::clicked , this , &ResultsWidget::on_send);
            send_return_button->setText(tr("Send and return"));
            cancel_button->show();
            break;
        }

        case(STATE::RETURN_ST):{

            disconnect(send_return_button , &QPushButton::clicked , this , &ResultsWidget::on_send);
            connect(send_return_button , &QPushButton::clicked , this , &ResultsWidget::on_return);
            send_return_button->setText(tr("Return"));
            cancel_button->hide();
            break;
        }

        default:{
            setState(STATE::SEND_ST);
            return;
        }

    }

}


std::vector<long long>& ResultsWidget::numbers(){return _numbers;}
std::vector<MNN::scalar> &ResultsWidget::probabilities(){return _probabilities;}

void ResultsWidget::updateNumbers(const std::vector<long long> &numbers , const std::vector<MNN::scalar> &probabilities){

    resetNumbers();
    unsigned label = 1;
//    for(const long long &number : numbers){
//        number_scroll->addNumber(number , pro tr("Label: ") + QString::number(label));
//        label++;
//    }


    for(unsigned index = 0 ; index < std::min(numbers.size() , probabilities.size()) ; index++){
        number_scroll->addNumber(numbers.at(index) , probabilities.at(index)*100 , QString::number(label));
        label++;
    }

}
void ResultsWidget::updateNumbers(){updateNumbers(_numbers , _probabilities);}

void ResultsWidget::resetNumbers(){number_scroll->reset();}


/*****************************************************************/

void ResultsWidget::on_send(){

    send();      //Send _numbers to the robot
    on_return(); //Return to the main screen

}
void ResultsWidget::on_return(){
    mainWindow->setState(MainWindow::STATE::CAMERA_DISPLAY);
    resetNumbers();
}
