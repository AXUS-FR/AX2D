#ifndef AXBORDER_H
#define AXBORDER_H

#include <QDebug>
#include <QStyle>
#include <QLine>

class AxBorder
{
public:

    // GETTERS

    QLine get_limits();
    //return the 2 points compasing the border

    QPoint p1() const;
    // return the 1st point of the border

    QPoint p2() const;
    // return the 2nd point of the border

    int x1() const;
    // return the x coordinate of the 1st point of the border

    int x2() const;
    // return the x coordinate of the 2st point of the border

    int y1() const;
    // return the y coordinate of the 1st point of the border

    int y2() const;
    // return the y coordinate of the 2st point of the border

    QPoint center() const;
    // return the center of the segment associated with the border limits

    virtual QPoint middle_point();
    // return the middle point of the border (if arc : middle point part of arc)

    virtual QLine get_mediatrice();
    //return the mediatrice of the object

    virtual int getObjectType();
    // return border type : if (border==line) return 0 if (border==arc) return 0

    void set_cw(bool);
    void set_ccw(bool);

    // SETTERS

    void setLimits(QLine limits);
    // change the value of limits

    void setP1(QPoint p);
    // change the value of the 1st border point

    void setP2(QPoint p);
    // change the value of the 2nd border point

    virtual void translate(int offset);
    // for AxLines
    // translate the line of offset value in perpendicular direction
    // + direction offset right from line
    // - direction offset left from line

    //for AxArc
    //adapt radius to offset


    // PLOT

    virtual void display(QPaintDevice *device, int width, const QColor &color);
    // graphical plot

    friend QDebug operator<<(QDebug dbg, AxBorder *type);
    virtual void f(QDebug& dbg) const;
    // command plot


    //OTHER

    virtual ~AxBorder(); // destructor

protected:

    QLine limits;
    QLine last;

};

#endif // AXBORDER_H
