#include "AxCircle.h"

AxCircle::AxCircle()
{
    center=QPoint(0,0);
    R=0;
}

AxCircle::AxCircle(int _R,QPoint _center)
{
    R=_R;
    center=_center;
}

AxCircle:: ~AxCircle()
{
};

QPoint AxCircle :: get_center()
{
    return center;
}
int AxCircle :: get_R()
{
    return R;
}
