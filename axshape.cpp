#include "AxArc.h"
#include "AxShape.h"
#include "QObject"
#include "QVector"
#include "common_functions.h"
#include "math.h"
#include <clipper.hpp>
#include <bisection_intersection.h>
#include <Vertice.h>

using namespace ClipperLib;

AxShape:: AxShape()
{
    shape=QVector<AxBorder*>();
}

void AxShape::append(AxBorder *border)
{

    shape.append(border);

}



void AxShape::insert(int k,AxBorder *border)
{

    shape.insert(k,border);
}

void AxShape::remove(int k)
{
    shape.remove(k);
}

void AxShape::replace(int k, AxBorder *border)
{

    shape.replace(k, border);
};


//AxShape::AxShape(QVector<QObject> _shape)
//{
//    shape=_shape;
//}

AxShape:: ~AxShape()
{
    for (int k=0;k<shape.size();k++)
    {
        shape[k]=nullptr;
    }
}

AxBorder* AxShape::operator[](int i) const
{
    return shape[i];
}

QVector<QVector<AxShape>> AxShape::convexe_concave()
{
    // renvoie les segments concave et comvexe de la shape

    // note : pour que les pointeurs soient commun remplacer tous les form par *this
    AxShape form=*this;
    int k2,k1=0;
    int teta;
    QVector<AxShape> concave_list;
    AxShape segments_concave;
    AxShape convexe(*this);
    AxShape segment_convexe,segment_concave, segment_convexe_bis;


    // enveloppe convexe calculation
    for (int k=0;k<convexe.size()*2;k++)
    {
        k2=convexe.next_point(k1);
        teta=angle(convexe[k1],convexe[k2]);
        if (teta>=M_PI)
        {
            convexe[k1]->setP2(convexe[k2]->p2());
            convexe.remove(k2);
            k=k-2;
            k1=convexe.previous_point(convexe.previous_point(k1));
        }
        k1=convexe.next_point(k1);
    }



   // starting segment for convexe shape initialisation
    k2=0;
    k1=0;
    QVector<AxShape> shape_convexe;
    bool ok=false;
    int j=0;

    // forme convexe initialisation
    while (ok==false)
    {
        for (int q=0;q<form.size();q++)
        {
            if (equal(form[q],convexe[j]))
            {
                break;
            }
            if (q==form.size()-1)
            {
                k1=j;
                ok=true;
            }
        }
        // form fully convexe ?
        if (j==(convexe.size()-1))
        {
            ok=true;
        }
        j++;
    }

    // forme convexe calculation

    for (int k=0;k<convexe.size();k++)
    {
        for (int q=0;q<form.size()*2;q++)
        {
            if (equal(form[k2],convexe[k1]))
            {
                segment_convexe.append(form[k2]);
                segment_convexe_bis.append(form[k2]);

                k1=convexe.next_point(k1);
                k++;
            }
            else if ((segment_convexe_bis.size()!=0))
            {
                shape_convexe.append(segment_convexe_bis);
                segment_convexe_bis=AxShape();
            }
            k2=form.next_point(k2);
        }
        k1=convexe.next_point(k1);
    }

    if ((segment_convexe_bis.size()!=0))
    {
        shape_convexe.append(segment_convexe_bis);
    }


    // forme concave starting segment initialisation

    QVector<AxShape> shape_concave;

    k2=0;
    k1=0;
    while (not(equal(form[k2],segment_convexe[k1])))
    {
        k2=form.next_point(k2);
    }

    // forme concave calculation
    for (int k=0;k<segment_convexe.size()+1;k++)
    {
        while (not(equal(form[k2],segment_convexe[k1])))
        {
            segment_concave.append(form[k2]);
            k2=form.next_point(k2);
        }
        if (segment_concave.size()!=0)
        {
            shape_concave.append(segment_concave);
            segment_concave=AxShape();
        }

        k1=segment_convexe.next_point(k1);
        k2=form.next_point(k2);
    }
    QVector<QVector<AxShape>> ret;
    ret.append(shape_convexe);
    ret.append(shape_concave);
    return ret;

}

AxBorder* AxShape::segment_brut_expand(AxBorder *border, int R)
{
    AxShape form=*this;

    QPoint pm, p1, p2,p1_para,p2_para,pm_line,closest_cross,center_,p1_extend,p2_extend;
    AxLine segment, segment1, segment2, segment_previous, segment_next;
    int a,b,a_para,b_para,D,R_;
    QVector<QVector<int>>cross_test,cross_med,cross_test1,cross_test2;
    AxShape stock_parallels,expand, brut_expland;
    QVector<QPoint> cross_pts,cross_pts1,cross_pts2;
    QLine arc_limits_,med;
    int left_cross,sens,inside_form,k_next,k_previous,clockwise_,k1=0,k_1=0,k_2=0,interne=0,closest_pos,number_of_cross,bord_externe=0,k,closest_k;

    for (int n=0;n<form.size();n++)// for all border of form
    {
        // initialisation
        left_cross=0;
        //Border = line ?
        if (border->getObjectType()==0)
        {
            // median point of the segment
            p1=border->p1();
            p2=border->p2();
            segment=AxLine(p1,p2);
            pm=segment.center();

            // 2 pts of the mediatrice

            // mediatrice :

            med=border->get_mediatrice();

            // count the number of time the mediatrice cross the object

            for (int k=0;k<form.size();k++)
            {
                //if segment form =! current segment
                if (not(equal(border,form[k])))
                {
                    // if border = line
                    if (form[k]->getObjectType()==0)
                    {
                        cross_test=intersect_line_segment(med,QLine(form[k]->p1(),form[k]->p2()));
                    }
                    // if border = arc
                    else
                    {
                        cross_test=intersect_arc_line(med,AxArc(form[k]));

                        // prendre le point d'intersection le plus proche du milieu du segment                         }
                    }

                    if (cross_test[0][0]==1)
                    {
                        cross_pts=intersecting_points(cross_test);

                        for (int q=0;q<cross_pts.size();q++)
                        {
                            // segment à étendre non vertical ?
                            if (p1.x()!=p2.x())
                            {
                                if (cross_pts[q].y()>pm.y())
                                {
                                    left_cross++;
                                }
                            }
                            // segment à étendre vertical
                            else
                            {
                                if (cross_pts[q].x()>pm.x())
                                {
                                    left_cross++;
                                }
                            }
                        }
                    }
                }
            }

            //                nombre pair de croisement à gauche ?
            if (left_cross%2==0)
                //ajouter R
            {
                sens=+1;
            }
            else
                // enlever R
            {
                sens=-1;
            }
            // equation du segment à expandre :
            if ((p2.x()!=p1.x()) && (p2.y()!=p1.y()))
            {
                a=(p2.y()-p1.y())/(p2.x()-p1.x());

                // odronée à l'origine à ajouter afin d'obtenir un décalage de R :
                D=sqrt(pow(a*R,2)+pow(R,2));

                b=p1.y()-(a*p1.x());
                // equation de la parallèle du segment distant de R :
                a_para=a;
                b_para=b+sens*D;
                // prendre 2 points appartenant à la droite (seule solution pour pouvoir appliquer la fct cross test avec le cas particulier)
                //              R*sqrt(1+a*a);
                if (sens==1)
                {
                    p1_para=QPoint((p1.y()+R/sqrt(1+a*a)-b_para)/a,p1.y()+R/sqrt(1+a*a));
                    p2_para=QPoint((p2.y()+R/sqrt(1+a*a)-b_para)/a,p2.y()+R/sqrt(1+a*a));
                }
                else if (sens==-1)
                {
                    p1_para=QPoint((p1.y()-R/sqrt(1+a*a)-b_para)/a,p1.y()-R/sqrt(1+a*a));
                    p2_para=QPoint((p2.y()-R/sqrt(1+a*a)-b_para)/a,p2.y()-R/sqrt(1+a*a));
                }


            }
            else if (p2.x()==p1.x())
            {
                D=R;
                // cas particulier segment vertical
                p1_para=QPoint(p1.x()+sens*D,p1.y());
                p2_para=QPoint(p2.x()+sens*D,p2.y());
            }
            else if (p2.y()==p1.y())
            {
                D=R;
                // cas particulier segment horizontal
                p1_para=QPoint(p1.x(),p1.y()+sens*D);
                p2_para=QPoint(p2.x(),p2.y()+sens*D);
            }
            // store expanded line

            return (new AxLine(p1_para,p2_para));
        }

        //Border = arc ?
        else
        {   // find pm and draw the mediatrice
            pm=border->middle_point();   // point in the middle of the arc
            pm_line=QPoint(pm.x()+10.0,pm.y()+10.0);
            //find out if arc circle is concave or convexe

            // pm inside form ?
            // count the number of time the border cross the object
            for (int k=0;k<form.size();k++)
            {
                //if border =! current arc
                if (not(equal(border,form[k])))
                {
                    // if border = line
                    if (form[k]->getObjectType()==0)
                    {
                        cross_test=intersect_line_segment(QLine(pm,pm_line),QLine(form[k]->p1(),form[k]->p2()));
                    }
                    // if border = arc
                    else
                    {
                        cross_test=intersect_arc_line(QLine(pm,pm_line),AxArc(form[k]));
                    }
                    //border = current arc
                }
                else
                {
                    //Do the cross test with the border of the arc
                    cross_test=intersect_line_segment(QLine(pm,pm_line),QLine(form[k]->get_limits()));
                }
                if (cross_test[0][0]==1)
                {
                    //add ont to left cross for each intersection
                    cross_pts=intersecting_points(cross_test);
                    for (int q=0;q<cross_pts.size();q++)
                    {
                        if (cross_pts[q].y()>pm.y())
                        {
                            left_cross++;
                        }
                    }
                }
            }
            //         nombre pair de croisement ?
            if (left_cross%2==0)
            {
                inside_form=0;
            }
            else
            {
                inside_form=1;
            }
            qDebug()<< "inside_form" << inside_form;


            // pm inside cercle ?
            if (inside_form==1) // cercle= concave
            {
                // arc big enought ?
                if (AxArc(border).get_R()-R>0)
                {

                    cross_test=intersect_arc_line(QLine(AxArc(border).get_center(),border->p1()),AxArc(QLine(0,0,0,0),AxArc(border).get_center(),AxArc(border).get_R()-R,AxArc(border).is_clockwise()));
                    cross_test=intersect_arc_line(QLine(AxArc(border).get_center(),border->p2()),AxArc(QLine(0,0,0,0),AxArc(border).get_center(),AxArc(border).get_R()-R,AxArc(border).is_clockwise()));
                    p1_extend=closest_cross_to_pt(cross_test,border->p1());
                    p2_extend=closest_cross_to_pt(cross_test,border->p2());
                    return (new AxArc(QLine(p1_extend,p2_extend),AxArc(border).get_center(),AxArc(border).get_R()-R,AxArc(border).is_clockwise()));
                }

            }
            else
            {
                // store the expanded Arc   // cercle= convexe


                //                        cross_test1=intersect_arc_line(QLine(border->get_center(),border->p1()),AxArc(QLine(0,0,0,0),border->get_center(),border->get_R()+R,border->get_clockwise()));
                cross_test1=intersect_arc_line(QLine(AxArc(border).get_center(),border->p1()),AxArc(border->get_limits(),AxArc(border).get_center(),AxArc(border).get_R()+R,AxArc(border).is_clockwise()));
                cross_pts1=intersecting_points(cross_test1);
                k_1=closest_pt_to_p1_border(cross_pts1, border);

                //cross_test2=intersect_arc_line(QLine(border->get_center(),border->p2()),AxArc(QLine(0,0,0,0),border->get_center(),border->get_R()+R,border->get_clockwise()));
                cross_test2=intersect_arc_line(QLine(AxArc(border).get_center(),border->p2()),AxArc(border->get_limits(),AxArc(border).get_center(),AxArc(border).get_R()+R,AxArc(border).is_clockwise()));
                cross_pts2=intersecting_points(cross_test2);
                k_2=closest_pt_to_p2_border(cross_pts2, border);

                return(new AxArc(QLine(cross_pts1[k_1],cross_pts2[k_2]),AxArc(border).get_center(),AxArc(border).get_R()+R,AxArc(border).is_clockwise()));

            }
        }
    }
}


AxBorder *AxShape::segment_brut_intern_expand(AxBorder *border, int R)
{
    AxShape form=*this;

    QPoint pm, p1, p2,p1_para,p2_para,pm_line,closest_cross,center_,p1_extend,p2_extend;
    AxLine segment, segment1, segment2, segment_previous, segment_next;
    int a,b,a_para,b_para,D,R_;
    QVector<QVector<int>>cross_test,cross_med,cross_test1,cross_test2;
    AxShape stock_parallels,expand, brut_expland;
    QVector<QPoint> cross_pts,cross_pts1,cross_pts2;
    QLine arc_limits_,med;
    int left_cross,sens,inside_form,k_next,k_previous,clockwise_,k1=0,k_1=0,k_2=0,interne=0,closest_pos,number_of_cross,bord_externe=0,k,closest_k;

    for (int n=0;n<form.size();n++)// for all border of form
    {
        // initialisation
        left_cross=0;
        //Border = line ?
        if (border->getObjectType()==0)
        {
            // median point of the segment
            p1=border->p1();
            p2=border->p2();
            segment=AxLine(p1,p2);
            pm=segment.center();

            // 2 pts of the mediatrice

            // mediatrice :

            med=border->get_mediatrice();

            // count the number of time the mediatrice cross the object

            for (int k=0;k<form.size();k++)
            {
                //if segment form =! current segment
                if (not(equal(border,form[k])))
                {
                    // if border = line
                    if (form[k]->getObjectType()==0)
                    {
                        cross_test=intersect_line_segment(med,QLine(form[k]->p1(),form[k]->p2()));
                    }
                    // if border = arc
                    else
                    {
                        cross_test=intersect_arc_line(med,AxArc(form[k]));

                        // prendre le point d'intersection le plus proche du milieu du segment                         }
                    }

                    if (cross_test[0][0]==1)
                    {
                        cross_pts=intersecting_points(cross_test);

                        for (int q=0;q<cross_pts.size();q++)
                        {
                            // segment à étendre non vertical ?
                            if (p1.x()!=p2.x())
                            {
                                if (cross_pts[q].y()>pm.y())
                                {
                                    left_cross++;
                                }
                            }
                            // segment à étendre vertical
                            else
                            {
                                if (cross_pts[q].x()>pm.x())
                                {
                                    left_cross++;
                                }
                            }
                        }
                    }
                }
            }

            //                nombre pair de croisement à gauche ?
            if (left_cross%2==0)
                //ajouter R
            {
                sens=-1;
            }
            else
                // enlever R
            {
                sens=+1;
            }
            // equation du segment à expandre :
            if ((p2.x()!=p1.x()) && (p2.y()!=p1.y()))
            {
                a=(p2.y()-p1.y())/(p2.x()-p1.x());

                // odronée à l'origine à ajouter afin d'obtenir un décalage de R :
                D=sqrt(pow(a*R,2)+pow(R,2));

                b=p1.y()-(a*p1.x());
                // equation de la parallèle du segment distant de R :
                a_para=a;
                b_para=b+sens*D;
                // prendre 2 points appartenant à la droite (seule solution pour pouvoir appliquer la fct cross test avec le cas particulier)
                //              R*sqrt(1+a*a);
                if (sens==1)
                {
                    p1_para=QPoint((p1.y()+R/sqrt(1+a*a)-b_para)/a,p1.y()+R/sqrt(1+a*a));
                    p2_para=QPoint((p2.y()+R/sqrt(1+a*a)-b_para)/a,p2.y()+R/sqrt(1+a*a));
                }
                else if (sens==-1)
                {
                    p1_para=QPoint((p1.y()-R/sqrt(1+a*a)-b_para)/a,p1.y()-R/sqrt(1+a*a));
                    p2_para=QPoint((p2.y()-R/sqrt(1+a*a)-b_para)/a,p2.y()-R/sqrt(1+a*a));
                }


            }
            else if (p2.x()==p1.x())
            {
                D=R;
                // cas particulier segment vertical
                p1_para=QPoint(p1.x()+sens*D,p1.y());
                p2_para=QPoint(p2.x()+sens*D,p2.y());
            }
            else if (p2.y()==p1.y())
            {
                D=R;
                // cas particulier segment horizontal
                p1_para=QPoint(p1.x(),p1.y()+sens*D);
                p2_para=QPoint(p2.x(),p2.y()+sens*D);
            }
            // store expanded line

            return (new AxLine(p1_para,p2_para));
        }

        //Border = arc ?
        else
        {   // find pm and draw the mediatrice
            pm=border->middle_point();   // point in the middle of the arc
            pm_line=QPoint(pm.x()+10.0,pm.y()+10.0);
            //find out if arc circle is concave or convexe

            // pm inside form ?
            // count the number of time the border cross the object
            for (int k=0;k<form.size();k++)
            {
                //if border =! current arc
                if (not(equal(border,form[k])))
                {
                    // if border = line
                    if (form[k]->getObjectType()==0)
                    {
                        cross_test=intersect_line_segment(QLine(pm,pm_line),QLine(form[k]->p1(),form[k]->p2()));
                    }
                    // if border = arc
                    else
                    {
                        cross_test=intersect_arc_line(QLine(pm,pm_line),AxArc(form[k]));
                    }
                    //border = current arc
                }
                else
                {
                    //Do the cross test with the border of the arc
                    cross_test=intersect_line_segment(QLine(pm,pm_line),QLine(form[k]->get_limits()));
                }
                if (cross_test[0][0]==1)
                {
                    //add ont to left cross for each intersection
                    cross_pts=intersecting_points(cross_test);
                    for (int q=0;q<cross_pts.size();q++)
                    {
                        if (cross_pts[q].y()>pm.y())
                        {
                            left_cross++;
                        }
                    }
                }
            }
            //         nombre pair de croisement ?
            if (left_cross%2==0)
            {
                inside_form=0;
            }
            else
            {
                inside_form=1;
            }
            qDebug()<< "inside_form" << inside_form;


            // pm inside cercle ?
            if (inside_form==1) // cercle= concave
            {
                // arc big enought ?
                if (AxArc(border).get_R()-R>0)
                {

                    cross_test=intersect_arc_line(QLine(AxArc(border).get_center(),border->p1()),AxArc(QLine(0,0,0,0),AxArc(border).get_center(),AxArc(border).get_R()-R,AxArc(border).is_clockwise()));
                    cross_test=intersect_arc_line(QLine(AxArc(border).get_center(),border->p2()),AxArc(QLine(0,0,0,0),AxArc(border).get_center(),AxArc(border).get_R()-R,AxArc(border).is_clockwise()));
                    p1_extend=closest_cross_to_pt(cross_test,border->p1());
                    p2_extend=closest_cross_to_pt(cross_test,border->p2());
                    return (new AxArc(QLine(p1_extend,p2_extend),AxArc(border).get_center(),AxArc(border).get_R()-R,AxArc(border).is_clockwise()));
                }

            }
            else
            {
                // store the expanded Arc   // cercle= convexe


                //                        cross_test1=intersect_arc_line(QLine(border->get_center(),border->p1()),AxArc(QLine(0,0,0,0),border->get_center(),border->get_R()+R,border->get_clockwise()));
                cross_test1=intersect_arc_line(QLine(AxArc(border).get_center(),border->p1()),AxArc(border->get_limits(),AxArc(border).get_center(),AxArc(border).get_R()+R,AxArc(border).is_clockwise()));
                cross_pts1=intersecting_points(cross_test1);
                k_1=closest_pt_to_p1_border(cross_pts1, border);

                //cross_test2=intersect_arc_line(QLine(border->get_center(),border->p2()),AxArc(QLine(0,0,0,0),border->get_center(),border->get_R()+R,border->get_clockwise()));
                cross_test2=intersect_arc_line(QLine(AxArc(border).get_center(),border->p2()),AxArc(border->get_limits(),AxArc(border).get_center(),AxArc(border).get_R()+R,AxArc(border).is_clockwise()));
                cross_pts2=intersecting_points(cross_test2);
                k_2=closest_pt_to_p2_border(cross_pts2, border);

                return(new AxArc(QLine(cross_pts1[k_1],cross_pts2[k_2]),AxArc(border).get_center(),AxArc(border).get_R()+R,AxArc(border).is_clockwise()));
            }
        }
    }
}

QVector<QPoint> AxShape::from_shape_to_pts()
{
    // calcule le nuage de point associé à une shape
    QVector<QPoint> nuage;
    // si le polygon est pas fermé, je le ferme
    if (shape[0]!=shape[shape.size()-1])
    {
        nuage.append(shape[0]->p1());
    }
    for (int k=0;k<shape.size();k++)
    {
        nuage.append(shape[k]->p2());
    }
    return nuage;
}

int AxShape::size()
{
    return shape.size();
}

int AxShape::next_point(int n)
{
    int n2=n+1;
    if (n2==shape.size())
    {
        n2=0;
    }
    return n2;
}

int AxShape::bord_externe()
{
    AxShape form=*this;
    QVector<QVector<int>>cross_test;
    QPoint pm;
    for (int q=0;q<form.size();q++)
    {
        int left_cross=0, right_cross=0;
        pm=form[q]->p1();
        QPoint pm_line=QPoint(pm.x()+10.0,pm.y()+10.0);
        for (int k=0;k<form.size();k++)
        {
            if ((k!=q) && (k!=form.previous_point(q)))
            {
                // if border = line
                if (form[k]->getObjectType()==0)
                {
                   cross_test=intersect_line_segment(QLine(pm,pm_line),QLine(form[k]->p1(),form[k]->p2()));
                }
                // if border = arc
                else
                {
                    cross_test=intersect_arc_line(QLine(pm,pm_line),AxArc(form[k]));
                }

                if (cross_test[0][0]==1)
                {
                    qDebug()<<"cross_test"<<cross_test;

                    //add ont to left cross for each intersection
                   if (cross_test[1][0]>pm.x())
                   {
                       left_cross++;
                   }
                   else
                   {
                      right_cross++;
                   }
                }
            }
            }
        if (left_cross==0 || right_cross==0)
        {
            return q;
         }
        //                nombre pair de croisement à gauche ?
    }
}

QVector<AxLine> AxShape::straight_skeletton()
{
    AxBorder *brut_segment_1,*brut_segment_2;
    QVector<QVector<int> > cross_test,cross_test1,cross_test2;
    QVector<AxLine> bisectrice_list;
    QPoint closest_cross,p1,p2;
    QVector<Vertice> Vertice_list;
    int k2, q1,q0,q2;
    int closest_distance, cross_distance,cross_distance1,cross_distance2,angle1,angle2;
    int k1_cross,k2_cross,k0, kprev, knext;
    AxLine segments_bisectrice;
    AxShape form;
    QVector<QPoint>form_const;
    int next_o;

    form=*this;



 //   ETAPE 1 : // Liste de sommets et calcul des bisectrices

    brut_segment_1=form.segment_brut_intern_expand(form[0],10);
    for (int k=1;k<form.size();k++)
    {
        brut_segment_2=form.segment_brut_intern_expand(form[k],10);

        cross_test=intersect_lignes(brut_segment_1,brut_segment_2);

        Vertice_list.append(Vertice(AxLine(form[k]->p1(),QPoint(cross_test[1][0],cross_test[1][1])),dynamic_cast<AxLine*>(form[k-1]),dynamic_cast<AxLine*>(form[k])));
        bisectrice_list.append(AxLine(form[k]->p1(),QPoint(cross_test[1][0],cross_test[1][1])));

        brut_segment_1=brut_segment_2;
    }
    brut_segment_2=form.segment_brut_intern_expand(form[0],10);
    cross_test=intersect_lignes(brut_segment_1,brut_segment_2);
    bisectrice_list.append(AxLine(form[0]->p1(),QPoint(cross_test[1][0],cross_test[1][1])));
    Vertice_list.append(Vertice(AxLine(form[0]->p1(),QPoint(cross_test[1][0],cross_test[1][1])),dynamic_cast<AxLine*>(form[form.size()-1]),dynamic_cast<AxLine*>(form[0])));


//    ETAPE 2 : MOTOYCYCLE
// calcul des pts de croisement des bisectrices
    QVector<bisection_intersection> bisection_intersection_list,bisection_intersection_list_unsorted;
    QVector<Vertice> priority_queue;

    for (int k1=0;k1<Vertice_list.size();k1++)
    {
        k0=previous_pt(k1,Vertice_list);
        k2=next_pt(k1,Vertice_list);

        cross_test1=intersect_demi_lines(Vertice_list[k1].get_bisection(),Vertice_list[k2].get_bisection());
        cross_test2=intersect_demi_lines(Vertice_list[k1].get_bisection(),Vertice_list[k0].get_bisection());

        if ((cross_test1[0][0]==1)&&(cross_test2[0][0]==1))
        {

            cross_distance1=distance(QPoint(cross_test1[1][0],cross_test1[1][1]),Vertice_list[k1].get_bisection().p1());
            cross_distance2=distance(QPoint(cross_test2[1][0],cross_test2[1][1]),Vertice_list[k1].get_bisection().p1());

            if (cross_distance1<cross_distance2)
            {
                knext=previous_pt(k2,Vertice_list);
                bisection_intersection_list.append(bisection_intersection(cross_distance1,QPoint(cross_test1[1][0],cross_test1[1][1]),new Vertice(Vertice_list[k1]),new Vertice(Vertice_list[k2])));
            }
            else
            {
                kprev=previous_pt(k0,Vertice_list);
                bisection_intersection_list.append(bisection_intersection(cross_distance2,QPoint(cross_test2[1][0],cross_test2[1][1]),new Vertice(Vertice_list[k0]),new Vertice(Vertice_list[k1])));
            }
        }
        else if ((cross_test1[0][0]==1)&&(cross_test2[0][0]==0))
        {
            cross_distance1=distance(QPoint(cross_test1[1][0],cross_test1[1][1]),Vertice_list[k1].get_bisection().p1());
            bisection_intersection_list.append(bisection_intersection(cross_distance1,QPoint(cross_test1[1][0],cross_test1[1][1]),new Vertice(Vertice_list[k1]),new Vertice(Vertice_list[k2])));

        }
        else if ((cross_test1[0][0]==0)&&(cross_test2[0][0]==1))
        {
            cross_distance2=distance(QPoint(cross_test2[1][0],cross_test2[1][1]),Vertice_list[k1].get_bisection().p1());
            bisection_intersection_list.append(bisection_intersection(cross_distance2,QPoint(cross_test2[1][0],cross_test2[1][1]),new Vertice(Vertice_list[k0]),new Vertice(Vertice_list[k1])));
        }


    }
    qDebug()<<"Avant tri :"<<endl<<bisection_intersection_list<<endl;

    bisection_intersection_list_unsorted=bisection_intersection_list;
    // je trie mes vertex du plus petit au plus grand suivant le critère de la distance du cross points
    std::sort(bisection_intersection_list.begin(), bisection_intersection_list.end(), [](const bisection_intersection& lhs, const bisection_intersection& rhs)
    {
        return(lhs.get_distance() < rhs.get_distance());
    });


    qDebug()<<"Apres tri :"<<endl<<bisection_intersection_list;
    AxLine bisection1;
    AxLine bisection2;
    int a,y2,y1,x2,x1,b,x3,y3;
    QVector<AxLine> Skeletton;

// ETAPE 3 :

    for (int mimi=0;mimi<4;mimi++)
//    while (Vertice_list.size()>2)
    {

        bisection1=bisection_intersection_list[0].get_vertice1().get_bisection();
        bisection1.setP2(bisection_intersection_list[0].get_cross_point());

        bisection2=bisection_intersection_list[0].get_vertice2().get_bisection();
        bisection2.setP2(bisection_intersection_list[0].get_cross_point());

        // add new bisectrice

        cross_test=intersect_lignes(bisection_intersection_list[0].get_vertice1().get_edge1(),bisection_intersection_list[0].get_vertice2().get_edge2());

        x1=bisection_intersection_list[0].get_cross_point().x();
        y1=bisection_intersection_list[0].get_cross_point().y();

        if (cross_test[0][0]==1)
        {

            segments_bisectrice=bisectrice(bisection_intersection_list[0].get_vertice1().get_edge1(),bisection_intersection_list[0].get_vertice2().get_edge2());


            p1=segments_bisectrice.p1();
            p2=segments_bisectrice.p2();

            if ((p2.x()-p1.x())!=0)
            {
                x2=cross_test[1][0];
                y2=cross_test[1][1];

                a=(p2.y()-p1.y())/(p2.x()-p1.x());
                b = y1-(a*x1);
                x3=x1+10;
                y3=a*x3+b;

                // détermination du sens de la bisectrice TODO : redéfinir/ améliorer
                int angle0=angle(bisection_intersection_list[0].get_vertice1().get_edge1(),bisection_intersection_list[0].get_vertice2().get_edge2());

                angle1=angle(new AxLine(QPoint(x1,y1),QPoint(x3,y3)), bisection_intersection_list[0].get_vertice1().get_edge1());
                angle2=angle(new AxLine(QPoint(x1,y1),QPoint(x3,y3)), bisection_intersection_list[0].get_vertice2().get_edge2());

                qDebug()<<"angle0"<<angle0<<"angle1"<<angle1<<"angle2"<<angle2;

                //if aigu
                if (angle0<M_PI && (angle1<M_PI/2 || angle2<M_PI/2))
                {
                    x3=x1-10;
                    y3=a*x3+b;
                }
                //if obtu
                if (angle0>M_PI && (angle1>M_PI/2 || angle2>M_PI/2))
                {
                    x3=x1-10;
                    y3=a*x3+b;
                }

                  // fin de la détermination du sens de la bisectrice

            }
            else
            {
                x3=x1;
                y3=y1+10;

                angle1=angle(new AxLine(QPoint(x1,y1),QPoint(x3,y3)),new AxLine(bisection1.p2(),bisection1.p1()));
                angle2=angle(new AxLine(QPoint(x1,y1),QPoint(x3,y3)),new AxLine(bisection2.p2(),bisection2.p1()));

                if ((angle1<M_PI/2)||(angle2<M_PI/2))
                {
                    x3=x1;
                    y3=y1-10;
                }
            }
        }
        // new bissector colinear ?
        else if (cross_test[0][0]==2)
        {
            p1=bisection_intersection_list[0].get_vertice2().get_edge2()->p1();
            p2=bisection_intersection_list[0].get_vertice2().get_edge2()->p2();

            if ((p2.x()-p1.x())!=0)
            {

                a=(p2.y()-p1.y())/(p2.x()-p1.x());
                b = y1-(a*x1);
                x3=x1+10;
                y3=a*x3+b;

                angle1=angle(new AxLine(QPoint(x1,y1),QPoint(x3,y3)),new AxLine(bisection1.p2(),bisection1.p1()));
                angle2=angle(new AxLine(QPoint(x1,y1),QPoint(x3,y3)),new AxLine(bisection2.p2(),bisection2.p1()));

                if ((angle1<M_PI/2)||(angle2<M_PI/2))
                {
                    x3=x1-10;
                    y3=a*x3+b;
                }
            }
            else
            {
                x3=x1;
                y3=y1+10;

                angle1=angle(new AxLine(QPoint(x1,y1),QPoint(x3,y3)),new AxLine(bisection1.p2(),bisection1.p1()));
                angle2=angle(new AxLine(QPoint(x1,y1),QPoint(x3,y3)),new AxLine(bisection2.p2(),bisection2.p1()));

                if ((angle1<M_PI/2)||(angle2<M_PI/2))
                {
                    x3=x1;
                    y3=y1-10;
                }
            }
        }

        Vertice newVertice=(Vertice(AxLine(bisection_intersection_list[0].get_cross_point(),QPoint(x3,y3)), bisection_intersection_list[0].get_vertice1().get_edge1(),bisection_intersection_list[0].get_vertice2().get_edge2()));

        q1=Vertice_list.indexOf(bisection_intersection_list[0].get_vertice1());
        q0=previous_pt(q1,Vertice_list);
        Vertice previousvertice=Vertice_list[q0];
        Vertice_list.remove(q1);

        qDebug()<<Vertice_list;


        q1=Vertice_list.indexOf(bisection_intersection_list[0].get_vertice2());
        q2=next_pt(q1,Vertice_list);

        qDebug()<<"q1"<<q1<<bisection_intersection_list[0].get_vertice2();


        Vertice nextvertice=Vertice_list[q2];

        qDebug()<<"crash1"<<Vertice_list << endl << bisection_intersection_list[0].get_vertice2();

        Vertice_list.remove(q1);

        qDebug()<<"avant"<<Vertice_list;
        Vertice_list.insert(q1, newVertice);
        qDebug()<<"après"<<Vertice_list;
        qDebug()<<"crash2"<<"q0"<<q0<<"q2"<<q2<<"size"<<Vertice_list.size();

        // bisection_intersection_list redefinition
        bisection_intersection_list.clear();

        for (int k1=0;k1<Vertice_list.size();k1++)
        {
            k0=previous_pt(k1,Vertice_list);
            k2=next_pt(k1,Vertice_list);

            cross_test1=intersect_demi_lines(Vertice_list[k1].get_bisection(),Vertice_list[k2].get_bisection());
            cross_test2=intersect_demi_lines(Vertice_list[k1].get_bisection(),Vertice_list[k0].get_bisection());



            if ((cross_test1[0][0]==1)&&(cross_test2[0][0]==1))
            {
//                cross_distance1=distance(QPoint(cross_test1[1][0],cross_test1[1][1]),Vertice_list[k1].get_bisection().p1());
//                cross_distance2=distance(QPoint(cross_test2[1][0],cross_test2[1][1]),Vertice_list[k1].get_bisection().p1());


                cross_distance1=distance(QPoint(cross_test1[1][0],cross_test1[1][1]),newVertice.get_bisection().p1());
                cross_distance2=distance(QPoint(cross_test2[1][0],cross_test2[1][1]),newVertice.get_bisection().p1());

                if (cross_distance1<cross_distance2)
                {
                    bisection_intersection_list.append(bisection_intersection(cross_distance1,QPoint(cross_test1[1][0],cross_test1[1][1]),new Vertice(Vertice_list[k1]),new Vertice(Vertice_list[k2])));
                }
                else
                {
                    bisection_intersection_list.append(bisection_intersection(cross_distance2,QPoint(cross_test2[1][0],cross_test2[1][1]),new Vertice(Vertice_list[k0]),new Vertice(Vertice_list[k1])));
                }
            }
            else if ((cross_test1[0][0]==1)&&(cross_test2[0][0]==0))
            {
                cross_distance1=distance(QPoint(cross_test1[1][0],cross_test1[1][1]),Vertice_list[k1].get_bisection().p1());
                bisection_intersection_list.append(bisection_intersection(cross_distance1,QPoint(cross_test1[1][0],cross_test1[1][1]),new Vertice(Vertice_list[k1]),new Vertice(Vertice_list[k2])));
            }
            else if ((cross_test1[0][0]==0)&&(cross_test2[0][0]==1))
            {
                cross_distance2=distance(QPoint(cross_test2[1][0],cross_test2[1][1]),Vertice_list[k1].get_bisection().p1());
                bisection_intersection_list.append(bisection_intersection(cross_distance2,QPoint(cross_test2[1][0],cross_test2[1][1]),new Vertice(Vertice_list[k0]),new Vertice(Vertice_list[k1])));
            }
        }
        Skeletton.append(bisection1);
        Skeletton.append(bisection2);

        std::sort(bisection_intersection_list.begin(), bisection_intersection_list.end(), [](const bisection_intersection& lhs, const bisection_intersection& rhs)
        {
            return(lhs.get_distance() < rhs.get_distance());
        });
    }
    // finalisation je relie les 2 dernières parties du squelette :

    Skeletton.append(AxLine(Vertice_list[0].get_bisection().p1(),Vertice_list[1].get_bisection().p1()));

    // affichage du squelette
    return Skeletton;
}

AxShape AxShape::expand_unfinished(int R)
{

        AxShape form=*this;


        //   le programme aggrandis d'un rayon R la forme

        QPoint pm, p1, p2,p1_para,p2_para,pm_line,closest_cross,center_,p1_extend,p2_extend;
        AxLine segment, segment1, segment2, segment_previous, segment_next;
        int a,b,a_para,b_para,D,R_;
        QVector<QVector<int>>cross_test,cross_med,cross_test1,cross_test2;
        AxShape stock_parallels,expand, brut_expland;
        QVector<QPoint> cross_pts,cross_pts1,cross_pts2;
        QLine arc_limits_,med;
        //            QVector<bool> yolo(20);
        //            yolo[8]=true;
        //            qDebug() << "yolo" << yolo;
        //            QVector<AxShape> expand;
        int left_cross,sens,inside_form,k_next,k_previous,clockwise_,k1=0,k_1=0,k_2=0,interne=0,closest_pos,number_of_cross,bord_externe=0,k,closest_k;

        // ETAPE 1 : extraction des bords convexe et concave de la forme

        QVector<QVector<AxShape>> convexeconcave;
        QVector<AxShape> concave, convexe_expand_list;

        AxShape convexe_expand;
        QVector<AxShape> convexe;


        AxBorder* brut_segment_1, *brut_segment_2;

        convexeconcave=form.convexe_concave();

    //    for (int k=0;k<convexeconcave[0].size();k++)
    //    {
    //        convexe.append(&convexeconcave[0][k]);
    //    }
    //    for (int k=0;k<convexeconcave[1].size();k++)
    //    {
    //        concave.append(&convexeconcave[1][k]);
    //    }

        convexe=convexeconcave[0];
        concave=convexeconcave[1];

        // ETAPE 2 extention des arc et des segments convexe

        for (int n=0;n<convexe.size();n++)// for all grp of segment convexe
        {
            brut_segment_1=form.segment_brut_expand(convexe[n][0],R);
            convexe_expand.append(brut_segment_1);

            for (int k=1;k<convexe[n].size();k++)// for all border of segment convexe
            {
                // draw his parallel segment

                brut_segment_2=form.segment_brut_expand(convexe[n][k],R);

                // define a new arc between the two segment
                if (not_equal(brut_segment_1->p2(), brut_segment_2->p1()))
                {
                    arc_limits_=QLine(brut_segment_1->p2(),brut_segment_2->p1());
                    center_=convexe[n][k]->p1();
                    R_=distance(brut_segment_1->p2(),center_);
                    clockwise_=1;
                    convexe_expand.append(new AxArc(arc_limits_,center_,R_,clockwise_));
                }
                convexe_expand.append(brut_segment_2);
                brut_segment_1=brut_segment_2;
            }

            qDebug()<<"lol"<<convexe_expand;


            convexe_expand_list.append(convexe_expand);
            qDebug()<<"lol";

            convexe_expand=AxShape();
        }

        AxShape concave_expand;
        QVector<AxShape> concave_expand_list;
    // ETAPE 3 : extention des segments concaves

        for (int n=0;n<concave.size();n++)
        {
            brut_segment_1=form.segment_brut_expand(concave[n][0],R);

            concave_expand.append(brut_segment_1);

            for (int k=1;k<concave[n].size();k++)
            {
//                expantion_limits=concave[n].get_expantion_limits();        // TO DO

//                if (expantion_limits<R)
//                {
                    brut_segment_2=form.segment_brut_expand(concave[n][k],R);
                    cross_test=intersect_border(brut_segment_1,brut_segment_2);

                    // si croisement
                    if (cross_test[0][0]==1)
                    {
                        // redéfinir les segments

                        brut_segment_1->setP2(QPoint(cross_test[1][0],cross_test[1][1]));
                        brut_segment_2->setP1(QPoint(cross_test[1][0],cross_test[1][1]));
                    }
                    else
                    {
                        // define a new arc between the two segments

                        if (not_equal(brut_segment_1->p2(), brut_segment_2->p1()))
                        {
                            arc_limits_=QLine(brut_segment_1->p2(),brut_segment_2->p1());
                            center_=concave[n][k]->p1();
                            R_=distance(brut_segment_1->p2(),center_);
                            clockwise_=1;
                            concave_expand.append(new AxArc(arc_limits_,center_,R_,clockwise_));

                        }
                    }

                    brut_segment_1=brut_segment_2;
                    concave_expand.append(brut_segment_2);
//                }

            }
            concave_expand_list.append(concave_expand);
            concave_expand=AxShape();
        }

        QVector<AxLine> bisectrice_list;

        for(int n=0;n<concave.size();n++)
        {
            brut_segment_1=form.segment_brut_expand(concave[n][0],10);

            for (int k=1;k<concave[n].size();k++)
            {
                brut_segment_2=form.segment_brut_expand(concave[n][k],10);

                cross_test=intersect_lignes(brut_segment_1,brut_segment_2);

                bisectrice_list.append(AxLine(concave[n][k]->p1(),QPoint(cross_test[1][0],cross_test[1][1])));

                brut_segment_1=brut_segment_2;
            }
        }
        qDebug()<<"convexe_expand_list"<<convexe_expand_list;



        int closest_distance, cross_distance;
        int k1_cross,k2_cross,k2;

        for (int k=0;k<bisectrice_list.size();k++)
        {
            qDebug()<<k;
             closest_distance=100000000000000000000000.0;
             // on cherche la "plus proche intersection parmis toutes les bisectrices
             for (int k1=0;k1<bisectrice_list.size();k1++)
             {
                 k2=next_pt(k1,bisectrice_list);
                 {
                     cross_test=intersect_demi_lines(bisectrice_list[k1],bisectrice_list[k2]);
                     cross_distance=distance(QPoint(cross_test[1][0],cross_test[1][1]),bisectrice_list[k1].p1());

                     if ((cross_test[0][0]==1) && (cross_distance<closest_distance))
                     {
                         closest_cross=QPoint(cross_test[1][0],cross_test[1][1]);
                         closest_distance=cross_distance;
                         k1_cross=k1;
                         k2_cross=k2;
                     }
                 }
             }
            }

            if (closest_distance!=100000000000000000000000.0)
            {

                //            qDebug()<<k1_cross<<k2_cross<<bisectrice_list;

                // on retire la bisectrice de la liste
                bisectrice_list.remove(k1_cross);
                if(k1_cross>k2_cross)
                {
                    bisectrice_list.remove(k2_cross);
                }
                else
                {
                    bisectrice_list.remove(k2_cross-1);
                }
                // on creer la nouvelle bisectrice résultante
                //                closest_cross;
                //                max_expand_distance[]=distance(cross_distance)
            }

            // ETAPE 4 : ajustement des bords : a faire

}
/*
//int AxShape::bord_externe()
//{
//    // renvoie le numéro de ligne du premier bord externe de l'axShape que l'on détecte
//    // cad le bord dont au moins le p1 est à l'extérieur
//    int left_cross,right_cross;
//    QPoint p1,p2,pp1,pp2,pm;
//    AxLine segment;
//    QLine med;
//    QVector<QVector<int>>cross_test;
//    QVector<QPoint> cross_pts;

//    for (int n=0;n<shape.size();n++)// for all border of form
//    {
//        // initialisation
//        left_cross=0;
//        right_cross=0;

//            // median point of the segment
//            p1=shape[n]->p1();
//            p2=shape[n]->p2();
//            segment=AxLine(p1,p2);
//            pm=segment.center();
//            qDebug() << "lol";

//            // mediatrice de la forme:
//            med=shape[n]->mediatrice();
//         // count the number of time the mediatrice cross the object
//            qDebug() << "lol";

//            for (int k=0;k<shape.size();k++)
//            {
//                qDebug()<<"lol";
//                 //if segment form =! current segment
//                 if (n!=k)
//                 {
//                     // if border = line
//                     if (shape[k]->getObjectType()==0)
//                     {
//                        cross_test=intersect_line_segment(med,QLine(shape[k]->p1(),shape[k]->p2()));
//                     }
//                     // if border = arc
//                     else
//                     {
//                         cross_test=intersect(med,AxArc(shape[k]));
//                     }
//                     qDebug()<<"lol"<<cross_test;
//                     if (cross_test[0][0]==1)
//                     {
//                        cross_pts=points_extraction(cross_test);
//                        // segment à étendre non vertical ?
//                        for (int q=0;k<cross_pts.size();q++)
//                        {
//                            if (p1.x()!=p2.x())
//                            {
//                                if (cross_pts[q].y()>pm.y())
//                                {
//                                    left_cross++;
//                                }
//                                else
//                                {
//                                    right_cross++;
//                                }
//                             }
//                                 // segment à étendre vertical
//                             else
//                             {
//                                if (cross_pts[q].x()>pm.x())
//                                {
//                                    left_cross++;
//                                }
//                                else
//                                {
//                                    right_cross++;
//                                }
//                             }
//                        }
//                     }
//                  }
//            }
//            if (right_cross==0 || left_cross==0)
//            {
//                return n;
//            }
//        }
//    }
*/
int AxShape::previous_point(int n)
{
    int n2=n-1;
    if (n2==-1)
    {
        n2=shape.size()-1;
    }
    return n2;
}

void AxShape::expand(int dec)
{
    offset(dec);

    link();

    separate();

    classify();

    clean();



/*
    AxShape shape, transit_shape;
    QVector<AxShape> expanded_shape;
    QVector<QPoint>shape_const ;
    QVector<QVector<int>> cross_test;
    int next_o;
    QPoint p1,p2;
    int k_prev, k_next;


    Path subj;
    Paths solution;


    shape=*this;

    // convertion de shape vers path
    for (int k=0;k<shape.size();k++)
    {
        if (shape[k]->getObjectType()==0)
        {
            subj << IntPoint(shape[k]->x1(),shape[k]->y1());
        }

        if (shape[k]->getObjectType()==1)
        {
            if (AxArc(shape[k]).get_R()<=D)
            {
                k_prev=shape.previous_point(k);
                k_next=shape.next_point(k);
                if ((shape[k_prev]->getObjectType()==0) && (shape[k_next]->getObjectType()==0))
                {
                    cross_test=intersect_demi_lines(shape[k_prev],AxLine(shape[k_next]->p2(),shape[k_next]->p1()));
                    if (cross_test[0][0]==1)
                    {
                        subj << IntPoint(cross_test[1][0],cross_test[1][1]);
                    }
                    else
                    {
                        qDebug("Error les droites avant et après l'arc ne se croisent pas => arc supprimé");
                    }
                }
                else
                {
                    qDebug("Error il n'y a pas de droite avant et après l'arc");
                }
            }
        }
    }

    ClipperOffset co(10000,0.25);
    co.AddPath(subj, jtMiter, etClosedPolygon);
    co.Execute(solution, D);

    // convertion de paths vers Shape

    for (int k1=0;k1<solution.size();k1++)
    {
        for (int k2=0;k2<solution[k1].size()-1;k2++)
        {
            p1=QPoint(solution[k1][k2].X,solution[k1][k2].Y);
            p2=QPoint(solution[k1][k2+1].X,solution[k1][k2+1].Y);
            transit_shape.append(new AxLine(p1,p2));
        }
        p1=QPoint(solution[k1][solution[k1].size()-1].X,solution[k1][solution[k1].size()-1].Y);
        p2=QPoint(solution[k1][0].X,solution[k1][0].Y);
        transit_shape.append(new AxLine(p1,p2));
        expanded_shape.append(transit_shape);
    }

    return expanded_shape;
*/
}

void AxShape::analyse_dir()
{
    for (int i = 0;i< shape.size();i++)
    {
        qDebug() << "-----------------Segment "<<i;
        int last,next ;
        if (i == shape.size()-1) next = 0;
        else next = i+1;

        if (i != 0) last = i-1;
        else last = shape.size()-1;

        if (shape[last]->getObjectType() == 0)// if last is line
        {
            if (angle(shape[last],shape[i])>M_PI)
            {
                shape[i]->set_p1_dir(2);
                qDebug() << "--------2 ";
            }
            else if (angle(shape[last],shape[i])<M_PI)
            {
                shape[i]->set_p1_dir(1);
                qDebug() << "--------1 ";
            }
            else
                shape[i]->set_p1_dir(0);

            qDebug() << "----------------------------------- ";
            if (angle(shape[i],shape[next])>M_PI)
            {
                shape[i]->set_p2_dir(2);
                 qDebug() << "--------2 ";
            }
            else if (angle(shape[i],shape[next])<M_PI)
            {
                shape[i]->set_p2_dir(1);
                 qDebug() << "--------1 ";
            }
            else shape[i]->set_p2_dir(0);

            qDebug() << "dir1" << shape[i]->get_p1_dir();
            qDebug() << "dir2" << shape[i]->get_p2_dir();
            qDebug() << "------------------------------";
        }


    }

}

void AxShape::link()
{
    QVector<AxBorder*> shape2;

    qDebug() << "inside link";


   for (int i = 0;i< shape.size();i++)
    {
       qDebug() << "-----------------Segment "<<i;

           //qDebug() << "for----------------------";
           int last,next ;
           if (i == shape.size()-1) next = 0;
           else next = i+1;

           if (i != 0) last = i-1;
           else last = shape.size()-1;







           if (shape[i]->get_p1_dir() == 1
                && shape[last]->getObjectType()==0
                && shape[i]->getObjectType()==0)//---------------first edge go left
           {
               qDebug() << "if 1";
               AxLine *first= new AxLine(shape[i]->get_last_p1(),shape[i]->p1());



               shape2.append(first);
           }

           else if (shape[i]->get_p1_dir() == 2
                && shape[last]->getObjectType()==0
                && shape[i]->getObjectType()==0)//---------------first edge go right
           {
               qDebug() << "if 1-2";

               // calculate radius of arc

               int radius =shape[i]->get_last_offset();
               QPoint center = shape[i]->get_last_p1();



               AxArc *firsta= new AxArc(QLine(shape[last]->p2(),
                                        shape[i]->p1()),center,radius,false);



               shape2.append(firsta);
           }

           shape2.append(shape[i]);



           if (shape[next]->get_p1_dir() == 1
                && shape[next]->getObjectType()==0
                && shape[i]->getObjectType()==0)//---------------second edge go left
           {
               qDebug() << "if 2";

               AxLine *second= new AxLine(shape[i]->p2(),shape[i]->get_last_p2());

               shape2.append(second);

           }






    }

   shape2.swap(shape);

}
void AxShape::separate()
{

}
void AxShape::classify()
{

}
void AxShape::clean()
{

}





void AxShape::display(QPaintDevice *device, int width, const QColor &color)
{
    for (int k=0;k<shape.size();k++)
    {
        shape[k]->display(device, width, color);
    }
}


bool AxShape::contain(QPoint pm)
{
    AxShape form=*this;
    QVector<QVector<int>>cross_test;
    int left_cross=0;
    QPoint pm_line=QPoint(pm.x()+10.0,pm.y()+10.0);

    for (int k=0;k<form.size();k++)
    {
        //if border =! current arc
//        if (n!=k)
        {
            // if border = line
            if (form[k]->getObjectType()==0)
            {
               cross_test=intersect_line_segment(QLine(pm,pm_line),QLine(form[k]->p1(),form[k]->p2()));
            }
            // if border = arc
            else
            {
                cross_test=intersect(QLine(pm,pm_line),AxArc(form[k]));
            }
            if (cross_test[0][0]==1)
            {
                //add ont to left cross for each intersection
               if (cross_test[1][0]>pm.x())
               {
                   left_cross++;
               }
            }
        }
     }
    //                nombre pair de croisement à gauche ?
      if (left_cross%2==0)
      //aller à gauche
      {
           return true;
      }
      else
      //aller à droite
      {
          return false;
      }
}

void AxShape::offset(int dec)
{


    for(int i =0;i<shape.size();i++)
    {
        shape[i]->translate(dec);
    }





}

void AxShape::set_cw(bool c)
{

    for(int i =0;i<shape.size();i++)
    {
        shape[i]->set_cw(c);
    }
}

void AxShape::set_ccw(bool c)
{

    for(int i =0;i<shape.size();i++)
    {
        shape[i]->set_cw(!c);
    }
}

QDebug operator<<(QDebug dbg, const AxShape &type)
{
    dbg.nospace() << "AxShape("<<type.shape<<")";
    return dbg.maybeSpace();
}

AxShape::AxShape(AxShape const& AxShapeacopier):shape(AxShapeacopier.shape)
{
    shape.clear();
    for (int k=0;k<AxShapeacopier.shape.size();k++)
    {
        if (AxShapeacopier.shape[k]->getObjectType()==0)
        {
            shape.append(new AxLine((AxShapeacopier[k])));
        }
        else
        {
            shape.append( new AxArc((AxShapeacopier[k])));
        }
    }
}


AxShape AxShape::operator=(const AxShape &shapeacopier)
{
    if(this !=&shapeacopier)
    {
        shape.clear();
        for (int k=0;k<shapeacopier.shape.size();k++)
        {
            if (shapeacopier[k]->getObjectType()==0)
            {
                shape.append( new AxLine(shapeacopier[k]));
            }
            else
            {
                shape.append( new AxArc(shapeacopier[k]));
            }
        }
    }
    return *this;
}
