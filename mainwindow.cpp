#include "AxLine.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStyle>
#include <qdesktopwidget.h>
#include <qrandom.h>
#include <istream>
#include <axcircle.h>
#include <common_functions.h>
#include <QDebug>
#include <delaunay.h>
#include <expand_form.h>
#include <math.h>
#include <axshape.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set the size of window
    QSize availableSize = qApp->desktop()->availableGeometry(this).size();
        int width = availableSize.width();
        int height = availableSize.height();
        qDebug() << "Available dimensions " << width << "x" << height;
        width *= 0.7; // 70% of the screen size
        height *= 0.7; // 70% of the screen size
        qDebug() << "Computed dimensions " << width << "x" << height;
        QSize newSize( width, height );

        setGeometry(
            QStyle::alignedRect(
                Qt::LeftToRight,
                Qt::AlignCenter,
                newSize,
                qApp->desktop()->availableGeometry(this)
            )
        );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    // size of window
    QSize availableSize = qApp->desktop()->availableGeometry(this).size();
    int width = int(availableSize.width()*0.7);
    int height = int(availableSize.height()*0.7);
//-----------------------------------------------------------------------------------------------
/* //   Cross functions test


    // variables
    QLineF arc_limits, line_trans1;
    QPointF pc,px,p1,p2;
    qreal R,teta1,teta2,startAngle,spanAngle;
    int clockwise;
    QVector<QVector<qreal>> segment_stock,cross_test;
    QVector<qreal> line_trans;



    //Initialisation

    R=300;
    clockwise=0;

   // Generate a random arc

    p1=QPointF(random(0,width),random(0,height));
    p2=QPointF(random(0,width),random(0,height));
    // Generate a random center
    pc=QPointF(random(R,width-R),random(R,height-R));

    arc_limits=QLineF(p1,p2);
    AxArc arc(arc_limits,pc,R,clockwise);

    // methode pour afficher l'arc de cercle (conversion pour outil )
    if (p1==p2) // les deux points sont les memes
    {
    // je considère le cercle en entier
        startAngle=0;
        spanAngle=2*M_PI*16*180/M_PI;
    }// les deux points ne sont les mêmes
    else
    {
    // from coordinates of arc
pts to angle

    teta1=arc.angle(p1);
    teta2=arc.angle(p2);

        //Sens de l'arc ?

        // sens anti horaire
        if (arc.get_clockwise()==0)
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


    QPainter painter(this);
    QPen pen,pen1,pen2;
    pen.setWidth(2);
    painter.setPen(pen);

    // print arc
    QRectF rectangle(pc.x()-R, pc.y()-R, 2*R, 2*R);
//    painter.drawArc(rectangle, startAngle, spanAngle);

 //   painter.drawArc()

    pen1.setWidth(10);
    pen1.setColor("red");
    painter.setPen(pen1);

    arc.affiche(this,5,"red");

//    QVector<qreal> plot_arc=arc.affiche();
//    painter.drawArc(plot_arc[0],plot_arc[1],plot_arc[2],plot_arc[3], plot_arc[4], plot_arc[5]);

       // segment arc cross test

    QVector<QVector<qreal>> cross_pts;
    for (int n=0;n<20;n++)
    {
        // generate a random line
        p1=QPointF(random(0,width),random(0,height));
        p2=QPointF(random(0,width),random(0,height));

        QLineF segment(p1,p2);

        pen.setColor(Qt::blue);
        painter.setPen(pen);
        painter.drawLine(segment);

        cross_pts=intersect(segment, arc);

        pen1.setColor(Qt::red);
        pen1.setWidth(10);
        pen1.setCapStyle(Qt::RoundCap);
        painter.setPen(pen1);

        // affiche tous les pts de croisement entre l'arc de cercle et le segment
        for (int n=1;n<cross_pts.size();n++)
        {
            painter.drawPoint(QPointF (cross_pts.at(n).at(0),cross_pts.at(n).at(1)));
        }
        // stocke les segments et affiche en vert les croisements

        pen2.setColor(Qt::green);
        pen2.setWidth(10);
        painter.setPen(pen2);

            for (int n=0;n<segment_stock.size();n++)
            {

            line_trans1=QLineF(QPointF(segment_stock.at(n).at(0),segment_stock.at(n).at(1)),QPointF(segment_stock.at(n).at(2),segment_stock.at(n).at(3)));
            cross_test=intersect(line_trans1,segment);
            if (cross_test.at(0).at(0)==1)
            {
                painter.drawPoint(QPointF (cross_test.at(1).at(0),cross_test.at(1).at(1)));
            }
            }
        line_trans.append(p1.x());
        line_trans.append(p1.y());
        line_trans.append(p2.x());
        line_trans.append(p2.y());
        segment_stock.append(line_trans);
        line_trans=QVector<qreal>();
    }
*/
 //   --------------------------------------------------------------------------------------------------

/* //    //cercle_inscrit test
 //   Initialisation
    QPointF p1,p2,p3;

    //Generate 3 random pts
    p1=QPointF(random(0,width/2),random(0,height/2));
    p2=QPointF(random(0,width/2),random(0,height/2));
    p3=QPointF(random(0,width/2),random(0,height/2));

    // cercle inscrit test

    QPainter painter(this);
    QPointF pc;
    QPen pen1, pen2;
    AxCircle cross_med;
    qreal R;

    pen1.setColor(Qt::red);
    pen1.setWidth(10);
    pen1.setCapStyle(Qt::RoundCap);
    painter.setPen(pen1);

    pen2.setColor(Qt::black);
    pen2.setWidth(3);

    painter.drawPoint(p1);
    painter.drawPoint(p2);
    painter.drawPoint(p3);


    cross_med=cercle_inscrit(p1,p2,p3);
    pc=cross_med.get_center();
    R=cross_med.get_R();
    qDebug() << pc << endl;
    AxCircle C= AxCircle(R,pc);
    painter.setPen(pen2);

     painter.drawEllipse(C.get_center(), C.get_R(), C.get_R());*/
//---------------------------------------------------------------------------------------

/*//     // delaunay test

    //Generate a list of random pts
    QVector<QPointF> nuage;
    QPolygonF polygon;
    QVector<AxTriangle> triangle_list_bis,triangle_list;
    QVector<QVector<qreal>>test_vector;
    int next_point, number_of_pts,test;
    AxCircle cross_med,cross_med1,cross_med2;
    QPointF random_point,pc1,pc2;

    QPainter painter(this);
    QPen pen,pen1,pen2;
    pen.setWidth(10);
    pen.setColor(Qt::red);
    pen.setCapStyle(Qt::RoundCap);

    painter.setPen(pen);

    pen2.setWidth(3);
    pen2.setColor(Qt::green);
    number_of_pts=int(random(3,20));
    number_of_pts=5;


    // figure
//    for (int k=0;k<number_of_pts;k++)
//    {
//        test=0;

//        random_point=QPointF(random(-100+width/2,+100+width/2),random(-100+height/2,+100+height/2));

//        // polygon aleatoire wip (ne marche pas)
//        if (k>2)
//        {
//            while (test==0)
//            {
//                test=1;
//                random_point=QPointF(random(-100+width/2,+100+width/2),random(-100+height/2,+100+height/2));
//                    for (int q=1;q<k;q++)
//                    {
//                        test_vector=intersect(QLineF(random_point, nuage[k-1]),QLineF(nuage[q],nuage[q-1]));
//                        // si colision détéctée,
//                        if (k==number_of_pts-1)
//                        {
//                            test_vector=intersect(QLineF(random_point, nuage[0]),QLineF(nuage[q],nuage[q-1]));
//                        }
//                        qDebug() << "test_vector" << test_vector[0][0] << endl;
//                        if (test_vector[0][0]!=0)
//                        {
//                            test=0;
//                            qDebug() << "out" << endl;

//                        }
//                    }
//                    qDebug() << "fin" << endl;

//            }
//        }
//        nuage.append(random_point);

//        pen1.setWidth(5);
//        pen1.setColor(Qt::blue);
//        painter.setPen(pen1);
//        if (k>0)
//        {
//            painter.drawLine(nuage[k],nuage[k-1]);
//        }
//    }


     nuage.append(QPointF(2,4)*100);
//     nuage.append(QPointF(1.5,7.1)*100);
     nuage.append(QPointF(1,1)*100);
     nuage.append(QPointF(5,1)*100);
     nuage.append(QPointF(2,2)*100);
     nuage.append(QPointF(5.5,5.2)*100);



    pen1.setWidth(10);
    pen1.setColor(Qt::red);
    painter.setPen(pen1);

    painter.drawLine(nuage[0],nuage[nuage.size()-1]);

 //plot polygon (le programme avec delaunay ne peut pas donner un bon resultat)
    for (int k=1;k<nuage.size();k++)
    {
        painter.drawLine(nuage[k-1],nuage[k]);
    }

    triangle_list=delaunay(nuage);

 //   qDebug() << "triangle_list" <<triangle_list << endl;

    for (int k=0;k<triangle_list.size();k++)
    {
        for (k=0;k<triangle_list.size();k++)
        {
            triangle_list[k].affiche(this, 5, "black");
        }
    }

    polygon=nuage;
    triangle_list_bis=delaunay(polygon);
 //    qDebug() << "polygon" << polygon << endl;


    for (int k=0;k<triangle_list_bis.size();k++)
    {
        triangle_list[k].affiche(this, 5,"blue");
    }

    // cercle conscrit
    for (int k=0;k<triangle_list.size();k++)
    {
        cross_med=triangle_list[k].cercle_inscrit();

        QPointF pc=cross_med.get_center();
        qreal R=cross_med.get_R();

        AxCircle C= AxCircle(R,pc);
        //painter.drawEllipse(C.get_center(), C.get_R(), C.get_R());
        AxLine (C.get_center(),C.get_center()).affiche(this,10,"red");
    }

    QVector<AxLine> voro=voronoi(triangle_list);
    for (int k=0;k<voro.size();k++)
    {
        voro[k].affiche(this, 5 ,"black");
    }

*///-------------------------------------------------------------------------------------------------------------------

   /* // concave/ convexe form extraction

    AxShape form;
//    form.append(new AxLine(150,220,266.47,220));

//    form.append(new AxLine(266.47,220,147.402,440.497));

//    form.append(new AxArc(QLineF(QPointF(147.402,440.497),QPointF(168.922,469.612)),QPointF(165,450),20, 1));

    //    form.append(new AxLine(168.922,469.612,168.922,429.612));

    form.append(new AxLine(126.109,207.469,450,420));

    //    form.append(new AxArc(QLineF(QPointF(393.922,444.612),QPointF(403.229,420)),QPointF(390,405),20, 1));

//    form.append(new AxLine(393.922,444.612,300,370));

//    form.append(new AxLine(300,370,329.874,304.121));

//    form.append(new AxLine(329.874,304.121,360.357,283.799));

//    form.append(new AxLine(360.357,283.799,450,420));

    form.append(new AxArc(QLineF(QPointF(450,420),QPointF(470,400)),QPointF(450,400),20, 1));

    form.append(new AxLine(470,400,470,150));

    form.append(new AxArc(QLineF(QPointF(470,150),QPointF(451.99,130.099)),QPointF(450,150),20, 1));

    form.append(new AxLine(451.99,130.099,201.99,105.099));

    form.append(new AxArc(QLineF(QPointF(201.99,105.099),QPointF(187.196,109.636)),QPointF(200,125),20, 1));

    form.append(new AxLine(187.196,109.636,112.196,172.136));

    form.append(new AxArc(QLineF(QPointF(112.196,172.136),QPointF(126.109,207.469)),QPointF(125,187.5),20, 1));

//    form.append(new AxLine(126.109,207.469,131.333,207.179));

//    form.append(new AxArc(QLineF(QPointF(131.333,207.179),QPointF(150,220)),QPointF(150,200),20, 1));

    int k2,k1=0;
    qreal teta;
    QVector<AxShape> concave_list;
    AxShape segments_concave;
    AxShape convexe(form);
    AxShape segment_convexe,segment_concave, segment_convexe_bis;


    // convexe form calculation
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


    // forme convexe calculation
    k2=0;
    k1=0;

   // starting segment initialisation
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

    convexe[0]->affiche(this, 30,"yellow");
    convexe[k1]->affiche(this, 30,"yellow");

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

    for (int k=0;k<shape_convexe.size();k++)
    {
        shape_convexe[k].affiche(this, 20,"red");
    }

    qDebug()<<"shape_convexe"<<shape_convexe;

    k2=0;
    k1=0;
    // forme concave calculation

    QVector<AxShape> shape_concave;


    while (not(equal(form[k2],segment_convexe[k1])))
    {
        k2=form.next_point(k2);
    }

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


qDebug()<< "shape_concave" << shape_concave ;
//qDebug()<< "shape_concave" << segment_concave ;




    for (int k=0;k<shape_concave.size();k++)
    {
        shape_concave[k].affiche(this, 20, "black");
    }

    segment_concave.affiche(this, 5,"blue");
    segment_convexe.affiche(this, 5,"green");
    form.affiche(this, 2, "red");

 //    affiche enveloppe convexe
//    for (int k1=0;k1<convexe.size();k1++)
//    {
//        if (convexe[k1]->getObjectType()==1)
//        {
//            AxLine(convexe[k1]->p1(),convexe[k1]->p2()).affiche(this, 5,"grey");
//        }
//        else
//        {
//            convexe[k1]->affiche(this, 5,"grey");
//        }
//    }

    // Delaunay fail

//    QVector<QPointF> nuage;
//    QVector<AxTriangle> triangles_list;
//    QVector<AxLine> line_list;
//    for (int k=0;k<shape_concave.size();k++)
//    {
//        nuage=shape_concave[k].from_shape_to_pts();
//        triangles_list=delaunay(nuage);

//        for (int q=0;q<triangles_list.size();q++)
//        {
//            triangles_list[q].affiche(this, 5, "blue");
//        }
//        line_list=voronoi(triangles_list);
//        for (int q=0;q<line_list.size();q++)
//        {
//            line_list[q].affiche(this, 2, "red");
//        }
//        qDebug()<< "triangles_list" << triangles_list;
//        qDebug()<< "line_list" << line_list;
//    }
*/
    //----------------------------------------------------------------------------------------------

    /*
// Motor cycle graph and staith squeletton


    AxShape form;
    form.append(new AxLine(393.922,444.612,300,370));
    form.append(new AxLine(300,370,329.874,304.121));
    form.append(new AxLine(329.874,304.121,360.357,283.799));
    form.append(new AxLine(360.357,283.799,450,420));

    form.append(new AxLine(450,420,393.922,444.612));
    form.affiche(this, 3,"red");

// calcul de la bisectrice

    int k2;
    for (int k1=0;k1<form.size();k1++)
    {
        k2=form.next_point(k1);
    }
// motorcycle algorithme

*/
//---------------------------------------------------------------------------------------

/*//   // Expand form test V1

    // painter initialisation & definition

    QPainter painter(this);
    QPen pen,pen1,pen2;
    pen.setWidth(10);
    pen.setColor(Qt::red);
    pen.setCapStyle(Qt::RoundCap);
    pen1.setWidth(10);
    pen1.setColor(Qt::blue);
    pen1.setCapStyle(Qt::RoundCap);
    pen2.setWidth(10);
    pen2.setColor(Qt::green);
    pen2.setCapStyle(Qt::RoundCap);

    // variables initialisation

    QVector<QPointF> form,expand,useless_test;
    qreal R=50;


    int m2;

    // form definition

    form.append(QPointF(1,1)*100);
    form.append(QPointF(4,2)*100);
    form.append(QPointF(2,4)*100);
    form.append(QPointF(7,6)*100);
    form.append(QPointF(5,0.5)*100);

    // Plot form
    painter.setPen(pen);

    for (int m1=0;m1<form.size();m1++)
    {
        m2=next_point(m1,form);
        painter.drawLine(form[m1],form[m2]);
    }

    QPointF pm, p1, p2,pp1,pp2,p1_para,p2_para;
    QLineF segment;
    qreal a,b,c,a_para,b_para,D;
    QVector<QVector<qreal>>cross_test;
    QVector<QLineF> stock_parallels;
    QVector<QPointF> expand_form;
    int left_cross,sens,n2,k2;

    for (int n1=0;n1<form.size();n1++)// for all segments of form
    {
        n2=next_point(n1,form);

        // initialisation
        left_cross=0;
        // median point of the segment
        p1=form[n1];
        p2=form[n2];
        segment=QLineF(p1,p2);
        pm=segment.center();

        // 2 pts of the mediatrice

        // mediatrice 1:
           pp1=QPointF((p1.x()+p2.x())/2,((p1.y()+p2.y())/2));
           pp2.setY(100);
           a=2*(p2.x()-p1.x());
           b=2*(p2.y()-p1.y());
           c=pow(p1.x(),2)+pow(p1.y(),2)-pow(p2.x(),2)-pow(p2.y(),2);
           pp2.setX((-c-b*pp2.y())/a);

           // count the number of time the segment cross the object
           for (int k1=0;k1<form.size();k1++)
           {
               k2=next_point(k1,form);
               //if segment form =! current segment
               if (n1!=k1)
               {
                    cross_test=intersect_line_segment(QLineF(pp1,pp2),QLineF(form[k1],form[k2]));

                    if (cross_test[0][0]==1)
                    {
                        // segment à étendre non vertical ?
                        if (p1.x()!=p2.x())
                        {
                            if (cross_test[1][1]>pm.y())
                            {
                                left_cross++;
                            }
                        }
                        // segment à étendre vertical
                        else
                        {
                            if (cross_test[1][0]>pm.x())
                            {
                                left_cross++;
                                qDebug() << "vertical" << endl;
                            }
                        }

                    }
                }
           }
//  nombre pair de croisement à gauche ?
           if (left_cross%2==0)
           // ajouter R
           {
                sens=+1;
           }
           else
           // enlever R
           {
               sens=-1;
           }

           // equation du segment à expandre :
           if (p2.x()-p1.x()!=0)
           {
                a=(p2.y()-p1.y())/(p2.x()-p1.x());

                // odronée à l'origine à ajouter afin d'obtenir un décalage de R :
                D=sqrt(pow(a*R,2)+pow(R,2));

                b=p1.y()-(a*p1.x());
                // equation de la parallèle du segment distant de R :
                a_para=a;
                b_para=b+sens*D;
                // prendre 2 points appartenant à la droite (seule solution pour pouvoir appliquer la fct cross test avec le cas particulier)

                if (sens==1)
                {
                    p1_para=QPointF((p1.y()+R/sqrt(1+a*a)-b_para)/a,p1.y()+R/sqrt(1+a*a));
                    p2_para=QPointF((p2.y()+R/sqrt(1+a*a)-b_para)/a,p2.y()+R/sqrt(1+a*a));

                    painter.drawPoint(p1);
                    painter.drawPoint(p2);
                }
                else if (sens==-1)

                {
                    p1_para=QPointF((p1.y()-R/sqrt(1+a*a)-b_para)/a,p1.y()-R/sqrt(1+a*a));
                    p2_para=QPointF((p2.y()-R/sqrt(1+a*a)-b_para)/a,p2.y()-R/sqrt(1+a*a));
                }

           }
           else
           {
               D=R;
               // cas particulier segment vertical
               p1_para=QPointF(p1.x()+sens*D,p1.y());
               p2_para=QPointF(p2.x()+sens*D,p2.y());
           }

           pen.setWidth(20);
           pen.setColor(Qt::black);
           pen.setCapStyle(Qt::RoundCap);
           painter.setPen(pen);
           painter.drawPoint(p1_para);
           painter.drawPoint(p2_para);


           // store the parallel
           stock_parallels.append(QLineF(p1_para,p2_para));
   //        painter.drawLine(QLineF(p1_para,p2_para));

    }
    qDebug() << " stock parallels :" << stock_parallels << endl;

    // find the cross point with adjacent segments
    for (int k1=0;k1<stock_parallels.size();k1++)
    {
        k2=next_point(k1,form);
        // test colision with next line
        cross_test=intersect_lignes(stock_parallels[k1],stock_parallels[k2]);
        qDebug() << " cross_test" << cross_test << endl;
        // colision detectee ?
        if (cross_test[0][0]==1)
        {
            // save point
            expand.append(QPointF(cross_test[1][0],cross_test[1][1]));
        }

    }

    painter.setPen(pen2);

    for (int k1=0;k1<expand.size();k1++)
    {
        k2=next_point(k1,form);
        painter.setPen(pen2);
        painter.drawLine(expand[k1],expand[k2]);
    }
    AxShape toust;
  //  qDebug() << "TEST"<< toust << endl;




//    AxArc arc;
//    QVector<AxBorder*> toto;

//    AxLine line(1,2,30,40);

//    toto.append(new AxLine(1,2,30,40));
////    toto[0]->affiche(this, 5, "red");


//    for(int i=0;i<toto.size();i++)
//    {
//        delete toto[i];
//        toto[i]=0;
//    }


    AxShape mich;
    mich.append(new AxLine(1,2,30,40));
    mich.append(new AxArc(QLineF(QPointF(10,20),QPointF(40,60)),QPointF(50,20),1.2,1));

    mich[0]->affiche(this, 5, "red");
    qDebug() <<"test"<< mich;

    mich.clear(); // je libère la mémoire */
 //   -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Arc_arc cross test
    /*
                // Generate a random arc
                QVector<AxArc> arc_stock;
                QVector<QVector<qreal>> cross_arc;

                // painter initialisation & definition

                QPainter painter(this);
                QPen pen,pen1,pen2;
                pen.setWidth(10);
                pen.setColor(Qt::red);
                pen.setCapStyle(Qt::RoundCap);
                pen1.setWidth(10);
                pen1.setColor(Qt::blue);
                pen1.setCapStyle(Qt::RoundCap);
                pen2.setWidth(10);
                pen2.setColor(Qt::green);
                pen2.setCapStyle(Qt::RoundCap);

                for (int k=0;k<5;k++)
                {
                    int clockwise=1;
                    qreal _R=300;
                    QPointF p1_(random(0,width),random(0,height));
                    QPointF p2_(random(0,width),random(0,height));
                    // Generate a random center
    //                QPointF pc_=QPointF(random(_R,width-_R),random(_R,height-_R));
                    QPointF pc_=QPointF(300,random(_R,height-_R));

                    QLineF arc_limits(p1_,p2_);
                    AxArc arc(arc_limits,pc_,_R,clockwise);
                    arc.affiche(this, 5,"black");
                    arc_stock.append(arc);
                }
                for (int k1=0;k1<arc_stock.size();k1++)
                {
                    for (int k2=0;k2<arc_stock.size();k2++)
                    {
                        cross_arc=intersect(arc_stock[k1],arc_stock[k2]);
                        if (cross_arc[0][0]==1)
                        {
                            qDebug() << "cross_arc" << cross_arc << QPointF(cross_arc[1][0],cross_arc[1][1]);

                            QPainter painter(this);
                            QPen pen2;
                            pen2.setWidth(10);
                            pen2.setColor(Qt::green);
                            painter.setPen(pen2);
                            painter.drawPoint(QPointF(cross_arc[1][0],cross_arc[1][1]));
                            if (cross_arc.size()==3)
                            {
                                painter.drawPoint(QPointF(cross_arc[2][0],cross_arc[2][1]));
                            }
                        }
                    }
                }
                */


//---------------------------------------------------------------------------------------

    /*// closest_pt_to_p1_border test

                // Generate a random arc
                QVector<QPointF> arc_stock;
                QVector<QVector<qreal>> cross_arc;
                int nearest_pos;
                QPointF p ;
                AxLine line;
                // painter initialisation & definition


                int clockwise=0;
                AxBorder *arc;
                qreal _R=300;
                QPointF p1_(random(0,width),random(0,height));
                QPointF p2_(random(0,width),random(0,height));
                // Generate a random center
                QPointF pc_=QPointF(random(_R,width-_R),random(_R,height-_R));

                QLineF arc_limits(p1_,p2_);
                arc=(new AxArc(arc_limits,pc_,_R,clockwise));
                arc->affiche(this, 5,"black");
                for (int k=0;k<3;k++)
                {
                    QPainter painter(this);
                    QPen pen;
                    pen.setWidth(10);
                    pen.setColor(Qt::red);
                    p=QPointF(random(0,width),random(0,height));
                    // generate a random point
                    painter.setPen(pen);
                    if (arc->arc_test(p))
                    {
                        line=AxLine(p,arc->get_center());

                        if (k==0)
                        {
                            line.affiche(this,10,"blue");
                        }
                        else
                        {
                            line.affiche(this,5,"red");
                        }
                        arc_stock.append(p);
                    }
                }
                nearest_pos=closest_pt_to_p1_border(arc_stock,arc);
                line=AxLine(arc_stock[nearest_pos],arc->get_center());
                AxLine line2(arc->p1(),arc->get_center());
                line.affiche(this,5,"green");
                line2.affiche(this,5,"green");

    }*/

    //-------------------------------------------------------------------
/*//  Expand form test V2

//ETAPE 1: construction de la forme

    AxShape form,test;
    QVector<QPointF>form_const;
    int next_o;

        form_const.append(QPointF(5,1.5));
        form_const.append(QPointF(1,2));
        form_const.append(QPointF(4,2));
        form_const.append(QPointF(1.3,7));
        form_const.append(QPointF(5.8,6.1));
        form_const.append(QPointF(4,4));
        form_const.append(QPointF(5.5,3));
        form_const.append(QPointF(6,6));
        form_const.append(QPointF(7,6));
        form_const.append(QPointF(7,1));
        form_const.append(QPointF(2,0.5));
        form_const.append(QPointF(0.5,1.75));

//    for (int o=0;o<form_const.size();o++)
//    {
//        next_o=next_point(o,form_const);
//        form.append(new AxLine(QPointF(100,100)+form_const[o]*50,QPointF(100,100)+form_const[next_o]*50));
//    }

        form.append(new AxArc(QLineF(QPointF(147.402,440.497),QPointF(168.922,469.612)),QPointF(165,450),20, 1));
        form.append(new AxLine(168.922,469.612,168.922,429.612));
        form.append(new AxLine(168.922,429.612,393.922,424.612));
        form.append(new AxArc(QLineF(QPointF(393.922,424.612),QPointF(403.229,420)),QPointF(390,405),20, 1));

        form.append(new AxLine(403.229,420,300,370));
        form.append(new AxLine(300,370,329.874,304.121));
        form.append(new AxLine(329.874,304.121,360.357,283.799));
        form.append(new AxLine(360.357,283.799,450,420));
        form.append(new AxArc(QLineF(QPointF(450,420),QPointF(470,400)),QPointF(450,400),20, 1));
        form.append(new AxLine(470,400,470,150));
        form.append(new AxArc(QLineF(QPointF(470,150),QPointF(451.99,130.099)),QPointF(450,150),20, 1));
        form.append(new AxLine(451.99,130.099,201.99,105.099));
        form.append(new AxArc(QLineF(QPointF(201.99,105.099),QPointF(187.196,109.636)),QPointF(200,125),20, 1));
        form.append(new AxLine(187.196,109.636,112.196,172.136));
        form.append(new AxArc(QLineF(QPointF(112.196,172.136),QPointF(126.109,207.469)),QPointF(125,187.5),20, 1));
        form.append(new AxLine(126.109,207.469,131.333,207.179));
        form.append(new AxArc(QLineF(QPointF(131.333,207.179),QPointF(150,220)),QPointF(150,200),20, 1));
        form.append(new AxLine(150,220,266.47,220));
        form.append(new AxLine(266.47,220,147.402,440.497));


        form.affiche(this, 2, "red");
        qreal R=10;


        //   le programme aggrandis d'un rayon R la forme

            QPointF pm, p1, p2,p1_para,p2_para,pm_line,closest_cross,center_,p1_extend,p2_extend;
            AxLine segment, segment1, segment2, segment_previous, segment_next;
            qreal a,b,a_para,b_para,D,R_;
            QVector<QVector<qreal>>cross_test,cross_med,cross_test1,cross_test2;
            AxShape stock_parallels,expand;
            QVector<QPointF> cross_pts,cross_pts1,cross_pts2;
            QLineF arc_limits_,med;
//            QVector<bool> yolo(20);
//            yolo[8]=true;
//            qDebug() << "yolo" << yolo;
//            QVector<AxShape> expand;
            int left_cross,sens,inside_form,k_next,k_previous,clockwise_,k1=0,k_1=0,k_2=0,interne=0,closest_pos,number_of_cross,bord_externe=0,k,closest_k;

// ETAPE 2 extention brut des arc et des segments

            for (int n=0;n<form.size();n++)// for all border of form
            {
                // initialisation
                left_cross=0;
                //Border = line ?
                if (form[n]->getObjectType()==0)
                {
                    // median point of the segment
                    p1=form[n]->p1();
                    p2=form[n]->p2();
                    segment=AxLine(p1,p2);
                    pm=segment.center();

                    // 2 pts of the mediatrice

                    // mediatrice :

                    med=form[n]->mediatrice();

                 // count the number of time the mediatrice cross the object

                    for (int k=0;k<form.size();k++)
                    {
                     //if segment form =! current segment
                     if (n!=k)
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
                             cross_pts=points_extraction(cross_test);

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

                    stock_parallels.append(new AxLine(p1_para,p2_para));
                }

                //Border = arc ?
                else
                {   // find pm and draw the mediatrice
                    pm=form[n]->middle_point();   // point in the middle of the arc
                    pm_line=QPointF(pm.x()+10.0,pm.y()+10.0);
                    //find out if arc circle is concave or convexe

                    // pm inside form ?
                    // count the number of time the border cross the object
                    for (int k=0;k<form.size();k++)
                    {
                        //if border =! current arc
                        if (n!=k)
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
                            cross_test=intersect_line_segment(QLineF(pm,pm_line),QLineF(form[k]->get_arc_limits()));
                        }
                        if (cross_test[0][0]==1)
                        {
                        //add ont to left cross for each intersection
                            cross_pts=points_extraction(cross_test);
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


                    // pm inside cercle ?
                    if (inside_form==1) // cercle= concave
                    {
                        // arc big enought ?
                        if (form[n]->get_R()-R>0)
                        {

                            cross_test=intersect_arc_line(QLineF(form[n]->get_center(),form[n]->p1()),AxArc(QLineF(0,0,0,0),form[n]->get_center(),form[n]->get_R()-R,form[n]->get_clockwise()));
                            cross_test=intersect_arc_line(QLineF(form[n]->get_center(),form[n]->p2()),AxArc(QLineF(0,0,0,0),form[n]->get_center(),form[n]->get_R()-R,form[n]->get_clockwise()));
                            p1_extend=closest_cross_to_pt(cross_test,form[n]->p1());
                            p2_extend=closest_cross_to_pt(cross_test,form[n]->p2());
                            stock_parallels.append(new AxArc(QLineF(p1_extend,p2_extend),form[n]->get_center(),form[n]->get_R()-R,form[n]->get_clockwise()));
                        }

                    }
                    else
                    {
                        // store the expanded Arc   // cercle= convexe


//                        cross_test1=intersect_arc_line(QLineF(form[n]->get_center(),form[n]->p1()),AxArc(QLineF(0,0,0,0),form[n]->get_center(),form[n]->get_R()+R,form[n]->get_clockwise()));
                        cross_test1=intersect_arc_line(QLineF(form[n]->get_center(),form[n]->p1()),AxArc(form[n]->get_arc_limits(),form[n]->get_center(),form[n]->get_R()+R,form[n]->get_clockwise()));
                        cross_pts1=points_extraction(cross_test1);
                        k_1=closest_pt_to_p1_border(cross_pts1, form[n]);

                        //cross_test2=intersect_arc_line(QLineF(form[n]->get_center(),form[n]->p2()),AxArc(QLineF(0,0,0,0),form[n]->get_center(),form[n]->get_R()+R,form[n]->get_clockwise()));
                        cross_test2=intersect_arc_line(QLineF(form[n]->get_center(),form[n]->p2()),AxArc(form[n]->get_arc_limits(),form[n]->get_center(),form[n]->get_R()+R,form[n]->get_clockwise()));
                        cross_pts2=points_extraction(cross_test2);
                        k_2=closest_pt_to_p2_border(cross_pts2, form[n]);

                        stock_parallels.append(new AxArc(QLineF(cross_pts1[k_1],cross_pts2[k_2]),form[n]->get_center(),form[n]->get_R()+R,form[n]->get_clockwise()));

                    }
                 }
            }
 //           stock_parallels.affiche(this, 8 ,"blue");

 // ETAPE 3 : ajustement des extentions : ajout d'un arc quand les segments extendus ne se croisent pas


            // find the cross point with adjacent arc and segments
            for (int k=0;k<stock_parallels.size();k++)
            {
                k_next=stock_parallels.next_point(k);
                k_previous=stock_parallels.previous_point(k);

                cross_test=intersect_border(stock_parallels[k_next],stock_parallels[k]);


                // Border=line ?
                if (stock_parallels[k]->getObjectType()==0)
                {
//                    next_border=line ?
                    if (stock_parallels[k_next]->getObjectType()==0)  // je ne prend pas en compte le cas ou les segments sont colinéaires
                    {
                        cross_test1=intersect_demi_line_line(stock_parallels[k_next],stock_parallels[k]);

                        //if colision
                        if (cross_test[0][0]==1)
                        {
                            //redefine the segment
                            stock_parallels[k]->setP2(QPointF(cross_test[1][0],cross_test[1][1]));
                            stock_parallels[k_next]->setP1(QPointF(cross_test[1][0],cross_test[1][1]));
                        }
                        else if (cross_test1[0][0]==1)
                        {
                            cross_pts=points_extraction(cross_test1);
                            stock_parallels[k]->affiche(this, 10,"black");
                            stock_parallels.remove(k);
                            //stock_parallels.remove(k_previous);
                            //destroy associated structure;
                            k=k-2;
                            k1=k1-1;

                            for (int po=0;po<cross_pts.size();po++)
                            {
                                AxLine mi(cross_pts[po],cross_pts[po]);
                                mi.affiche(this, 20,"green");
                            }
                        }
                        else if (not_equal(p1,p2))
                        {
                            // define a new arc between the two segment
                            arc_limits_=QLineF(stock_parallels[k]->p2(),stock_parallels[k_next]->p1());
                            center_=form[k1]->p2();
                            R_=distance(stock_parallels[k]->p2(),center_);
                            clockwise_=1;
                            stock_parallels.insert(k_next,new AxArc(arc_limits_,center_,R_,clockwise_));
                            k++;

                        }
                     }

                    //if next Border=arc

                    else
                    {

                        AxArc associated_circle(stock_parallels[k_next]);
                        associated_circle.setP1(associated_circle.p2());
                        cross_test1=intersect_arc_demi_line(stock_parallels[k], associated_circle);

                        if (cross_test[0][0]==1)
                        {
                            //redefine the segment
                            cross_pts=points_extraction(cross_test);
                            closest_k=closest_pt_to_p1_border(cross_pts,stock_parallels[k]);
                            stock_parallels[k]->setP2(cross_pts[closest_k]);
                            stock_parallels[k_next]->setP1(cross_pts[closest_k]);
                        }
                        else
                        {
                            // if p1 inside circle associated with form

                            if (cross_test1[0][0]==1 && cross_test1.size()==2)
                            {
                                stock_parallels[k]->affiche(this, 20,"black");
                                stock_parallels.remove(k);
                                k=k-2;
                                k1=k1-1;
                            }
                            else if (not_equal(p1,p2))
                            {
                                arc_limits_=QLineF(stock_parallels[k]->p2(),stock_parallels[k_next]->p1());
                                center_=form[k1]->p2();
                                R_=distance(stock_parallels[k]->p2(),center_);
                                clockwise_=1;
                                stock_parallels.insert(k_next,new AxArc(arc_limits_,center_,R_,clockwise_));
                                k++;
                            }
                        }
                    }
                }
//                Border==arc
                else
                {
//                  next_border==line
                    if (stock_parallels[k_next]->getObjectType()==0)  // je ne prend pas en compte le cas ou les segments sont colinéaires

                    {
                        AxArc associated_circle(stock_parallels[k]);
                        associated_circle.setP1(associated_circle.p2());
                        cross_test1=intersect_arc_demi_line(stock_parallels[k_next], associated_circle);
                        if (cross_test[0][0]==1)
                        {
                            //redefine the arc
                            cross_pts=points_extraction(cross_test);
                            closest_k=closest_pt_to_p1_border(cross_pts,stock_parallels[k]);
                            stock_parallels[k]->setP2(cross_pts[closest_k]);
                            stock_parallels[k_next]->setP1(cross_pts[closest_k]);
                        }
//                        else if (cross_test1[0][0]==1)
//                        {
////                            stock_parallels.remove(k);
////                           k=k-2;
//                        }

                        else if (true)
                        {

                            // define a new arc between the two segment
                            p1=stock_parallels[k]->p2();
                            p2=stock_parallels[k_next]->p1();


                            if (not_equal(p1,p2))
                            {

                                arc_limits_=QLineF(stock_parallels[k]->p2(),stock_parallels[k_next]->p1());
                                center_=form[k1]->p2();
                                R_=distance(stock_parallels[k]->p2(),center_);
                                clockwise_=1;
//                                cross=0;
//                                for (q=0;q<form.size();q++)
//                                    intersect()
                                AxArc(arc_limits_,center_,R_,clockwise_).affiche(this, 5,"red");
                                    stock_parallels.insert(k_next,new AxArc(arc_limits_,center_,R_,clockwise_));
                                k++;
                            }
                        }
                    }
                    // if next border== arc
                    else
                    {
                        stock_parallels[k]->affiche(this, 10,"black");
                        stock_parallels[k_next]->affiche(this, 10, "black");
                        if (cross_test[0][0]==1)
                        {
                            //redefine the segment
                            cross_pts=points_extraction(cross_test);
                            closest_k=closest_pt_to_p1_border(cross_pts,stock_parallels[k]);
                            stock_parallels[k]->setP2(cross_pts[closest_k]);
                            stock_parallels[k_next]->setP1(cross_pts[closest_k]);
                        }
                        else
                        {
                            // define a new arc between the arc and segment
                            p1=stock_parallels[k]->p2();
                            p2=stock_parallels[k_next]->p1();

                            if (not_equal(p1,p2))
                            {
                                arc_limits_=QLineF(stock_parallels[k]->p2(),stock_parallels[k_next]->p1());
                                center_=form[k1]->p2();
                                R_=distance(stock_parallels[k]->p2(),center_);
                                clockwise_=1;
                                stock_parallels.insert(k_next,new AxArc(arc_limits_,center_,R_,clockwise_));
                                k++;
                            }
                        }
                    }
                }
                k1++;
            }
stock_parallels.affiche(this, 2,"green");
// ETAPE 4 : suppression et ajustement des segments/arcs problématiques

        // on commence un bord externe !

            k1=stock_parallels.bord_externe(); // optimisable en prenant le bord le plus grand de tous


//            stock_parallels.affiche(this, 5,"black");

            // Pour tous les segments de la forme en commencant par le bord externe
            for (int q=0;q<stock_parallels.size();q++)
            {
                cross_pts=QVector<QPointF>();

                for (int k2=0;k2<stock_parallels.size();k2++)
                {
                    if ((k1!=k2) && (k2!=stock_parallels.previous_point(k1)) && (k2!=stock_parallels.next_point(k1)))
                    {
                        intersect_border(stock_parallels[k1],stock_parallels[k2]);

                       if (cross_test[0][0]==1)
                        {
                           for (int n=1;n<cross_test.size();n++)
                           {
                               cross_pts.append(QPointF(cross_test[n][0],cross_test[n][1]));
                           }
                        }
                    }
                }
                number_of_cross=cross_pts.size();
                // stockle le segment
                for (int n=0;n<number_of_cross;n++)
                {
                    closest_pos=closest_pt_to_p1_border(cross_pts,stock_parallels[k1]);

                    if (stock_parallels[k1]->getObjectType()==0)
                    {
                        expand.append(new AxLine(stock_parallels[k1]));
                    }
                    else
                    {
                        expand.append(new AxArc(stock_parallels[k1]));
                    }

                    if (interne==0)// internal cross ?
                    {
                        expand[expand.size()-1]->setP2(cross_pts[closest_pos]);
                    }
                    if (interne==1) // external cross ?
                    {
                        expand[expand.size()-1]->setP1(cross_pts[closest_pos]);
                    }
                    interne=toggle(interne);
                    cross_pts.remove(closest_pos);
                }


                if ((interne==0)&& (number_of_cross==0))
                {
                    if (stock_parallels[k1]->getObjectType()==0)
                    {
                        expand.append(new AxLine(stock_parallels[k1]));
                    }
                    else
                    {
                        expand.append(new AxArc(stock_parallels[k1]));
                    }
                }
                k1=k1+1;
                if (k1==stock_parallels.size())
                {
                    k1=0;
                }
            }


          //renvoie des pts

           //stock_parallels.affiche(this,2,"black");
            expand.affiche(this,5,"green");

            qDebug()<<expand;



            expand.clear();

            stock_parallels.clear();
        }
*/
//  Expand form test V3

    //ETAPE 0: construction de la forme

    AxShape form,test;
    QVector<QPointF>form_const;
    int next_o;

    form_const.append(QPointF(5,1.5));
    form_const.append(QPointF(1,2));
    form_const.append(QPointF(4,2));
    form_const.append(QPointF(1.3,7));
    form_const.append(QPointF(5.8,6.1));
    form_const.append(QPointF(4,4));
    form_const.append(QPointF(5.5,3));
    form_const.append(QPointF(6,6));
    form_const.append(QPointF(7,6));
    form_const.append(QPointF(7,1));
    form_const.append(QPointF(2,0.5));
    form_const.append(QPointF(0.5,1.75));

        for (int o=0;o<form_const.size();o++)
        {
            next_o=next_point(o,form_const);
            form.append(new AxLine(QPointF(100,100)+form_const[o]*50,QPointF(100,100)+form_const[next_o]*50));
        }

//    form.append(new AxArc(QLineF(QPointF(147.402,440.497),QPointF(168.922,469.612)),QPointF(165,450),20, 1));
//    form.append(new AxLine(168.922,469.612,168.922,429.612));
//    form.append(new AxLine(168.922,429.612,393.922,424.612));
//    form.append(new AxArc(QLineF(QPointF(393.922,424.612),QPointF(403.229,420)),QPointF(390,405),20, 1));

//    form.append(new AxLine(403.229,420,300,370));
//    form.append(new AxLine(300,370,329.874,304.121));
//    form.append(new AxLine(329.874,304.121,360.357,283.799));
//    form.append(new AxLine(360.357,283.799,450,420));
//    form.append(new AxArc(QLineF(QPointF(450,420),QPointF(470,400)),QPointF(450,400),20, 1));
//    form.append(new AxLine(470,400,470,150));
//    form.append(new AxArc(QLineF(QPointF(470,150),QPointF(451.99,130.099)),QPointF(450,150),20, 1));
//    form.append(new AxLine(451.99,130.099,201.99,105.099));
//    form.append(new AxArc(QLineF(QPointF(201.99,105.099),QPointF(187.196,109.636)),QPointF(200,125),20, 1));
//    form.append(new AxLine(187.196,109.636,112.196,172.136));
//    form.append(new AxArc(QLineF(QPointF(112.196,172.136),QPointF(126.109,207.469)),QPointF(125,187.5),20, 1));
//    form.append(new AxLine(126.109,207.469,131.333,207.179));
//    form.append(new AxArc(QLineF(QPointF(131.333,207.179),QPointF(150,220)),QPointF(150,200),20, 1));
//    form.append(new AxLine(150,220,266.47,220));
//    form.append(new AxLine(266.47,220,147.402,440.497));


    form.affiche(this, 2, "red");
    qreal R=10;


    //   le programme aggrandis d'un rayon R la forme

    QPointF pm, p1, p2,p1_para,p2_para,pm_line,closest_cross,center_,p1_extend,p2_extend;
    AxLine segment, segment1, segment2, segment_previous, segment_next;
    qreal a,b,a_para,b_para,D,R_;
    QVector<QVector<qreal>>cross_test,cross_med,cross_test1,cross_test2;
    AxShape stock_parallels,expand, brut_expland;
    QVector<QPointF> cross_pts,cross_pts1,cross_pts2;
    QLineF arc_limits_,med;
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
                arc_limits_=QLineF(brut_segment_1->p2(),brut_segment_2->p1());
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
//        concave[n]=concave[n].straight_squeletton(); // suppr segments problématiques
        brut_segment_1=form.segment_brut_expand(concave[n][0],R);

        concave_expand.append(brut_segment_1);

        for (int k=1;k<concave[n].size();k++)
        {
            brut_segment_2=form.segment_brut_expand(concave[n][k],R);
            cross_test=intersect_border(brut_segment_1,brut_segment_2);

            // si croisement
            if (cross_test[0][0]==1)
            {
                // redéfinir les segments

                brut_segment_1->setP2(QPointF(cross_test[1][0],cross_test[1][1]));
                brut_segment_2->setP1(QPointF(cross_test[1][0],cross_test[1][1]));
            }
            else
            {
                // define a new arc between the two segments

                if (not_equal(brut_segment_1->p2(), brut_segment_2->p1()))
                {
                    arc_limits_=QLineF(brut_segment_1->p2(),brut_segment_2->p1());
                    center_=concave[n][k]->p1();
                    R_=distance(brut_segment_1->p2(),center_);
                    clockwise_=1;
                    concave_expand.append(new AxArc(arc_limits_,center_,R_,clockwise_));

                }
            }

            brut_segment_1=brut_segment_2;
            concave_expand.append(brut_segment_2);

        }
        concave_expand_list.append(concave_expand);
        concave_expand=AxShape();
    }

    for (int k=0; k<concave_expand_list.size();k++)
    {
        concave_expand_list[k].affiche(this, 5,"green");
    }
    for (int k=0; k<convexe_expand_list.size();k++)
    {
        convexe_expand_list[k].affiche(this, 5,"green");
    }
/*
    QVector<AxLine> bisectrice_list;

    for(int n=0;n<concave.size();n++)
    {
        brut_segment_1=form.segment_brut_expand(concave[n][0],10);

        for (int k=1;k<concave[n].size();k++)
        {
            brut_segment_2=form.segment_brut_expand(concave[n][k],10);

            cross_test=intersect_lignes(brut_segment_1,brut_segment_2);

            bisectrice_list.append(AxLine(concave[n][k]->p1(),QPointF(cross_test[1][0],cross_test[1][1])));

            brut_segment_1=brut_segment_2;
            brut_segment_2->affiche(this, 10, "black");
        }
    }
    qDebug()<<"convexe_expand_list"<<convexe_expand_list;


    for (int k=0;k<bisectrice_list.size();k++)
    {
        bisectrice_list[k].affiche(this, 5,"blue");
    }

    qreal closest_distance, cross_distance;
    int k1_cross,k2_cross,k2;

    for (int k=0;k<bisectrice_list.size();k++)
    {
        qDebug()<<k;
         closest_distance=100000000000000000000000.0;
         // on cherche la "plus proche intersection parmis toutes les bisectrices
         for (int k1=0;k1<bisectrice_list.size();k1++)
         {
             k2=next_point(k1,bisectrice_list);
             {
                 cross_test=intersect_demi_lines(bisectrice_list[k1],bisectrice_list[k2]);
                 cross_distance=distance(QPointF(cross_test[1][0],cross_test[1][1]),bisectrice_list[k1].p1());

                 if ((cross_test[0][0]==1) && (cross_distance<closest_distance))
                 {
                     closest_cross=QPointF(cross_test[1][0],cross_test[1][1]);
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

*/

    QVector<AxLine> bisectrice_list;

    for(int n=0;n<concave.size();n++)
    {
        brut_segment_1=form.segment_brut_expand(concave[n][0],10);

        for (int k=1;k<concave[n].size();k++)
        {
            brut_segment_2=form.segment_brut_expand(concave[n][k],10);

            cross_test=intersect_lignes(brut_segment_1,brut_segment_2);

            bisectrice_list.append(AxLine(concave[n][k]->p1(),QPointF(cross_test[1][0],cross_test[1][1])));

            brut_segment_1=brut_segment_2;
            brut_segment_2->affiche(this, 10, "black");
        }
    }
//    ETAPE 4 finition
//for (int k=0;k<convexe_expand_list.size();k++);
//{
//    p1=convexe_expand_list[][];
//}


//    qDebug()<<"convexe_expand_list"<<convexe_expand_list;
/*
    //           stock_parallels.affiche(this, 8 ,"blue");

    // ETAPE 3 : ajustement des extentions : ajout d'un arc quand les segments extendus ne se croisent pas


    // find the cross point with adjacent arc and segments
    for (int k=0;k<stock_parallels.size();k++)
    {
        k_next=stock_parallels.next_point(k);
        k_previous=stock_parallels.previous_point(k);

        cross_test=intersect_border(stock_parallels[k_next],stock_parallels[k]);


        // Border=line ?
        if (stock_parallels[k]->getObjectType()==0)
        {
            //                    next_border=line ?
            if (stock_parallels[k_next]->getObjectType()==0)  // je ne prend pas en compte le cas ou les segments sont colinéaires
            {
                cross_test1=intersect_demi_line_line(stock_parallels[k_next],stock_parallels[k]);

                //if colision
                if (cross_test[0][0]==1)
                {
                    //redefine the segment
                    stock_parallels[k]->setP2(QPointF(cross_test[1][0],cross_test[1][1]));
                    stock_parallels[k_next]->setP1(QPointF(cross_test[1][0],cross_test[1][1]));
                }
                else if (cross_test1[0][0]==1)
                {
                    cross_pts=points_extraction(cross_test1);
                    stock_parallels[k]->affiche(this, 10,"black");
                    stock_parallels.remove(k);
                    //stock_parallels.remove(k_previous);
                    //destroy associated structure;
                    k=k-2;
                    k1=k1-1;

                    for (int po=0;po<cross_pts.size();po++)
                    {
                        AxLine mi(cross_pts[po],cross_pts[po]);
                        mi.affiche(this, 20,"green");
                    }
                }
                else if (not_equal(p1,p2))
                {
                    // define a new arc between the two segment
                    arc_limits_=QLineF(stock_parallels[k]->p2(),stock_parallels[k_next]->p1());
                    center_=form[k1]->p2();
                    R_=distance(stock_parallels[k]->p2(),center_);
                    clockwise_=1;
                    stock_parallels.insert(k_next,new AxArc(arc_limits_,center_,R_,clockwise_));
                    k++;
                }
            }

            //if next Border=arc

            else
            {

                AxArc associated_circle(stock_parallels[k_next]);
                associated_circle.setP1(associated_circle.p2());
                cross_test1=intersect_arc_demi_line(stock_parallels[k], associated_circle);

                if (cross_test[0][0]==1)
                {
                    //redefine the segment
                    cross_pts=points_extraction(cross_test);
                    closest_k=closest_pt_to_p1_border(cross_pts,stock_parallels[k]);
                    stock_parallels[k]->setP2(cross_pts[closest_k]);
                    stock_parallels[k_next]->setP1(cross_pts[closest_k]);
                }
                else
                {
                    // if p1 inside circle associated with form

                    if (cross_test1[0][0]==1 && cross_test1.size()==2)
                    {
                        stock_parallels[k]->affiche(this, 20,"black");
                        stock_parallels.remove(k);
                        k=k-2;
                        k1=k1-1;
                    }
                    else if (not_equal(p1,p2))
                    {
                        arc_limits_=QLineF(stock_parallels[k]->p2(),stock_parallels[k_next]->p1());
                        center_=form[k1]->p2();
                        R_=distance(stock_parallels[k]->p2(),center_);
                        clockwise_=1;
                        stock_parallels.insert(k_next,new AxArc(arc_limits_,center_,R_,clockwise_));
                        k++;
                    }
                }
            }
        }
        //                Border==arc
        else
        {
            //                  next_border==line
            if (stock_parallels[k_next]->getObjectType()==0)  // je ne prend pas en compte le cas ou les segments sont colinéaires

            {
                AxArc associated_circle(stock_parallels[k]);
                associated_circle.setP1(associated_circle.p2());
                cross_test1=intersect_arc_demi_line(stock_parallels[k_next], associated_circle);
                if (cross_test[0][0]==1)
                {
                    //redefine the arc
                    cross_pts=points_extraction(cross_test);
                    closest_k=closest_pt_to_p1_border(cross_pts,stock_parallels[k]);
                    stock_parallels[k]->setP2(cross_pts[closest_k]);
                    stock_parallels[k_next]->setP1(cross_pts[closest_k]);
                }
                //                        else if (cross_test1[0][0]==1)
                //                        {
                ////                            stock_parallels.remove(k);
                ////                           k=k-2;
                //                        }

                else if (true)
                {

                    // define a new arc between the two segment
                    p1=stock_parallels[k]->p2();
                    p2=stock_parallels[k_next]->p1();


                    if (not_equal(p1,p2))
                    {

                        arc_limits_=QLineF(stock_parallels[k]->p2(),stock_parallels[k_next]->p1());
                        center_=form[k1]->p2();
                        R_=distance(stock_parallels[k]->p2(),center_);
                        clockwise_=1;
                        //                                cross=0;
                        //                                for (q=0;q<form.size();q++)
                        //                                    intersect()
                        AxArc(arc_limits_,center_,R_,clockwise_).affiche(this, 5,"red");
                        stock_parallels.insert(k_next,new AxArc(arc_limits_,center_,R_,clockwise_));
                        k++;
                    }
                }
            }
            // if next border== arc
            else
            {
                stock_parallels[k]->affiche(this, 10,"black");
                stock_parallels[k_next]->affiche(this, 10, "black");
                if (cross_test[0][0]==1)
                {
                    //redefine the segment
                    cross_pts=points_extraction(cross_test);
                    closest_k=closest_pt_to_p1_border(cross_pts,stock_parallels[k]);
                    stock_parallels[k]->setP2(cross_pts[closest_k]);
                    stock_parallels[k_next]->setP1(cross_pts[closest_k]);
                }
                else
                {
                    // define a new arc between the arc and segment
                    p1=stock_parallels[k]->p2();
                    p2=stock_parallels[k_next]->p1();

                    if (not_equal(p1,p2))
                    {
                        arc_limits_=QLineF(stock_parallels[k]->p2(),stock_parallels[k_next]->p1());
                        center_=form[k1]->p2();
                        R_=distance(stock_parallels[k]->p2(),center_);
                        clockwise_=1;
                        stock_parallels.insert(k_next,new AxArc(arc_limits_,center_,R_,clockwise_));
                        k++;
                    }
                }
            }
        }
        k1++;
    }
    stock_parallels.affiche(this, 2,"green");
    // ETAPE 4 : suppression et ajustement des segments/arcs problématiques

    // on commence un bord externe !

    k1=stock_parallels.bord_externe(); // optimisable en prenant le bord le plus grand de tous


    //            stock_parallels.affiche(this, 5,"black");

    // Pour tous les segments de la forme en commencant par le bord externe
    for (int q=0;q<stock_parallels.size();q++)
    {
        cross_pts=QVector<QPointF>();

        for (int k2=0;k2<stock_parallels.size();k2++)
        {
            if ((k1!=k2) && (k2!=stock_parallels.previous_point(k1)) && (k2!=stock_parallels.next_point(k1)))
            {
                intersect_border(stock_parallels[k1],stock_parallels[k2]);

                if (cross_test[0][0]==1)
                {
                    for (int n=1;n<cross_test.size();n++)
                    {
                        cross_pts.append(QPointF(cross_test[n][0],cross_test[n][1]));
                    }
                }
            }
        }
        number_of_cross=cross_pts.size();
        // stockle le segment
        for (int n=0;n<number_of_cross;n++)
        {
            closest_pos=closest_pt_to_p1_border(cross_pts,stock_parallels[k1]);

            if (stock_parallels[k1]->getObjectType()==0)
            {
                expand.append(new AxLine(stock_parallels[k1]));
            }
            else
            {
                expand.append(new AxArc(stock_parallels[k1]));
            }

            if (interne==0)// internal cross ?
            {
                expand[expand.size()-1]->setP2(cross_pts[closest_pos]);
            }
            if (interne==1) // external cross ?
            {
                expand[expand.size()-1]->setP1(cross_pts[closest_pos]);
            }
            interne=toggle(interne);
            cross_pts.remove(closest_pos);
        }


        if ((interne==0)&& (number_of_cross==0))
        {
            if (stock_parallels[k1]->getObjectType()==0)
            {
                expand.append(new AxLine(stock_parallels[k1]));
            }
            else
            {
                expand.append(new AxArc(stock_parallels[k1]));
            }
        }
        k1=k1+1;
        if (k1==stock_parallels.size())
        {
            k1=0;
        }
    }


    //renvoie des pts

    //stock_parallels.affiche(this,2,"black");
    expand.affiche(this,5,"green");

    qDebug()<<expand;



    expand.clear();

    stock_parallels.clear();*/
}

