#ifndef PREVIEWRENDERER_H
#define PREVIEWRENDERER_H

#include <QGraphicsView>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

class View;

class PreviewRenderer : public QGraphicsView{

        Q_OBJECT

public:
    PreviewRenderer(QWidget *parent, View *v);

    View* getView();


protected:
#if QT_CONFIG(wheelevent)
        void wheelEvent(QWheelEvent *)override; //Méthode permettant de contrôler le zoom à l'aide de la molette de la souris
#endif
        void mousePressEvent(QMouseEvent *event) override;
        void keyPressEvent(QKeyEvent *) override;

private:

    View * view;




};

#endif // PREVIEWRENDERER_H
