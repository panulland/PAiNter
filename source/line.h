#ifndef LINE_H
#define LINE_H

#include "primitive.h"

class Line : public Primitive
{
public:
    Line();
    Line(int x1, int y1, int x2, int y2, QString alg, QColor c);
    void drawDDA(QPainter* painter, int x1, int y1, int x2, int y2);
    void drawBresenham(QPainter* painter, int x1, int y1, int x2, int y2);
    void edit_(QPainter *painter) override;
    bool clipCS(QPainter* painter, int x1, int y1, int x2, int y2) override;
    bool clipLB(QPainter* painter, int x1, int y1, int x2, int y2) override;
    void draw_(QPainter *painter, QPoint p1, QPoint p2) override;
    bool translate_(QPainter *painter, int dx, int dy) override;
    bool rotate_(QPainter *painter, int x, int y, double r) override;
    bool scale_(QPainter *painter, int x, int y, float s) override;
};

#endif // LINE_H
