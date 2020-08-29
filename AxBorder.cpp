#include "AxBorder.h"
#include <AxLine.h>

AxBorder:: ~AxBorder()
{};

void AxBorder::display(QPaintDevice *, int , const QColor &)
{

}

void AxBorder::f(QDebug &) const
{

}


int AxBorder::getObjectType()
{
    return 0;
}

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

void AxBorder:: set_kind(int _kind)
{
    kind = _kind;
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

int AxBorder::get_kind()
{
    return kind;
}

QLine AxBorder::get_limits()
{
    return limits;
}

int AxBorder::get_last_offset()
{
    return last_offset;
}

void AxBorder::set_pocket()
{
    pocket = true;
}

void AxBorder::set_island()
{
    pocket = false;
}

void AxBorder::set_p1_dir(int dir)
{
    p1_dir = dir;
}

void AxBorder::set_p2_dir(int dir)
{
    p2_dir = dir;
}

int AxBorder::get_p1_dir()
{
    return p1_dir;
}

int AxBorder::get_p2_dir()
{
    return p2_dir;
}

QPoint AxBorder::get_last_p1()
{
    return last.p1();
}

QPoint AxBorder::get_last_p2()
{
    return last.p2();
}



bool AxBorder::is_pocket()
{
    return pocket;
}

bool AxBorder::is_island()
{
    return !pocket;
}

void AxBorder::set_cw(bool )
{

}

void AxBorder::set_ccw(bool )
{

}

QPoint AxBorder::middle_point()
{
    return QPoint();
}

void AxBorder::translate(int )
{

}

QLine AxBorder::get_mediatrice()
{
    return QLine();
}



QDebug operator<<(QDebug dbg, AxBorder *type)
{
    type->f(dbg);
    return dbg.maybeSpace();
}
