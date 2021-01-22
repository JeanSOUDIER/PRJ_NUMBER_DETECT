#ifndef RESULTSWIDGET_H
#define RESULTSWIDGET_H

#include <QGroupBox>

#include <QPushButton>

#include <QLabel>

#include <QScrollArea>
#include <QSplitter>

#include <QWidget>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include "numberscroll.h"

#include "MatrixNN/MatrixNN_global.h"

class MainWindow;

class ResultsWidget : public QWidget {

    friend class MainWindow;
    friend class CameraWidget;

    Q_OBJECT

public:

    explicit ResultsWidget(MainWindow *parent = nullptr);
    ~ResultsWidget();

    enum class STATE{
        SEND_ST,
        RETURN_ST,
    };

protected:

    void setSplitPhotos(const QImage &photo);
    void setBWImage(const QImage &photo);
    void setDetection(const QImage &photo);

    void setSplitPhotos(const QPixmap &photo);
    void setBWImage(const QPixmap &photo);
    void setDetection(const QPixmap &photo);

    void setState(const ResultsWidget::STATE &state);

    void send();
    void send(const std::vector<long long> &numbers);

    void resetNumbers();
    void updateNumbers(const std::vector<long long> &numbers, const std::vector<MNN::scalar> &probabilities);
    void updateNumbers();
    std::vector<long long> &numbers();
    std::vector<MNN::scalar> &probabilities();

private:

    MainWindow* mainWindow;

    QLabel *split_images;
    QLabel *bw_image;
    QLabel *detection_image;

    NumberScroll *number_scroll;

    QPushButton *send_return_button;
    QPushButton *cancel_button;

    std::vector<long long> _numbers;
    std::vector<MNN::scalar> _probabilities;

private slots:

    void on_send();
    void on_return();


};

#endif // RESULTSWIDGET_H
