#ifndef AXLINE_H
#define AXLINE_H

#include <QLineF>
#include <QPointF>
#include <QStyle>
#include <AxBorder.h>
#include <QDebug>

#include <iostream>
using namespace std;


class AxLine : public AxBorder
{
    public:

        //Constructor

        AxLine(const QLine &line);
        AxLine(AxBorder *border);
        AxLine(QLineF _line);
        AxLine(qreal x1, qreal y1, qreal x2, qreal y2);
        AxLine(const QPointF &p1, const QPointF &p2);
        AxLine();

        // Destructor
        virtual ~AxLine();

        //getters

        virtual QLineF get_mediatrice();// return 2 points of the mediatrice of the line
        virtual QPointF middle_point();// return the middle point between p1 and p2
        virtual int getObjectType();

        //Plot

        virtual void affiche(QPaintDevice *device, int width, const QColor &color); //graphical plot
        friend QDebug operator<<(QDebug dbg, const AxLine &type);//command plot
        virtual void f(QDebug& dbg) const;

        // operator overloading

        bool operator!=(const QLineF &line) const;
        bool operator==(const AxLine &line) const;


    private:
};

#endif // AXLINE_H
