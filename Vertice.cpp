#include "Vertice.h"
#include "AxLine.h"

Vertice::Vertice()
{

}

Vertice::Vertice(AxLine _bisection, AxLine *_edge1, AxLine *_edge2)
{
    bisection=_bisection;
    edge1=_edge1;
    edge2=_edge2;
}


AxLine* Vertice::get_edge1()
{
    return edge1;
}

AxLine* Vertice::get_edge2()
{
    return edge2;
}

AxLine Vertice::get_bisection()
{
    return bisection;
}

void Vertice::set_edge1(AxLine *edge)
{
        edge1=edge;
}
void Vertice::set_edge2(AxLine *edge)
{
    edge2=edge;
}
void Vertice::set_bisection(AxLine line)
{
    bisection=line;
}
QDebug operator<<(QDebug dbg, const Vertice &type)
{
    dbg.nospace() << "Vertice("<< type.bisection << "," << type.edge1 << ","<< type.edge2 << ")";
    return dbg.maybeSpace();
}

bool Vertice::operator==(const Vertice &vertice) const
{
    if ((edge1==vertice.edge1) && (edge2==vertice.edge2) && (bisection==vertice.bisection))
    {
        return true;
    }
    else
    {
        return false;
    }
}
