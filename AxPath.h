#ifndef AXPATH_H
#define AXPATH_H
#include "AxLine.h"
#include <QStyle>

#include <QObject>
#include <AxArc.h>
#include <iostream>

class AxPath
{
public:
    //getters
    int* get_pointer_x() const;
    int* get_pointer_y() const;
    int* get_pointer_z() const;
    int* get_pointer_A() const;
    int* get_pointer_B() const;
    int* get_pointer_C() const;
    int* get_pointer_U() const;
    int* get_pointer_W() const;
    int* get_pointer_I() const;
    int* get_pointer_K() const;

    int get_x() const;
    int get_y() const;
    int get_z() const;
    int get_A() const;
    int get_B() const;
    int get_C() const;
    int get_U() const;
    int get_W() const;
    int get_I() const;
    int get_K() const;

    //setters
    void set_x(int _x);
    void set_y(int _y);
    void set_z(int _z);
    void set_A(int _A);
    void set_B(int _B);
    void set_C(int _C);
    void set_U(int _U);
    void set_W(int _W);
    void set_I(int _I);
    void set_K(int _K);
    void flush();       //flush the content of AxPath
    //contructors
    AxPath();

    // operator overloading
    friend QDebug operator<<(QDebug dbg, const AxPath _axpath);

private:    //axe x=-Z axe y=x
   int *x;
   int *y;
   int *z;
   int *A;
   int *B;
   int *C;
   int *U;
   int *W;
   int *I;   //offset of the center of the circle from the starting point in x
   int *K;   //offset of the center of the circle from the starting point in z
};

#endif // AXPATH_H
