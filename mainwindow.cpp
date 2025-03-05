#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //myopenglptr = new MyOpenGLWidget(this);
    //rectangle = new OpenGLRectangle(this);
    trectangleptr = new TexturedRectangle(this);
    //triangleptr = new TriangleWidget(this);
    setCentralWidget(trectangleptr);
    //myopenglptr->update();  // 手动触发重绘
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actiondraw_triggered()
{
    trectangleptr->setDrawRectangle(true);

}


void MainWindow::on_actionclear_triggered()
{
    trectangleptr->clearRectangle();
}


// void MainWindow::on_actiondraw_triggered(bool checked)
// {
//     if(checked)
//     {
//         rectangle->setDrawRectangle(true);
//     }
//     else
//         rectangle->clearRectangle();
// }

