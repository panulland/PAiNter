#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <QPainter>
#include <QVector>
#include <QDebug>
#include <QtMath>

#define _USE_MATH_DEFINES

enum type_draw
{
    typeNone,typeLine,typeEllipse,typePolygon,typeCurveBezier,typeCurveBspline
};

enum type_alg
{
    None,DDA,Bresenham,Bezier,Bspline
};

class Primitive
{
public:
    Primitive();
    void set_start_p(QPoint start);
    void set_end_p(QPoint end);
    void set_alg(type_alg alg);
    bool is_near(QPoint a, QPoint b);
    bool polygonFinished();
    bool polygonNext(QPoint &t);
    QPoint get_start_p();
    QPoint get_end_p();
    QPoint get_center_p();
    void set_color(QColor c);
    QPoint rotatePoint(QPoint a, QPoint b, double cos, double sin);
    int get_point_num();
    void add_point(QPoint temp);
    void add_cut(QPoint temp);
    void set_cut(QPoint temp);
    void set_cuting(bool t);
    bool cut_line(QPainter* painter);
    int toZoom(QPoint temp);
    bool toTranslate(QPoint temp);
    bool toRotate(QPoint temp);
    int toScale(QPoint temp);
    void drawDotline(QPainter* painter, QPoint p1, QPoint p2);
    void rotate(QPainter* painter, QPoint end);
    void translate(QPainter* painter, int dx, int dy);
    virtual void zoom(QPainter* painter, QPoint end, int n);
    virtual void scale(QPainter* painter, QPoint end, int n);
    virtual void edit_(QPainter* painter) = 0;
    virtual bool clipCS(QPainter* painter, int x1, int y1, int x2, int y2) = 0;
    virtual bool clipLB(QPainter* painter, int x1, int y1, int x2, int y2) = 0;
    virtual void draw_(QPainter* painter, QPoint p1, QPoint p2) = 0;
    virtual bool translate_(QPainter* painter, int dx, int dy) = 0;
    virtual bool rotate_(QPainter* painter, int x, int y, double r) = 0;
    virtual bool scale_(QPainter* painter, int x, int y, float s) = 0;
protected:
    int x_min,y_min,x_max,y_max;
    QPoint ps1,ps2,ps3,ps4;
    QPoint ps[4];
    QPoint edit1,edit2,edit3,edit4;
    QPoint editR;
    double cosA;
    double sinA;
    type_alg useAlg;
    type_draw type;
    bool rotating;
    bool cuting;
    int penWidth;
    QColor color;
    QPoint startP;
    QPoint endP;
    QPoint centerP;
    QVector<QPoint*> scaleP;
    QPoint rotateP;
    QVector<QPoint> keyP;
    QVector<QPoint> keyTemp;
    QVector<QPoint> cutPoint;
};

#endif // PRIMITIVE_H
