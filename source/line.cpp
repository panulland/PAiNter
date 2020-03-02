#include "line.h"

Line::Line()
{
    useAlg = None;
    type = typeLine;
}

Line::Line(int x1, int y1, int x2, int y2, QString alg, QColor c)
{
    startP.setX(x1);
    startP.setY(y1);
    endP.setX(x2);
    endP.setY(y2);
    if(alg=="DDA")
    {
        useAlg=DDA;
    }
    else if(alg=="Bresenham")
    {
        useAlg=Bresenham;
    }
    else
    {
        qDebug()<<"wrong algorithm!";
    }
    color = c;
}

void Line::drawDDA(QPainter *painter, int x1, int y1, int x2, int y2)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    double e = qAbs(dx) > qAbs(dy) ? qAbs(dx) : qAbs(dy);
    double de_x = dx/e;
    double de_y = dy/e;
    double tempx = x1;
    double tempy = y1;
    for(int i = 0; i <= e; i++)
    {
        QPoint tempP((int)(tempx+0.5),(int)(tempy+0.5));
        painter->drawPoint(tempP);
        tempx += de_x;
        tempy += de_y;
    }
}

void Line::drawBresenham(QPainter *painter, int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int e = qAbs(dx) > qAbs(dy) ? qAbs(dx) : qAbs(dy);
    int e_ = qAbs(dx) > qAbs(dy) ? qAbs(dy) : qAbs(dx);
    int ddx = qAbs(dx) > qAbs(dy) ? dx : dy;
    int ddy = qAbs(dx) > qAbs(dy) ? dy : dx;
    int ex = ddx > 0 ? 1 : -1;
    int ey = ddy > 0 ? 1 : -1;
    int p = 2*e_ - e;
    int tempx = qAbs(dx) > qAbs(dy) ? x1 : y1;
    int tempy = qAbs(dx) > qAbs(dy) ? y1 : x1;
    for(int i = 0; i <= e; i++)
    {
        QPoint tempP(tempx,tempy);
       if(qAbs(dx)<qAbs(dy))
       {
           tempP.setX(tempy);
           tempP.setY(tempx);
       }
       painter->drawPoint(tempP);
       if(p >= 0)
       {
           tempy += ey;
           p += 2*(e_ - e);
       }
       else
       {
           p += 2*e_;
       }
       tempx += ex;
    }
}

void Line::edit_(QPainter *painter)
{
    if(startP.rx() < 0)
        return;
    QPen pen;
    pen.setWidth(10);
    pen.setColor(Qt::blue);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);
    painter->drawPoint(centerP);
    pen.setColor(Qt::red);
    painter->setPen(pen);
    for(int i = 0; i < scaleP.size(); i++)
    {
        painter->drawPoint(*scaleP[i]);
    }
    pen.setColor(Qt::green);
    painter->setPen(pen);
    painter->drawPoint(rotateP);
    if(cuting == true)
    {
        pen.setWidth(1);
        pen.setColor(Qt::black);
        int style = static_cast<int>(Qt::DashDotLine);
        pen.setStyle((Qt::PenStyle)style);
        painter->setPen(pen);
        QPoint temp1(cutPoint[0].rx(),cutPoint[1].ry());
        QPoint temp2(cutPoint[1].rx(),cutPoint[0].ry());
        drawDotline(painter,cutPoint[0],temp1);
        drawDotline(painter,cutPoint[1],temp1);
        drawDotline(painter,cutPoint[0],temp2);
        drawDotline(painter,cutPoint[1],temp2);
    }
}

bool Line::clipCS(QPainter *painter, int x1, int y1, int x2, int y2)
{
    int sx = startP.rx();
    int sy = startP.ry();
    int ex = endP.rx();
    int ey = endP.ry();
    double dx = ex - sx;
    double dy = ey - sy;
    uint8_t b1,b2;
    b1 = b2 = 0;
    b1 |= sy > y2 ? 8 : 0;
    b1 |= sy < y1 ? 4 : 0;
    b1 |= sx > x2 ? 2 : 0;
    b1 |= sx < x1 ? 1 : 0;
    b2 |= ey > y2 ? 8 : 0;
    b2 |= ey < y1 ? 4 : 0;
    b2 |= ex > x2 ? 2 : 0;
    b2 |= ex < x1 ? 1 : 0;
    qDebug()<<b1<<b2;
    if((b1&b2) != 0)
    {
        sx = -1;
        sy = -1;
        ex = -1;
        ey = -1;
    }
    else if(b1 != 0 || b2 != 0)
    {
        if((b1&1) == 1)
        {
            sy = (dy/dx)*(x1 - sx) + sy + 0.5;
            sx = x1;
            b1 |= sy > y2 ? 8 : 0;
            b1 |= sy < y1 ? 4 : 0;
        }
        else if((b2&1) == 1)
        {
            ey = (dy/dx)*(x1 - ex) + ey + 0.5;
            ex = x1;
            b2 |= ey > y2 ? 8 : 0;
            b2 |= ey < y1 ? 4 : 0;
        }
        if((b1&2) == 2)
        {
            sy = (dy/dx)*(x2 - sx) + sy + 0.5;
            sx = x2;
            b1 |= sy > y2 ? 8 : 0;
            b1 |= sy < y1 ? 4 : 0;
        }
        else if((b2&2) == 2)
        {
            ey = (dy/dx)*(x2 - ex) + ey + 0.5;
            ex = x2;
            b2 |= ey > y2 ? 8 : 0;
            b2 |= ey < y1 ? 4 : 0;
        }
        if((b1&4) == 4)
        {
            sx = (dx/dy)*(y1 - sy) + sx + 0.5;
            sy = y1;
        }
        else if((b2&4) == 4)
        {
            ex = (dx/dy)*(y1 - ey) + ex + 0.5;
            ey = y1;
        }
        if((b1&8) == 8)
        {
            sx = (dx/dy)*(y2 - sy) + sx + 0.5;
            sy = y2;
        }
        else if((b2&8) == 8)
        {
            ex = (dx/dy)*(y2 - ey) + ex + 0.5;
            ey = y2;
        }
    }
    startP.setX(sx);
    startP.setY(sy);
    endP.setX(ex);
    endP.setY(ey);
    painter->setPen(color);
    draw_(painter,startP,endP);
    return true;
}

bool Line::clipLB(QPainter *painter, int x1, int y1, int x2, int y2)
{
    int sx = startP.rx();
    int sy = startP.ry();
    int ex = endP.rx();
    int ey = endP.ry();
    double u1 = 0;
    double u2 = 1;
    double p[4],q[4];
    p[0] = sx - ex;
    p[1] = -p[0];
    p[2] = sy - ey;
    p[3] = -p[2];
    q[0] = sx - x1;
    q[1] = x2 - sx;
    q[2] = sy - y1;
    q[3] = y2 - sy;
    double r;
    bool flag = false;
    for(int i = 0; i < 4; i++)
    {
        r = q[i]/p[i];
        if(p[i] < 0)
        {
            u1 = u1 > r ? u1 : r;
            if(u1 > u2)
            {
                flag = true;
                break;
            }
        }
        else if(p[i] > 0)
        {
            u2 = u2 < r ? u2 : r;
            if(u1 > u2)
            {
                flag = true;
                break;
            }
        }
        else if(p[i] == 0 && q[i] < 0)
        {
            flag = true;
            break;
        }
    }
    if(flag)
    {
        startP.setX(-1);
        startP.setY(-1);
        endP.setX(-1);
        endP.setY(-1);
    }
    else
    {
        startP.setX(sx - (sx - ex)*u1 + 0.5);
        startP.setY(sy - (sy - ey)*u1 + 0.5);
        endP.setX(sx - (sx - ex)*u2 + 0.5);
        endP.setY(sy - (sy - ey)*u2 + 0.5);
    }
    painter->setPen(color);
    draw_(painter,startP,endP);
    return true;
}

void Line::draw_(QPainter *painter, QPoint p1, QPoint p2)
{
    penWidth = painter->pen().width();
    switch (useAlg) {
    case DDA:
    {
        drawDDA(painter,p1.rx(),p1.ry(),p2.rx(),p2.ry());
        break;
    }
    case Bresenham:
    {
        drawBresenham(painter,p1.rx(),p1.ry(),p2.rx(),p2.ry());
        break;
    }
    case None:
    {
        drawDDA(painter,p1.rx(),p1.ry(),p2.rx(),p2.ry());
        break;
    }
    default:
    {
        qDebug()<<"empty algorithm!";
    }
    }
    x_min = startP.rx() < endP.rx() ? startP.rx() : endP.rx();
    y_min = startP.ry() < endP.ry() ? startP.ry() : endP.ry();
    x_max = startP.rx() > endP.rx() ? startP.rx() : endP.rx();
    y_max = startP.ry() < endP.ry() ? startP.ry() : endP.ry();
    centerP.setX((startP.rx()+endP.rx())/2.0 + 0.5);
    centerP.setY((startP.ry()+endP.ry())/2.0 + 0.5);
    rotateP.setX(startP.rx()+(endP.rx()-startP.rx())/4.0 + 0.5);
    rotateP.setY(startP.ry()+(endP.ry()-startP.ry())/4.0 + 0.5);
    scaleP.clear();
    scaleP.push_back(&startP);
    scaleP.push_back(&endP);
}

bool Line::translate_(QPainter *painter, int dx, int dy)
{
    startP.setX(startP.rx()+dx);
    startP.setY(startP.ry()+dy);
    endP.setX(endP.rx()+dx);
    endP.setY(endP.ry()+dy);
    QPen t;
    t.setWidth(penWidth);
    t.setColor(color);
    painter->setPen(t);
    draw_(painter,startP,endP);
    return true;
}

bool Line::rotate_(QPainter *painter, int x, int y, double rad)
{
    double x1 = startP.rx();
    double y1 = startP.ry();
    double x2 = endP.rx();
    double y2 = endP.ry();
    double tempx1;
    double tempx2;
    double tempy1;
    double tempy2;
    tempx1 = x + (x1 - x)*qCos(rad) - (y1 - y)*qSin(rad);
    tempy1 = y + (x1 - x)*qSin(rad) + (y1 - y)*qCos(rad);
    tempx2 = x + (x2 - x)*qCos(rad) - (y2 - y)*qSin(rad);
    tempy2 = y + (x2 - x)*qSin(rad) + (y2 - y)*qCos(rad);
    startP.setX((int)(tempx1+0.5));
    startP.setY((int)(tempy1+0.5));
    endP.setX((int)(tempx2+0.5));
    endP.setY((int)(tempy2+0.5));
    painter->setPen(color);
    draw_(painter,startP,endP);
    return true;
}

bool Line::scale_(QPainter *painter, int x, int y, float s)
{
    float x1 = startP.rx();
    float y1 = startP.ry();
    float x2 = endP.rx();
    float y2 = endP.ry();
    float tempx1,tempx2,tempy1,tempy2;
    tempx1 = x1*s + x*(1-s);
    tempy1 = y1*s + y*(1-s);
    tempx2 = x2*s + x*(1-s);
    tempy2 = y2*s + y*(1-s);
    startP.setX(tempx1+0.5);
    startP.setY(tempy1+0.5);
    endP.setX(tempx2+0.5);
    endP.setY(tempy2+0.5);
    painter->setPen(color);
    draw_(painter,startP,endP);
    return true;
}
