//#ifndef DISTANCE_H
//#define DISTANCE_H

#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

#include <QPointF>
#include <QLine>
#include <QVector>
#include <QPolygonF>
#include <axshape.h>
#include <axcircle.h>
#include <axtriangle.h>


QVector<QVector<QPointF>> delaunay(QVector<QPointF> nuage); // return Delaunay's triangle points organized 3 by 3 in the QVector

QVector<AxTriangle> delaunay(QVector<QPointF> nuage, int a); // a is useless //return Delaunay's triangle

QVector<AxTriangle> delaunay(QPolygonF nuage); // return Delaunay's of a polygon, the polygon must be convexe !

QVector<AxLine> voronoi(QVector<AxTriangle> triangle_list); // voronoy'segment based on Delaunay's triangl

qreal distance(QPointF p1, QPointF p2); // return the distance between p1 and p2

int sign(qreal a);// return 1 if a>0, -1 if a<0, 0 if a=0

qreal random(qreal min, qreal max);// generate a random qreal  between min and max

bool not_equal(QPointF p1, QPointF p2,double epsilon=0.0001);   //return 1 if p1, and p2 are not equal with a deflaut tolerence of 0.0001 or 0 else
bool equal(AxBorder *border1, AxBorder *border2); // return 1 if the borders are the same and -1 if not

int closest_pt_to_p1_border(QVector<QPointF> cross_list, AxBorder *border);// return the closest point of cross list to p1 follwing the border from p1 to p2
int closest_pt_to_p2_border(QVector<QPointF> cross_list, AxBorder *border);// return the closest point of cross list to p2 follwing the border from p1 to p2

QVector<QPointF> intersecting_points(QVector<QVector<qreal>> cross_test); // put all the crossing points of cross test on a QVector<QPointF>

int next_point(int n, QVector<QPointF> form); // return the next point of the form : ex ; if current point = 3 -> return 4, if current point = 7 -> return 8, if current point = form.size-1, return 0
int next_point(int n, QVector<AxLine> form);  // return the next point of the form : ex ; if current point = 3 -> return 4, if current point = 7 -> return 8, if current point = form.size-1, return 0
int next_point(int n, AxShape form);          // return the next point of the form : ex ; if current point = 3 -> return 4, if current point = 7 -> return 8, if current point = form.size-1, return 0

QPointF closest_cross_to_pt(QVector<QVector<qreal>> cross_test, QPointF pm); // return the closest cross point contain in cross_test to pm

int border_sens(AxBorder *border); // return 1 (if x is growing between p1 and p2) OR (if x stay the same && y is growing), return 0 else.

qreal angle(AxBorder *border1, AxBorder *border2); // return the angle between the two arc moving clockwise (border1.p2 must be equal to border2.p1)

AxCircle cercle_inscrit(QPointF p1, QPointF p2, QPointF p3);//take 3 points in argument and return the center and the ray of their inscribe circle

// intersect functions

/*
the intersect functions returns a QVector <QVector<qreal>> composed this way :
( intersection ?           -        )
(x intersection 1   y intersection 1)
(x intersection 2   y intersection 2)

intersection=1 if there is a cross, 0 if no cross and 2 if colinear
then, each additional line correspond to a cross between the two objects
*/
QVector <QVector<qreal>> intersect(QLineF line1, QLineF line2);
QVector <QVector<qreal>> intersect(AxLine line1, AxLine line2); // ligne sur ligne
QVector<QVector<qreal>> intersect(AxArc arc1, AxArc arc2);      // arc sur arc
QVector<QVector<qreal>> intersect(AxLine line,AxArc arc);       // ligne sur arc

QVector<QVector<qreal>> intersect(QLineF line, AxArc arc);

QVector <QVector<qreal>> intersect_lignes(QLineF line1, QLineF line2); // return the potential cross point between the two lines
QVector <QVector<qreal>> intersect_lignes(AxLine line1, AxLine line2); // return the potential cross point between the two lines

QVector<QVector<qreal>> intersect_arc_demi_line(AxLine line, AxArc arc); // return the potential cross point between the demi line and the arc
QVector<QVector<qreal>> intersect_demi_line_line(AxLine demi_line, AxLine line);// return the potential cross point between the demi line and the line
QVector<QVector<qreal>> intersect_demi_lines(AxLine demi_line1, AxLine demi_line);// return the potential cross point between the demi line and the line

QVector <QVector<qreal>> intersect_line_segment(QLineF line, QLineF segment);// return the potential cross point between the line and the segment
QVector <QVector<qreal>> intersect_line_segment(AxLine line, AxLine segment);// return the potential cross point between the line and the segment
QVector<QVector<qreal>> intersect_shape(AxBorder *border, AxShape shape); // return the potential cross point between the border and the shape

QVector<QVector<qreal>> intersect_border(AxBorder *border1,AxBorder *border2); // return the potential cross point between the two borders

QVector<QVector<qreal>> intersect_arc_line(QLineF line, AxArc arc); // return the potential cross point between the line and the arc

QVector<QVector<QPointF>> delaunay(QVector<QPointF> nuage); // return Delaunay's triangle points organized 3 by 3 in the QVector

QVector<AxTriangle> delaunay(QVector<QPointF> nuage, int a); // a is useless //return Delaunay's triangle

QVector<AxTriangle> delaunay(QPolygonF nuage); // return Delaunay's of a polygon, the polygon must be convexe !

QVector<AxLine> voronoi(QVector<AxTriangle> triangle_list); // voronoy'segment based on Delaunay's triangle



#endif // COMMON_FUNCTIONS_H


