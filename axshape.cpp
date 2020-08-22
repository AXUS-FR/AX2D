#include "AxArc.h"
#include "AxShape.h"
#include "QObject"
#include "QVector"
#include "common_functions.h"
#include "math.h"

AxShape:: AxShape()
{
    shape=QVector<AxBorder*>();
}

void AxShape::append(AxBorder* border)
{
    shape.append(border);

}

AxShape AxShape::straight_skeletton()
{

}


void AxShape::insert(int k,AxBorder* border)
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
        shape[k]=0;
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
    qreal teta;
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

AxBorder* AxShape::segment_brut_expand(AxBorder *border, qreal R)
{
    AxShape form=*this;

    QPointF pm, p1, p2,p1_para,p2_para,pm_line,closest_cross,center_,p1_extend,p2_extend;
    AxLine segment, segment1, segment2, segment_previous, segment_next;
    qreal a,b,a_para,b_para,D,R_;
    QVector<QVector<qreal>>cross_test,cross_med,cross_test1,cross_test2;
    AxShape stock_parallels,expand, brut_expland;
    QVector<QPointF> cross_pts,cross_pts1,cross_pts2;
    QLineF arc_limits_,med;
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
                        cross_test=intersect_line_segment(med,QLineF(form[k]->p1(),form[k]->p2()));
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
                    p1_para=QPointF((p1.y()+R/sqrt(1+a*a)-b_para)/a,p1.y()+R/sqrt(1+a*a));
                    p2_para=QPointF((p2.y()+R/sqrt(1+a*a)-b_para)/a,p2.y()+R/sqrt(1+a*a));
                }
                else if (sens==-1)
                {
                    p1_para=QPointF((p1.y()-R/sqrt(1+a*a)-b_para)/a,p1.y()-R/sqrt(1+a*a));
                    p2_para=QPointF((p2.y()-R/sqrt(1+a*a)-b_para)/a,p2.y()-R/sqrt(1+a*a));
                }


            }
            else if (p2.x()==p1.x())
            {
                D=R;
                // cas particulier segment vertical
                p1_para=QPointF(p1.x()+sens*D,p1.y());
                p2_para=QPointF(p2.x()+sens*D,p2.y());
            }
            else if (p2.y()==p1.y())
            {
                D=R;
                // cas particulier segment horizontal
                p1_para=QPointF(p1.x(),p1.y()+sens*D);
                p2_para=QPointF(p2.x(),p2.y()+sens*D);
            }
            // store expanded line

            return (new AxLine(p1_para,p2_para));
        }

        //Border = arc ?
        else
        {   // find pm and draw the mediatrice
            pm=border->middle_point();   // point in the middle of the arc
            pm_line=QPointF(pm.x()+10.0,pm.y()+10.0);
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
                        cross_test=intersect_line_segment(QLineF(pm,pm_line),QLineF(form[k]->p1(),form[k]->p2()));
                    }
                    // if border = arc
                    else
                    {
                        cross_test=intersect_arc_line(QLineF(pm,pm_line),AxArc(form[k]));
                    }
                    //border = current arc
                }
                else
                {
                    //Do the cross test with the border of the arc
                    cross_test=intersect_line_segment(QLineF(pm,pm_line),QLineF(form[k]->get_limits()));
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

                    cross_test=intersect_arc_line(QLineF(AxArc(border).get_center(),border->p1()),AxArc(QLineF(0,0,0,0),AxArc(border).get_center(),AxArc(border).get_R()-R,AxArc(border).is_clockwise()));
                    cross_test=intersect_arc_line(QLineF(AxArc(border).get_center(),border->p2()),AxArc(QLineF(0,0,0,0),AxArc(border).get_center(),AxArc(border).get_R()-R,AxArc(border).is_clockwise()));
                    p1_extend=closest_cross_to_pt(cross_test,border->p1());
                    p2_extend=closest_cross_to_pt(cross_test,border->p2());
                    return (new AxArc(QLineF(p1_extend,p2_extend),AxArc(border).get_center(),AxArc(border).get_R()-R,AxArc(border).is_clockwise()));
                }

            }
            else
            {
                // store the expanded Arc   // cercle= convexe


                //                        cross_test1=intersect_arc_line(QLineF(border->get_center(),border->p1()),AxArc(QLineF(0,0,0,0),border->get_center(),border->get_R()+R,border->get_clockwise()));
                cross_test1=intersect_arc_line(QLineF(AxArc(border).get_center(),border->p1()),AxArc(border->get_limits(),AxArc(border).get_center(),AxArc(border).get_R()+R,AxArc(border).is_clockwise()));
                cross_pts1=intersecting_points(cross_test1);
                k_1=closest_pt_to_p1_border(cross_pts1, border);

                //cross_test2=intersect_arc_line(QLineF(border->get_center(),border->p2()),AxArc(QLineF(0,0,0,0),border->get_center(),border->get_R()+R,border->get_clockwise()));
                cross_test2=intersect_arc_line(QLineF(AxArc(border).get_center(),border->p2()),AxArc(border->get_limits(),AxArc(border).get_center(),AxArc(border).get_R()+R,AxArc(border).is_clockwise()));
                cross_pts2=intersecting_points(cross_test2);
                k_2=closest_pt_to_p2_border(cross_pts2, border);

                return(new AxArc(QLineF(cross_pts1[k_1],cross_pts2[k_2]),AxArc(border).get_center(),AxArc(border).get_R()+R,AxArc(border).is_clockwise()));

            }
        }
    }

}

QVector<QPointF> AxShape::from_shape_to_pts()
{
    // calcule le nuage de point associé à une shape
    QVector<QPointF> nuage;
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
    QVector<QVector<qreal>>cross_test;
    QPointF pm;
    for (int q=0;q<form.size();q++)
    {
        int left_cross=0, right_cross=0;
        pm=form[q]->p1();
        QPointF pm_line=QPointF(pm.x()+10.0,pm.y()+10.0);
        for (int k=0;k<form.size();k++)
        {
            if ((k!=q) && (k!=form.previous_point(q)))
            {
                // if border = line
                if (form[k]->getObjectType()==0)
                {
                   cross_test=intersect_line_segment(QLineF(pm,pm_line),QLineF(form[k]->p1(),form[k]->p2()));
                }
                // if border = arc
                else
                {
                    cross_test=intersect_arc_line(QLineF(pm,pm_line),AxArc(form[k]));
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

//int AxShape::bord_externe()
//{
//    // renvoie le numéro de ligne du premier bord externe de l'axShape que l'on détecte
//    // cad le bord dont au moins le p1 est à l'extérieur
//    int left_cross,right_cross;
//    QPointF p1,p2,pp1,pp2,pm;
//    AxLine segment;
//    QLineF med;
//    QVector<QVector<qreal>>cross_test;
//    QVector<QPointF> cross_pts;

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
//                        cross_test=intersect_line_segment(med,QLineF(shape[k]->p1(),shape[k]->p2()));
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

int AxShape::previous_point(int n)
{
    int n2=n-1;
    if (n2==-1)
    {
        n2=shape.size()-1;
    }
    return n2;
}


void AxShape::affiche(QPaintDevice *device, int width, const QColor &color)
{
    for (int k=0;k<shape.size();k++)
    {
        shape[k]->affiche(device, width, color);
    }
}


bool AxShape::contain(QPointF pm)
{
    AxShape form=*this;
    QVector<QVector<qreal>>cross_test;
    int left_cross=0;
    QPointF pm_line=QPointF(pm.x()+10.0,pm.y()+10.0);

    for (int k=0;k<form.size();k++)
    {
        //if border =! current arc
//        if (n!=k)
        {
            // if border = line
            if (form[k]->getObjectType()==0)
            {
               cross_test=intersect_line_segment(QLineF(pm,pm_line),QLineF(form[k]->p1(),form[k]->p2()));
            }
            // if border = arc
            else
            {
                cross_test=intersect(QLineF(pm,pm_line),AxArc(form[k]));
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

//AxShape AxShape::operator=(const qreal k)
//{
//}

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
