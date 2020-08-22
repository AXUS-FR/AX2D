#ifndef AXTRIANGLE_H
#define AXTRIANGLE_H

#include <AxLine.h>
#include <axcircle.h>
#include <QDebug>

class AxTriangle : public QPolygon
{
    public:

        //constructor

        AxTriangle();
        AxTriangle(QPoint _p1, QPoint _p2, QPoint p3);

        //getters

        QPoint getp1();
        QPoint getp2();
        QPoint getp3();
        AxLine getline1();
        AxLine getline2();
        AxLine getline3();

        AxCircle cercle_inscrit();// return the inscribed circle of the triangle

        //plot

        void affiche(QPaintDevice *device, int width, const QColor &color);
        friend QDebug operator<<(QDebug dbg, const AxTriangle &type);

    private:

        QPoint p1;
        QPoint p2;
        QPoint p3;

};

#endif // AXTRIANGLE_H
