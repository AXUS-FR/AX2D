#include "AxCircle.h"

AxCircle::AxCircle()
{
    center=QPointF(0,0);
    R=0;
}

AxCircle::AxCircle(qreal _R,QPointF _center)
{
    R=_R;
    center=_center;
}

AxCircle:: ~AxCircle()
{
};

QPointF AxCircle :: get_center()
{
    return center;
}
qreal AxCircle :: get_R()
{
    return R;
}
