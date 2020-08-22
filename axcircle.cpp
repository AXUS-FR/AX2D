#include "AxCircle.h"

AxCircle::AxCircle()
{
    center=QPoint(0,0);
    R=0;
}

AxCircle::AxCircle(qreal _R,QPoint _center)
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
qreal AxCircle :: get_R()
{
    return R;
}
