#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <QVector>
#include <QLineF>
#include <QPointF>
#include <QPolygonF>
#include <axtriangle.h>


QVector<QVector<QPointF>> delaunay(QVector<QPointF> nuage); // return Delaunay's triangle points organized 3 by 3 in the QVector

QVector<AxTriangle> delaunay(QVector<QPointF> nuage, int a); // a is useless //return Delaunay's triangle

QVector<AxTriangle> delaunay(QPolygonF nuage); // return Delaunay's of a polygon, the polygon must be convexe !

QVector<AxLine> voronoi(QVector<AxTriangle> triangle_list); // voronoy'segment based on Delaunay's triangle


#endif // DELAUNAY_H
