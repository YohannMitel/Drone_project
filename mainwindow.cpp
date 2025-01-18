#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->actionCircles->setChecked(ui->widget->showCircles);
    ui->actionTriangles->setChecked(ui->widget->showTriangles);
    ui->actionCenters->setChecked(ui->widget->showCenters);
    ui->widget->loadMesh("../../models/config2.json");
    QObject::connect(ui->widget, SIGNAL(updateSB(QString)), ui->statusbar, SLOT(showMessage(QString)));


    /* preset initial positions of the drones */

    mapDrones = &ui->widget->getMap();
    // Ajouter chaque drone au QListWidget
    for (Drone* drone : *mapDrones) {
        QListWidgetItem *LWitems= new QListWidgetItem(ui->listDronesInfo);
        ui->listDronesInfo->addItem(LWitems);
        ui->listDronesInfo->setItemWidget(LWitems,drone);
    }

    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start();

    elapsedTimer.start();
}


MainWindow::~MainWindow() {
    delete ui;
    delete timer;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::update() {
    /*QListWidgetItem *LWitems= new QListWidgetItem(ui->listDronesInfo);
    ui->listDronesInfo->addItem(LWitems);
    ui->listDronesInfo->setItemWidget(LWitems,mapDrones[1]);*/
    //qDebug() << mapDrones->size();
    static int last=elapsedTimer.elapsed();
    static int steps=5;
    int current=elapsedTimer.elapsed();
    double dt=(current-last)/(1000.0*steps);
    for (int step=0; step<steps; step++) {
        // update positions of drones
        for (auto &drone:*mapDrones) {
            // detect collisions between drone and other flying drones
            if (drone->getStatus()!=Drone::landed) {
                drone->initCollision();
                for (auto &obs:*mapDrones) {
                    if (obs->getStatus()!=Drone::landed && obs->getName()!=drone->getName()) {
                        Vector2D B=obs->getPosition();
                        drone->addCollision(B,ui->widget->droneCollisionDistance);
                    }
                }
            }
            drone->update(dt);
        }
    }
    int d = elapsedTimer.elapsed()-current;
    ui->statusbar->showMessage("duree:"+QString::number(d)+" steps="+QString::number(steps));
    if (d>90) {
        steps/=2;
    } else {

        if (steps<10) steps++;
    }
    last=current;
    ui->widget->repaint();
}

void MainWindow::on_actionCircles_triggered(bool checked)
{
    ui->widget->showCircles=checked;
    update();
}


void MainWindow::on_actionCenters_triggered(bool checked)
{
    ui->widget->showCenters=checked;
    update();
}


void MainWindow::on_actionVoronoi_transparency_triggered(bool checked)
{

    ui->widget->voronoiTransparency=checked;
    update();
}


void MainWindow::on_actionTriangles_triggered(bool checked)
{
    ui->widget->showTriangles=checked;
    update();
}


void MainWindow::on_actionCheck_Delaunay_triggered()
{
    ui->widget->checkDelaunay();
}


void MainWindow::on_actionProcess_flip_triggered()
{
    // Its like a solve delaunay function
    ui->widget->flippAll();
}


void MainWindow::on_actionProcess_polygons_triggered()
{
    ui->widget->processPoly();
}


void MainWindow::on_actionLoad_triggered()
{


    QString fileName = QFileDialog::getOpenFileName(this,"Load Polygon file",".","*.json");
    // Parcourir et supprimer chaque widget associé à un QListWidgetItem

    if (fileName.isEmpty()) {
        return; // Arrêter si aucun fichier n'a été sélectionné
    }

    for (int i = 0; i < ui->listDronesInfo->count() ; i++) {
        QListWidgetItem* item = ui->listDronesInfo->item(i);
        // Supprimer l'élément de la liste
        delete item;

    }

    // Nettoyer complètement la liste
    ui->listDronesInfo->clear();


    ui->widget->loadMesh(fileName);
    ui->statusbar->showMessage("Stats: |V|="+QString::number(ui->widget->getSizeofV())+
                               "  |T|="+QString::number(ui->widget->getSizeofT()));

    mapDrones = &ui->widget->getMap();
    // Ajouter chaque drone au QListWidget
    for (Drone* drone : *mapDrones) {
        QListWidgetItem *LWitems= new QListWidgetItem(ui->listDronesInfo);
        ui->listDronesInfo->addItem(LWitems);
        ui->listDronesInfo->setItemWidget(LWitems,drone);
    }



}

