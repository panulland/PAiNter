#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QFile>
#include <QDebug>
#include "canvas.h"

class Command
{
private:
    QString fileName;
    QString path;
    Canvas* cur_canvas;
public:
    Command(char *File, char *Path);
    void parsing();
};

#endif // COMMAND_H
