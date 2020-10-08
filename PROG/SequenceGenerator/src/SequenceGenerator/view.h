#ifndef VIEW_H
#define VIEW_H

#include <QObject>

#include "pointentity.h"
#include "movement.h"

#include <QScreen>

#include <QVector>

#include <QWidget>



#include <Qt3DInput/QInputAspect>

#include <Qt3DRender/QCamera>
#include <Qt3DRender/QPointLight>

#include <Qt3DExtras/QFirstPersonCameraController>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QForwardRenderer>

#include <QResizeEvent>

#include <QHBoxLayout>


class View : public QWidget {

    Q_OBJECT

    friend class MainWindow;

public:

    explicit View(QWidget *parent =nullptr);
    ~View();
    QVector<PointEntity*> getEntities();
    QWidget *getContents();


protected:

    void addEntity(const Movement * , const unsigned int index);
    void addEntity(const double x , const double y , const double z , const unsigned int index);

    void deleteEntity(const unsigned index);
    void clear();

    void moveEntity(Movement * , const unsigned int index);
    void moveEntity(const double x , const double y , const double z , const unsigned int index);

    void resizeEntity(const unsigned int index , const double size);
    void resizeAll(const double size);

    void goToEntity(const int index);


    void editEntityColour(QColor colour , const unsigned int index);

    void hideAll();
    void showAll();


private:

    Qt3DExtras::Qt3DWindow *view_3d;
    QWidget *contents;

    QVector<PointEntity*> entities;

    Qt3DCore::QEntity *rootEntity;

    Qt3DInput::QInputAspect *input;
    Qt3DRender::QCamera *cameraEntity;
    Qt3DExtras::QFirstPersonCameraController *camController;

    Qt3DCore::QEntity *lightEntity;
    Qt3DRender::QPointLight *light;
    Qt3DCore::QTransform *lightTransform;



};

#endif // VIEW_H
