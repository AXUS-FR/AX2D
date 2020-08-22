#ifndef AXTRIANGLE_H
#define AXTRIANGLE_H

#include <AxLine.h>
#include <axcircle.h>
#include <QDebug>

class AxTriangle : public QPolygonF
{
    public:

        //constructor

        AxTriangle();
        AxTriangle(QPointF _p1, QPointF _p2, QPointF p3);

        //getters

        QPointF getp1();
        QPointF getp2();
        QPointF getp3();
        AxLine getline1();
        AxLine getline2();
        AxLine getline3();

        AxCircle cercle_inscrit();// return the inscribed circle of the triangle

        //plot

        void affiche(QPaintDevice *device, int width, const QColor &color);
        friend QDebug operator<<(QDebug dbg, const AxTriangle &type);

    private:

        QPointF p1;
        QPointF p2;
        QPointF p3;

};

#endif // AXTRIANGLE_H
