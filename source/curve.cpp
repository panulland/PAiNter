#include "curve.h"
#include "line.h"

Curve::Curve()
{

}

void Curve::set(QPainter *painter, int n, QStringList &list)
{
    color = painter->pen().color();
    int x1;
    int y1;
    for(int i = 0; i < n; i++)
    {
        if(i == n - 1)
        {
            x1 = list[i*2].trimmed().toInt();
            y1 = list[i*2+1].trimmed().toInt();
        }
        else
        {
            x1 = list[i*2].toInt();
            y1 = list[i*2+1].toInt();
        }
        QPoint temp(x1,y1);
        keyP.push_back(temp);
        keyTemp.push_back(temp);
    }
}

void Curve::drawBezier(QPainter *painter)
{
    useAlg = Bezier;

    int tempx = keyP[0].rx();
    int tempy = keyP[0].ry();
    for(double t = 0; t < 1; t += 0.01)
    {
        double *qx = new double[keyP.size()];
        double *qy = new double[keyP.size()];
        double x,y;
        for(int i = 0; i < keyP.size(); i++)
        {
            qx[i]=keyP[i].rx();
            qy[i]=keyP[i].ry();
        }
        for(int i = 0; i < keyP.size(); i++)
        {
            for(int k = 0; k < keyP.size() - i - 1; k++)
            {
                x = qx[k]+t*(qx[k+1]-qx[k]);
                y = qy[k]+t*(qy[k+1]-qy[k]);
                qx[k]=x;
                qy[k]=y;
            }
        }
        Line line;
        line.drawDDA(painter,tempx,tempy,(int)(qx[0]+0.5),(int)(qy[0]+0.5));
        tempx = (int)(qx[0]+0.5);
        tempy = (int)(qy[0]+0.5);
    }
    Line line_;
    line_.drawDDA(painter,tempx,tempy,keyP[keyP.size()-1].rx(),keyP[keyP.size()-1].ry());
}

void Curve::drawBspline(QPainter *painter)
{
    useAlg = Bspline;
    int m = keyP.size()-3;
    double x,y;
    int tempx1,tempy1,tempx2,tempy2;
    for(int i = 0; i < m; i++)
    {
        for(double t = 0; t < 1; t += 0.01)
        {
            x = ((-pow(t,3)+3*pow(t,2)-3*t+1)/6)*keyP[i].rx() + ((3*pow(t,3)-6*pow(t,2)+4)/6)*keyP[i+1].rx() + ((-3*pow(t,3)+3*pow(t,2)+3*t+1)/6)*keyP[i+2].rx() + (pow(t,3)/6)*keyP[i+3].rx();
            y = ((-pow(t,3)+3*pow(t,2)-3*t+1)/6)*keyP[i].ry() + ((3*pow(t,3)-6*pow(t,2)+4)/6)*keyP[i+1].ry() + ((-3*pow(t,3)+3*pow(t,2)+3*t+1)/6)*keyP[i+2].ry() + (pow(t,3)/6)*keyP[i+3].ry();
            if(t == 0.0)
            {
                tempx1 = (int)(x+0.5);
                tempy1 = (int)(y+0.5);
            }
            else
            {
                tempx2 = (int)(x+0.5);
                tempy2 = (int)(y+0.5);
                Line line;
                line.drawDDA(painter,tempx1,tempy1,tempx2,tempy2);
                tempx1 = tempx2;
                tempy1 = tempy2;
            }
        }
    }
}

void Curve::draw_(QPainter *painter, QPoint p1, QPoint p2)
{
    painter->pen().width();
    switch (useAlg) {
    case Bezier:
    {
        drawBezier(painter);
        type = typeCurveBezier;
        break;
    }
    case Bspline:
    {
        drawBspline(painter);
        type = typeCurveBspline;
        break;
    }
    default:
    {
        qDebug()<<"empty algorithm!";
    }
    }
    x_min = x_max = keyP[0].rx();
    y_min = y_max = keyP[0].ry();
    scaleP.clear();
    scaleP.push_back(&keyP[0]);
    if(!rotating)
    {
        keyTemp[0] = keyP[0];
    }
    for(int i = 1; i < keyP.size(); i++)
    {
        qDebug()<<i;
        scaleP.push_back(&keyP[i]);
        x_min = x_min < keyP[i].rx() ? x_min : keyP[i].rx();
        x_max = x_max > keyP[i].rx() ? x_max : keyP[i].rx();
        y_min = y_min < keyP[i].ry() ? y_min : keyP[i].ry();
        y_max = y_max > keyP[i].ry() ? y_max : keyP[i].ry();
        if(!rotating)
        {
            keyTemp[i]=keyP[i];
        }
    }
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
    {   centerP.setX((x_min + x_max)/2);
        centerP.setY((y_min + y_max)/2);
        rotateP.setX((x_min + x_max)/2);
        rotateP.setY(centerP.ry()-(y_max - y_min)/4);
        editR = rotateP;
    }
}

void Curve::edit_(QPainter *painter)
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
    pen.setColor(Qt::red);
    painter->setPen(pen);
    for(int i = 0; i < keyP.size() - 1; i++)
    {
        drawDotline(painter,keyP[i],keyP[i+1]);
    }
    pen.setColor(Qt::black);
    pen.setWidth(8);
    pen.setCapStyle(Qt::FlatCap);
    painter->setPen(pen);
    for(int i = 0; i < 4; i++)
    {
        if(ps[i].rx() != -1)
            painter->drawPoint(ps[i]);
    }
}

bool Curve::translate_(QPainter *painter, int dx, int dy)
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

bool Curve::rotate_(QPainter *painter, int x, int y, double rad)
{
    double x1,y1,tempx1,tempy1;
    x = (double) x;
    y = (double) y;
    for(int i = 0; i < keyP.size(); i++)
    {
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

bool Curve::scale_(QPainter *painter, int x, int y, float s)
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

bool Curve::clipCS(QPainter *painter, int x1, int y1, int x2, int y2)
{
    return true;
}

bool Curve::clipLB(QPainter *painter, int x1, int y1, int x2, int y2)
{
    return true;
}
