
#include <QDebug>
#include <QRect>
#include <common_functions.h>
#include <AxArc.h>
#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <AxBorder.h>
#include <AxLine.h>


int main(int argc, char *argv[])
{
    // test intersect

//    QLineF line1,line2;

//    QPointF p1,p2,p3,p4,pc;

//    p1 = QPointF(0,0);
//    p2 = QPointF(3,3);
//    p3 = QPointF(5,5);
//    p4 = QPointF(9,9);

//    line1 = QLineF(p1,p2);
//    line2 = QLineF(p3,p4);


//    QVector<qreal> result = intersect(line1,line2);

//    qDebug() << result << endl;

//    // test angle
//    int clockwise=1;
//    p1=QPointF(286.5,58);// random+plot
//    p2=QPointF(100,277);
//    pc=QPointF(250,250);
//    qreal R=100;

//    QLineF arc_limits=QLineF(p1,p2);

//    AxArc arc(arc_limits,pc,R,clockwise);

 //   QPoint px(100,100);


 //    Plotting section

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

//    // test

//    QApplication app(argc, argv);
//    QWidget fenetre;
//    fenetre.show();

//    return app.exec();





}
