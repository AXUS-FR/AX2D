#ifndef AXARC_H
#define AXARC_H

#include <QLineF>
#include <QPointF>
#include <QVector>
#include <QStyle>
#include <AxBorder.h>


class AxArc : public AxBorder
{

    public:

        // constructors

        AxArc();
        AxArc(QLineF _arc_limits, QPointF _center, qreal _R, int clockwise);
        AxArc(AxBorder *border);

        //class destructor

        virtual ~AxArc();

        //getters

        QPointF get_center(); // return the attribute center of the arc
        qreal get_R();  // return the attribute R of the arc
        bool is_clockwise();    // return the attribute clockwise of the arc
        bool is_cw();
        bool is_ccw();
        virtual QLineF get_mediatrice(); // return the mediatrice of the arc
        virtual QPointF middle_point(); // return the middle point of the arc
        virtual int getObjectType(); // return 1


        // functions

        int arc_test(QPointF px); // chack if px in inside the angles form by the arcarc
        qreal angle(QPointF p); // calculate the angle of p based on the center of the arc clockwise
        void set_cw(bool);
        void set_ccw(bool);

        // plot

        virtual void affiche(QPaintDevice *device, int width, const QColor &color); // plot if MainWindow
        friend QDebug operator<<(QDebug dbg, const AxArc &type);// operator overloading
        virtual void f(QDebug& dbg) const;

        // operator overloading

        bool operator==(const AxArc &arc) const;

private:

        QPointF C;
        qreal R;
        int clockwise;
};

#endif // AXARC_H
