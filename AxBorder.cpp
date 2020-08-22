#include "AxBorder.h"
#include <AxLine.h>

AxBorder:: ~AxBorder()
{};

void AxBorder::affiche(QPaintDevice *device, int width, const QColor &color)
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

qreal AxBorder::x1() const
{
    return limits.p1().x();
}

qreal AxBorder::x2() const
{
    return limits.p2().x();
}

qreal AxBorder::y1() const
{
    return limits.p1().y();
}

qreal AxBorder::y2() const
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

QPoint AxBorder::middle_point()
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
