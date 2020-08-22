#ifndef AXCIRCLE_H
#define AXCIRCLE_H

#include <QLineF>
#include <QPointF>


class AxCircle
{
public:
    AxCircle();
    virtual ~AxCircle(); //destructeur de classe
    AxCircle(qreal _R, QPointF _center);
    QPointF get_center();
    qreal get_R();
private:
    qreal R;
    QPointF center;
};

#endif // AXCIRCLE_H
