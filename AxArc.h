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

        // CONSTRUCTOR

        AxArc();
        AxArc(QLineF _arc_limits, QPointF _center, qreal _R, int clockwise);
        AxArc(AxBorder *border);
        AxArc(QPointF _center, qreal _R);


        //GETTERS

        QPointF get_center();
        // return the attribute C of the arc

        qreal get_R();
        // return the attribute R of the arc
		bool is_cw();
        bool is_ccw();		
        void set_cw(bool);
        void set_ccw(bool);		

        bool is_clockwise();
        // return the attribute clockwise of the arc

        virtual QLineF get_mediatrice();
        // return the mediatrice of the arc

        virtual QPointF middle_point();
        // return the middle point of the arc

        virtual int getObjectType();
        // return 1


        // FUNCTIONS

        int arc_test(QPointF px);
        // check if px is on arc (WARNING check only angles)

        qreal angle(QPointF p);
        // calculate the angle of p based on the center of the arc clockwise


        // PLOT

        virtual void affiche(QPaintDevice *device, int width, const QColor &color);
        // graphical plot

        friend QDebug operator<<(QDebug dbg, const AxArc &type);
        virtual void f(QDebug& dbg) const;
        // command plot


        // OTHER

        bool operator==(const AxArc &arc) const;
        virtual ~AxArc(); // class destructor

private:

        QPointF C;
        qreal R;
        int clockwise;
};

#endif // AXARC_H
