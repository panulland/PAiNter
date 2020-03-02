#ifndef CURVE_H
#define CURVE_H

#include "primitive.h"

class Curve : public Primitive
{
public:
    Curve();
    void set(QPainter* painter, int n, QStringList &list);
    void drawBezier(QPainter* painter);
    void drawBspline(QPainter* painter);
    void edit_(QPainter *painter) override;
    void draw_(QPainter *painter, QPoint p1, QPoint p2) override;
    bool translate_(QPainter *painter, int dx, int dy) override;
    bool rotate_(QPainter *painter, int x, int y, double r) override;
    bool scale_(QPainter *painter, int x, int y, float s) override;
    bool clipCS(QPainter *painter, int x1, int y1, int x2, int y2) override;
    bool clipLB(QPainter *painter, int x1, int y1, int x2, int y2) override;
};

#endif // CURVE_H
