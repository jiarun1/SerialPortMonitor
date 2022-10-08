#include "GraphWindow.h"
#include "ui_GraphWindow.h"

GraphWindow::GraphWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphWindow)
{
    ui->setupUi(this);

    Graph_Area = new QChartView;

}

GraphWindow::~GraphWindow()
{
    delete ui;
}

void GraphWindow::on_action_Format_triggered()
{
    ui->Setting_StackedWidget->setCurrentWidget(ui->Format_Page);
}


void GraphWindow::on_action_Axis_triggered()
{
    ui->Setting_StackedWidget->setCurrentWidget(ui->Axis_Page);
}


