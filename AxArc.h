#ifndef AXARC_H
#define AXARC_H

#include <QLine>
#include <QPoint>
#include <QVector>
#include <QStyle>
#include <AxBorder.h>


class AxArc : public AxBorder
{

    public:

        // CONSTRUCTOR

        AxArc();
        AxArc(QLine _arc_limits, QPoint _center, int _R, int clockwise);
        AxArc(AxBorder *border);
        AxArc(QPoint _center, int _R);


        //GETTERS

        QPoint get_center();
        // return the attribute C of the arc

        int get_R();
        // return the attribute R of the arc
		bool is_cw();
        bool is_ccw();		
        void set_cw(bool);
        void set_ccw(bool);		

        bool is_clockwise();
        // return the attribute clockwise of the arc

        virtual QLine get_mediatrice();
        // return the mediatrice of the arc

        virtual QPoint middle_point();
        // return the middle point of the arc

        virtual int getObjectType();
        // return 1

       void translate(int offset);

        // FUNCTIONS

        int arc_test(QPoint px);
        // check if px is on arc (WARNING check only angles)

        int angle(QPoint p);
        // calculate the angle of p based on the center of the arc clockwise


        // PLOT

        virtual void display(QPaintDevice *device, int width, const QColor &color);
        // graphical plot

        friend QDebug operator<<(QDebug dbg, const AxArc &type);
        virtual void f(QDebug& dbg) const;
        // command plot


        // OTHER

        bool operator==(const AxArc &arc) const;
        virtual ~AxArc(); // class destructor

private:

        QPoint C;
        int R;
        int clockwise;
};

#endif // AXARC_H
