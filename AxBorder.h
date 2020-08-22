#ifndef AXBORDER_H
#define AXBORDER_H

#include <QDebug>
#include <QStyle>
#include <QLineF>

class AxBorder
{
public:

    // GETTERS

    QLineF get_limits();
    //return the 2 points compasing the border

    QPointF p1() const;
    // return the 1st point of the border

    QPointF p2() const;
    // return the 2nd point of the border

    qreal x1() const;
    // return the x coordinate of the 1st point of the border

    qreal x2() const;
    // return the x coordinate of the 2st point of the border

    qreal y1() const;
    // return the y coordinate of the 1st point of the border

    qreal y2() const;
    // return the y coordinate of the 2st point of the border

    QPointF center() const;
    // return the center of the segment associated with the border limits

    virtual QPointF middle_point();
    // return the middle point of the border (if arc : middle point part of arc)

    virtual QLineF get_mediatrice();
    //return the mediatrice of the object

    virtual int getObjectType();
    // return border type : if (border==line) return 0 if (border==arc) return 0


    // SETTERS

    void setLimits(QLineF limits);
    // change the value of limits

    void setP1(QPointF p);
    // change the value of the 1st border point

    void setP2(QPointF p);
    // change the value of the 2nd border point


    // PLOT

    virtual void affiche(QPaintDevice *device, int width, const QColor &color);
    // graphical plot

    friend QDebug operator<<(QDebug dbg, AxBorder *type);
    virtual void f(QDebug& dbg) const;
    // command plot


    //OTHER

    virtual ~AxBorder(); // destructor

protected:

    QLineF limits;

};

#endif // AXBORDER_H
