#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showType = new QLabel(this);
    location = new QLabel(this);
    showOption = new QLabel(this);
    showType->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    location->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    showOption->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    showType->setMinimumWidth(150);
    showOption->setMinimumWidth(150);
    location->setMinimumWidth(100);
    showType->setText("None");
    showOption->setText("Waiting");
    ui->statusBar->addPermanentWidget(showOption);
    ui->statusBar->addPermanentWidget(showType);
    ui->statusBar->addPermanentWidget(location);

    this->cur_canvas = nullptr;
    this->ui->mdiArea->closeAllSubWindows();
    Canvas* draw = new Canvas;
    QMdiSubWindow* t = ui->mdiArea->addSubWindow(draw);
    draw->showMaximized();
    cur_canvas = draw;
    cur_canvas->location = location;
    cur_canvas->showOption = showOption;
    ui->mdiArea->setActiveSubWindow(t);
    t->setWindowTitle(tr("Canvas"));
    t->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSave_triggered()
{
    if(cur_canvas == nullptr)
        return;
    cur_canvas->save();
}

void MainWindow::on_Line_clicked()
{
    if(cur_canvas == nullptr)
        return;
    qDebug()<<"Line";
    showType->setText("Line");
    cur_canvas->setTypeLine();
}

void MainWindow::on_Ellipse_clicked()
{
    if(cur_canvas == nullptr)
        return;
    qDebug()<<"Ellipse";
    showType->setText("Ellipse");
    cur_canvas->setTypeEllipse();
}

void MainWindow::on_Polygon_clicked()
{

    if(cur_canvas == nullptr)
        return;
    qDebug()<<"Polygon";
    showType->setText("Polygon");
    cur_canvas->setTypePolygon();
}

void MainWindow::on_Bezier_clicked()
{
    if(cur_canvas == nullptr)
        return;
    qDebug()<<"Bezier";
    showType->setText("Bezier Curve");
    cur_canvas->setTypeCurveBezier();
}

void MainWindow::on_Bspline_clicked()
{
    if(cur_canvas == nullptr)
        return;
    qDebug()<<"Bspline";
    showType->setText("B-spline Curve");
    cur_canvas->setTypeCurveBspline();
}

void MainWindow::on_PenColor_activated(const QString &arg1)
{
    if(cur_canvas == nullptr)
        return;
    if(arg1 != "more")
    {
        cur_canvas->setPenColor(arg1);
        cur_canvas->update();
    }
    else
    {
        QColorDialog tempDialog;
        QColor temp = tempDialog.getColor(Qt::black,this,"Choose Color of Pen");
        if(temp != QColor::Invalid)
        {
            cur_canvas->setPenColor(temp);
            cur_canvas->update();
        }
    }
}

void MainWindow::on_PenWidth_currentIndexChanged(int index)
{
    if(cur_canvas == nullptr)
        return;
    cur_canvas->setPenWidth(index+1);
    cur_canvas->update();
}

void MainWindow::on_Cut_clicked()
{
    if(cur_canvas == nullptr)
        return;
    cur_canvas->setCut();
}

void MainWindow::on_actionNew_triggered()
{
    if(cur_canvas == nullptr)
        return;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,nullptr,"Do you want to save it?",QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
    if(reply == QMessageBox::Save)
    {
        on_actionSave_triggered();
        ui->PenColor->setCurrentIndex(0);
        ui->CanvasColor->setCurrentIndex(0);
        ui->PenWidth->setCurrentIndex(0);
        ui->mdiArea->closeAllSubWindows();
        cur_canvas = nullptr;
        Canvas *canvas = new Canvas;
        QMdiSubWindow* t = ui->mdiArea->addSubWindow(canvas);
        canvas->showMaximized();
        cur_canvas = canvas;
        cur_canvas->location = location;
        cur_canvas->showOption = showOption;
        ui->mdiArea->setActiveSubWindow(t);
        t->setWindowTitle("canvas");
        t->show();
    }
    else if(reply == QMessageBox::No)
    {
        ui->PenColor->setCurrentIndex(0);
        ui->CanvasColor->setCurrentIndex(0);
        ui->PenWidth->setCurrentIndex(0);
        ui->mdiArea->closeAllSubWindows();
        cur_canvas = nullptr;
        Canvas *canvas = new Canvas;
        QMdiSubWindow* t = ui->mdiArea->addSubWindow(canvas);
        canvas->showMaximized();
        cur_canvas = canvas;
        cur_canvas->location = location;
        cur_canvas->showOption = showOption;
        ui->mdiArea->setActiveSubWindow(t);
        t->setWindowTitle("canvas");
        t->show();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    if(cur_canvas == nullptr)
        return;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,nullptr,"Do you want to save it?",QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
    if(reply == QMessageBox::Save)
    {
        on_actionSave_triggered();
        ui->PenColor->setCurrentIndex(0);
        ui->CanvasColor->setCurrentIndex(0);
        ui->PenWidth->setCurrentIndex(0);
        ui->mdiArea->closeAllSubWindows();
        cur_canvas = nullptr;
        Canvas *canvas = new Canvas;
        canvas->open();
        QMdiSubWindow* t = ui->mdiArea->addSubWindow(canvas);
        canvas->showMaximized();
        cur_canvas = canvas;
        cur_canvas->location = location;
        cur_canvas->showOption = showOption;
        ui->mdiArea->setActiveSubWindow(t);
        t->setWindowTitle("canvas");
        t->show();
    }
    else if(reply == QMessageBox::No)
    {
        ui->PenColor->setCurrentIndex(0);
        ui->CanvasColor->setCurrentIndex(0);
        ui->PenWidth->setCurrentIndex(0);
        ui->mdiArea->closeAllSubWindows();
        cur_canvas = nullptr;
        Canvas *canvas = new Canvas;
        canvas->open();
        QMdiSubWindow* t = ui->mdiArea->addSubWindow(canvas);
        canvas->showMaximized();
        cur_canvas = canvas;
        cur_canvas->location = location;
        cur_canvas->showOption = showOption;
        ui->mdiArea->setActiveSubWindow(t);
        t->setWindowTitle("canvas");
        t->show();
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    if(cur_canvas == nullptr)
        return;
    cur_canvas->saveAs();
}

void MainWindow::on_Clear_clicked()
{
    if(cur_canvas == nullptr)
        return;
    cur_canvas->clear();
    showType->setText("None");
    showOption->setText("Waiting");
}

void MainWindow::on_CanvasColor_activated(const QString &arg1)
{
    if(cur_canvas == nullptr)
        return;
    if(arg1 != "more")
    {
        cur_canvas->setCanvasColor(arg1);
        cur_canvas->update();
    }
    else
    {
        QColorDialog tempDialog;
        QColor temp = tempDialog.getColor(Qt::black,this,"Choose Color of Canvas");
        if(temp != QColor::Invalid)
        {
            cur_canvas->setCanvasColor(temp);
            cur_canvas->update();
        }
    }
}

void MainWindow::on_actiontoDraw_triggered(bool checked)
{
    if(checked)
    {
        ui->Draw->show();
    }
    else
    {
        ui->Draw->close();
    }
}

void MainWindow::on_actionTools_triggered(bool checked)
{

    if(checked)
    {
        ui->Tools->show();
    }
    else
    {
        ui->Tools->close();
    }
}

void MainWindow::on_Draw_visibilityChanged(bool visible)
{
    if(visible)
    {
        ui->actiontoDraw->setChecked(true);
    }
    else
    {
        ui->actiontoDraw->setChecked(false);
    }
}

void MainWindow::on_Tools_visibilityChanged(bool visible)
{
    if(visible)
    {
        ui->actionTools->setChecked(true);
    }
    else
    {
        ui->actionTools->setChecked(false);
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(cur_canvas == nullptr)
        return;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,nullptr,"Do you want to save it?",QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
    if(reply == QMessageBox::Save)
    {
        on_actionSave_triggered();
    }
    else if(reply == QMessageBox::Cancel)
    {
        e->ignore();
    }
}
