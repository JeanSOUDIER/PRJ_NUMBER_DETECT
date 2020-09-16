#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <filenamefield.h>

#include "global.h"

#include "movementbox.h"

#include "sequence.h"
#include "sequencebox.h"
#include "sequencewriter.h"
#include "sequencereader.h"


#include <QAction>

#include <QDir>

#include <QFileDialog>

#include <QGroupBox>
#include <QGuiApplication>

#include <QLabel>
#include <QLineEdit>

#include <QTabWidget>
#include <QToolButton>

#include <QMainWindow>
#include <QtMath>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

#include <QPushButton>

#include <QRadioButton>

#include <QScreen>
#include <QSplitter>
#include <QSystemTrayIcon>

#include <QHBoxLayout>
#include <QFormLayout>

class MainWindow : public QMainWindow{

    friend class SequenceBox;
    friend class MovementBox;

                Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    QSplitter *mainSplitter;
    QSystemTrayIcon *trayIcon;

    QMenu *file_menu;
    QAction *open_action;
    QAction *save_action;
    QAction *close_action;


    SequenceBox *sequence_box;
    QRadioButton *direction_mode_rb;
    QRadioButton *coordinates_mode_rb;

    QComboBox *format_combox;
    QLineEdit *path_field;
    FileNameField *file_name_field;


    QGroupBox *preview_parameter_box;
    QSpinBox *begin_x_spinbox;
    QSpinBox *begin_y_spinbox;
    QSpinBox *begin_z_spinbox;

    QSpinBox *speed_spinbox;

    QLabel *total_time_label;
    QLabel *total_distance_label;

    QString last_path;

    void setup_splitter();
    void setup_tray();
    void setup_menu();

    void open_Path(const QString& path);


protected:

    void resizeEvent(QResizeEvent *event) override;

protected slots:

    void updateInfo();

private slots:

    void open();
    void close();
    void save();

    void browse();

    void addMovement();
    void updateSequenceMode();

    bool generate();
    void preview();





};
#endif // MAINWINDOW_H
