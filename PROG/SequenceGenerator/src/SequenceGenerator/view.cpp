#include "view.h"

#include <QDebug>


View::View(QWidget *parent) : QWidget(parent){

    QHBoxLayout *main_layout = new QHBoxLayout(this);

    view_3d = new Qt3DExtras::Qt3DWindow();
    view_3d->defaultFrameGraph()->setClearColor(QColor(QRgb(0xCED2D1)));

                    /*------------------------------*/

    contents = QWidget::createWindowContainer(view_3d , this);
    contents->setMinimumSize(QSize(500, 500));
    contents->setMaximumSize(view_3d->screen()->size());
    //contents->resize(parent->width()/2 , parent->height()*0.7);

                    /*------------------------------*/

    input = new Qt3DInput::QInputAspect();
    view_3d->registerAspect(input);

                    /*------------------------------*/

    rootEntity = new Qt3DCore::QEntity();

                    /*------------------------------*/

    cameraEntity = view_3d->camera();
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 0, 1000.0f));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));
    cameraEntity->setFarPlane(5000.f);


                        /*------------------------------*/

    lightEntity = new Qt3DCore::QEntity(rootEntity);

        light = new Qt3DRender::QPointLight(lightEntity);
        light->setColor(0xFFFFFF);
        light->setIntensity(100);
        light->setConstantAttenuation(0);
        light->setQuadraticAttenuation(0);

        lightTransform = new Qt3DCore::QTransform(lightEntity);
        lightTransform->setTranslation(cameraEntity->position());

    lightEntity->addComponent(light);
    lightEntity->addComponent(lightTransform);
    lightEntity->setEnabled(true);

                        /*------------------------------*/

     camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
     camController->setCamera(cameraEntity);
     camController->setLinearSpeed(200);
     camController->setLookSpeed(180);

     //addEntity(1 , 1 , 1, 1);
                         /*------------------------------*/

     view_3d->setRootEntity(rootEntity);


                         /*------------------------------*/

     main_layout->addWidget(contents , 1);


}

View::~View(){

    delete view_3d;
    delete contents;
}


/**************************************************************/

void View::addEntity(const Movement *mov, const unsigned int index){
    //PointEntity *point = new PointEntity(rootEntity , mov->getX() , mov->getY() , mov->getZ() , index);
    PointEntity *point = new PointEntity(rootEntity , -mov->getX() , mov->getZ() , -mov->getY() , index); //Axis of the robot are not the same as the one of the view
    entities.append(point);

}

void View::addEntity(const double x, const double y, const double z, const unsigned int index){

    PointEntity *point = new PointEntity(rootEntity , x , y, z , index);
    entities.append(point);
}

void View::deleteEntity(const unsigned index){delete entities.takeAt(index);}

void View::clear(){

    if(entities.size() > 1){

        qDeleteAll(entities.begin()+1 , entities.end());
        entities.resize(1);
    }



}

void View::moveEntity(Movement *mov , const unsigned int index){entities[index]->move(-mov->getX() , mov->getZ() , -mov->getY());} //Axis  of the robot and of the view are not the same
void View::moveEntity(const double x , const double y , const double z , const unsigned int index){entities[index]->move(x , y , z);}

void View::resizeEntity(const unsigned int index , const double size){entities[index]->resize(size);}
void View::resizeAll(const double size){

    for(PointEntity *item:entities){item->resize(size);}

}

void View::editEntityColour(QColor colour , const unsigned int index){entities[index]->setColor(colour);}

void View::goToEntity(const int index){

    if(index >= entities.size()){return;}

    const PointEntity * point = entities.at(index);
    cameraEntity->setPosition(QVector3D(point->getX(), point->getY(), point->getZ()+ (500.f)*(entities.at(0)->transform->scale()/100.)));
    cameraEntity->setViewCenter(QVector3D(point->getX(), point->getY(), point->getZ()));
}

void View::showAll(){for(int index = 0 ; index < entities.size() ; index++){entities[index]->setEnabled(true);}}
void View::hideAll(){for(int index = 0 ; index < entities.size() ; index++){entities[index]->setEnabled(false);}}
/**************************************************************/

QWidget* View::getContents(){return contents;}

QVector<PointEntity*> View::getEntities(){return entities;}
