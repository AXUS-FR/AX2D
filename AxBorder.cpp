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

QPointF AxBorder::p1() const
{
    return limits.p1();
}

QPointF AxBorder::p2() const
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

QPointF AxBorder::center() const
{
    return limits.center();
}

void AxBorder::setLimits(QLineF _border)
{
    limits=_border;
}

void AxBorder::setP1(QPointF p)
{
    limits.setP1(p);
}

void AxBorder::setP2(QPointF p)
{
    limits.setP2(p);
}

QLineF AxBorder::get_limits()
{
    return limits;
}

QPointF AxBorder::middle_point()
{
}


QLineF AxBorder::get_mediatrice()
{
}



QDebug operator<<(QDebug dbg, AxBorder *type)
{
    type->f(dbg);
    return dbg.maybeSpace();
}
