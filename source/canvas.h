#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QLabel>
#include "primitive.h"
namespace Ui {
class Canvas;
}

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    bool newPrimitive(Primitive *& temp);
    void Paint(QPixmap* thePix);
    void setTypeLine();
    void setTypeEllipse();
    void setTypePolygon();
    void setTypeCurveBezier();
    void setTypeCurveBspline();
    void setPenColor(QString arg);
    void setPenWidth(int i);
    void setPenColor(QColor color);
    void setCanvasColor(QColor color);
    void setCut();
    void reset(int width, int height);
    void save(QString file, QString name);
    void save();
    void open();
    void saveAs();
    void clear();
    void setColor(int r, int g, int b);
    void line(int id, int x1, int y1, int x2, int y2, QString alg);
    void polygon(int id, int n, QString alg, QStringList &list);
    void ellipse(int id, int x, int y, int rx, int ry);
    void curve(int id, int n, QString alg, QStringList &list);
    void translate(int id, int dx, int dy);
    void rotate(int id, int x, int y, int r);
    void scale(int id, int x, int y, float s);
    void clip(int id, int x1, int y1, int x2, int y2, QString alg);

    QLabel *location;
    QLabel *showOption;

private:
    type_draw type_of_draw;
    int Width;
    int Height;
    QVector<QPixmap*> pix;
    QVector<int> id_vec;
    QVector<Primitive*> primitives;
    QPixmap* cur_pix;
    QPixmap* temp_pix;
    QPen pen;
    QColor canvasColor;
    QPoint start_p;
    QPoint end_p;

    Primitive* cur_prim;
    bool drawing;
    bool toDraw;
    bool editing;
    int zooming;
    bool translating;
    bool rotating;
    bool cuting;
    int scaling;
    bool reseting;

    QString *cur_file;
};

#endif // CANVAS_H
