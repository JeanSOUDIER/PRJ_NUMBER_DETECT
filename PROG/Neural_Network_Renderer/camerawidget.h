#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include "Camera_handler/camerahandler.h"
#include "Camera_handler/utilityocv.h"


#include <QCheckBox>

#include <QLabel>

#include <QGroupBox>

#include <QPushButton>

#include <QTimer>

#include <QWidget>


#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

class MainWindow;

class CameraWidget : public QWidget {

    friend class MainWindow;
    friend class ResultsWidget;

    Q_OBJECT

public:

    explicit CameraWidget(MainWindow *parent = nullptr);
    ~CameraWidget();

protected:

    void setPhoto(const QImage &photo);
    void setPhoto(const QPixmap &photo);
    cv::Mat takePhoto();

    QTimer* timer();
    QLabel* photo();

private:

    MainWindow *mainWindow;

    QPushButton *photo_button;

    QCheckBox *autosend_cb;

    QLabel *image_label;

    CameraHandler *camera;

    QTimer *refreshTimer;

private slots:

    void on_photo_button();
    void on_autosend_cb();

};

#endif // CAMERAWIDGET_H
