#ifndef Vertice_H
#define Vertice_H
#include "AxLine.h"


class Vertice
{


public:
    Vertice();
    Vertice(AxLine bisection, AxLine *edge1, AxLine *edge2);

    AxLine* get_edge1();
    AxLine* get_edge2();
    AxLine get_bisection();
//    AxLine isActive();

    void set_edge1(AxLine *edge);
    void set_edge2(AxLine *edge);
    void set_bisection(AxLine line);


    friend QDebug operator<<(QDebug dbg, const Vertice &type);// operator overloading

    bool operator==(const Vertice &arc) const;

private:
    AxLine bisection;
    AxLine *edge1;
    AxLine *edge2;
};

#endif // Vertice_H
