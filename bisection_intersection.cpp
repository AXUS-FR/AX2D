#include "bisection_intersection.h"

bisection_intersection::bisection_intersection()
{
}

bisection_intersection::bisection_intersection(qreal _distance, QPoint _cross_point, Vertice *_vertice1, Vertice *_vertice2)
{
    cross_point=_cross_point;
    distance=_distance;
    vertice1=_vertice1;
    vertice2=_vertice2;
}

qreal bisection_intersection::get_distance() const
{
    return distance;
}

Vertice bisection_intersection::get_vertice1() const
{
    return *vertice1;
}

Vertice bisection_intersection::get_vertice2() const
{
    return *vertice2;
}

QPoint bisection_intersection::get_cross_point() const
{
    return cross_point;
}

QDebug operator<<(QDebug dbg, const bisection_intersection &type)
{
    dbg.nospace() << "bisection_intersection("<< type.distance << "," << *type.vertice1  << ","<< *type.vertice2 << ")"<<endl;
    return dbg.maybeSpace();
}
