/**
 * @brief Drone_demo project
 * @author B.Piranda
 * @date dec. 2024
 **/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <drone.h>
#include <QListWidget>
#include <QMap>
#include <QTimer>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();
    void update();

    void on_actionCircles_triggered(bool checked);

    void on_actionCenters_triggered(bool checked);

    void on_actionVoronoi_transparency_triggered(bool checked);

    void on_actionTriangles_triggered(bool checked);

    void on_actionCheck_Delaunay_triggered();

    void on_actionProcess_flip_triggered();

    void on_actionProcess_polygons_triggered();

    void on_actionLoad_triggered();

private:
    Ui::MainWindow *ui;
    QVector<Drone*> *mapDrones=nullptr;

    QTimer *timer;
    QElapsedTimer elapsedTimer;

};
#endif // MAINWINDOW_H
