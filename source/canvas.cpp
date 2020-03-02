#include "canvas.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "line.h"
#include "ellipse.h"
#include "polygon.h"
#include "curve.h"

Canvas::Canvas(QWidget *parent) :
    QWidget(parent)
{
    setMouseTracking(true);
    setAutoFillBackground(true);
    setPalette(QPalette(Qt::gray));

    showOption = nullptr;
    location = nullptr;

    Width = 500;
    Height = 400;
    cur_pix = new QPixmap(500,400);
    cur_pix->fill(Qt::white);
    canvasColor = Qt::white;

    QPixmap* temp = new QPixmap;
    *temp = *cur_pix;
    pix.push_back(temp);

    pen.setColor(Qt::black);
    pen.setWidth(1);

    type_of_draw = typeNone;
    drawing = false;
    toDraw = false;
    editing = false;
    zooming = -1;
    rotating = false;
    translating = false;
    cuting = false;
    scaling = -1;
    cur_prim = nullptr;
    cur_file = nullptr;
    reseting = false;
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(drawing || editing)
    {
        painter.drawPixmap(QPoint(0,0),*temp_pix);
    }
    else if(!drawing && !editing)
    {
        painter.drawPixmap(QPoint(0,0),*cur_pix);
    }
}

bool Canvas::newPrimitive(Primitive *&temp)
{
    switch (type_of_draw)
    {
    case typeLine:
    {
        temp = new Line;
        break;
    }
    case typeEllipse:
    {
        temp = new Ellipse;
        break;
    }
    case typePolygon:
    {
        temp = new Polygon;
        break;
    }
    case typeCurveBezier:
    {
        temp = new Curve;
        temp->set_alg(Bezier);
        break;
    }
    case typeCurveBspline:
    {
        temp = new Curve;
        temp->set_alg(Bspline);
        break;
    }
    }
    if(temp == nullptr)
        return false;
    else
        return true;
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    temp_pix = new QPixmap(Width,Height);
    temp_pix->fill(Qt::transparent);
    *temp_pix = *cur_pix;
    qDebug()<<"mousePress";
    if(event->button() & Qt::LeftButton)
    {
        qDebug()<<"start"<<start_p.rx()<<start_p.ry();
        if(!editing && qAbs(event->pos().rx() - Width)<3 && qAbs(event->pos().ry() - Height)<3)
        {
            reseting = true;
            showOption->setText("Reseting Canvas");
        }
        else if(type_of_draw == typeNone)
        {
            qDebug()<<"nothing";
        }
        else if(!drawing && !editing)
        {
            start_p = event->pos();
            qDebug()<<"start drawing";
            bool succ = newPrimitive(cur_prim);
            if(!succ)
            {
                qDebug()<<"error primitive";
                assert(0);
            }
            drawing = true;
            showOption->setText("Drawing");
            if(type_of_draw != typePolygon && type_of_draw != typeCurveBezier && type_of_draw != typeCurveBspline)
            {
                cur_prim->set_start_p(start_p);
            }
            else if(type_of_draw == typePolygon)
            {
                if(cur_prim->get_point_num() == 0)
                {
                    cur_prim->add_point(start_p);
                    cur_prim->set_start_p(start_p);
                }
                else if(cur_prim->polygonNext(start_p))
                {
                    cur_prim->add_point(start_p);
                    *temp_pix = *cur_pix;
                    Paint(temp_pix);
                }
                else
                {
                    cur_prim->add_point(start_p);
                    *temp_pix = *cur_pix;
                    Paint(temp_pix);
                    drawing = false;
                    editing = true;
                    showOption->setText("Editing");
                }
            }
            else if(type_of_draw == typeCurveBezier || type_of_draw == typeCurveBspline)
            {
                if(cur_prim->get_point_num() == 0)
                    cur_prim->set_start_p(start_p);
                cur_prim->add_point(start_p);
                *temp_pix = *cur_pix;
                Paint(temp_pix);
            }
        }
        else if(!drawing && editing)
        {
            start_p = event->pos();
            end_p = event->pos();
            translating = cur_prim->toTranslate(start_p);
            rotating = cur_prim->toRotate(start_p);
            scaling = cur_prim->toScale(start_p);
            zooming = cur_prim->toZoom(start_p);
            if(!translating && !rotating && scaling == -1 && zooming == -1)
            {
                if(type_of_draw == typeLine && cuting)
                {
                    cur_prim->set_cuting(true);
                    cur_prim->add_cut(start_p);
                    //Paint(temp_pix);
                    return;
                }
                editing = false;
                showOption->setText("Waiting");
                Paint(cur_pix);
                delete temp_pix;
                if(cur_prim != nullptr)
                {
                    delete cur_prim;
                    cur_prim = nullptr;
                }
                qDebug()<<"finish";
            }
            if(rotating || scaling != -1)
            {
                setCursor(Qt::ClosedHandCursor);
            }
        }
        else if(drawing && !editing)
        {
            if(type_of_draw == typePolygon)
            {
                start_p = event->pos();
                if(cur_prim->get_point_num() == 0)
                {
                    cur_prim->add_point(start_p);
                    cur_prim->set_start_p(start_p);
                }
                else if(cur_prim->polygonNext(start_p))
                {
                    cur_prim->add_point(start_p);
                    *temp_pix = *cur_pix;
                    Paint(temp_pix);
                }
                else
                {
                    cur_prim->add_point(start_p);
                    *temp_pix = *cur_pix;
                    Paint(temp_pix);
                    drawing = false;
                    editing = true;
                    showOption->setText("Editing");
                }
            }
            else if(type_of_draw == typeCurveBezier || type_of_draw == typeCurveBspline)
            {
                start_p = event->pos();
                if(cur_prim->get_point_num() == 0)
                {
                    cur_prim->set_start_p(start_p);
                    cur_prim->add_point(start_p);
                }
                else if(cur_prim->polygonNext(start_p))
                {
                    cur_prim->add_point(start_p);
                    *temp_pix = *cur_pix;
                    Paint(temp_pix);
                }
            }
        }
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
    int a = event->pos().rx();
    int b = event->pos().ry();
    if(location!=nullptr)
        location->setText(QString("(%1,%2)").arg(a).arg(b));
    if(qAbs(event->pos().rx() - Width)<3 && qAbs(event->pos().ry() - Height)<3)
    {
        setCursor(Qt::SizeFDiagCursor);
    }
    else if(reseting)
    {
        setCursor(Qt::SizeFDiagCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }
    //qDebug()<<event->pos().rx();
    if(event->buttons() & Qt::LeftButton)
    {
        qDebug()<<"Move";
        QPoint* temp_s = new QPoint;
        QPoint* temp_e = new QPoint;
        *temp_s = start_p;
        *temp_e = end_p;
        end_p = event->pos();
        if(reseting)
        {
            if(end_p.rx() < 770)
            {
                Width = end_p.rx();
            }
            if(end_p.ry() < 580)
            {
                Height = end_p.ry();
            }
            QPixmap temp = *cur_pix;
            delete cur_pix;
            cur_pix = new QPixmap(Width,Height);
            cur_pix->fill(canvasColor);
            temp_pix = cur_pix;
            QPainter* painter = new QPainter;
            painter->begin(cur_pix);
            painter->setPen(pen);
            painter->drawPixmap(QPoint(0,0),temp);
            //painter->drawPixmap(QPoint(0,0),*temp_pix);
            painter->end();
            update();
        }
        else if(type_of_draw == typeNone)
        {

        }
        else
        {
            qDebug()<<"else";
            if(drawing && !editing)
            {
                *temp_pix = *cur_pix;
                if(type_of_draw == typePolygon)
                {
                    if(cur_prim->get_point_num() == 1)
                    {
                        cur_prim->add_point(end_p);
                        cur_prim->set_end_p(end_p);
                        Paint(temp_pix);
                    }
                    else
                    {
                        cur_prim->set_end_p(end_p);
                        Paint(temp_pix);
                    }
                }
                else if(type_of_draw == typeCurveBezier || type_of_draw == typeCurveBspline)
                {
                    if(cur_prim->get_point_num() == 1)
                    {
                        cur_prim->add_point(end_p);
                        cur_prim->set_end_p(end_p);
                        Paint(temp_pix);
                    }
                    else
                    {
                        cur_prim->set_end_p(end_p);
                        Paint(temp_pix);
                    }
                    //cur_prim->add_point(end_p);
                }
                else
                {
                    cur_prim->set_end_p(end_p);
                    //qDebug()<<"Paint";
                    Paint(temp_pix);
                }
            }
            else if(!drawing && editing)
            {
                *temp_pix = *cur_pix;
                //qDebug()<<"start editing";
                if(translating)
                {
                    setCursor(Qt::SizeAllCursor);
                    QPainter *painter = new QPainter;
                    painter->begin(temp_pix);
                    painter->setPen(pen);
                    cur_prim->translate(painter,(end_p.rx()-temp_e->rx()),(end_p.ry()-temp_e->ry()));
                    cur_prim->edit_(painter);
                    painter->end();
                    update();
                }
                else if(rotating)
                {
                    setCursor(Qt::ClosedHandCursor);
                    QPainter *painter = new QPainter;
                    painter->begin(temp_pix);
                    painter->setPen(pen);
                    cur_prim->rotate(painter,end_p);
                    cur_prim->edit_(painter);
                    painter->end();
                    update();
                }
                else if(scaling != -1)
                {
                    setCursor(Qt::ClosedHandCursor);
                    QPainter *painter = new QPainter;
                    painter->begin(temp_pix);
                    painter->setPen(pen);
                    cur_prim->scale(painter,end_p,scaling);
                    cur_prim->edit_(painter);
                    painter->end();
                    update();
                }
                else if(zooming != -1)
                {
                    if(zooming == 0 || zooming == 2)
                        setCursor(Qt::SizeHorCursor);
                    else
                        setCursor(Qt::SizeVerCursor);
                    QPainter *painter = new QPainter;
                    painter->begin(temp_pix);
                    painter->setPen(pen);
                    cur_prim->zoom(painter,end_p,zooming);
                    cur_prim->edit_(painter);
                    painter->end();
                    update();
                }
                else if(type_of_draw == typeLine)
                {
                    cur_prim->set_cut(end_p);
                    Paint(temp_pix);
                }
            }
            else
            {
                qDebug()<<drawing<<editing;
            }
        }
    }
    else if(cur_prim != nullptr)
    {
        if(cur_prim->toTranslate(event->pos()))
        {
            setCursor(Qt::SizeAllCursor);
        }
        else if(cur_prim->toRotate(event->pos()) || cur_prim->toScale(event->pos()) != -1)
        {
            setCursor(Qt::OpenHandCursor);
        }
        else if(cur_prim->toZoom(event->pos()) == 0 || cur_prim->toZoom(event->pos()) == 2)
        {
            setCursor(Qt::SizeHorCursor);
        }
        else if(cur_prim->toZoom(event->pos()) == 1 || cur_prim->toZoom(event->pos()) == 3)
        {
            setCursor(Qt::SizeVerCursor);
        }
        else
        {
            setCursor(Qt::ArrowCursor);
        }
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<<"Finish";
    setCursor(Qt::ArrowCursor);
    end_p = event->pos();
    if(reseting)
    {
        reseting = false;
        showOption->setText("Waiting");
    }
    else if(type_of_draw == typePolygon)
    {
        if(drawing || editing)
        {
            if(drawing && !editing && cur_prim->polygonFinished())
            {
                *temp_pix = *cur_pix;
                Paint(temp_pix);
                drawing = false;
                editing = true;
            }
            else if(!drawing && editing && cur_prim->polygonFinished())
            {
                *temp_pix = *cur_pix;
                Paint(temp_pix);
            }
            if(editing)
            {
                QPainter* painter = new QPainter;
                painter->begin(temp_pix);
                painter->setPen(pen);
                cur_prim->edit_(painter);
                painter->end();
                update();
            }
        }
    }
    else if(type_of_draw == typeCurveBezier || type_of_draw == typeCurveBspline)
    {
        if(drawing && !editing)
        {
            *temp_pix = *cur_pix;
            Paint(temp_pix);
        }
    }
    else if(drawing)
    {
        cur_prim->set_end_p(end_p);
        drawing = false;
        editing = true;
        showOption->setText("Editing");
        QPainter* painter = new QPainter;
        painter->begin(temp_pix);
        cur_prim->edit_(painter);
        painter->end();
        update();
    }
    else if(editing)
    {
        QPainter* painter = new QPainter;
        painter->begin(temp_pix);
        painter->setPen(pen);
        cur_prim->edit_(painter);
        painter->end();
        update();
    }
    else
    {
        qDebug()<<"nothing";
    }
}

void Canvas::Paint(QPixmap *thePix)
{
    if(cur_prim == nullptr)
        return;
    QPainter* painter = new QPainter;
    painter->begin(thePix);
    painter->setPen(pen);
    cur_prim->draw_(painter,cur_prim->get_start_p(),cur_prim->get_end_p());
    if(type_of_draw == typePolygon || type_of_draw == typeCurveBezier || type_of_draw == typeCurveBspline)
    {
        if(editing || drawing)
        {
            cur_prim->edit_(painter);
        }
    }
    else if(editing)
    {
        cur_prim->edit_(painter);
    }
    painter->end();
    update();
}

void Canvas::reset(int width, int height)
{
    Width=width;
    Height=height;
    pix.clear();
    id_vec.clear();
    primitives.clear();
    QPixmap* new_pix = new QPixmap(width,height);
    new_pix->fill(Qt::white);
    *cur_pix = *new_pix;
    update();
}

void Canvas::save(QString file, QString name)
{
    QString fileName = file+tr("\\")+name+tr(".bmp");
    qDebug()<<fileName;
    QPainter *painter = new QPainter;
    QPixmap* new_pix = new QPixmap(Width,Height);
    new_pix->fill(Qt::white);
    for(int i = 0; i < pix.size(); i++)
    {
        painter->begin(new_pix);
        painter->drawPixmap(QPoint(0,0),*pix[i]);
        painter->end();
        qDebug()<<i;
    }
    if(fileName != nullptr)
    {
        new_pix->save(fileName);
    }
    else
    {
        QMessageBox::warning(nullptr,tr("警告"),tr("文件名未指定！"),QMessageBox::Ok);
    }
    update();
}

void Canvas::setColor(int r, int g, int b)
{
    pen.setColor(QColor(r,g,b));
}

void Canvas::line(int id, int x1, int y1, int x2, int y2, QString alg)
{
    QPixmap* new_pix = new QPixmap(Width,Height);
    new_pix->fill(Qt::transparent);
    QPainter *painter = new QPainter;
    painter->begin(new_pix);
    painter->setPen(pen);
    Line *line=new Line(x1,y1,x2,y2,alg,pen.color());
    if(alg == "DDA")
    {
        line->drawDDA(painter,x1,y1,x2,y2);
    }
    else if(alg == "Bresenham")
    {
        line->drawBresenham(painter,x1,y1,x2,y2);
    }
    else
    {
        qDebug()<<"wrong algorithm!";
    }
    painter->end();
    id_vec.push_back(id);
    pix.push_back(new_pix);
    primitives.push_back(line);
    update();
}

void Canvas::polygon(int id, int n, QString alg, QStringList &list)
{
    QPixmap* new_pix = new QPixmap(Width,Height);
    new_pix->fill(Qt::transparent);
    QPainter *painter = new QPainter;
    painter->begin(new_pix);
    painter->setPen(pen);
    Polygon *polygon = new Polygon;
    polygon->drawPolygon(painter,alg,n,list);
    id_vec.push_back(id);
    pix.push_back(new_pix);
    primitives.push_back(polygon);
    update();
}

void Canvas::ellipse(int id, int x, int y, int rx, int ry)
{
    QPixmap* new_pix = new QPixmap(Width,Height);
    new_pix->fill(Qt::transparent);
    QPainter *painter = new QPainter;
    painter->begin(new_pix);
    painter->setPen(pen);
    Ellipse *ellipse = new Ellipse;
    ellipse->drawEllipse(painter,x,y,rx,ry);
    painter->end();
    id_vec.push_back(id);
    pix.push_back(new_pix);
    primitives.push_back(ellipse);
    update();
}

void Canvas::curve(int id, int n, QString alg, QStringList &list)
{
    QPixmap* new_pix = new QPixmap(Width,Height);
    new_pix->fill(Qt::transparent);
    QPainter *painter = new QPainter;
    painter->begin(new_pix);
    painter->setPen(pen);
    Curve *curve = new Curve;
    if(alg == "Bezier")
    {
        curve->set(painter,n,list);
        curve->drawBezier(painter);
    }
    else if(alg == "B-spline")
    {
        curve->set(painter,n,list);
        curve->drawBspline(painter);
    }
    else
    {
        qDebug()<<"wrong algorithm!";
    }
    painter->end();
    id_vec.push_back(id);
    pix.push_back(new_pix);
    primitives.push_back(curve);
    update();
}

void Canvas::translate(int id, int dx, int dy)
{
    int i;
    for(i=0;id_vec[i]!=id;i++);
    QPixmap* new_pix = new QPixmap(Width,Height);
    new_pix->fill(Qt::transparent);
    QPainter *painter = new QPainter;
    painter->begin(new_pix);
    primitives[i]->translate_(painter,dx,dy);
    painter->end();
    pix[i] = new_pix;
    update();
}

void Canvas::rotate(int id, int x, int y, int r)
{
    int i;
    for(i=0;id_vec[i]!=id;i++);
    QPixmap* new_pix = new QPixmap(Width,Height);
    new_pix->fill(Qt::transparent);
    QPainter *painter = new QPainter;
    painter->begin(new_pix);
    double rad = qDegreesToRadians((double)r);
    primitives[i]->rotate_(painter,x,y,rad);
    painter->end();
    pix[i] = new_pix;
    update();
}

void Canvas::scale(int id, int x, int y, float s)
{
    int i;
    for(i=0;id_vec[i]!=id;i++);
    QPixmap* new_pix = new QPixmap(Width,Height);
    new_pix->fill(Qt::transparent);
    QPainter *painter = new QPainter;
    painter->begin(new_pix);
    primitives[i]->scale_(painter,x,y,s);
    painter->end();
    pix[i] = new_pix;
    update();
}

void Canvas::clip(int id, int x1, int y1, int x2, int y2, QString alg)
{
    int i;
    for(i=0;id_vec[i]!=id;i++);
    QPixmap* new_pix = new QPixmap(Width,Height);
    new_pix->fill(Qt::transparent);
    QPainter *painter = new QPainter;
    painter->begin(new_pix);
    if(alg == "Cohen-Sutherland")
    {
        primitives[i]->clipCS(painter,x1,y1,x2,y2);
    }
    else if(alg == "Liang-Barsky")
    {
        primitives[i]->clipLB(painter,x1,y1,x2,y2);
    }
    else
    {
        qDebug()<<"Wrong algorithm!";
    }
    painter->end();
    pix[i] = new_pix;
    update();
}

void Canvas::setTypeLine()
{
    if(drawing || editing)
        return;
    toDraw = true;
    type_of_draw = typeLine;
}

void Canvas::setTypeEllipse()
{
    if(drawing || editing)
        return;
    toDraw = true;
    type_of_draw = typeEllipse;
}

void Canvas::setTypePolygon()
{
    if(drawing || editing)
        return;
    toDraw = true;
    type_of_draw = typePolygon;
}

void Canvas::setTypeCurveBezier()
{
    if(drawing || editing)
    {
        if(type_of_draw != typeCurveBezier)
        return;
    }
    toDraw = true;
    if(type_of_draw == typeCurveBezier && (drawing || editing) && cur_prim != nullptr)
    {
        if(drawing)
        {
            drawing = false;
            editing = true;
            showOption->setText("Editing");
        }
        Paint(temp_pix);
    }
    type_of_draw = typeCurveBezier;
}

void Canvas::setTypeCurveBspline()
{
    if(drawing || editing)
    {
        if(type_of_draw != typeCurveBspline)
        return;
    }
    toDraw = true;
    if(type_of_draw == typeCurveBspline && (drawing || editing) && cur_prim != nullptr)
    {
        if(drawing)
        {
            drawing = false;
            editing = true;
            showOption->setText("Editing");
        }
        Paint(temp_pix);
    }
    type_of_draw = typeCurveBspline;
}

void Canvas::setPenColor(QString arg)
{
    pen.setColor(arg);
    if(cur_prim != nullptr)
    {
        cur_prim->set_color(pen.color());
        *temp_pix = *cur_pix;
        QPainter* painter = new QPainter;
        painter->begin(temp_pix);
        painter->setPen(pen);
        cur_prim->draw_(painter,cur_prim->get_start_p(),cur_prim->get_end_p());
        cur_prim->edit_(painter);
        painter->end();
    }
}

void Canvas::setPenColor(QColor c)
{
    pen.setColor(c);
    if(cur_prim != nullptr)
    {
        cur_prim->set_color(pen.color());
        *temp_pix = *cur_pix;
        QPainter* painter = new QPainter;
        painter->begin(temp_pix);
        painter->setPen(pen);
        cur_prim->draw_(painter,cur_prim->get_start_p(),cur_prim->get_end_p());
        cur_prim->edit_(painter);
        painter->end();
    }
}

void Canvas::setCanvasColor(QColor color)
{
    canvasColor = color;
}

void Canvas::setPenWidth(int i)
{
    pen.setWidth(i);
    if(cur_prim != nullptr)
    {
        *temp_pix = *cur_pix;
        QPainter* painter = new QPainter;
        painter->begin(temp_pix);
        painter->setPen(pen);
        cur_prim->draw_(painter,cur_prim->get_start_p(),cur_prim->get_end_p());
        cur_prim->edit_(painter);
        painter->end();
    }
}

void Canvas::setCut()
{
    if(type_of_draw == typeLine && editing && cur_prim != nullptr)
    {
        if(!cuting)
        {
            cuting = true;
        }
        else
        {
            cuting = false;
            cur_prim->set_cuting(false);
            *temp_pix = *cur_pix;
            QPainter* painter = new QPainter;
            painter->begin(temp_pix);
            painter->setPen(pen);
            cur_prim->cut_line(painter);
            painter->end();
            Paint(temp_pix);
        }
    }
}

void Canvas::save()
{
    if(cur_file == nullptr)
    {
        QString fileName = QFileDialog::getSaveFileName(nullptr,tr("Save"),tr("../pix.bmp"),tr("BMP(*.bmp);;JPEG(*.jpg;*.jpeg);;PNG(*.png)"));
        cur_file = new QString(fileName);
    }
    cur_pix->save(*cur_file);
}

void Canvas::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr,tr("Save"),tr("../pix.bmp"),tr("BMP(*.bmp);;JPEG(*.jpg;*.jpeg);;PNG(*.png)"));
    if(fileName != nullptr)
    {
        cur_pix->save(fileName);
    }
}

void Canvas::open()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr,tr("Open"),tr(".."),tr("BMP(*.bmp);;JPEG(*.jpg;*.jpeg);;PNG(*.png)"));
    cur_file = new QString(fileName);
    delete cur_pix;
    cur_pix = nullptr;
    cur_pix = new QPixmap(fileName);
    Width = cur_pix->width();
    Height = cur_pix->height();
    bool t = false;
    if(Width > 770)
    {
        Width = 770;
        t = true;
    }
    if(Height > 580)
    {
        Height = 580;
        t = true;
    }
    if(t)
    {
        QPixmap temp = *cur_pix;
        delete cur_pix;
        cur_pix = new QPixmap(Width,Height);
        cur_pix->fill(canvasColor);
        temp_pix = cur_pix;
        QPainter* painter = new QPainter;
        painter->begin(cur_pix);
        painter->setPen(pen);
        painter->drawPixmap(QPoint(0,0),temp);
        //painter->drawPixmap(QPoint(0,0),*temp_pix);
        painter->end();
        update();
    }
}

void Canvas::clear()
{
    QPixmap *newpix = new QPixmap(Width,Height);
    newpix->fill(canvasColor);
    *cur_pix = *newpix;
    if(cur_prim != nullptr)
    {
        delete cur_prim;
        cur_prim = nullptr;
    }
    qDebug()<<"delete";
    temp_pix = cur_pix;
    type_of_draw = typeNone;
    drawing = false;
    toDraw = false;
    editing = false;
    zooming = -1;
    rotating = false;
    translating = false;
    cuting = false;
    scaling = -1;
    reseting = false;
    qDebug()<<"clear";
    update();
}
