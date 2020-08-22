#include "AxBorder.h"
#include <AxLine.h>

AxBorder:: ~AxBorder()
{};

void AxBorder::display(QPaintDevice *device, int width, const QColor &color)
{}

void AxBorder::f(QDebug &dbg) const
{

}


int AxBorder::getObjectType()
{}

QPoint AxBorder::p1() const
{
    return limits.p1();
}

QPoint AxBorder::p2() const
{
    return limits.p2();
}

int AxBorder::x1() const
{
    return limits.p1().x();
}

int AxBorder::x2() const
{
    return limits.p2().x();
}

int AxBorder::y1() const
{
    return limits.p1().y();
}

int AxBorder::y2() const
{
    return limits.p2().y();
}

QPoint AxBorder::center() const
{
    return limits.center();
}

void AxBorder::setLimits(QLine _border)
{
    limits=_border;
}

void AxBorder::setP1(QPoint p)
{
    limits.setP1(p);
}

void AxBorder::setP2(QPoint p)
{
    limits.setP2(p);
}

QLine AxBorder::get_limits()
{
    return limits;
}

void AxBorder::set_cw(bool c)
{

}

void AxBorder::set_ccw(bool c)
{

}

QPoint AxBorder::middle_point()
{
}

void AxBorder::translate(int offset)
{

}

QLine AxBorder::get_mediatrice()
{
}



QDebug operator<<(QDebug dbg, AxBorder *type)
{
    type->f(dbg);
    return dbg.maybeSpace();
}
