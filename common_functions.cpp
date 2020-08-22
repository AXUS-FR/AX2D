#include "common_functions.h"
#include <math.h>
#include <QRandomGenerator64>
#include <QDebug>
#include <iostream>
using namespace std;
#include <AxShape.h>

qreal random(qreal min, qreal max)
{
    QRandomGenerator64 rd;
    rd=QRandomGenerator64::securelySeeded();
    return (min+max*std::generate_canonical<qreal, std::numeric_limits<qreal>::digits>(rd));
}

QVector<QVector<qreal>> intersect_border(AxBorder *border1,AxBorder *border2)
{
    // detect the intersection between 2 borders

    if  (border1->getObjectType()==0 && border2->getObjectType()==0)
    {
        return intersect(AxLine(border1), AxLine(border2));
    }
    if  (border1->getObjectType()==1 && border2->getObjectType()==0)
    {
        return intersect(AxLine(border2), AxArc(border1));
    }
    if  (border1->getObjectType()==0 && border2->getObjectType()==1)
    {
        return intersect(AxLine(border1), AxArc(border2));
    }
    if  (border1->getObjectType()==1 && border2->getObjectType()==1)
    {
        return intersect(AxArc(border1), AxArc(border2));
    }
}




QVector<QVector<qreal>> intersect_shape(AxBorder *border, AxShape shape)
{
    // detect the intersection between a border and a shape
    QVector<QVector<qreal>> cross_test,cross_list;
    int cross=0;
    QVector<qreal> vect;
    for (int k=0;k<shape.size();k++)
    {
        cross_test=intersect_border(border,shape[k]);
        if (cross_test[0][0]==0)
        {
            cross=1;
            for (int n=1;n<cross_test.size();n++)
            {
                cross_list.append(cross_test[n]);
            }
        }
    }
    vect.append(cross);
    cross_list.prepend(vect);
    return cross_list;

}


bool not_equal(QPointF p1, QPointF p2,double epsilon)
{


    if ((fabs(p1.x()-p2.x())<epsilon) && (fabs(p1.y()-p2.y())<epsilon))
    {
        return false;
    }

    return true;
}

bool not_equal(qreal a,qreal b)
{
    double epsilon=0.0001;

    return !(fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon));
}

bool equal(qreal a,qreal b)
{
    double epsilon=0.0001;

    return (fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon));
}

bool toggle(bool a)
{
    if (a==true)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int toggle(int a)
{
    if (a==1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

qreal distance(QPointF p1, QPointF p2)
{
    return(sqrt(pow((p1.x()-p2.x()),2)+pow((p1.y()-p2.y()),2)));
}

int sign(qreal a)
{
    if (a<0)
    {
        return(-1);
    }
    if (a>0)
    {
        return(1);
    }
    if (a==0)
    {
        return 0;
    }
}

QVector<QPointF> intersecting_points(QVector<QVector<qreal>> cross_test)
{
    // extract the cross pts of a intersect fct
    QVector<QPointF> cross_pts;
    if(cross_test[0][0]==0)
    {
        return cross_pts;
    }
    for (int k=0;k<cross_test.size()-1;k++)
    {
        cross_pts.append(QPointF(cross_test[k+1][0],cross_test[k+1][1]));
    }

    return cross_pts;
}


QPointF closest_cross_to_pt(QVector<QVector<qreal>> cross_test, QPointF pm)
{
//    return the closest point to pm
    QPointF closest_point=QPointF(cross_test[1][0],cross_test[1][1]);
    qreal closest_distance=distance(closest_point,pm);
    for  (int k=2;k<cross_test.size();k++)
    {
        qreal d2=distance(QPointF(cross_test[k][0],cross_test[k][1]),pm);
        if (d2<closest_distance)
        {
            closest_distance=d2;
            closest_point=QPointF(cross_test[k][0],cross_test[k][1]);
        }
    }
    return closest_point;
}


int closest_pt_to_p1_border(QVector<QPointF> cross_list, AxBorder *border)
{
    // return the position of the closest point on the list to the border (based on the 1st point on the border)
    // cas particuliers pas pris en compte
    QPointF pm=border->p1();
    qreal closest_distance;
    int closest_pos;
    // border = segment ?
    if (border->getObjectType()==0)
    {
        closest_pos=0;
        closest_distance=distance(cross_list[closest_pos],pm);
        for  (int k=1;k<cross_list.size();k++)
        {
            qreal d2=distance(cross_list[k],pm);
            if (d2<closest_distance)
            {
                closest_distance=d2;
                closest_pos=k;
            }
        }
    }
// border= arc
    else
    {
        qreal tetax, dteta;
        qreal teta1=AxArc(border).angle(border->p1());

        tetax=AxArc(border).angle(cross_list[0]);

        if (AxArc(border).is_clockwise()==0)
        {
            if (float(tetax)>=float(teta1))
            {
                closest_distance=tetax-teta1;
                closest_pos=0;
            }
            else
            {
                closest_distance=(tetax-teta1)+2*M_PI;
                closest_pos=0;
            }
        }
        // clockwise=1

        else
        {

            // initialisation
            if (float(teta1)>=float(tetax))
            {
                closest_distance=teta1-tetax;
                closest_pos=0;
            }
            else
            {
                closest_distance=(teta1-tetax)+2*M_PI;
                closest_pos=0;
            }

        }

        // test
        for (int k=1;k<cross_list.size();k++)
        {
            tetax=AxArc(border).angle(cross_list[k]);

            if (AxArc(border).is_clockwise()==0)
            {
                if (float(tetax)>=float(teta1))
                {
                    dteta=tetax-teta1;
                }
                else
                {
                    dteta=(tetax-teta1)+2*M_PI;                            ;
                }
            }
            // clockwise=1
            else
            {
                if (float(teta1)>=float(tetax))
                {
                    dteta=teta1-tetax;
                }
                else
                {
                    dteta=(teta1-tetax)+2*M_PI;
                }
            }
            if (dteta<closest_distance)
            {
                closest_pos=k;
                closest_distance=dteta;
            }
        }
    }
    return closest_pos;
}

int closest_pt_to_p2_border(QVector<QPointF> cross_list, AxBorder *border)
{
    // return the position of the closest point on the list to the border (based on the 1st point on the border)
    // cas particuliers pas pris en compte
    QPointF pm=border->p2();
    qreal closest_distance;
    int closest_pos;
    // border = segment ?
    if (border->getObjectType()==0)
    {
        closest_pos=0;
        closest_distance=distance(cross_list[closest_pos],pm);
        for  (int k=1;k<cross_list.size();k++)
        {
            qreal d2=distance(cross_list[k],pm);
            if (d2<closest_distance)
            {
                closest_distance=d2;
                closest_pos=k;
            }
        }
    }
// border= arc
    else
    {
        qreal tetax, dteta;
        qreal teta1=AxArc(border).angle(border->p1());

        tetax=AxArc(border).angle(cross_list[0]);

        if (AxArc(border).is_clockwise()==1)
        {
            if (float(tetax)>=float(teta1))
            {
                closest_distance=tetax-teta1;
                closest_pos=0;
            }
            else
            {
                closest_distance=(tetax-teta1)+2*M_PI;
                closest_pos=0;
            }
        }
        // clockwise=1

        else
        {

            // initialisation
            if (float(teta1)>=float(tetax))
            {
                closest_distance=teta1-tetax;
                closest_pos=0;
            }
            else
            {
                closest_distance=(teta1-tetax)+2*M_PI;
                closest_pos=0;
            }

        }

        // test
        for (int k=1;k<cross_list.size();k++)
        {
            tetax=AxArc(border).angle(cross_list[k]);

            if (AxArc(border).is_clockwise()==1)
            {
                if (float(tetax)>=float(teta1))
                {
                    dteta=tetax-teta1;
                }
                else
                {
                    dteta=(tetax-teta1)+2*M_PI;                            ;
                }
            }
            // clockwise=1
            else
            {
                if (float(teta1)>=float(tetax))
                {
                    dteta=teta1-tetax;
                }
                else
                {
                    dteta=(teta1-tetax)+2*M_PI;
                }
            }
            if (dteta<closest_distance)
            {
                closest_pos=k;
                closest_distance=dteta;
            }
        }
    }
    return closest_pos;
}

bool equal(AxBorder *border1, AxBorder *border2)
{
    if ((border1->getObjectType()==0) && (border2->getObjectType()==0))
    {

        if (border2->p1()==border1->p2())
        {
            if (border2->p2()==border1->p1())
            {
                return true;
            }
        }
        if (border2->p2()==border1->p2())
        {
            if (border2->p1()==border1->p1())
            {
                return true;
            }
        }
        return false;

    }
    else if ((border1->getObjectType()==0) && (border2->getObjectType()==1))
    {
        return false;
    }
    else if ((border1->getObjectType()==1) && (border2->getObjectType()==0))
    {
        return false;
    }
    else if ((border1->getObjectType()==1) && (border2->getObjectType()==1))
    {
        if ((AxArc(border2).get_center()==AxArc(border1).get_center()) && (AxArc(border2).get_R()==AxArc(border1).get_R()) && (AxArc(border2).is_clockwise()==AxArc(border1).is_clockwise()) && (border2->get_limits()==border1->get_limits()))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

}

qreal angle(AxBorder *border1, AxBorder *border2)
{
    QPointF p11 = border1->p1();
    QPointF p12 = border1->p2();
    QPointF p21 = border2->p1();
    QPointF p22 = border2->p2();

    qreal teta1,teta2,teta;

    // I calculate my reals angle in positive

    teta1=atan((p11.y()-p12.y())/(p11.x()-p12.x()));
    if (p11.x()-p12.x()<0)
    {
        teta1=teta1+M_PI;
    }

    // If angle négative => positive

    if (teta1<0)
    {
        teta1=teta1+2*M_PI;
    }

    teta2=atan((p21.y()-p22.y())/(p21.x()-p22.x()));
    if (p21.x()-p22.x()<0)
    {
        teta2=teta2+M_PI;
    }

    // If angle négative => positive

    if (teta2<0)
    {
        teta2=teta2+2*M_PI;
    }
    teta=teta1-teta2;

    if (teta<0)
    {
        teta=teta+2*M_PI;
    }
    return teta;
}

//int inside_form(QPointF pm, AxShape form)
//{
//    QPointF p2=QPointF(pm.x(),pm.y());
//    QVector<QVector<qreal>>cross_test;
//    int left_cross=0;
//    // count the number of time the segment pm-p2 cross the object

//    for (int k=0;k<form.size();k++)
//    {
//        // if border = line
//        if (form[k]->getObjectType()==0)
//        {
//        cross_test=intersect_line_segment(QLineF(pp1,pp2),QLineF(form[k]->p1(),form[k]->p2()));
//        }
//        // if border = arc
//        else
//            {
//                cross_test=intersect(QLineF(pp1,pp2),AxArc(form[k]));
//            }
//            if (cross_test[0][0]==1)
//            {
//                //add ont to left cross for each intersection
//               if (cross_test[1][0]>pm.x())
//               {
//                   left_cross++;
//               }
//            }
//     }
////                nombre pair de croisement à gauche ?
//      if (left_cross%2==0)
//      //aller à gauche
//      {
//           sens=1;
//      }
//      else
//      //aller à droite
//      {
//          sens=-1;
//      }

//}

QVector<QVector<qreal>> intersect_arc_line(QLineF line, AxArc arc)
{
    QVector<QVector<qreal>> ret;
    qreal x1,y1,x2,y2,R,xc,yc;
    QVector<qreal> ret_line;

    x1=line.x1();
    y1=line.y1();
    x2=line.x2();
    y2=line.y2();

    R=arc.get_R();
    xc=arc.get_center().x();
    yc=arc.get_center().y();

    int cross=0;
    qreal a, b, A, B, C, delta, X1, X2,Y1,Y2,X,Y;

    // segment almost vertical ?
     if (float(x2)==float(x1))
    {
        // calcul coeficient directeur impossible donc methode géométrique
        X1=x1;
        X2=x2;

        if (pow(R,2)-pow((X1-xc),2)>0)
        {
            Y1=sqrt(pow(R,2)-pow((x1-xc),2))+yc;
            Y2=-sqrt(pow(R,2)-pow((x1-xc),2))+yc;


            if (arc.arc_test(QPointF(X1,Y1))) // point de croisement 1 sur arc de cercle ?
            {
//                if ((Y1<y1 && Y1>y2) || (Y1<y2 && Y1>y1)) // le point d'intersection appartient au segment ?
                {
                    cross=1;
                    ret_line.append(X1);
                    ret_line.append(Y1);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
            }
            if (arc.arc_test(QPointF(X2,Y2))) // point de croisement 1 sur arc de cercle ?
            {
//                if ((Y2<y1 && Y2>y2) || (Y2<y2 && Y2>y1)) // le point d'intersection appartient au segment ?
                {
                    cross=1;
                    ret_line.append(X2);
                    ret_line.append(Y2);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
            }
        }
    }
    else // segment non vertical
    {
        a = (y2-y1)/(x2-x1);
        b=y1-(a*x1);

        A=1+(pow(a,2));
        B=2*(-xc+(a*b)-(a*yc));
        C=(pow(xc,2))+(pow((b-yc),2))-(pow(R,2));

        delta=(pow(B,2))-(4*A*C);

        if (delta>0) // 2 pt d'intersection entre le cercle et la droite asociée ?
        {
            X1=(-B-sqrt(delta))/(2*A);
            X2=(-B+sqrt(delta))/(2*A);

            Y1=a*X1+b;
            Y2=a*X2+b;

            if (arc.arc_test(QPointF(X1,Y1))) // point de croisement 1 sur arc de cercle ?
            {
//                if ((X1<x1 && X1>x2) || (X1<x2 && X1>x1)) //point de croisement sur le segment ?
                {
                    cross=1;
                    ret_line.append(X1);
                    ret_line.append(Y1);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
            }
            if (arc.arc_test(QPointF(X2,Y2))) // point de croisement 2 sur arc de cercle ?
            {
//                if ((X2<x1 && X2>x2) || (X2<x2 && X2>x1)) //point de croisement sur le segment ?
                {
                    cross=1;
                    ret_line.append(X2);
                    ret_line.append(Y2);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    }
                }
            }
        else if (delta==0)
        {
            X=(-B)/(2*A);
            Y=a*X+b;
            if (arc.arc_test(QPointF(X,Y))) // point de croisement 1 sur arc de cercle ?
            {
//                if ((X<x1 && X>x2) || (X<x2 && X>x1)) //point de croisement sur le segment ?
                {
                    cross=1;
                    ret_line.append(X);
                    ret_line.append(Y);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
            }
        }
    }
    ret_line.append(cross);
    ret.prepend(ret_line);
    return ret;
}


QVector<QVector<qreal>> intersect_arc_demi_line(AxLine line, AxArc arc)
{
    QVector<QVector<qreal>> ret;
    qreal x1,y1,x2,y2,R,xc,yc;
    QVector<qreal> ret_line;
    bool sens;

    x1=line.x1();
    y1=line.y1();
    x2=line.x2();
    y2=line.y2();

    R=arc.get_R();
    xc=arc.get_center().x();
    yc=arc.get_center().y();

    int cross=0;
    qreal a, b, A, B, C, delta, X1, X2,Y1,Y2,X,Y;

    // segment almost vertical ?
     if (float(x2)==float(x1))
    {
        // calcul coeficient directeur impossible donc methode géométrique
        sens=sign(y2-y1);
        X1=x1;
        X2=x2;

        if (pow(R,2)-pow((X1-xc),2)>0)
        {
            Y1=sqrt(pow(R,2)-pow((x1-xc),2))+yc;
            Y2=-sqrt(pow(R,2)-pow((x1-xc),2))+yc;


            if (arc.arc_test(QPointF(X1,Y1))) // point de croisement 1 sur arc de cercle ?
            {
                if (sign(Y1-y1)==sens) // point d'intersection dans le bon sens ?
                {
                    cross=1;
                    ret_line.append(X1);
                    ret_line.append(Y1);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
            }
            if (arc.arc_test(QPointF(X2,Y2))) // point de croisement 1 sur arc de cercle ?
            {
                if (sign(Y2-y1)==sens) // le point d'intersection appartient au segment ?
                {
                    cross=1;
                    ret_line.append(X2);
                    ret_line.append(Y2);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
            }
        }
    }
    else // segment non vertical
    {
        sens=sign(x2-x1);
        a = (y2-y1)/(x2-x1);
        b=y1-(a*x1);

        A=1+(pow(a,2));
        B=2*(-xc+(a*b)-(a*yc));
        C=(pow(xc,2))+(pow((b-yc),2))-(pow(R,2));

        delta=(pow(B,2))-(4*A*C);

        if (delta>0) // 2 pt d'intersection entre le cercle et la droite asociée ?
        {
            X1=(-B-sqrt(delta))/(2*A);
            X2=(-B+sqrt(delta))/(2*A);

            Y1=a*X1+b;
            Y2=a*X2+b;

            if (arc.arc_test(QPointF(X1,Y1))) // point de croisement 1 sur arc de cercle ?
            {
                if (sign(X1-x1)==sens) //point de croisement sur le segment ?
                {
                    cross=1;
                    ret_line.append(X1);
                    ret_line.append(Y1);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
            }
            if (arc.arc_test(QPointF(X2,Y2))) // point de croisement 2 sur arc de cercle ?
            {
                if (sign(X2-x1)==sens) //point de croisement sur le segment ?
                {
                    cross=1;
                    ret_line.append(X2);
                    ret_line.append(Y2);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    }
                }
            }
        else if (delta==0)
        {
            X=(-B)/(2*A);
            Y=a*X+b;
            if (arc.arc_test(QPointF(X,Y))) // point de croisement 1 sur arc de cercle ?
            {
                if (sign(X-x1)==sens)
                {
                    cross=1;
                    ret_line.append(X);
                    ret_line.append(Y);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
            }
        }
    }
    ret_line.append(cross);
    ret.prepend(ret_line);
    return ret;
}

QVector <QVector<qreal>> intersect_lignes(QLineF line1, QLineF line2)
{
    int cross=0;
    qreal x1, y1, x2, y2, x3, y3, x4, y4,a1,a2,b1,b2;
    qreal xc =0,yc =0;
    QVector<qreal> ret_line;

    x1=line1.x1();
    y1=line1.y1();
    x2=line1.x2();
    y2=line1.y2();

    x3=line2.x1();
    y3=line2.y1();
    x4=line2.x2();
    y4=line2.y2();

    QVector<QVector<qreal>> ret;

    cross =0;
    xc=0;
    yc=0;

    if (x2!=x1 && x4!=x3)	//If not vertical
    {
        a1 = (y2-y1)/(x2-x1) ;
        a2 = (y4-y3)/(x4-x3);
        cross=0;

        if (a1==a2) //If colinear
        {

            b1=y1-(a1*x1);
            b2=y3-(a2*x3);
            cross=2;
            ret_line.append(xc);
            ret_line.append(yc);
            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();

            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;

         }
        else // if they are not parallels
        {
            b1=y1-(a1*x1);
            b2=y3-(a2*x3);
            xc=(b2-b1)/(a1-a2);
            yc=a1*xc+b1;
            cross =1;

            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            ret_line.append(xc);
            ret_line.append(yc);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;
        }

    }
    else if ((x2==x1 && x4!=x3))  //  If one segments or both of my segment are vertical
    {
        a2 = (y4-y3)/(x4-x3);
        b2=y3-(a2*x3);
        xc=x2;
        yc=a2*x2+b2;
        cross =1;

        ret_line.append(cross);
        ret.append(ret_line);
        ret_line=QVector<qreal>();
        ret_line.append(xc);
        ret_line.append(yc);
        ret.append(ret_line);
        ret_line=QVector<qreal>();
        return ret;
    }
    else if (x2!=x1 && x4==x3)
    {
        a1 = (y2-y1)/(x2-x1);
        b1=y1-(a1*x1);
        xc=x3;
        yc=a1*x3+b1;
        cross =1;

        ret_line.append(cross);
        ret.append(ret_line);
        ret_line=QVector<qreal>();
        ret_line.append(xc);
        ret_line.append(yc);
        ret.append(ret_line);
        ret_line=QVector<qreal>();
        return ret;
    }
    else if (x2==x1 && x4==x3)
    {
        cross=2;
        xc=0;
        yc=0;
    }
    ret_line.append(cross);
    ret.append(ret_line);
    ret_line=QVector<qreal>();
    ret_line.append(xc);
    ret_line.append(yc);
    ret.append(ret_line);
    ret_line=QVector<qreal>();
    return ret;
}

QVector <QVector<qreal>> intersect_lignes(AxLine line1, AxLine line2)
{
    int cross=0;
    qreal x1, y1, x2, y2, x3, y3, x4, y4,a1,a2,b1,b2;
    qreal xc =0,yc =0;
    QVector<qreal> ret_line;

    x1=line1.x1();
    y1=line1.y1();
    x2=line1.x2();
    y2=line1.y2();

    x3=line2.x1();
    y3=line2.y1();
    x4=line2.x2();
    y4=line2.y2();

    QVector<QVector<qreal>> ret;

    cross =0;
    xc=0;
    yc=0;

    if (x2!=x1 && x4!=x3)	//If not vertical
    {
        a1 = (y2-y1)/(x2-x1) ;
        a2 = (y4-y3)/(x4-x3);
        cross=0;

        if (a1==a2) //If colinear
        {

            b1=y1-(a1*x1);
            b2=y3-(a2*x3);
            cross=2;
            ret_line.append(xc);
            ret_line.append(yc);
            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();

            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;

         }
        else // if they are not parallels
        {
            b1=y1-(a1*x1);
            b2=y3-(a2*x3);
            xc=(b2-b1)/(a1-a2);
            yc=a1*xc+b1;
            cross =1;

            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            ret_line.append(xc);
            ret_line.append(yc);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;
        }

    }
    else if ((x2==x1 && x4!=x3))  //  If one segments or both of my segment are vertical
    {
        a2 = (y4-y3)/(x4-x3);
        b2=y3-(a2*x3);
        xc=x2;
        yc=a2*x2+b2;
        cross =1;

        ret_line.append(cross);
        ret.append(ret_line);
        ret_line=QVector<qreal>();
        ret_line.append(xc);
        ret_line.append(yc);
        ret.append(ret_line);
        ret_line=QVector<qreal>();
        return ret;
    }
    else if (x2!=x1 && x4==x3)
    {
        a1 = (y2-y1)/(x2-x1);
        b1=y1-(a1*x1);
        xc=x3;
        yc=a1*x3+b1;
        cross =1;

        ret_line.append(cross);
        ret.append(ret_line);
        ret_line=QVector<qreal>();
        ret_line.append(xc);
        ret_line.append(yc);
        ret.append(ret_line);
        ret_line=QVector<qreal>();
        return ret;
    }
    else if (x2==x1 && x4==x3)
    {
        cross=2;
        xc=0;
        yc=0;
    }
    ret_line.append(cross);
    ret.append(ret_line);
    ret_line=QVector<qreal>();
    ret_line.append(xc);
    ret_line.append(yc);
    ret.append(ret_line);
    ret_line=QVector<qreal>();
    return ret;
}

int next_point(int n, AxShape form)
{
    int n2=n+1;
    if (n2==form.size())
    {
        n2=0;
    }
    return n2;
}

int next_point(int n, QVector<QPointF> form)
{
    int n2=n+1;
    if (n2==form.size())
    {
        n2=0;
    }
    return n2;
}

int next_point(int n, QVector<AxLine> form)
{
    int n2=n+1;
    if (n2==form.size())
    {
        n2=0;
    }
    return n2;
}


QVector <QVector<qreal>> intersect_line_segment(QLineF line, QLineF segment)
{
    int cross=0;
    qreal x1, y1, x2, y2, x3, y3, x4, y4,a1,a2,b1,b2;
    qreal xc_temp =0, yc_temp =0,xc =0,yc =0;
    QVector<qreal> ret_line;

    x1=line.x1();
    y1=line.y1();
    x2=line.x2();
    y2=line.y2();

    x3=segment.x1();
    y3=segment.y1();
    x4=segment.x2();
    y4=segment.y2();

    QVector<QVector<qreal>> ret;

    cross =0;
    xc=0;
    yc=0;

    if (x2!=x1 && x4!=x3)	//If not vertical
    {
        a1 = (y2-y1)/(x2-x1) ;
        a2 = (y4-y3)/(x4-x3);
        cross=0;

        if (a1==a2) //If colinear
        {

            b1=y1-(a1*x1);
            b2=y3-(a2*x3);
                if (b2 == b1)
                {
                    xc=(x3+x1)/2;
                    yc=(y3+y1)/2;

                    cross=2;

                    ret_line.append(cross);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    ret_line.append(xc);
                    ret_line.append(yc);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    return ret;
                }
                else
                {
                    cross =0;
                    xc=0;
                    yc=0;
                    cross=2;

                    ret_line.append(cross);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    ret_line.append(xc);
                    ret_line.append(yc);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    return ret;
                }


        }

        else // if they are not parallels
        {

            b1=y1-(a1*x1);
            b2=y3-(a2*x3);
            xc_temp=(b2-b1)/(a1-a2);
            yc_temp=a1*xc_temp+b1;

            if ((xc_temp<=x3 && xc_temp>=x4)||(xc_temp<=x4 && xc_temp>=x3)) // If my intersection pt is include inside the coordinate of my segments, this mean my segments cross
            {

                xc=xc_temp;
                yc=yc_temp;
                cross =1;

                ret_line.append(cross);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
                ret_line.append(xc);
                ret_line.append(yc);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
                return ret;
            }
        }
 }


    else if ((x2==x1 && x4!=x3))  //  If one segments or both of my segment are vertical
    {
        a2 = (y4-y3)/(x4-x3);
        b2=y3-(a2*x3);
        xc_temp=x2;
        yc_temp=a2*x2+b2;
        if ((x3<=xc_temp && x4>=xc_temp) || (x4<=xc_temp && x3>=xc_temp))
        {
            xc=xc_temp;
            yc=yc_temp;
            cross =1;

            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            ret_line.append(xc);
            ret_line.append(yc);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;
        }
    }
    else if (x2!=x1 && x4==x3)
    {
        a1 = (y2-y1)/(x2-x1);
        b1=y1-(a1*x1);
        xc_temp=x3;
        yc_temp=a1*x3+b1;
        if ((yc_temp<=y3 && yc_temp>=y4) || (yc_temp<=y4 && yc_temp>=y3))
        {
            xc=xc_temp;
            yc=yc_temp;
            cross =1;

            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            ret_line.append(xc);
            ret_line.append(yc);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;
        }
    }
    else if (x2==x1 && x4==x3)
    {
        cross=0;//inutile assignement ?
        xc=0;//inutile assignement ?
        yc=0;//inutile assignement ?
        if (x2==x4)
        {
           xc=x1;
           yc=(y3+y4)/2	;
        }
    }
    ret_line.append(cross);
    ret.append(ret_line);
    ret_line=QVector<qreal>();
    ret_line.append(xc);
    ret_line.append(yc);
    ret.append(ret_line);
    ret_line=QVector<qreal>();
    return ret;
}


QVector <QVector<qreal>> intersect_line_segment(AxLine line, AxLine segment)
{
    int cross=0;
    qreal x1, y1, x2, y2, x3, y3, x4, y4,a1,a2,b1,b2;
    qreal xc_temp =0, yc_temp =0,xc =0,yc =0;
    QVector<qreal> ret_line;

    x1=line.x1();
    y1=line.y1();
    x2=line.x2();
    y2=line.y2();

    x3=segment.x1();
    y3=segment.y1();
    x4=segment.x2();
    y4=segment.y2();

    QVector<QVector<qreal>> ret;

    cross =0;
    xc=0;
    yc=0;

    if (x2!=x1 && x4!=x3)	//If not vertical
    {
        a1 = (y2-y1)/(x2-x1) ;
        a2 = (y4-y3)/(x4-x3);
        cross=0;

        if (a1==a2) //If colinear
        {

            b1=y1-(a1*x1);
            b2=y3-(a2*x3);
                if (b2 == b1)
                {
                    xc=(x3+x1)/2;
                    yc=(y3+y1)/2;

                    cross=2;

                    ret_line.append(cross);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    ret_line.append(xc);
                    ret_line.append(yc);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    return ret;
                }
                else
                {
                    cross =0;
                    xc=0;
                    yc=0;
                    cross=2;

                    ret_line.append(cross);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    ret_line.append(xc);
                    ret_line.append(yc);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    return ret;
                }


        }

        else // if they are not parallels
        {

            b1=y1-(a1*x1);
            b2=y3-(a2*x3);
            xc_temp=(b2-b1)/(a1-a2);
            yc_temp=a1*xc_temp+b1;

            if ((xc_temp<=x3 && xc_temp>=x4)||(xc_temp<=x4 && xc_temp>=x3)) // If my intersection pt is include inside the coordinate of my segments, this mean my segments cross
            {

                xc=xc_temp;
                yc=yc_temp;
                cross =1;

                ret_line.append(cross);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
                ret_line.append(xc);
                ret_line.append(yc);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
                return ret;
            }
        }
 }


    else if ((x2==x1 && x4!=x3))  //  If one segments or both of my segment are vertical
    {
        a2 = (y4-y3)/(x4-x3);
        b2=y3-(a2*x3);
        xc_temp=x2;
        yc_temp=a2*x2+b2;
        if ((x3<=xc_temp && x4>=xc_temp) || (x4<=xc_temp && x3>=xc_temp))
        {
            xc=xc_temp;
            yc=yc_temp;
            cross =1;

            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            ret_line.append(xc);
            ret_line.append(yc);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;
        }
    }
    else if (x2!=x1 && x4==x3)
    {
        a1 = (y2-y1)/(x2-x1);
        b1=y1-(a1*x1);
        xc_temp=x3;
        yc_temp=a1*x3+b1;
        if ((yc_temp<=y3 && yc_temp>=y4) || (yc_temp<=y4 && yc_temp>=y3))
        {
            xc=xc_temp;
            yc=yc_temp;
            cross =1;

            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            ret_line.append(xc);
            ret_line.append(yc);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;
        }
    }
    else if (x2==x1 && x4==x3)
    {
        cross=0;//inutile assignement ?
        xc=0;//inutile assignement ?
        yc=0;//inutile assignement ?
        if (x2==x4)
        {
           xc=x1;
           yc=(y3+y4)/2	;
        }
    }
    ret_line.append(cross);
    ret.append(ret_line);
    ret_line=QVector<qreal>();
    ret_line.append(xc);
    ret_line.append(yc);
    ret.append(ret_line);
    ret_line=QVector<qreal>();
    return ret;
}

int border_sens(AxBorder *border)
{
    qreal x1(border->x1());
    qreal x2(border->x2());
    qreal y1(border->y1());
    qreal y2(border->y2());
    if (border->getObjectType()==0)
    {
        if (x1!=x2)
        {
            return sign(x2-x1);
        }
        else
        {
            return sign(y2-y1);
        }
    }
    else
    {
        qDebug("non traité in border sens");
    }
}


QVector <QVector<qreal>> intersect_demi_line_line(AxLine demi_line, AxLine line)
{
    int cross=0;
    qreal x1, y1, x2, y2, x3, y3, x4, y4,a1,a2,b1,b2;
    qreal xc =0,yc =0;
    QVector<qreal> ret_line;

    x1=demi_line.x1();
    y1=demi_line.y1();
    x2=demi_line.x2();
    y2=demi_line.y2();

    x3=line.x1();
    y3=line.y1();
    x4=line.x2();
    y4=line.y2();

    int sens=sign(x2-x1);
    QVector<QVector<qreal>> ret;

    cross =0;
    xc=0;
    yc=0;

    if (x2!=x1 && x4!=x3)	//If not vertical
    {
        a1 = (y2-y1)/(x2-x1) ;
        a2 = (y4-y3)/(x4-x3);
        cross=0;

        if (a1==a2) //If colinear
        {

            b1=y1-(a1*x1);
            b2=y3-(a2*x3);
            cross=2;
            ret_line.append(xc);
            ret_line.append(yc);
            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();

            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;

         }
        else // if they are not parallels
        {
            b1=y1-(a1*x1);
            b2=y3-(a2*x3);
            xc=(b2-b1)/(a1-a2);
            yc=a1*xc+b1;
            if (sign(xc-x1)==sens)
            {
                cross =1;
                ret_line.append(cross);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
                ret_line.append(xc);
                ret_line.append(yc);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
                return ret;
            }
        }

    }
    else if ((x2==x1 && x4!=x3))  //  If one segments or both of my segment are vertical
    {
        sens=sign(y2-y1);
        a2 = (y4-y3)/(x4-x3);
        b2=y3-(a2*x3);
        xc=x2;
        yc=a2*x2+b2;
        if (sign(yc-y1)==sens)
        {
            cross =1;
            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            ret_line.append(xc);
            ret_line.append(yc);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;
        }
    }
    else if (x2!=x1 && x4==x3)
    {
        a1 = (y2-y1)/(x2-x1);
        b1=y1-(a1*x1);
        xc=x3;
        yc=a1*x3+b1;
        cross =1;
        if (sign(xc-x1)==sens)
        {
            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            ret_line.append(xc);
            ret_line.append(yc);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;
        }
    }
    else if (x2==x1 && x4==x3)
    {
        cross=2;
        xc=0;
        yc=0;
    }
    ret_line.append(cross);
    ret.append(ret_line);
    ret_line=QVector<qreal>();
    ret_line.append(xc);
    ret_line.append(yc);
    ret.append(ret_line);
    ret_line=QVector<qreal>();
    return ret;
}

QVector <QVector<qreal>> intersect(QLineF line1, QLineF line2)
{
    int cross=0;
    qreal x1, y1, x2, y2, x3, y3, x4, y4,a1,a2,b1,b2;
    qreal xc_temp =0, yc_temp =0,xc =0,yc =0;
    QVector<qreal> ret_line;

    x1=line1.x1();
    y1=line1.y1();
    x2=line1.x2();
    y2=line1.y2();

    x3=line2.x1();
    y3=line2.y1();
    x4=line2.x2();
    y4=line2.y2();

    QVector<QVector<qreal>> ret;

    cross =0;
    xc=0;
    yc=0;

    if (x2!=x1 && x4!=x3)	//If not vertical
    {
        a1 = (y2-y1)/(x2-x1) ;
        a2 = (y4-y3)/(x4-x3);
        cross=0;

        if (a1==a2) //If colinear
        {

            b1=y1-(a1*x1);
            b2=y3-(a2*x3);
                if (y3-a1*x3 == b1 && y4-a1*x4 == b1)
                {
                    if ((x3<=x1 && x3>=x2) || (x3>=x1 && x3<=x2))
                    {
                        if((x1>=x3 && x1<=x4) || (x1>=x4 && x1<=x3))
                        {

                            cross=2;
                            xc=(x3+x1)/2;
                            yc=(y3+y1)/2;

                            ret_line.append(cross);
                            ret.append(ret_line);
                            ret_line=QVector<qreal>();
                            ret_line.append(xc);
                            ret_line.append(yc);
                            ret.append(ret_line);
                            ret_line=QVector<qreal>();
                            return ret;
                        }
                        else
                        {
                            cross=2;
                            xc=(x3+x2)/2;
                            yc=(y3+y2)/2;

                            ret_line.append(cross);
                            ret.append(ret_line);
                            ret_line=QVector<qreal>();
                            ret_line.append(xc);
                            ret_line.append(yc);
                            ret.append(ret_line);
                            ret_line=QVector<qreal>();
                            return ret;

                        }

                    }
                    if ((x4<=x1 && x4>=x2)||(x4>=x1 && x4<=x2))
                    {
                        if((x1>=x3 && x1<=x4) || (x1>=x4 && x1<=x3))
                        {
                            cross=2;
                            xc=(x4+x1)/2;
                            yc=(y4+y1)/2;

                            ret_line.append(cross);
                            ret.append(ret_line);
                            ret_line=QVector<qreal>();
                            ret_line.append(xc);
                            ret_line.append(yc);
                            ret.append(ret_line);
                            ret_line=QVector<qreal>();
                            return ret;
                        }
                        else
                        {
                            cross=2;
                            xc=(x4+x2)/2;
                            yc=(y4+y2)/2;

                            ret_line.append(cross);
                            ret.append(ret_line);
                            ret_line=QVector<qreal>();
                            ret_line.append(xc);
                            ret_line.append(yc);
                            ret.append(ret_line);
                            ret_line=QVector<qreal>();
                            return ret;
                        }
                    }
                }
                else // if parallels
                {
                    cross =0;
                    xc=0;
                    yc=0;

                    ret_line.append(cross);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    ret_line.append(xc);
                    ret_line.append(yc);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    return ret;
                }
        }

        else // if they are not parallels
        {

            b1=y1-(a1*x1);
            b2=y3-(a2*x3);
            xc_temp=(b2-b1)/(a1-a2);
            yc_temp=a1*xc_temp+b1;

            if (((x1<=xc_temp && x2>=xc_temp) || (x2<=xc_temp && x1>=xc_temp)) && ((x3<=xc_temp && x4>=xc_temp) || (x4<=xc_temp && x3>=xc_temp))) // If my intersection pt is include inside of the coordinate of at least one of my segments, this mean my segments cross
            {

                xc=xc_temp;
                yc=yc_temp;
                cross =1;

                ret_line.append(cross);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
                ret_line.append(xc);
                ret_line.append(yc);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
                return ret;
            }
        }
 }


    else if ((x2==x1 && x4!=x3))  //  If one segments or both of my segment are vertical
    {
        a2 = (y4-y3)/(x4-x3);
        b2=y3-(a2*x3);
        xc_temp=x2;
        yc_temp=a2*x2+b2;
        if (((x3<=xc_temp && x4>=xc_temp) || (x4<=xc_temp && x3>=xc_temp)) && ((y1<=yc_temp && y2>=yc_temp) || (y2<=yc_temp && y1>=yc_temp)))
        {
            xc=xc_temp;
            yc=yc_temp;
            cross =1;

            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            ret_line.append(xc);
            ret_line.append(yc);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;
        }
    }
    else if (x2!=x1 && x4==x3)
    {
        a1 = (y2-y1)/(x2-x1);
        b1=y1-(a1*x1);
        xc_temp=x3;
        yc_temp=a1*x3+b1;
        if (((x1<=xc_temp && x2>=xc_temp) || (x2<=xc_temp && x1>=xc_temp))&& ((y3<=yc_temp && y4>=yc_temp) || (y4<=yc_temp && y3>=yc_temp)))
        {
            xc=xc_temp;
            yc=yc_temp;
            cross =1;

            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            ret_line.append(xc);
            ret_line.append(yc);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;
        }
    }
    else if (x2==x1 && x4==x3)
    {
        cross=0;
        xc=0;
        yc=0;
        if (x2==x4)
        {
            xc=x4;
            if ((y3<=y1 && y3>=y2)||(y3>=y1 && y3<=y2))
            {
                if((y1>=y3 && y1<=y4) || (y1>=y4 && y1<=y3))
                {
                cross=2;
                xc=x4;
                yc=(y3+y1)/2;

                ret_line.append(cross);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
                ret_line.append(xc);
                ret_line.append(yc);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
                return ret;
                }
                else
                {
                    cross=2;
                    xc=x4;
                    yc=(y3+y2)/2;

                    ret_line.append(cross);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    ret_line.append(xc);
                    ret_line.append(yc);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    return ret;
                }
            }
            if ((y4<=x1 && y4>=y2)||(y4>=x1 && y4<=y2))
            {
                if((y1>=y3 && y1<=y4) || (y1>=y4 && y1<=y3))
                {
                    cross=2;
                    xc=x4;
                    yc=(y4+y1)/2;

                    ret_line.append(cross);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    ret_line.append(xc);
                    ret_line.append(yc);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    return ret;
                }
                else
                {
                    cross=2;
                    xc=x4;
                    yc=(y4+y2)/2;

                    ret_line.append(cross);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    ret_line.append(xc);
                    ret_line.append(yc);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    return ret;
                }
            }

        }
    }
    ret_line.append(cross);
    ret.append(ret_line);
    ret_line=QVector<qreal>();
    ret_line.append(xc);
    ret_line.append(yc);
    ret.append(ret_line);
    ret_line=QVector<qreal>();
    return ret;
}

//---------------------------------------------------------------------------------------------------------

QVector <QVector<qreal>> intersect(AxLine line1, AxLine line2)
{
    int cross=0;
    qreal x1, y1, x2, y2, x3, y3, x4, y4,a1,a2,b1,b2;
    qreal xc_temp =0, yc_temp =0,xc =0,yc =0;
    QVector<qreal> ret_line;

    x1=line1.x1();
    y1=line1.y1();
    x2=line1.x2();
    y2=line1.y2();

    x3=line2.x1();
    y3=line2.y1();
    x4=line2.x2();
    y4=line2.y2();

    QVector<QVector<qreal>> ret;

    cross =0;
    xc=0;
    yc=0;

    if (x2!=x1 && x4!=x3)	//If not vertical
    {
        a1 = (y2-y1)/(x2-x1) ;
        a2 = (y4-y3)/(x4-x3);
        cross=0;

        if (a1==a2) //If colinear
        {

            b1=y1-(a1*x1);
            b2=y3-(a2*x3);
                if (y3-a1*x3 == b1 && y4-a1*x4 == b1)
                {
                    if ((x3<=x1 && x3>=x2) || (x3>=x1 && x3<=x2))
                    {
                        if((x1>=x3 && x1<=x4) || (x1>=x4 && x1<=x3))
                        {

                            cross=2;
                            xc=(x3+x1)/2;
                            yc=(y3+y1)/2;

                            ret_line.append(cross);
                            ret.append(ret_line);
                            ret_line=QVector<qreal>();
                            ret_line.append(xc);
                            ret_line.append(yc);
                            ret.append(ret_line);
                            ret_line=QVector<qreal>();
                            return ret;
                        }
                        else
                        {
                            cross=2;
                            xc=(x3+x2)/2;
                            yc=(y3+y2)/2;

                            ret_line.append(cross);
                            ret.append(ret_line);
                            ret_line=QVector<qreal>();
                            ret_line.append(xc);
                            ret_line.append(yc);
                            ret.append(ret_line);
                            ret_line=QVector<qreal>();
                            return ret;

                        }

                    }
                    if ((x4<=x1 && x4>=x2)||(x4>=x1 && x4<=x2))
                    {
                        if((x1>=x3 && x1<=x4) || (x1>=x4 && x1<=x3))
                        {
                            cross=2;
                            xc=(x4+x1)/2;
                            yc=(y4+y1)/2;

                            ret_line.append(cross);
                            ret.append(ret_line);
                            ret_line=QVector<qreal>();
                            ret_line.append(xc);
                            ret_line.append(yc);
                            ret.append(ret_line);
                            ret_line=QVector<qreal>();
                            return ret;
                        }
                        else
                        {
                            cross=2;
                            xc=(x4+x2)/2;
                            yc=(y4+y2)/2;

                            ret_line.append(cross);
                            ret.append(ret_line);
                            ret_line=QVector<qreal>();
                            ret_line.append(xc);
                            ret_line.append(yc);
                            ret.append(ret_line);
                            ret_line=QVector<qreal>();
                            return ret;
                        }
                    }
                }
                else // if parallels
                {
                    cross =0;
                    xc=0;
                    yc=0;

                    ret_line.append(cross);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    ret_line.append(xc);
                    ret_line.append(yc);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    return ret;
                }
        }

        else // if they are not parallels
        {

            b1=y1-(a1*x1);
            b2=y3-(a2*x3);
            xc_temp=(b2-b1)/(a1-a2);
            yc_temp=a1*xc_temp+b1;

            if (((x1<=xc_temp && x2>=xc_temp) || (x2<=xc_temp && x1>=xc_temp)) && ((x3<=xc_temp && x4>=xc_temp) || (x4<=xc_temp && x3>=xc_temp))) // If my intersection pt is include inside of the coordinate of at least one of my segments, this mean my segments cross
            {

                xc=xc_temp;
                yc=yc_temp;
                cross =1;

                ret_line.append(cross);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
                ret_line.append(xc);
                ret_line.append(yc);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
                return ret;
            }
        }
 }


    else if ((x2==x1 && x4!=x3))  //  If one segments or both of my segment are vertical
    {
        a2 = (y4-y3)/(x4-x3);
        b2=y3-(a2*x3);
        xc_temp=x2;
        yc_temp=a2*x2+b2;
        if (((x3<=xc_temp && x4>=xc_temp) || (x4<=xc_temp && x3>=xc_temp)) && ((y1<=yc_temp && y2>=yc_temp) || (y2<=yc_temp && y1>=yc_temp)))
        {
            xc=xc_temp;
            yc=yc_temp;
            cross =1;

            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            ret_line.append(xc);
            ret_line.append(yc);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;
        }
    }
    else if (x2!=x1 && x4==x3)
    {
        a1 = (y2-y1)/(x2-x1);
        b1=y1-(a1*x1);
        xc_temp=x3;
        yc_temp=a1*x3+b1;
        if (((x1<=xc_temp && x2>=xc_temp) || (x2<=xc_temp && x1>=xc_temp))&& ((y3<=yc_temp && y4>=yc_temp) || (y4<=yc_temp && y3>=yc_temp)))
        {
            xc=xc_temp;
            yc=yc_temp;
            cross =1;

            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            ret_line.append(xc);
            ret_line.append(yc);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;
        }
    }
    else if (x2==x1 && x4==x3)
    {
        cross=0;
        xc=0;
        yc=0;
        if (x2==x4)
        {
            xc=x4;
            if ((y3<=y1 && y3>=y2)||(y3>=y1 && y3<=y2))
            {
                if((y1>=y3 && y1<=y4) || (y1>=y4 && y1<=y3))
                {
                cross=2;
                xc=x4;
                yc=(y3+y1)/2;

                ret_line.append(cross);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
                ret_line.append(xc);
                ret_line.append(yc);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
                return ret;
                }
                else
                {
                    cross=2;
                    xc=x4;
                    yc=(y3+y2)/2;

                    ret_line.append(cross);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    ret_line.append(xc);
                    ret_line.append(yc);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    return ret;
                }
            }
            if ((y4<=x1 && y4>=y2)||(y4>=x1 && y4<=y2))
            {
                if((y1>=y3 && y1<=y4) || (y1>=y4 && y1<=y3))
                {
                    cross=2;
                    xc=x4;
                    yc=(y4+y1)/2;

                    ret_line.append(cross);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    ret_line.append(xc);
                    ret_line.append(yc);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    return ret;
                }
                else
                {
                    cross=2;
                    xc=x4;
                    yc=(y4+y2)/2;

                    ret_line.append(cross);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    ret_line.append(xc);
                    ret_line.append(yc);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    return ret;
                }
            }

        }
    }
    ret_line.append(cross);
    ret.append(ret_line);
    ret_line=QVector<qreal>();
    ret_line.append(xc);
    ret_line.append(yc);
    ret.append(ret_line);
    ret_line=QVector<qreal>();
    return ret;
}


AxCircle cercle_inscrit(QPointF p1, QPointF p2, QPointF p3)
{
    //prend 3 point et renvoir le rayon et le centre de leur cercle inscrit
    QPointF pm1,pm2,pm3,pm4,pc;
    qreal a,b,c,R;
    QVector<QVector<qreal>> cross_med;
//  calcul de 2 pts appartement aux médiatrice

// equation mediatrice : 2(x2-x1)*x+2(y2-y1)*y+(x1^2+y1^2-x2^2-y2^2)=0

 // mediatrice 1:
    pm1=QPointF((p1.x()+p2.x())/2,((p1.y()+p2.y())/2));
    pm2.setY(100);
    a=2*(p2.x()-p1.x());
    b=2*(p2.y()-p1.y());
    c=pow(p1.x(),2)+pow(p1.y(),2)-pow(p2.x(),2)-pow(p2.y(),2);
    pm2.setX((-c-b*pm2.y())/a);

// mediatrice 2:
    pm3=QPointF((p3.x()+p2.x())/2,(p3.y()+p2.y())/2);
    pm4.setY(100);
    a=2*(p2.x()-p3.x());
    b=2*(p2.y()-p3.y());
    c=pow(p3.x(),2)+pow(p3.y(),2)-pow(p2.x(),2)-pow(p2.y(),2);
    pm4.setX((-c-b*pm4.y())/a);

    cross_med=intersect_lignes(QLineF(pm1,pm2),QLineF(pm3,pm4));
    pc=QPointF(cross_med[1][0],cross_med[1][1]);
    R=distance(pc,p1);

    return AxCircle(R,pc);
}

QVector<QVector<qreal>> intersect(QLineF line,AxArc arc)
{
    QVector<QVector<qreal>> ret;
    qreal x1,y1,x2,y2,R,xc,yc;
    QVector<qreal> ret_line;

    x1=line.x1();
    y1=line.y1();
    x2=line.x2();
    y2=line.y2();

    R=arc.get_R();
    xc=arc.get_center().x();
    yc=arc.get_center().y();

    int cross=0;
    qreal a, b, A, B, C, delta, X1, X2,Y1,Y2,X,Y;

    // segment vertical ?
    if (float(x2)==float(x1))
    {
        // calcul coeficient directeur impossible donc methode géométrique
        X1=x1;
        X2=x2;

        if (pow(R,2)-pow((X1-xc),2)>0)
        {
            Y1=sqrt(pow(R,2)-pow((x1-xc),2))+yc;
            Y2=-sqrt(pow(R,2)-pow((x1-xc),2))+yc;
            qDebug() << "Y1=" << Y1 << "pow(R,2)-pow((x1-xc),2)"<< pow(R,2)-pow((x1-xc),2) << endl;
            qDebug() << "Y2=" << Y2 << endl;


            if (arc.arc_test(QPointF(X1,Y1))) // point de croisement 1 sur arc de cercle ?
            {
                qDebug() << "ok" << endl;
                if ((Y1<y1 && Y1>y2) || (Y1<y2 && Y1>y1)) // le point d'intersection appartient au segment ?
                {
                    cross=1;
                    ret_line.append(X1);
                    ret_line.append(Y1);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
            }
            if (arc.arc_test(QPointF(X2,Y2))) // point de croisement 1 sur arc de cercle ?
            {
                if ((Y2<y1 && Y2>y2) || (Y2<y2 && Y2>y1)) // le point d'intersection appartient au segment ?
                {
                    cross=1;
                    ret_line.append(X2);
                    ret_line.append(Y2);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
            }
        }
    }
    else // segment non vertical
    {
        a = (y2-y1)/(x2-x1);
        b=y1-(a*x1);

        A=1+(pow(a,2));
        B=2*(-xc+(a*b)-(a*yc));
        C=(pow(xc,2))+(pow((b-yc),2))-(pow(R,2));

        delta=(pow(B,2))-(4*A*C);

        if (delta>0) // 2 pt d'intersection entre le cercle et la droite asociée ?
        {
            X1=(-B-sqrt(delta))/(2*A);
            X2=(-B+sqrt(delta))/(2*A);

            Y1=a*X1+b;
            Y2=a*X2+b;

            if (arc.arc_test(QPointF(X1,Y1))) // point de croisement 1 sur arc de cercle ?
            {
                if ((X1<x1 && X1>x2) || (X1<x2 && X1>x1)) //point de croisement sur le segment ?
                {
                    cross=1;
                    ret_line.append(X1);
                    ret_line.append(Y1);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
            }
            if (arc.arc_test(QPointF(X2,Y2))) // point de croisement 2 sur arc de cercle ?
            {
                if ((X2<x1 && X2>x2) || (X2<x2 && X2>x1)) //point de croisement sur le segment ?
                {
                    cross=1;
                    ret_line.append(X2);
                    ret_line.append(Y2);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    }
                }
            }
        else if (delta==0)
        {
            X=(-B)/(2*A);
            Y=a*X+b;
            if (arc.arc_test(QPointF(X,Y))) // point de croisement 1 sur arc de cercle ?
            {
                if ((X<x1 && X>x2) || (X<x2 && X>x1)) //point de croisement sur le segment ?
                {
                    cross=1;
                    ret_line.append(X);
                    ret_line.append(Y);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
            }
        }
    }
    ret_line.append(cross);
    ret.prepend(ret_line);
    return ret;
}


QVector<QVector<qreal>> intersect(AxLine line,AxArc arc)
{
    QVector<QVector<qreal>> ret;
    qreal x1,y1,x2,y2,R,xc,yc;
    QVector<qreal> ret_line;

    x1=line.x1();
    y1=line.y1();
    x2=line.x2();
    y2=line.y2();

    R=arc.get_R();
    xc=arc.get_center().x();
    yc=arc.get_center().y();

    int cross=0;
    qreal a, b, A, B, C, delta, X1, X2,Y1,Y2,X,Y;

    // segment vertical ?
    if (float(x2)==float(x1))
    {
        // calcul coeficient directeur impossible donc methode géométrique
        X1=x1;
        X2=x2;

        if (pow(R,2)-pow((X1-xc),2)>0)
        {
            Y1=sqrt(pow(R,2)-pow((x1-xc),2))+yc;
            Y2=-sqrt(pow(R,2)-pow((x1-xc),2))+yc;
            qDebug() << "Y1=" << Y1 << "pow(R,2)-pow((x1-xc),2)"<< pow(R,2)-pow((x1-xc),2) << endl;
            qDebug() << "Y2=" << Y2 << endl;


            if (arc.arc_test(QPointF(X1,Y1))) // point de croisement 1 sur arc de cercle ?
            {
                qDebug() << "ok" << endl;
                if ((Y1<=y1 && Y1>=y2) || (Y1<=y2 && Y1>=y1)) // le point d'intersection appartient au segment ?
                {
                    cross=1;
                    ret_line.append(X1);
                    ret_line.append(Y1);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
            }
            if (arc.arc_test(QPointF(X2,Y2))) // point de croisement 1 sur arc de cercle ?
            {
                if ((Y2<y1 && Y2>y2) || (Y2<y2 && Y2>y1)) // le point d'intersection appartient au segment ?
                {
                    cross=1;
                    ret_line.append(X2);
                    ret_line.append(Y2);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
            }
        }
    }
    else // segment non vertical
    {
        a = (y2-y1)/(x2-x1);
        b=y1-(a*x1);

        A=1+(pow(a,2));
        B=2*(-xc+(a*b)-(a*yc));
        C=(pow(xc,2))+(pow((b-yc),2))-(pow(R,2));

        delta=(pow(B,2))-(4*A*C);

        if (delta>0) // 2 pt d'intersection entre le cercle et la droite asociée ?
        {
            X1=(-B-sqrt(delta))/(2*A);
            X2=(-B+sqrt(delta))/(2*A);

            Y1=a*X1+b;
            Y2=a*X2+b;

            if (arc.arc_test(QPointF(X1,Y1))) // point de croisement 1 sur arc de cercle ?
            {
                if ((X1<x1 && X1>x2) || (X1<x2 && X1>x1)) //point de croisement sur le segment ?
                {
                    cross=1;
                    ret_line.append(X1);
                    ret_line.append(Y1);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
            }
            if (arc.arc_test(QPointF(X2,Y2))) // point de croisement 2 sur arc de cercle ?
            {
                if ((X2<x1 && X2>x2) || (X2<x2 && X2>x1)) //point de croisement sur le segment ?
                {
                    cross=1;
                    ret_line.append(X2);
                    ret_line.append(Y2);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                    }
                }
            }
        else if (delta==0)
        {
            X=(-B)/(2*A);
            Y=a*X+b;
            if (arc.arc_test(QPointF(X,Y))) // point de croisement 1 sur arc de cercle ?
            {
                if ((X<x1 && X>x2) || (X<x2 && X>x1)) //point de croisement sur le segment ?
                {
                    cross=1;
                    ret_line.append(X);
                    ret_line.append(Y);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
            }
        }
    }
    ret_line.append(cross);
    ret.prepend(ret_line);
    return ret;
}


QVector<QVector<qreal>> intersect(AxArc arc1, AxArc arc2)
{
    qreal Yc1,Yc2,Xc1,Xc2,XIb,Rc2,Rc1;
    QVector<QVector<qreal>> ret;
    QVector<qreal> ret_line;
    int cross=0;

    Xc1=arc1.get_center().x();
    Yc1=arc1.get_center().y();
    Xc2=arc2.get_center().x();
    Yc2=arc2.get_center().y();
    Rc1=arc1.get_R();
    Rc2=arc2.get_R();

    if (Yc1==Yc2) // si les deux cercles sont sur la même abscisse, on utilise un simple Pythagore...
    {
        if (Xc1==Xc2) // le center des 2 arc de cercle est le même
        {
            cross=2;
        }
        else
        {
            qreal a = abs(Xc1-Xc2);
            qreal XIa = XIb = ((pow(Rc2,2)-pow(a,2)-pow(Rc1,2))/(-2*a))+min(Xc1,Xc2);

            if (pow(Rc1,2)-pow(XIa-Xc1,2)>0)
            {
                qreal YIa = Yc1+sqrt(pow(Rc1,2)-pow(XIa-Xc1,2));
                qreal YIb = Yc1-sqrt(pow(Rc1,2)-pow(XIa-Xc1,2));
                if (arc1.arc_test(QPointF(XIa,YIa)) && arc2.arc_test(QPointF(XIa,YIa))) // if cross point inside both arc
                {
                    cross=1;
                    ret_line.append(XIa);
                    ret_line.append(YIa);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
                if (arc1.arc_test(QPointF(XIb,YIb)) && arc2.arc_test(QPointF(XIb,YIb))) // if cross point inside both arc
                {
                    cross=1;
                    ret_line.append(XIb);
                    ret_line.append(YIb);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }

            }
            if (pow(Rc1,2)-pow(XIa-Xc1,2)==0)
            {
                qreal YI = Yc1;
                qreal XI=XIa;

                if (arc1.arc_test(QPointF(XI,YI)) && arc2.arc_test(QPointF(XI,YI))) // if cross point inside both arc
                {
                    cross=1;
                    ret_line.append(XI);
                    ret_line.append(YI);
                    ret.append(ret_line);
                    ret_line=QVector<qreal>();
                }
            }


        }

    }
    else // ...sinon, un peu de trigo des familles !
    {
        qreal a = (-pow(Xc1,2)-pow(Yc1,2)+pow(Xc2,2)+pow(Yc2,2)+pow(Rc1,2)-pow(Rc2,2))/(2*(Yc2-Yc1));
        qreal d = (Xc2-Xc1)/(Yc2-Yc1);
        qreal A = pow(d,2)+1;
        qreal B = -2*Xc1+2*Yc1*d-2*a*d;
        qreal C = pow(Xc1,2)+pow(Yc1,2)-2*Yc1*a+pow(a,2)-pow(Rc1,2);
        qreal delta = pow(B,2)-4*A*C;
        if (delta>0)
        {
            qreal XIa = (-B+sqrt(delta))/(2*A);
            qreal XIb = (-B-sqrt(delta))/(2*A);
            qreal YIa = a-((-B+sqrt(delta))/(2*A))*d;
            qreal YIb = a-((-B-sqrt(delta))/(2*A))*d;


            if (arc1.arc_test(QPointF(XIa,YIa)) && arc2.arc_test(QPointF(XIa,YIa))) // if cross point inside both arc
            {
                cross=1;
                ret_line.append(XIa);
                ret_line.append(YIa);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
            }
            if (arc1.arc_test(QPointF(XIb,YIb)) && arc2.arc_test(QPointF(XIb,YIb))) // if cross point inside both arc
            {
                cross=1;
                ret_line.append(XIb);
                ret_line.append(YIb);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
            }
        }
        else if (delta==0)
        {
            qreal XI = (-B)/(2*A);
            qreal YI = (-B)/(2*A);

            if (arc1.arc_test(QPointF(XI,YI)) && arc2.arc_test(QPointF(XI,YI))) // if cross point inside both arc
            {
                cross=1;
                ret_line.append(XI);
                ret_line.append(YI);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
            }
        }
    }
    ret_line.append(cross);
    ret.prepend(ret_line);
    return ret;
}

QVector<QVector<qreal> > intersect_demi_lines(AxLine demi_line1, AxLine demi_line2)
{

    int cross=0;
    qreal x1, y1, x2, y2, x3, y3, x4, y4,a1,a2,b1,b2;
    qreal xc =0,yc =0;
    QVector<qreal> ret_line;

    x1=demi_line1.x1();
    y1=demi_line1.y1();
    x2=demi_line1.x2();
    y2=demi_line1.y2();

    x3=demi_line2.x1();
    y3=demi_line2.y1();
    x4=demi_line2.x2();
    y4=demi_line2.y2();

    int sens1=sign(x2-x1);
    int sens2=sign(x2-x3);
    QVector<QVector<qreal>> ret;

    cross =0;
    xc=0;
    yc=0;

    if (x2!=x1 && x4!=x3)	//If not vertical
    {
        a1 = (y2-y1)/(x2-x1) ;
        a2 = (y4-y3)/(x4-x3);
        cross=0;

        if (a1==a2) //If colinear
        {

            b1=y1-(a1*x1);
            b2=y3-(a2*x3);
            cross=2;
            ret_line.append(xc);
            ret_line.append(yc);
            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();

            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;

         }
        else // if they are not parallels
        {
            b1=y1-(a1*x1);
            b2=y3-(a2*x3);
            xc=(b2-b1)/(a1-a2);
            yc=a1*xc+b1;
            if ((sign(xc-x1)==sens1)&&(sign(xc-x3)==sens2))
            {
                cross =1;
                ret_line.append(cross);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
                ret_line.append(xc);
                ret_line.append(yc);
                ret.append(ret_line);
                ret_line=QVector<qreal>();
                return ret;
            }
        }

    }
    else if ((x2==x1 && x4!=x3))  //  If one segments or both of my segment are vertical
    {
        sens1=sign(y2-y1);
        a2 = (y4-y3)/(x4-x3);
        b2=y3-(a2*x3);
        xc=x2;
        yc=a2*x2+b2;
        if ((sign(yc-y1)==sens1)&&(sign(yc-y3)==sens2))
        {
            cross =1;
            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            ret_line.append(xc);
            ret_line.append(yc);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;
        }
    }
    else if (x2!=x1 && x4==x3)
    {
        sens2=sign(y4-y3);
        a1 = (y2-y1)/(x2-x1);
        b1=y1-(a1*x1);
        xc=x3;
        yc=a1*x3+b1;
        cross =1;
        if ((sign(xc-x1)==sens1)&&(sign(yc-y3)==sens2))
        {
            ret_line.append(cross);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            ret_line.append(xc);
            ret_line.append(yc);
            ret.append(ret_line);
            ret_line=QVector<qreal>();
            return ret;
        }
    }
    else if (x2==x1 && x4==x3)
    {
        cross=2;
        xc=0;
        yc=0;
    }
    ret_line.append(cross);
    ret.append(ret_line);
    ret_line=QVector<qreal>();
    ret_line.append(xc);
    ret_line.append(yc);
    ret.append(ret_line);
    ret_line=QVector<qreal>();
    return ret;
}
