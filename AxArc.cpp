#include "AxArc.h"
#include <iostream>
using namespace std;
#include "math.h"
#include <QDebug>
#include <QPainter>
#include <QDebug>
#include <AxLine.h>
#include <common_functions.h>

AxArc:: AxArc()
{
    limits=QLine();
    C=QPoint(0,0);
    R=0;
    clockwise=0;
};

AxArc:: AxArc(QLine _limits, QPoint _center, int _R, int _clockwise)
{
    limits=_limits;
    C=_center;
    R=_R;
    clockwise=_clockwise;
}

AxArc::AxArc(AxBorder *border)
{
    AxArc *arc;
    arc=dynamic_cast<AxArc*>(border);

    limits=border->get_limits();
    C=arc->get_center();
    R=arc->get_R();
    clockwise=arc->is_clockwise();

}

AxArc::AxArc(QPoint _center, int _R)
{
    C=_center;
    R=_R;
    clockwise=0;
    limits=QLine();
}
QPoint AxArc::middle_point()
{
    // return the point in the middle of the arc
    // error if circle
    QPoint p1=get_limits().p1();
    QPoint p2=get_limits().p2();
    QVector<QPoint> cross_pts;

    AxLine segment=AxLine(p1,p2);

    if (segment.center()==C)
    {
        qDebug()<<"possibly a problem in AxArc-> middle_point ";
    }

    else
    {
        QVector<QVector<int>> cross_med=intersect_arc_line(QLine(C,segment.center()),AxArc(*this));

        cross_pts=intersecting_points(cross_med);
        if (p1!=p2)
        {
            for (int k=0;k<cross_pts.size();k++)
            {
                if (arc_test(cross_pts[k]))
                {
                    return cross_pts[k];
                }
            }
        }
        if (p1==p2)
        {
            for (int k=0;k<cross_pts.size();k++)
            {
                if (p1!=(cross_pts[k]))
                {
                    return cross_pts[k];
                }
            }
        }
    }
}



AxArc:: ~AxArc()
{
};

QPoint AxArc :: get_center()
{
    return C;
}
int AxArc :: get_R()
{
    return R;
}
bool AxArc :: is_clockwise()
{
    return clockwise ;
}

bool AxArc::is_cw()
{
    return clockwise;
}
bool AxArc::is_ccw()
{
    return !clockwise;
}

void AxArc::set_cw(bool way)
{
    if (this->is_cw()!= way)
    {
     limits = QLine(limits.p2(),limits.p1()) ;

    }
    clockwise = way;

}

void AxArc::set_ccw(bool way)
{
    way = !way;
    if (this->is_cw()!= way)
    {
     limits = QLine(limits.p2(),limits.p1()) ;

    }
    clockwise = way;

}

void AxArc :: display(QPaintDevice *device, int width, const QColor &color)
{
    // display l'arc associé

    int startAngle,spanAngle,teta1,teta2;
    QPoint p1=limits.p1();
    QPoint p2=limits.p2();
    QPoint pc=C;

    if (p1==p2) // les deux points sont les memes
    {
    // je considère le cercle en entier
        startAngle=0;
        spanAngle=2*M_PI*16*180/M_PI;
    }// les deux points ne sont les mêmes
    else
    {
    // from coordinates of arc pts to angle

    teta1=this->angle(p1);
    teta2=this->angle(p2);

        //Sens de l'arc ?

        // sens anti horaire
        if (clockwise==0)
        {
            startAngle = -16*(teta2)*180/(M_PI);
            if (teta2>teta1)
            {
                spanAngle = 16*((teta2-teta1))*(180/(M_PI));//
            }
            else
            {
                spanAngle = 16*(2*M_PI-(teta1-teta2))*(180/(M_PI));
            }
        }
        // Sens horaire
        else
        {
            startAngle = -16*(teta1)*180/(M_PI);
            if (teta2>teta1)
            {
               spanAngle = 16*(2*M_PI-(teta2-teta1))*(180/(M_PI));
            }
            else
            {
                spanAngle =-16*((teta2-teta1))*(180/(M_PI));
            }
        }
    }
    QRectF rectangle(pc.x()-R, pc.y()-R, 2*R, 2*R);

    QVector<int> rect_angle;

    rect_angle.append(pc.x()-R);
    rect_angle.append(pc.y()-R);
    rect_angle.append(2*R);
    rect_angle.append(2*R);
    rect_angle.append(startAngle);
    rect_angle.append(spanAngle);

    QPainter painter(device);
    QPen pen;
    pen.setWidth(width);
    pen.setColor(color);
    painter.setPen(pen);
    painter.drawArc(rect_angle[0],rect_angle[1],rect_angle[2],rect_angle[3], rect_angle[4], rect_angle[5]);

   // return(rect_angle);


}

int AxArc::getObjectType()
{
    return 1;
}

void AxArc::f(QDebug& dbg) const
{
    dbg.nospace() << "AxArc("<< limits << "," << C  << ","<< R << ", " <<clockwise<<")"<<endl;
}

void AxArc::translate(int offset)
{

}

bool AxArc::operator==(const AxArc &arc) const
{
    if ((C==arc.C) && (R==arc.R) && (clockwise==arc.clockwise) && (limits==arc.limits))
    {
        return true;
    }
    else
    {
        return false;
    }
}

QDebug operator<<(QDebug dbg, const AxArc &type)
{
    dbg.nospace() << "AxArc("<< type.limits << "," << type.C  << ","<< type.R << ", " <<type.clockwise<<")"<<endl;
    return dbg.maybeSpace();
}

int AxArc :: angle(QPoint p)// Calculate the positive angle of the point based on the certer of my arc
{
    QPoint pc = this->get_center();
    int teta;

    // I calculate my reals angle in positive

    teta=atan((p.y()-pc.y())/(p.x()-pc.x()));


    if (p.x()-pc.x()<0)
    {
        teta=teta+M_PI;
    }

    // If angle négative => positive

    if (teta<0)
    {
        teta=teta+2*M_PI;
    }
    return teta;

}


int AxArc :: arc_test(QPoint px)// test si l'angle d'un point selon l'arc de cercle est compris dans l'angle des 2 points d'un arc
{

    // prend un un arc de cercle et un point appartement au cercle en argument et défini si le point se situe sur l'arc
    int teta1, teta2, tetax;
    QPoint p1= limits.p1();
    QPoint p2= limits.p2();
    int clockwise = this->is_clockwise();
    // les deux points formant l'arc sont les mêmes ?
    if (p1==p2)
    {
        // l'arc est un cercle
        return 1;
    }
    else
    {
        teta1=this->angle(p1);
        teta2=this->angle(p2);
        tetax=this->angle(px);

        //methode Charles

        if (clockwise==1) // clockwise
        {

            if (float(tetax)<=float(teta1))
            {
                tetax=tetax+360;
            }
            if (teta2<teta1)
            {
                teta2=teta2+360;
            }
            if (float(tetax)>=float(teta2))
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        else //counter clockwise
        {
            if (float(teta2)<=float(teta1))
            {
                teta2=teta2+360;
            }
            if (tetax<teta1)
            {
                 tetax=tetax+360;
            }
            if (float(tetax)<=float(teta2))
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
}


QLine AxArc::get_mediatrice()
{
    QLine line(limits);
    QPoint pp1((line.p1().x()+line.p2().x())/2,((line.p1().y()+line.p2().y())/2));
    QPoint pp2(this->middle_point());
    return (QLine(pp1,pp2));
}
