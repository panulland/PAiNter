#include "ellipse.h"

Ellipse::Ellipse()
{

}

void Ellipse::drawPoint(QPainter *painter, int x, int y, int px, int py)
{
    QPoint temp1(x+px, y+py);
    QPoint temp2(x+px, y-py);
    QPoint temp3(x-px, y+py);
    QPoint temp4(x-px, y-py);
    painter->drawPoint(temp1);
    painter->drawPoint(temp2);
    painter->drawPoint(temp3);
    painter->drawPoint(temp4);
}

void Ellipse::drawEllipse(QPainter* painter, int x, int y, int rx, int ry)
{
    color = painter->pen().color();
    startP.setX(x);
    startP.setY(y);
    centerP.setX(x);
    centerP.setY(y);
    ra = rx;
    rb = ry;
    double sqx = rx * rx;
    double sqy = ry * ry;
    double p1 = sqy - sqx*ry + 0.25*sqx;
    int tempx = 0;
    int tempy = ry;
    while(sqy * tempx < sqx * tempy)
    {
        drawPoint(painter,x,y,tempx,tempy);
        if(p1 < 0)
        {
            p1 = p1 + 2*sqy*tempx + 3*sqy;
        }
        else
        {
            p1 = p1 + 2*sqy*tempx - 2*sqx*tempy + 3*sqy + 2*sqx;
            tempy--;
        }
        tempx++;
    }
    double p2 = sqy*(tempx + 0.5)*(tempx+0.5) + sqx*(tempy-1)*(tempy-1) - sqx*sqy;
    while(tempy >= 0)
    {
        drawPoint(painter,x,y,tempx,tempy);
        if(p2 > 0)
        {
            p2 = p2 - 2*sqx*tempy + 3*sqx;
        }
        else
        {
            p2 = p2 - 2*sqx*tempy + 2*sqy*tempx + 3*sqx + 2*sqy;
            tempx++;
        }
        tempy--;
    }
}

void Ellipse::edit_(QPainter *painter)
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
    pen.setWidth(1);
    pen.setColor(Qt::black);
    int style = static_cast<int>(Qt::DashDotLine);
    pen.setStyle((Qt::PenStyle)style);
    painter->setPen(pen);
    drawDotline(painter,ps1,ps2);
    drawDotline(painter,ps2,ps3);
    drawDotline(painter,ps3,ps4);
    drawDotline(painter,ps4,ps1);
    pen.setWidth(8);
    pen.setCapStyle(Qt::FlatCap);
    painter->setPen(pen);
    for(int i = 0; i < 4; i++)
    {
        if(ps[i].rx() != -1)
            painter->drawPoint(ps[i]);
    }
}

void Ellipse::draw_(QPainter *painter, QPoint p1, QPoint p2)
{
    penWidth = painter->pen().width();
    centerP = p1;
    startP = p1;
    endP = p2;
    ra = qAbs(p2.rx()-p1.rx());
    rb = qAbs(p2.ry()-p1.ry());
    x_min = centerP.rx() - ra;
    x_max = centerP.rx() + ra;
    y_min = centerP.ry() - rb;
    y_max = centerP.ry() + rb;
    ps1.setX(x_min);
    ps1.setY(y_min);
    ps2.setX(x_min);
    ps2.setY(y_max);
    ps3.setX(x_max);
    ps3.setY(y_max);
    ps4.setX(x_max);
    ps4.setY(y_min);
    ps[0].setX(x_min);
    ps[0].setY((y_min+y_max)/2);
    ps[1].setX((x_min+x_max)/2);
    ps[1].setY(y_max);
    ps[2].setX(x_max);
    ps[2].setY((y_min+y_max)/2);
    ps[3].setX((x_min+x_max)/2);
    ps[3].setY(y_min);
    drawEllipse(painter,centerP.rx(),centerP.ry(),ra,rb);
    scaleP.clear();
    scaleP.push_back(&endP);
}

bool Ellipse::translate_(QPainter *painter, int dx, int dy)
{
    centerP.setX(centerP.rx()+dx);
    centerP.setY(centerP.ry()+dy);
    endP.setX(endP.rx()+dx);
    endP.setY(endP.ry()+dy);
    startP = centerP;
    QPen t;
    t.setWidth(penWidth);
    t.setColor(color);
    painter->setPen(t);
    draw_(painter,startP,endP);
    return true;
}

bool Ellipse::rotate_(QPainter *painter, int x, int y, double r)
{
    return true;
}

bool Ellipse::scale_(QPainter *painter, int x, int y, float s)
{
    qDebug()<<"float"<<s;
    float x1 = centerP.rx();
    float y1 = centerP.ry();
    float tempx1,tempy1,tempx2,tempy2;
    tempx1 = x1*s + x*(1-s);
    tempy1 = y1*s + y*(1-s);
    tempx2 = ra*s;
    tempy2 = rb*s;
    centerP.setX(tempx1+0.5);
    centerP.setY(tempy1+0.5);
    ra = tempx2+0.5;
    rb = tempy2+0.5;
    endP.setX(centerP.rx() + ra);
    endP.setY(centerP.ry() + rb);
    startP = centerP;
    painter->setPen(color);
    draw_(painter,startP,endP);
    return true;
}

bool Ellipse::clipCS(QPainter *painter, int x1, int y1, int x2, int y2)
{
    return true;
}

bool Ellipse::clipLB(QPainter *painter, int x1, int y1, int x2, int y2)
{
    return true;
}

void Ellipse::zoom(QPainter *painter, QPoint end, int n)
{
    qDebug()<<"zoom"<<n;
    if(n == 0)
    {
        if(x_max - end.rx() <= 1)
            return;
        centerP.setX(x_max - (x_max - end.rx())/2);
        if(endP.rx() != x_max)
        {
            endP.setX(centerP.rx()*2 - x_max);
        }
    }
    else if(n == 1)
    {
        if(end.ry() - y_min <= 1)
            return;
        centerP.setY(y_min + (end.ry() - y_min)/2);
        if(endP.ry() != y_min)
        {
            endP.setY(centerP.ry()*2 - y_min);
        }
    }
    else if(n == 2)
    {
        if(end.rx() - x_min <= 1)
            return;
        centerP.setX(x_min + (end.rx() - x_min)/2);
        if(endP.rx() != x_min)
        {
            endP.setX(centerP.rx()*2 - x_min);
        }
    }
    else if(n == 3)
    {
        if(y_max - end.ry() <= 1)
            return;
        centerP.setY(y_max - (y_max - end.ry())/2);
        if(endP.ry() != y_max)
        {
            endP.setY(centerP.ry()*2 - y_max);
        }
    }
    startP = centerP;
    draw_(painter,startP,endP);
}


