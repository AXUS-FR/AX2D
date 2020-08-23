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

        // CONSTRUCTOR

        AxShape();
        AxShape(AxShape const& AxShapeacopier); //copy constructor


        // SETTERS

        void append(AxBorder *border);
        // insert a shape on last position

        void insert(int k,AxBorder *border);
        // insert a shape on k position

        void remove(int k);
        // remove the shape at k position

        void replace(int k,AxBorder *border);
        // replace the shape at k position


        //GETTERS

        AxBorder* operator[](int i) const;
        //  operator overloading

        int size();
        // return the size of the shape

        int next_point(int k);
        // go to the next point of the shape (ex : if k=6 return 7, if k=2 return 3, if k=shape_size-1 return 0;

        int previous_point(int k);
        // return pervious point of the shape (ex : if k=6 return 5, if k=2 return 1, if k=0 return shape_size-1;

        //TO IMPLEMENT

        void analyse_dir();
        //analyse the direction of each corner

        void offset(int offset);
        //translate all AxLine/AxArc of offset value (can be positive or negative)

        void link();
        //made the link between all AxBorders(radius and lines)

        void separate();
        // separate at all intersection in several AxShapes

        void classify();
        // classify all AxShapes using winding numbers

        void clean();
        // keep only Axshape with winding number == 1, -1/-2/-3 are erased, 0 is outside

        void set_cw(bool);

        void set_ccw(bool);



        // FUNCTIONS

        void expand(int offset);
        // return a QVector of AxShape composed of the expantion of the shape :
        // Warning the final Shape is composed of lines only.
        // There can be arc in the initial shape but their previous and next border must be AxLine and they must cross in arc direction

        QVector<QVector<AxShape>> convexe_concave();
        // return a QVector of all set of convexe border and all concave border

        AxBorder* segment_brut_expand(AxBorder *border, int R);
        // return the associated expanded border out of the shape with a distance of R (note : the border must be part of the shape)

        AxBorder* segment_brut_intern_expand(AxBorder *border, int R);
        // return the associated expanded border out of the shape with a distance of R (note : the border must be part of the shape)

        QVector<QPoint> from_shape_to_pts();
        // convert the shape to a polygon : the arcs are converted to lines

        bool contain(QPoint p);
        // return true if the point is inside the figure, return false else

        int bord_externe(); //determine if the border is outside the figure

        QVector<AxLine> straight_skeletton();
        //WARNING function unfished !
        // return the skeletton of the polygon

        AxShape expand_unfinished(int R);



        //PLOT

        void display(QPaintDevice *device, int width, const QColor &color);
        // draw the hole figure on the selected device

        friend QDebug operator<<(QDebug dbg, const AxShape &type);
        // operator overloading


        // OTHER

        AxShape operator=(AxShape const& shapeacopier);
        //copy constructor

        virtual ~AxShape();
        //destructor


    private:
        QVector<AxBorder*> shape;
        int last_offset;

};

#endif // AXSHAPE_H
