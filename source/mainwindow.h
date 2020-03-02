#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>
#include <QLabel>
#include <QMouseEvent>
#include "canvas.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *e);

private slots:
    void on_actionSave_triggered();

    void on_Line_clicked();

    void on_Ellipse_clicked();

    void on_Polygon_clicked();

    void on_Bezier_clicked();

    void on_Bspline_clicked();

    void on_PenColor_activated(const QString &arg1);

    void on_PenWidth_currentIndexChanged(int index);

    void on_Cut_clicked();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSaveAs_triggered();


    void on_Clear_clicked();

    void on_CanvasColor_activated(const QString &arg1);

    void on_actiontoDraw_triggered(bool checked);

    void on_actionTools_triggered(bool checked);

    void on_Draw_visibilityChanged(bool visible);

    void on_Tools_visibilityChanged(bool visible);

private:
    Ui::MainWindow *ui;
    QLabel *showType;
    QLabel *location;
    QLabel *showOption;
    Canvas* cur_canvas;
};
#endif // MAINWINDOW_H
