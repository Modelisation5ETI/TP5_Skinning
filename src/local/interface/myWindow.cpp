#include "myWindow.hpp"

#include "myWidgetGL.hpp"
#include "../../lib/common/error_handling.hpp"
#include "ui_mainwindow.h"

#include <iostream>

myWindow::myWindow(QWidget *parent)
    :QMainWindow(parent),ui(new Ui::MainWindow)
{
    try
    {
        //Setup window layout
        ui->setupUi(this);

        //Create openGL context
        QGLFormat qglFormat;
        qglFormat.setVersion(1,2);

        //Create OpenGL Widget renderer
        glWidget=new myWidgetGL(qglFormat);

        //Add the OpenGL Widget into the layout
        ui->layout_scene->addWidget(glWidget);
    }
    catch(cpe::exception_cpe const& e)
    {
        std::cout<<std::endl<<e.report_exception()<<std::endl;
    }

    //Connect slot and signals
    connect(ui->quit,SIGNAL(clicked()),this,SLOT(action_quit()));
    connect(ui->draw,SIGNAL(clicked()),this,SLOT(action_draw()));
    connect(ui->wireframe,SIGNAL(clicked()),this,SLOT(action_wireframe()));
    connect(ui->skeleton,SIGNAL(clicked()),this,SLOT(action_skeleton()));
    connect(ui->staticPos,SIGNAL(clicked()),this,SLOT(action_staticPos()));
    connect(ui->weight,SIGNAL(clicked()),this,SLOT(action_weight()));
    connect(ui->blink,SIGNAL(clicked()),this,SLOT(action_blink()));

}

myWindow::~myWindow()
{}

void myWindow::action_quit()
{
    close();
}

void myWindow::action_draw()
{
    glWidget->change_draw_state();
}

void myWindow::action_wireframe()
{
    bool const state_wireframe=ui->wireframe->isChecked();
    glWidget->wireframe(state_wireframe);
}

void myWindow::action_skeleton()
{
    bool const state_wireframe=ui->skeleton->isChecked();
   // glWidget->wireframe(state_wireframe);
}

void myWindow::action_weight()
{
    bool const state_wireframe=ui->weight->isChecked();
    //glWidget->wireframe(state_wireframe);
}

void myWindow::action_staticPos()
{
    glWidget->change_staticPos_state();
}

void myWindow::action_blink()
{
    bool const state_wireframe=ui->blink->isChecked();
   // glWidget->wireframe(state_wireframe);
}

