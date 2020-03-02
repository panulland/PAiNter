#include "polygon.h"
#include <QString>

Polygon::Polygon()
{
    useAlg = None;
    type = typePolygon;
}

void Polygon::drawPolygon(QPainter *painter, QString alg, int n, QStringList &list)
{
    color = painter->pen().color();
    int x2,y2;
    int x1 = list[0].toInt();
    int y1 = list[1].toInt();
    for(int i = 0; i < n; i++)
    {
        QPoint temp(x1,y1);
        keyP.push_back(temp);
        keyTemp.push_back(temp);
        if(i == n - 1)
        {
            x2 = list[0].toInt();
            y2 = list[1].toInt();
            temp.setX(x2);
            temp.setY(y2);
            keyP.push_back(temp);
            keyTemp.push_back(temp);
        }
        else
        {
            x2 = list[i*2+2].toInt();
            y2 = list[i*2+3].trimmed().toInt();
        }
        Line line;
        qDebug()<<x2<<" "<<y2;
        if(alg == "DDA")
        {
            useAlg = DDA;
            line.drawDDA(painter,x1,y1,x2,y2);
        }
        else if(alg == "Bresenham")
        {
            useAlg = Bresenham;
            line.drawBresenham(painter,x1,y1,x2,y2);
        }
        else
        {
            qDebug()<<"wrong algorithm!";
        }
        x1 = x2;
        y1 = y2;
    }
}

void Polygon::draw_(QPainter *painter, QPoint p1, QPoint p2)
{
    penWidth = painter->pen().width();
    int x1,y1,x2,y2;
    x_min = x_max = keyP[0].rx();
    y_min = y_max = keyP[0].ry();
    scaleP.clear();
    for(int i = 0; i < keyP.size() - 1; i++)
    {
        x1 = keyP[i].rx();
        y1 = keyP[i].ry();
        x2 = keyP[i+1].rx();
        y2 = keyP[i+1].ry();
        scaleP.push_back(&keyP[i]);
        Line line;
        if(useAlg == DDA || useAlg == None)
        {
            line.drawDDA(painter,x1,y1,x2,y2);
        }
        else if(useAlg == Bresenham)
        {
            line.drawBresenham(painter,x1,y1,x2,y2);
        }
        else
        {
            qDebug()<<"wrong algorithm!";
        }
        x_min = x_min < x1 ? x_min : x1;
        x_max = x_max > x1 ? x_max : x1;
        y_min = y_min < y1 ? y_min : y1;
        y_max = y_max > y1 ? y_max : y1;
        if(!rotating)
        {
            keyTemp[i] = keyP[i];
        }
    }
    x_min = x_min < keyP.back().rx() ? x_min : keyP.back().rx();
    x_max = x_max > keyP.back().rx() ? x_max : keyP.back().rx();
    y_min = y_min < keyP.back().ry() ? y_min : keyP.back().ry();
    y_max = y_max > keyP.back().ry() ? y_max : keyP.back().ry();
    if(!rotating)
    {
        keyTemp.back() = keyP.back();
    }
    scaleP.push_back(&keyP.back());
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
    if(!rotating)
    {
        centerP.setX((x_min + x_max)/2);
        centerP.setY((y_min + y_max)/2);
        rotateP.setX((x_min + x_max)/2);
        rotateP.setY(centerP.ry()-(y_max - y_min)/4);
        editR = rotateP;
    }
}

void Polygon::edit_(QPainter *painter)
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
    painter->drawPoint(editR);
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

bool Polygon::translate_(QPainter *painter, int dx, int dy)
{
    for(int i = 0; i < keyP.size(); i++)
    {
        keyP[i].setX(keyP[i].rx()+dx);
        keyP[i].setY(keyP[i].ry()+dy);
    }
    QPen t;
    t.setWidth(penWidth);
    t.setColor(color);
    painter->setPen(t);
    draw_(painter,startP,endP);
    return true;
}

bool Polygon::rotate_(QPainter *painter, int x, int y, double rad)
{
    double x1,y1,tempx1,tempy1;
    x = (double) x;
    y = (double) y;
    for(int i = 0; i < keyP.size(); i++)
    {
        //qDebug()<<i;
        x1 = keyP[i].rx();
        y1 = keyP[i].ry();
        tempx1 = x + (x1 - x)*qCos(rad) - (y1 - y)*qSin(rad);
        tempy1 = y + (x1 - x)*qSin(rad) + (y1 - y)*qCos(rad);
        keyP[i].setX(tempx1+0.5);
        keyP[i].setY(tempy1+0.5);
    }
    painter->setPen(color);
    draw_(painter,startP,endP);
    return true;
}

bool Polygon::scale_(QPainter *painter, int x, int y, float s)
{
    float x1,y1,tempx1,tempy1;
    for(int i = 0; i < keyP.size(); i++)
    {
        x1 = keyP[i].rx();
        y1 = keyP[i].ry();
        tempx1 = x1*s + x*(1-s);
        tempy1 = y1*s + y*(1-s);
        keyP[i].setX(tempx1+0.5);
        keyP[i].setY(tempy1+0.5);
    }
    painter->setPen(color);
    draw_(painter,startP,endP);
    return true;
}

bool Polygon::clipCS(QPainter *painter, int x1, int y1, int x2, int y2)
{
    return true;
}

bool Polygon::clipLB(QPainter *painter, int x1, int y1, int x2, int y2)
{
    return true;
}

void Polygon::scale(QPainter *painter, QPoint end, int n)
{
    if(n == 0)
    {
        startP = end;
        endP = end;
        scaleP[0]->setX(end.rx());
        scaleP[0]->setY(end.ry());
        scaleP.back()->setX(end.rx());
        scaleP.back()->setY(end.ry());
    }
    else
    {
        scaleP[n]->setX(end.rx());
        scaleP[n]->setY(end.ry());
    }
    rotating = false;
    draw_(painter,startP,endP);
}
