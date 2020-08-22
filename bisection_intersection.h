#ifndef bisection_intersection_H
#define bisection_intersection_H
#include "AxLine.h"
#include "Vertice.h"

class bisection_intersection
{
public:
    bisection_intersection();
    bisection_intersection(qreal distance, QPointF cross_point,Vertice *vertice1, Vertice *vertice2);

    friend QDebug operator<<(QDebug dbg, const bisection_intersection &type);// operator overloading

    qreal get_distance() const;
    Vertice get_vertice1() const;
    Vertice get_vertice2() const;
    Vertice get_previous_vertrice() const;
    Vertice get_next_vertrice() const;
    QPointF get_cross_point() const;


private:
    qreal distance;
    QPointF cross_point;
    Vertice *vertice1;
    Vertice *vertice2;
};
#endif // bisection_intersection_H
