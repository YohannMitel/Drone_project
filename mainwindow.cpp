#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionCircles->setChecked(ui->canvas->showCircles);
    ui->actionTriangles->setChecked(ui->canvas->showTriangles);
    ui->actionCenter->setChecked(ui->canvas->showCenters);
    ui->canvas->loadMesh("C:/Users/MITEL Yohann/Desktop/QT/QtCircumcircle_voronoi/QtCircumcircle/models/mesh2.json");
    QObject::connect(ui->canvas, SIGNAL(updateSB(QString)), ui->statusBar, SLOT(showMessage(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_triggered() {
    ui->canvas->clear();
    QVector<Vector2D> tab={ {100,100},{600,30},{200,400},{800,800},{1500,500}};
    ui->canvas->addPoints(tab);
    ui->canvas->addTriangle(0,1,2);
    ui->canvas->addTriangle(2,1,3);
    ui->canvas->addTriangle(1,4,3);
    ui->statusBar->showMessage("Stats: |V|="+QString::number(ui->canvas->getSizeofV())+
                               "  |T|="+QString::number(ui->canvas->getSizeofT()));
}



void MainWindow::on_actionQuit_triggered() {
    QApplication::quit();
}


void MainWindow::on_actionLoad_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this,"Load Polygon file",".","*.json");
    ui->canvas->loadMesh(fileName);
    ui->statusBar->showMessage("Stats: |V|="+QString::number(ui->canvas->getSizeofV())+
                               "  |T|="+QString::number(ui->canvas->getSizeofT()));
}


void MainWindow::on_actionCircles_triggered(bool checked) {
    ui->canvas->showCircles=checked;
    update();
}


void MainWindow::on_actionTriangles_triggered(bool checked) {
    ui->canvas->showTriangles=checked;
    update();
}


void MainWindow::on_actionCenter_triggered(bool checked) {
    ui->canvas->showCenters=checked;
    update();
}


void MainWindow::on_actionAbout_triggered() {
    QMessageBox::information(this,"About Circumcircle","Initial program to compute the circumcenter of the loaded triangles.");
}


void MainWindow::on_actionCheck_Delaunay_triggered()
{
    ui->canvas->checkDelaunay();
}


void MainWindow::on_actionProcess_flip_triggered()
{
    // Its like a solve delaunay function
    ui->canvas->flippAll();
}


void MainWindow::on_actionProcess_polygons_triggered()
{
    ui->canvas->processPoly();
}


void MainWindow::on_actionVoronoi_transparency_triggered(bool checked)
{
    ui->canvas->voronoiTransparency=checked;
    update();
}

