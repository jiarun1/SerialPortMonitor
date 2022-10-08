#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>

#include <QChart>
#include <QChartView>



namespace Ui {
class GraphWindow;
}

class GraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GraphWindow(QWidget *parent = nullptr);
    ~GraphWindow();

private slots:
    void on_action_Format_triggered();

    void on_action_Axis_triggered();

private:
    Ui::GraphWindow *ui;

    QChartView* Graph_Area;

};

#endif // GRAPHWINDOW_H
