#ifndef POLYGON_H
#define POLYGON_H

#include "primitive.h"
#include "line.h"

class Polygon : public Primitive
{
public:
    Polygon();
    void drawPolygon(QPainter* painter, QString alg, int n, QStringList &list);
    void scale(QPainter*painter, QPoint end, int n) override;
    void draw_(QPainter *painter, QPoint p1, QPoint p2) override;
    bool translate_(QPainter *painter, int dx, int dy) override;
    void edit_(QPainter *painter) override;
    bool rotate_(QPainter *painter, int x, int y, double r) override;
    bool scale_(QPainter *painter, int x, int y, float s) override;
    bool clipCS(QPainter *painter, int x1, int y1, int x2, int y2) override;
    bool clipLB(QPainter *painter, int x1, int y1, int x2, int y2) override;
};

#endif // POLYGON_H
