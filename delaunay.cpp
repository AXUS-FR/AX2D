#include "delaunay.h"
#include <common_functions.h>
#include <QDebug>
#include <axtriangle.h>

QVector<AxTriangle> delaunay(QVector<QPointF> nuage, int a)
{
    // Je suppose que form est un tableau qui confient une liste de points selon la forme :
//    [p1
//     p2
//     p3
//     p4
//     ...]
//   le programme applique la methode de delaunay au nuage de pts

    // cercle instrit
    QPointF p1,p2,p3,pc;
    AxCircle cercle_inscrit_test;
    QVector <QVector<QPointF>> triangle_list;
    QVector<QPointF> triangle;
    qreal R;
    int keep_triangle;
    QVector<AxTriangle> tri_list;

    // For all possible triangle on nuage:
    for (int n=0;n<nuage.size();n++)
    {
                p1=QPointF(nuage[n]);
                for (int k=n+1;k<nuage.size();k++)
                {
                    p2=QPointF(nuage[k]);
                    {
                        for (int q=k+1;q<nuage.size();q++)
                        {
                            p3=QPointF(nuage[q]);
                            cercle_inscrit_test=cercle_inscrit(p1,p2,p3);
                            pc=cercle_inscrit_test.get_center();
                            R=cercle_inscrit_test.get_R();
                            keep_triangle=1;
                            // for all pts on nuage exept those of the triangle
                            for (int m=0;m<nuage.size();m++)
                            {
                                // point not on triangle ?

                                if (nuage[m]!=p1 && nuage[m]!=p2 && nuage[m]!=p3)
                                {

                                        //distance du point par rapport au centre inférieure à R
                                        // qDebug() << "distance" << distance(nuage[m],pc) << endl<< "R"<< R << endl;

                                        if (distance(nuage[m],pc)<R)
                                        {
                                            keep_triangle=0;
                                        }
                                }
                            }
                            //save triangle
                            if (keep_triangle==1)
                            {
                                triangle.append(p1);
                                triangle.append(p2);
                                triangle.append(p3);
                                triangle_list.append(triangle);
                                triangle=QVector<QPointF>();
                            }
                    }
                }
            }
    }

    for (int k=0;k<triangle_list.size();k++)
    {
        AxTriangle tri(triangle_list[k][0],triangle_list[k][1],triangle_list[k][2]);
        tri_list.append(tri);
    }

    return (tri_list);
}

QVector<QVector<QPointF>> delaunay(QVector<QPointF> nuage)
{
    // Je suppose que form est un tableau qui confient une liste de points selon la forme :
    //    [p1
    //     p2
    //     p3
    //     p4
    //     ...]
    //   le programme applique la methode de delaunay au nuage de pts

    // cercle instrit
    QPointF p1,p2,p3,pc;
    AxCircle cercle_inscrit_test;
    QVector <QVector<QPointF>> triangle_list;
    QVector<QPointF> triangle;
    qreal R;
    int keep_triangle;
    QVector<AxTriangle> tri_list;

    // For all possible triangle on nuage:
    for (int n=0;n<nuage.size();n++)
    {
        p1=QPointF(nuage[n]);
        for (int k=n+1;k<nuage.size();k++)
        {
            p2=QPointF(nuage[k]);
            {
                for (int q=k+1;q<nuage.size();q++)
                {
                    p3=QPointF(nuage[q]);
                    cercle_inscrit_test=cercle_inscrit(p1,p2,p3);
                    pc=cercle_inscrit_test.get_center();
                    R=cercle_inscrit_test.get_R();
                    keep_triangle=1;
                    // for all pts on nuage exept those of the triangle
                    for (int m=0;m<nuage.size();m++)
                    {
                        // point not on triangle ?

                        if (nuage[m]!=p1 && nuage[m]!=p2 && nuage[m]!=p3)
                        {

                            //distance du point par rapport au centre inférieure à R
                            // qDebug() << "distance" << distance(nuage[m],pc) << endl<< "R"<< R << endl;

                            if (distance(nuage[m],pc)<R)
                            {
                                keep_triangle=0;
                            }
                        }
                    }
                    //save triangle
                    if (keep_triangle==1)
                    {
                        triangle.append(p1);
                        triangle.append(p2);
                        triangle.append(p3);
                        triangle_list.append(triangle);
                        triangle=QVector<QPointF>();
                    }
                }
            }
        }
    }
    return triangle_list;
}



//useless
QVector<AxTriangle> delaunay(QPolygonF nuage)
{
    // Je suppose que form est un tableau qui confient une liste de points selon la forme :
//    [p1
//     p2
//     p3
//     p4
//     ...]
//   le programme applique la methode de delaunay au nuage de pts

    // cercle instrit
    QPointF p1,p2,p3,pc,pm1,pm2,pm3,p_grav;
    AxCircle cercle_inscrit_test;
    QVector <QVector<QPointF>> triangle_list;
    QVector<QPointF> triangle;
    qreal R;
    int keep_triangle;
    QLineF L1,L2,L3;
    bool test;
    QVector<QVector<qreal>> cross_test;
    QVector<AxTriangle> tri_list;

    // For all possible triangle on nuage:
    for (int n=0;n<nuage.size();n++)
    {
                p1=QPointF(nuage[n]);
                for (int k=n+1;k<nuage.size();k++)
                {
                    p2=QPointF(nuage[k]);
                    {
                        // Si point milieu n'appartient pas au polygon

                        for (int q=k+1;q<nuage.size();q++)
                        {
                            p3=QPointF(nuage[q]);
                            cercle_inscrit_test=cercle_inscrit(p1,p2,p3);
                            pc=cercle_inscrit_test.get_center();
                            R=cercle_inscrit_test.get_R();
                            keep_triangle=1;

                            // for all pts on nuage exept those of the triangle
                            for (int m=0;m<nuage.size();m++)
                            {
                                // point not on triangle ?

                                if (nuage[m]!=p1 && nuage[m]!=p2 && nuage[m]!=p3)
                                {

                                        //distance du point par rapport au centre inférieure à R
                                        // qDebug() << "distance" << distance(nuage[m],pc) << endl<< "R"<< R << endl;

                                        if (distance(nuage[m],pc)<R)
                                        {
                                            keep_triangle=0;
                                        }
                                }
                            }
                            //save triangle
                            if (keep_triangle==1)
                            {
                                triangle.append(p1);
                                triangle.append(p2);
                                triangle.append(p3);
                                triangle_list.append(triangle);
                                triangle=QVector<QPointF>();
                            }
                    }
                }
            }
    }
    // for all segments of selected triangles
//remove those which aren't inside the triangle

    for (int n=0;n<triangle_list.size();n++)
    {
        L1=QLineF(triangle_list[n][0],triangle_list[n][1]);
        L2=QLineF(triangle_list[n][0],triangle_list[n][2]);
        L3=QLineF(triangle_list[n][1],triangle_list[n][2]);

//        pm1=L1.center();
//        pm2=L2.center();
//        pm3=L3.center();


        // centre de grav
        cross_test=intersect(QLineF(pm1,triangle_list[n][2]),QLineF(pm3,triangle_list[n][0]));
        p_grav=QPointF(cross_test[1][0],cross_test[1][1]);
        test=nuage.containsPoint(p_grav,Qt::OddEvenFill);

        qDebug() << "pm1"<< pm1 << "pm3 "<<pm3 << "p_grav" << p_grav << endl;


        // If p_grav oustide polygon
        if (test==false)
        {
            //retirer triangle
            triangle_list.remove(n);
        }
    }

// Convert QVector<QVector<QPointF>> to QVector triangle
    for (int k=0;k<triangle_list.size();k++)
    {
        AxTriangle tri(triangle_list[k][0],triangle_list[k][1],triangle_list[k][2]);
        tri_list.append(tri);
    }
//    return (triangle_list);
    return tri_list;
}


QVector<AxLine> voronoi(QVector<AxTriangle> triangle_list)
{
    QPointF pc1,pc2;
    QVector<AxLine> voronoi;
    for (int k1=0;k1<triangle_list.size();k1++)
    {
        for (int k2=0;k2<triangle_list.size();k2++)
        {
            if (k1!=k2)
            {
                AxLine line11=triangle_list[k1].getline1();
                AxLine line12=triangle_list[k1].getline2();
                AxLine line13=triangle_list[k1].getline3();

                AxLine line21=triangle_list[k2].getline1();
                AxLine line22=triangle_list[k2].getline2();
                AxLine line23=triangle_list[k2].getline3();

                if (line11==line21 || line11==line22 || line11==line23 || line12==line21 || line12==line22 || line12==line23 || line13==line21 || line13==line22 || line13==line23)
                {
                    pc1=triangle_list[k1].cercle_inscrit().get_center();
                    pc2=triangle_list[k2].cercle_inscrit().get_center();
                    voronoi.append(AxLine(pc1,pc2));
                }
            }
        }
    }
    return voronoi;
}
