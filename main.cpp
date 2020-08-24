
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

//    QLine line1,line2;

//    QPoint p1,p2,p3,p4,pc;

//    p1 = QPoint(0,0);
//    p2 = QPoint(3,3);
//    p3 = QPoint(5,5);
//    p4 = QPoint(9,9);

//    line1 = QLine(p1,p2);
//    line2 = QLine(p3,p4);


//    QVector<int> result = intersect(line1,line2);

//    qDebug() << result << endl;

//    // test angle
//    int clockwise=1;
//    p1=QPoint(286.5,58);// random+plot
//    p2=QPoint(100,277);
//    pc=QPoint(250,250);
//    int R=100;

//    QLine arc_limits=QLine(p1,p2);

//    AxArc arc(arc_limits,pc,R,clockwise);

 //   QPoint px(100,100);


 //    Plotting section

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

// test

//    QApplication app(argc, argv);
//    QWidget fenetre;
//    fenetre.show();

//    return app.exec();





}
