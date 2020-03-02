#include "primitive.h"

QPoint Primitive::rotatePoint(QPoint a, QPoint b, double cos, double sin)
{
    double x1 = a.rx();
    double y1 = a.ry();
    double x = b.rx();
    double y = b.ry();
    QPoint temp(x + (x1 - x)*cosA - (y1 - y)*sinA + 0.5,y + (x1 - x)*sinA + (y1 - y)*cosA + 0.5);
    return temp;
}

Primitive::Primitive()
{
    rotating = false;
    cosA = 1;
    sinA = 0;
    for(int i = 0; i < 4; i++)
    {
        ps[i].setX(-1);
        ps[i].setY(-1);
    }
    scaleP.clear();
    keyP.clear();
    keyTemp.clear();
    cutPoint.clear();
}

void Primitive::set_start_p(QPoint start)
{
    startP = start;
}

void Primitive::set_end_p(QPoint end)
{
    endP = end;
    if(type == typePolygon || type == typeCurveBezier || type == typeCurveBspline)
    {
        if(keyP.size() > 1)
        {
            keyP[keyP.size() - 1] = end;
            keyTemp.back() = end;
        }
    }
}

void Primitive::set_alg(type_alg alg)
{
    useAlg = alg;
}

void Primitive::set_color(QColor c)
{
    color = c;
}

bool Primitive::is_near(QPoint a, QPoint b)
{
    int dx = a.rx() - b.rx();
    int dy = a.ry() - b.ry();
    int d = dx*dx + dy*dy;
    qDebug()<<dx<<dy<<d;
    return (d <= 25);
}

bool Primitive::polygonNext(QPoint &t)
{
    if(is_near(t,keyP[0]))
    {
        t = keyP[0];
        return false;
    }
    else
        return true;
}

bool Primitive::polygonFinished()
{
    if(is_near(keyP.back(),keyP[0]))
    {
        keyP.back().setX(keyP[0].rx());
        keyP.back().setY(keyP[0].ry());
        keyTemp.back() = keyP[0];
        return true;
    }
    return false;
}

QPoint Primitive::get_start_p()
{
    return startP;
}

QPoint Primitive::get_end_p()
{
    return endP;
}

QPoint Primitive::get_center_p()
{
    return centerP;
}

int Primitive::get_point_num()
{
    return keyP.size();
}

void Primitive::add_point(QPoint temp)
{
    keyP.push_back(temp);
    keyTemp.push_back(temp);
}

void Primitive::add_cut(QPoint temp)
{
    cutPoint.clear();
    cutPoint.push_back(temp);
}

void Primitive::set_cut(QPoint temp)
{
    if(cutPoint.size() == 1)
    {
        cutPoint.push_back(temp);
    }
    else
    {
        cutPoint.back() = temp;
    }
}

bool Primitive::cut_line(QPainter* painter)
{
    int x1,x2,y1,y2;
    x1 = cutPoint[0].rx();
    x2 = cutPoint[1].rx();
    y1 = cutPoint[0].ry();
    y2 = cutPoint[1].ry();
    if(x1 > x2)
    {
        x1 = x2;
        x2 = cutPoint[0].rx();
    }
    if(y1 > y2)
    {
        y1 = y2;
        y2 = cutPoint[0].ry();
    }
    return clipLB(painter,x1,y1,x2,y2);
}

void Primitive::set_cuting(bool t)
{
    cuting = t;
}

bool catchPoint(QPoint &a, QPoint &b)
{
    int dis = (a.rx()-b.rx())*(a.rx()-b.rx()) + (a.ry()-b.ry())*(a.ry()-b.ry());
    if(dis <= 9)
    {
        return true;
    }
    else
        return false;
}

int Primitive::toZoom(QPoint temp)
{
    for(int i = 0; i < 8; i++)
    {
        if(catchPoint(temp,ps[i]))
            return i;
    }
    return -1;
}

bool Primitive::toTranslate(QPoint temp)
{
    return catchPoint(centerP,temp);
}

bool Primitive::toRotate(QPoint temp)
{
    if(type == typeLine)
        return catchPoint(rotateP,temp);
    else
        return catchPoint(editR,temp);
}

int Primitive::toScale(QPoint temp)
{
    for(int i = 0; i < scaleP.size(); i++)
    {
        //qDebug()<<i;
        if(catchPoint(*scaleP[i],temp))
            return i;
    }
    return -1;
}

void Primitive::drawDotline(QPainter *painter, QPoint p1, QPoint p2)
{
    int x1 = p1.rx();
    int y1 = p1.ry();
    int x2 = p2.rx();
    int y2 = p2.ry();
    double dx = x2 - x1;
    double dy = y2 - y1;
    double e = qAbs(dx) > qAbs(dy) ? qAbs(dx) : qAbs(dy);
    double de_x = dx/e;
    double de_y = dy/e;
    double tempx = x1;
    double tempy = y1;
    for(int i = 0; i <= e; i += 3)
    {
        QPoint tempP((int)(tempx+0.5),(int)(tempy+0.5));
        painter->drawPoint(tempP);
        tempx += de_x*3;
        tempy += de_y*3;
    }
}

double distance2(QPoint &a, QPoint &b)
{
    double x1 = a.rx();
    double y1 = a.ry();
    double x2 = b.rx();
    double y2 = b.ry();
    return ((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

void Primitive::rotate(QPainter *painter, QPoint end)
{
    rotating = true;
    double a2 = distance2(rotateP,end);
    double b2 = distance2(centerP,rotateP);
    double c2 = distance2(centerP,end);
    if(b2 == 0 || c2 == 0)
    {
        cosA = 1;
    }
    else
    {
        cosA = (b2+c2-a2)/(2*sqrt(b2*c2));
    }
    sinA = sqrt(1 - cosA*cosA);
    //double a = qAcos(cosA);
    if(rotateP.rx() == centerP.rx())
    {
        if(rotateP.ry() > centerP.ry())
        {
            if(end.rx() > rotateP.rx())
            {
                sinA = -sinA;//a = 2*M_PI - a;
            }
        }
        else
        {
            if(end.rx() < rotateP.rx())
            {
                sinA = -sinA;//a = 2*M_PI - a;
            }
        }
    }
    else
    {
        double k = (double)(rotateP.ry()-centerP.ry())/(rotateP.rx()-centerP.rx());
        if(k*(end.rx()-centerP.rx())+centerP.ry() < end.ry())
        {
            if(rotateP.rx() < centerP.rx())
                sinA = -sinA;//a = 2*M_PI - a;
        }
        else
        {
            if(rotateP.rx() > centerP.rx())
                sinA = -sinA;//a = 2*M_PI - a;
        }
    }
    editR = rotatePoint(rotateP,centerP,cosA,sinA);
    startP = rotatePoint(startP,centerP,cosA,sinA);
    endP = rotatePoint(endP,centerP,cosA,sinA);
    for(int i = 0; i < keyP.size(); i++)
    {
        keyP[i] = rotatePoint(keyTemp[i],centerP,cosA,sinA);
    }
    draw_(painter,startP,endP);
    //qDebug()<<"rotate"<<startP.rx();
    //rotating = false;
}

void Primitive::translate(QPainter *painter, int dx, int dy)
{
    rotating = false;
    color = painter->pen().color();
    translate_(painter,dx,dy);
}

void Primitive::scale(QPainter *painter, QPoint end, int n)
{
    scaleP[n]->setX(end.rx());
    scaleP[n]->setY(end.ry());
    rotating = false;
    draw_(painter,startP,endP);
}

void Primitive::zoom(QPainter *painter, QPoint end, int n)
{
    //qDebug()<<"zoom"<<n;
    double s;
    rotating = false;
    for(int i = 0; i < keyP.size(); i++)
    {
        if(n == 0)
        {
            if(x_max - end.rx() <= 20)
                return;
            s = (x_max - end.rx())/(double)(x_max - x_min);
            keyP[i].setX(x_max - s*(x_max - keyP[i].rx()) + 0.5);
        }
        else if(n == 1)
        {
            if(end.ry() - y_min <= 20)
                return;
            s = (end.ry() - y_min)/(double)(y_max - y_min);
            keyP[i].setY(y_min + s*(keyP[i].ry() - y_min) + 0.5);
        }
        else if(n == 2)
        {
            if(end.rx() - x_min <= 20)
                return;
            s = (end.rx() - x_min)/(double)(x_max - x_min);
            keyP[i].setX(x_min + s*(keyP[i].rx() - x_min) + 0.5);
        }
        else if(n == 3)
        {
            if(y_max - end.ry() <= 20)
                return;
            s = (y_max - end.ry())/(double)(y_max - y_min);
            keyP[i].setY(y_max - s*(y_max - keyP[i].ry()) + 0.5);
        }
    }
    draw_(painter,startP,endP);
}
