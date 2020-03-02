#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "primitive.h"

class Ellipse : public Primitive
{
public:
    Ellipse();
    void drawEllipse(QPainter* painter, int x, int y, int rx, int ry);
    void drawPoint(QPainter* painter, int x, int y, int px, int py);
    void zoom(QPainter *painter, QPoint end, int n) override;
    void draw_(QPainter *painter, QPoint p1, QPoint p2) override;
    void edit_(QPainter *painter) override;
    bool translate_(QPainter *painter, int dx, int dy) override;
    bool rotate_(QPainter *painter, int x, int y, double r) override;
    bool scale_(QPainter *painter, int x, int y, float s) override;
    bool clipCS(QPainter *painter, int x1, int y1, int x2, int y2) override;
    bool clipLB(QPainter *painter, int x1, int y1, int x2, int y2) override;
private:
    int ra,rb;
};

#endif // ELLIPSE_H
