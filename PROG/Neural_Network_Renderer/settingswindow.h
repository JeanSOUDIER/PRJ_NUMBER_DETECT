#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QCloseEvent>

#include <QDialog>

#include <QGroupBox>

#include <QLabel>

#include <QPushButton>

#include <QSlider>
#include <QSpinBox>



#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

class MainWindow;

class SettingsWindow : public QDialog {

    friend class MainWindow;

             Q_OBJECT
public:

    SettingsWindow(MainWindow *parent = nullptr);
    ~SettingsWindow();


protected:

    void closeEvent(QCloseEvent *event) override;

    void reset();

private:

    MainWindow *mainWindow;

    QSpinBox *padding_spin;

    QSpinBox *max_size_spin;
    QSpinBox *min_size_spin;

    QSlider *threshold_slider;
    QLabel *threshold_value;


    QPushButton *reset_button;

    QPushButton *ok_button;
    QPushButton *cancel_button;

private slots:

    void on_ok_button();

};

#endif // SETTINGSWINDOW_H
