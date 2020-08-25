#include "AxPath.h"

//getters
int* AxPath::get_pointer_x() const
{
    return x;
};
int* AxPath::get_pointer_y() const
{
    return y;
};
int* AxPath::get_pointer_z() const
{
    return z;
};
int* AxPath::get_pointer_A() const
{
    return A;
};
int* AxPath::get_pointer_B() const
{
    return B;
};
int* AxPath::get_pointer_C() const
{
    return C;
};
int* AxPath::get_pointer_U() const
{
    return U;
};
int* AxPath::get_pointer_W() const
{
    return W;
};
int* AxPath::get_pointer_I() const
{
    return I;
};
int* AxPath::get_pointer_K() const
{
    return K;
};

int AxPath::get_x() const
{
    return *x;
};
int AxPath::get_y() const
{
    return *y;
};
int AxPath::get_z() const
{
    return *z;
};
int AxPath::get_A() const
{
    return *A;
};
int AxPath::get_B() const
{
    return *B;
};
int AxPath::get_C() const
{
    return *C;
};
int AxPath::get_U() const
{
    return *U;
};
int AxPath::get_W() const
{
    return *W;
};
int AxPath::get_I() const
{
    return *I;
};
int AxPath::get_K() const
{
    return *K;
};

//setters
void AxPath::set_x(int _x)
{
    x=new int(_x);
};
void AxPath::set_y(int _y)
{
    y=new int(_y);
};
void AxPath::set_z(int _z)
{
    z=new int(_z);
};
void AxPath::set_A(int _A)
{
    A=new int(_A);
};
void AxPath::set_B(int _B)
{
    B=new int(_B);
};
void AxPath::set_C(int _C)
{
    C=new int(_C);
};
void AxPath::set_U(int _U)
{
    U=new int(_U);
};
void AxPath::set_W(int _W)
{
    W=new int(_W);
};
void AxPath::set_I(int _I)
{
    I=new int(_I);
};
void AxPath::set_K(int _K)
{
    K=new int(_K);
};
void AxPath::flush()
{
    delete x;
    x=0;
    delete y;
    y=0;
    delete z;
    z=0;
    delete A;
    A=0;
    delete B;
    B=0;
    delete C;
    C=0;
    delete U;
    U=0;
    delete W;
    W=0;
    delete I;
    I=0;
    delete K;
    K=0;
};
//constructor

AxPath::AxPath()
{
  x=0;
  y=0;
  z=0;
  A=0;
  B=0;
  C=0;
  U=0;
  W=0;
  I=0;
  K=0;
};

// operator overloading
QDebug operator<<(QDebug dbg, const AxPath _axpath)
{
    int* pointer(0);
    for(int i=0; i<10; ++i)
    {
        switch (i) {
        case(0):
            pointer=_axpath.get_pointer_x();
            if(pointer!=NULL)
            {
                dbg.nospace() << "x= ";
                dbg.nospace() << *pointer << endl;
            }
            break;
        case(1):
            pointer=_axpath.get_pointer_y();
            if(pointer!=NULL)
            {
                dbg.nospace() << "y= ";
                dbg.nospace() << *pointer << endl;
            }
            break;
        case(2):
            pointer=_axpath.get_pointer_z();
            if(pointer!=NULL)
            {
                dbg.nospace() << "z= ";
                dbg.nospace() << *pointer << endl;
            }
            break;
        case(3):
            pointer=_axpath.get_pointer_A();
            if(pointer!=NULL)
            {
                dbg.nospace() << "A= ";
                dbg.nospace() << *pointer << endl;
            }
            break;
        case(4):
            pointer=_axpath.get_pointer_B();
            if(pointer!=NULL)
            {
                dbg.nospace() << "B= ";
                dbg.nospace() << *pointer << endl;
            }
            break;
        case(5):
            pointer=_axpath.get_pointer_C();
            if(pointer!=NULL)
            {
                dbg.nospace() << "C= ";
                dbg.nospace() << *pointer << endl;
            }
            break;
        case(6):
            pointer=_axpath.get_pointer_U();
            if(pointer!=NULL)
            {
                dbg.nospace() << "U= ";
                dbg.nospace() << *pointer << endl;
            }
            break;
        case(7):
            pointer=_axpath.get_pointer_W();
            if(pointer!=NULL)
            {
                dbg.nospace() << "W= ";
                dbg.nospace() << *pointer << endl;
            }
            break;
        case(8):
            pointer=_axpath.get_pointer_I();
            if(pointer!=NULL)
            {
                dbg.nospace() << "I= ";
                dbg.nospace() << *pointer << endl;
            }
            break;
        case(9):
            pointer=_axpath.get_pointer_K();
            if(pointer!=NULL)
            {
                dbg.nospace() << "K= ";
                dbg.nospace() << *pointer << endl;
            }
            break;
        }
    }
    return dbg.maybeSpace();
};
