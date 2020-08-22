#include "axtriangle.h"
#include "math.h"
#include "common_functions.h"
#include <QPainter>

AxTriangle::AxTriangle()
{

}

AxTriangle::AxTriangle(QPoint _p1, QPoint _p2, QPoint _p3)
{
    p1=_p1;
    p2=_p2;
    p3=_p3;
}

QPoint AxTriangle::getp1()
{
    return p1;
}

QPoint AxTriangle::getp2()
{
    return p2;
}

QPoint AxTriangle::getp3()
{
    return p3;
}

AxLine AxTriangle::getline1()
{
    return AxLine(p1,p2);
}

AxLine AxTriangle::getline2()
{
    return AxLine(p2,p3);
}

AxLine AxTriangle::getline3()
{
    return AxLine(p3,p1);
}

AxCircle AxTriangle::cercle_inscrit()
{
    QPoint pm1,pm2,pm3,pm4,pc;
    int a,b,c,R;
    QVector<QVector<int>> cross_med;

    //  calcul de 2 pts appartement aux médiatrice

    // equation mediatrice : 2(x2-x1)*x+2(y2-y1)*y+(x1^2+y1^2-x2^2-y2^2)=0

    // mediatrice 1:
    pm1=QPoint((p1.x()+p2.x())/2,((p1.y()+p2.y())/2));
    pm2.setY(100);
    a=2*(p2.x()-p1.x());
    b=2*(p2.y()-p1.y());
    c=pow(p1.x(),2)+pow(p1.y(),2)-pow(p2.x(),2)-pow(p2.y(),2);
    pm2.setX((-c-b*pm2.y())/a);

    // mediatrice 2:
    pm3=QPoint((p3.x()+p2.x())/2,(p3.y()+p2.y())/2);
    pm4.setY(100);
    a=2*(p2.x()-p3.x());
    b=2*(p2.y()-p3.y());
    c=pow(p3.x(),2)+pow(p3.y(),2)-pow(p2.x(),2)-pow(p2.y(),2);
    pm4.setX((-c-b*pm4.y())/a);

    cross_med=intersect_lignes(QLine(pm1,pm2),QLine(pm3,pm4));
    pc=QPoint(cross_med[1][0],cross_med[1][1]);
    R=distance(pc,p1);

    return AxCircle(R,pc);
}

void AxTriangle::display(QPaintDevice *device, int width, const QColor &color)
{
    QPainter painter(device);
    QPen pen;
    pen.setWidth(width);
    pen.setColor(color);
    painter.setPen(pen);
    painter.drawLine(p1,p2);
    painter.drawLine(p2,p3);
    painter.drawLine(p1,p3);
}

QDebug operator<<(QDebug dbg, const AxTriangle &type)
{
    dbg.nospace() << "AxTriangle(" << type.p1 << "," << type.p2 << "," << type.p3 << ")"<<endl;
    return dbg.maybeSpace();
}
