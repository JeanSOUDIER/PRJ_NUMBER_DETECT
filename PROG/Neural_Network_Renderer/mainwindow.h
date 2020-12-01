#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "aboutwindow.h"

#include "camerawidget.h"

#include "resultswidget.h"

#include "Camera_handler/utilityocv.h"
#include "NeuralNetwork/PROJECT_FILES/project_network.h"


#include <thread>


#include <QAction>
#include <QApplication>

#include <QCloseEvent>

#include <QDir>

#include <QFileDialog>

#include <QGroupBox>
#include <QGuiApplication>

#include <QImage>

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>


#include <QPushButton>

#include <QScreen>
#include <QSplitter>


class MainWindow : public QMainWindow {

    friend class CameraWidget;
    friend class ResultsWidget;

    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    enum class STATE{
        CAMERA_DISPLAY,
        RESULTS_DISPLAY
    };

    CameraWidget *cameraWidget();
    ResultsWidget *resultsWidget();
    Project_network *neuralNetwork();

protected:

    void setState(const MainWindow::STATE &state);
    void process(const cv::Mat &);

    void closeEvent(QCloseEvent *event) override;

private:

    void setup_widgets();
    void setup_windows();
    void setup_menu();
    void setup_neuralnetwork();


    QMenu *system_menu;
        QAction *load_nn_ac;
        QAction *exit_action;

    QMenu *settings_menu; //Camera choice
        QAction *select_camera_action;

    QMenu *help_menu;
        QAction *about_action;


    CameraWidget *camera_widget;
    ResultsWidget *results_widget;

    AboutWindow *about_window;


    Project_network *neural_network;

};
#endif // MAINWINDOW_H
