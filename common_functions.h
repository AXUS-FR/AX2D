//#ifndef DISTANCE_H
//#define DISTANCE_H

#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

#include <QPointF>
#include <QLine>
#include <QVector>
#include <axshape.h>
#include <axcircle.h>
#include <Vertice.h>
#include <axtriangle.h>

qreal distance(QPointF p1, QPointF p2);
// return the distance between p1 and p2

int sign(qreal a);
// return 1 if a>0, -1 if a<0, 0 if a=0

AxLine bisectrice(AxLine line1, AxLine line2);
// return two points of the bisectrice from line 1 to line 2

int border_sens(AxBorder *border);
// return 1 if(p1.x<p2.x) OR if(p1.x<p2.x && p1.y<p2.y), return 0 else.

qreal angle(AxBorder *border1, AxBorder *border2);
// return the angle between the two arc moving clockwise (border1.p2 must be equal to border2.p1)

AxCircle cercle_inscrit(QPointF p1, QPointF p2, QPointF p3);
//take 3 points in argument and return the center and the radius of their inscribe circle

QVector<QPointF> intersecting_points(QVector<QVector<qreal>> cross_test);
// take in argument the output of a intersect function and return the cross points on a list


qreal random(qreal min, qreal max);
// generate a random qreal  between min and max

//---------------------------------------------------------------------------------------------

bool not_equal(QPointF p1, QPointF p2,double epsilon=0.0001);
//return true if p1, and p2 are not equal with a deflaut tolerence of 0.0001 or return false else

bool equal(AxBorder *border1, AxBorder *border2);
// return true if the borders are the same and false if not

//----------------------------------------------------------------------------------------------

int closest_pt_to_p1_border(QVector<QPointF> cross_list, AxBorder *border);
// return the closest point of cross list to p1 follwing the border from p1 to p2

int closest_pt_to_p2_border(QVector<QPointF> cross_list, AxBorder *border);
// return the closest point of cross list to p2 follwing the border from p1 to p2

QPointF closest_cross_to_pt(QVector<QVector<qreal>> cross_test, QPointF pm);
// return the closest cross point contain in cross_test to pm

//-------------------------------------------------------------------------------------------------

int next_point(int n, QVector<Vertice>);
int next_point(int n, QVector<Vertice>);
int next_point(int n, QVector<QPointF> form);
int next_point(int n, QVector<AxLine> form);
int next_pt(int n, QVector<AxLine> form);
int next_point(int n, AxShape form);
// return the next point of the form : ex ; if current point = 3 -> return 4, if current point = 7 -> return 8, if current point = form.size-1, return 0


int previous_pt(int n, QVector<Vertice> vertice_list);
int next_pt(int n, QVector<Vertice> vertice_list);

// return the previous point of the form : ex ; if current point = 3 -> return 2, if current point = 7 -> return 6, if current point = 0, return form.size-1

//--------------------------------------------------------------------------------------------------------------------------------------

// INTERSECT FUNCTIONS

/*
the intersect functions returns a QVector <QVector<qreal>> composed this way :
( intersection ?           -        )
(x intersection 1   y intersection 1)
(x intersection 2   y intersection 2)

if there is a cross between the two objects, intersection=1
if the two objects, are colinear, intersection=2
if the two objects, are neither cossing nor parallel intersection=2

then, each additional line correspond to a cross between the two objects
*/
QVector <QVector<qreal>> intersect(QLineF line1, QLineF line2);
QVector <QVector<qreal>> intersect(AxLine line1, AxLine line2);
// between 2 segments

QVector<QVector<qreal>> intersect(AxArc arc1, AxArc arc2);
// between two arcs

QVector<QVector<qreal>> intersect(QLineF line, AxArc arc);
QVector<QVector<qreal>> intersect(AxLine line,AxArc arc);
// between a segment and a arc


QVector <QVector<qreal>> intersect_lignes(QLineF line1, QLineF line2);
QVector <QVector<qreal>> intersect_lignes(AxLine line1, AxLine line2);
// between 2 lines

QVector<QVector<qreal>> intersect_arc_demi_line(AxLine line, AxArc arc);

QVector<QVector<qreal>> intersect_demi_line_line(AxLine demi_line, AxLine line);
// between a line and a half-line

QVector<QVector<qreal>> intersect_demi_lines(AxLine demi_line1, AxLine demi_line);
// between two half-lines

QVector <QVector<qreal>> intersect_line_segment(QLineF line, QLineF segment);
QVector <QVector<qreal>> intersect_line_segment(AxLine line, AxLine segment);
// between a line and a segment

QVector<QVector<qreal>> intersect_shape(AxBorder *border, AxShape shape);
// between a border and a shape

QVector<QVector<qreal>> intersect_border(AxBorder *border1,AxBorder *border2);
// between two borders

QVector<QVector<qreal>> intersect_arc_line(AxLine line, AxArc arc);
// between a arc and a line


//---------------------------------------------------------------------------------------------------------------------------------------

// DELAUNAY AND VORONOI

QVector<QVector<QPointF>> delaunay(QVector<QPointF> nuage);
// return delaunay triangulation of a points list in this form :

//( triangle1.p1        triangle1.p2        triangle1.p3 )
//( triangle2.p1        triangle2.p2        triangle2.p3 )
//( triangle3.p1        triangle3.p2        triangle3.p3 )
//( triangle4.p1        triangle4.p2        triangle4.p3 )
//(...                  ...                 ...          )

QVector<AxTriangle> delaunay(QVector<QPointF> nuage, int a);
// a=1  //return triangles of delaunay triangulation of a points list



QVector<AxTriangle> delaunay(QPolygonF nuage);
// return Delaunay triangulation of a polygon,
// Warning : the polygon must be convex !

// In order to implement delaunay for a concave polygon,
// we need to décompose our polygon in a finite amount of convexe polygon



QVector<AxLine> voronoi(QVector<AxTriangle> triangle_list);
// return voronoi diagram based on Delaunay triangulation

#endif // COMMON_FUNCTIONS_H


