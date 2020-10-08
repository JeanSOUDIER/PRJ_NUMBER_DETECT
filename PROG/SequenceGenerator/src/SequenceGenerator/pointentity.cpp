#include "pointentity.h"

PointEntity::PointEntity(Qt3DCore::QEntity *_parent , const double _x, const double _y, const double _z, const unsigned int _index) : Qt3DCore::QEntity(_parent){

    x= _x;
    y =_y;
    z = _z;
    index = _index;
    parent = _parent;

    mesh = new Qt3DExtras::QCuboidMesh();
//        mesh->setRings(20);
//        mesh->setSlices(20);
//        mesh->setRadius(2); //Sphere parameters
//        mesh->setWidth(2);
//        mesh->setHeight(2); //Plane paramaters

    transform = new Qt3DCore::QTransform();
    transform->setScale(100.);
    transform->setTranslation(QVector3D(x, y, z));

    material = new Qt3DExtras::QGoochMaterial();
    material->setDiffuse(QColor(QRgb(0x189E84))); //0x338FFF ?



    addComponent(mesh);
    addComponent(material);
    addComponent(transform);


}


PointEntity::~PointEntity(){

        delete mesh;
        delete transform;
        delete material;

}
/**************************************************************/

void PointEntity::move(const double _x, const double _y, const double _z){
    x = _x;
    y = _y;
    z = _z;
    transform->setTranslation(QVector3D(x , y , z));
}

void PointEntity::resize(const double size){transform->setScale(size);}

/**************************************************************/

void PointEntity::setX(const int _x){
    x = _x;
    move(x , y , z);
}
void PointEntity::setY(const int _y){
    y = _y;
    move(x , y , z);
}
void PointEntity::setZ(const int _z){
    z = _z;
    move(x , y , z);
}
void PointEntity::setIndex(const unsigned int _index){index = _index;}

void PointEntity::setColor(const QColor& color){material->setDiffuse(color);}

int PointEntity::getX(){return x;}
int PointEntity::getX() const {return x;}
int PointEntity::getY(){return y;}
int PointEntity::getY() const {return y;}
int PointEntity::getZ(){return z;}
int PointEntity::getZ() const {return z;}

unsigned int PointEntity::getIndex(){return index;}

unsigned int PointEntity::getIndex() const{return index;}
