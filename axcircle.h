#ifndef AXCIRCLE_H
#define AXCIRCLE_H

#include <QLine>
#include <QPoint>


class AxCircle
{
public:
    AxCircle();
    virtual ~AxCircle(); //destructeur de classe
    AxCircle(int _R, QPoint _center);
    QPoint get_center();
    int get_R();
private:
    int R;
    QPoint center;
};

#endif // AXCIRCLE_H
