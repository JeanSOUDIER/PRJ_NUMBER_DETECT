#ifndef POINT_H
#define POINT_H



#include <QtCore/QObject>

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>



#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QGoochMaterial>



class View;

class PointEntity : public Qt3DCore::QEntity{

    Q_OBJECT

    friend class View;

public:

    PointEntity(Qt3DCore::QEntity *_parent, const double _x , const double _y , const double _z , const unsigned int _index);
    ~PointEntity();
    void setX(const int _x);
    void setY(const int _y);
    void setZ(const int _z);
    void setIndex(const unsigned int _index);
    void setColor(const QColor&);

    int getX();
    int getX()const;
    int getY();
    int getY()const;
    int getZ();
    int getZ()const;

    unsigned int getIndex();
    unsigned int getIndex() const;


protected:

    void move(const double _x , const double _y , const double _z);
    void resize(const double size);

private:

    double x;
    double y;
    double z;
    unsigned int index;

    Qt3DCore::QEntity* parent;
    Qt3DExtras::QCuboidMesh* mesh;
    Qt3DCore::QTransform *transform;

    Qt3DExtras::QGoochMaterial *material;


};

#endif // POINT_H
