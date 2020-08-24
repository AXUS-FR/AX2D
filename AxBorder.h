#ifndef AXBORDER_H
#define AXBORDER_H

#include <QDebug>
#include <QStyle>
#include <QLine>

class AxBorder
{
public:

    // GETTERS

    int get_kind();
    //return the kind of element : -1=construction / 0=piece / 1=brut

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
    // return border type : if (border==line) return 0 if (border==arc) return 1

    virtual void set_cw(bool);
    virtual void set_ccw(bool);
    void set_pocket();
    void set_island();
    void set_p1_dir(int);
    void set_p2_dir(int);
    bool is_pocket();
    bool is_island();
    int get_p1_dir();
    int get_p2_dir();
    QPoint get_last_p1();
    QPoint get_last_p2();
    int get_last_offset();


    // SETTERS

    void set_kind(int _kind);
    //set the kind of the element : -1=construction / 0=pièce / 1=brut

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

    int p1_dir;//0 is colinear,1 is left, 2 is right;
    int p2_dir;
    QLine limits;
    QLine last;
    bool pocket;
    int last_offset;
    int kind;   //kind of element : brut, contruction, piece

};

#endif // AXBORDER_H
