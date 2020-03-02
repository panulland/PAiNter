#include "command.h"

Command::Command(char *File, char *Path)
{
    fileName = File;
    path = Path;
    cur_canvas = nullptr;
}

void Command::parsing()
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString str(line);
        //qDebug()<<str;
        QStringList list = str.split(" ");
        if(list[0]=="resetCanvas")
        {
            qDebug()<<list[0];
            int width = list[1].toInt();
            int height = list[2].trimmed().toInt();
            cur_canvas = new Canvas;
            cur_canvas->reset(width,height);
        }
        else if(list[0]=="saveCanvas")
        {
            qDebug()<<list[0];
            cur_canvas->save(path,list[1].trimmed());
        }
        else if(list[0]=="setColor")
        {
            qDebug()<<list[0];
            int r = list[1].toInt();
            int g = list[2].toInt();
            int b = list[3].trimmed().toInt();
            cur_canvas->setColor(r,g,b);
        }
        else if(list[0]=="drawLine")
        {
            qDebug()<<list[0];
            int id = list[1].toInt();
            int x1 = list[2].toInt();
            int y1 = list[3].toInt();
            int x2 = list[4].toInt();
            int y2 = list[5].toInt();
            cur_canvas->line(id,x1,y1,x2,y2,list[6].trimmed());
        }
        else if(list[0]=="drawPolygon")
        {
            qDebug()<<list[0];
            int id = list[1].toInt();
            int n = list[2].toInt();
            line = file.readLine();
            QString alg = list[3].trimmed();
            QString str(line);
            list = str.split(" ");
            cur_canvas->polygon(id,n,alg,list);
        }
        else if(list[0]=="drawEllipse")
        {
            qDebug()<<list[0];
            int id = list[1].toInt();
            int x = list[2].toInt();
            int y = list[3].toInt();
            int rx = list[4].toInt();
            int ry = list[5].trimmed().toInt();
            cur_canvas->ellipse(id,x,y,rx,ry);
        }
        else if(list[0]=="drawCurve")
        {
            qDebug()<<list[0];
            int id = list[1].toInt();
            int n = list[2].toInt();
            line = file.readLine();
            QString alg = list[3].trimmed();
            QString str(line);
            list = str.split(" ");
            cur_canvas->curve(id,n,alg,list);
        }
        else if(list[0]=="translate")
        {
            qDebug()<<list[0];
            int id = list[1].toInt();
            int dx = list[2].toInt();
            int dy = list[3].trimmed().toInt();
            cur_canvas->translate(id,dx,dy);
        }
        else if(list[0]=="rotate")
        {
            qDebug()<<list[0];
            int id = list[1].toInt();
            int x = list[2].toInt();
            int y = list[3].toInt();
            int r = list[4].trimmed().toInt();
            cur_canvas->rotate(id,x,y,r);
        }
        else if(list[0]=="scale")
        {
            qDebug()<<list[0];
            int id = list[1].toInt();
            int x = list[2].toInt();
            int y = list[3].toInt();
            float s = list[4].trimmed().toFloat();
            cur_canvas->scale(id,x,y,s);
        }
        else if(list[0]=="clip")
        {
            qDebug()<<list[0]<<list[6].trimmed();
            int id = list[1].toInt();
            int x1 = list[2].toInt();
            int y1 = list[3].toInt();
            int x2 = list[4].toInt();
            int y2 = list[5].toInt();
            cur_canvas->clip(id,x1,y1,x2,y2,list[6].trimmed());
        }
    }
    file.close();
}
