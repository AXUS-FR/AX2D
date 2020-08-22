#ifndef AXSHAPE_H
#define AXSHAPE_H

#include "AxLine.h"
#include <QStyle>

#include <QObject>
#include <QVector>
#include <AxArc.h>
#include <iostream>
using namespace std;

class AxShape
{
    public:

        // constructor

        AxShape();
        AxShape(AxShape const& AxShapeacopier); //copy constructor

        // destructor

        virtual ~AxShape(); //destructeur de classe

        // setters

        void append(AxBorder* border); // insert a shape on last position
        void insert(int k,AxBorder* border); // insert a shape on k position
        void remove(int k); // remove the shape at k position
        void replace(int k,AxBorder* border); // replace the shape at k position

        //getters

        AxBorder* operator[](int i) const; //  operator overloading

        int size(); // return the size of the shape
        int next_point(int k); // go to the next point of the shape (ex : if k=6 return 7, if k=2 return 3, if k=shape_size-1 return 0;
        int previous_point(int k);// return pervious point of the shape (ex : if k=6 return 5, if k=2 return 1, if k=0 return shape_size-1;

        // functions

        AxShape straight_skeletton(); // return a QVector of AxLine conposed of the straight squeletton on the shape
        QVector<QVector<AxShape>> convexe_concave(); // return a QVector of all set of convexe border and all concave border
        AxBorder* segment_brut_expand(AxBorder *border, qreal R); // return the associated expanded border out of the shape with a distance of R (note : the border must be part of the shape)
        QVector<QPointF> from_shape_to_pts(); // convert the shape to a polygon : the arcs are converted to lines

        bool contain(QPointF p); // deternine if the point is inside or outside of the figure
        int bord_externe(); //determine if the border is inside the figure

        //Plot

        void affiche(QPaintDevice *device, int width, const QColor &color);// drow the hole figure on the selected device
        friend QDebug operator<<(QDebug dbg, const AxShape &type);// operator overloading

        // operator overloading

        AxShape operator=(AxShape const& shapeacopier);


    private:
        QVector<AxBorder*> shape;

};

#endif // AXSHAPE_H
