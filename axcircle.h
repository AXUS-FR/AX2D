#ifndef AXCIRCLE_H
#define AXCIRCLE_H

#include <QLine>
#include <QPoint>


class AxCircle
{
public:
    AxCircle();
    virtual ~AxCircle(); //destructeur de classe
    AxCircle(qreal _R, QPoint _center);
    QPoint get_center();
    qreal get_R();
private:
    qreal R;
    QPoint center;
};

#endif // AXCIRCLE_H
