/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <canvas.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad;
    QAction *actionQuit;
    QAction *actionCircles;
    QAction *actionCenters;
    QAction *actionTriangles;
    QAction *actionVoronoi_transparency;
    QAction *actionCheck_Delaunay;
    QAction *actionProcess_flip;
    QAction *actionProcess_polygons;
    QAction *actionAbout;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    Canvas *widget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QListWidget *listDronesInfo;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuShow;
    QMenu *menuCompile;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1268, 797);
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName("actionLoad");
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName("actionQuit");
        actionCircles = new QAction(MainWindow);
        actionCircles->setObjectName("actionCircles");
        actionCircles->setCheckable(true);
        actionCenters = new QAction(MainWindow);
        actionCenters->setObjectName("actionCenters");
        actionCenters->setCheckable(true);
        actionTriangles = new QAction(MainWindow);
        actionTriangles->setObjectName("actionTriangles");
        actionTriangles->setCheckable(true);
        actionVoronoi_transparency = new QAction(MainWindow);
        actionVoronoi_transparency->setObjectName("actionVoronoi_transparency");
        actionVoronoi_transparency->setCheckable(true);
        actionCheck_Delaunay = new QAction(MainWindow);
        actionCheck_Delaunay->setObjectName("actionCheck_Delaunay");
        actionProcess_flip = new QAction(MainWindow);
        actionProcess_flip->setObjectName("actionProcess_flip");
        actionProcess_polygons = new QAction(MainWindow);
        actionProcess_polygons->setObjectName("actionProcess_polygons");
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        widget = new Canvas(centralwidget);
        widget->setObjectName("widget");

        horizontalLayout->addWidget(widget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(centralwidget);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        listDronesInfo = new QListWidget(centralwidget);
        listDronesInfo->setObjectName("listDronesInfo");
        listDronesInfo->setMinimumSize(QSize(200, 0));
        listDronesInfo->setFrameShape(QFrame::Shape::StyledPanel);
        listDronesInfo->setLineWidth(2);
        listDronesInfo->setMidLineWidth(1);
        listDronesInfo->setAutoScrollMargin(12);
        listDronesInfo->setMovement(QListView::Movement::Snap);
        listDronesInfo->setResizeMode(QListView::ResizeMode::Adjust);
        listDronesInfo->setSpacing(0);
        listDronesInfo->setGridSize(QSize(0, 48));

        verticalLayout->addWidget(listDronesInfo);


        horizontalLayout->addLayout(verticalLayout);

        horizontalLayout->setStretch(0, 1);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1268, 25));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuShow = new QMenu(menubar);
        menuShow->setObjectName("menuShow");
        menuCompile = new QMenu(menubar);
        menuCompile->setObjectName("menuCompile");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuShow->menuAction());
        menubar->addAction(menuCompile->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionLoad);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuShow->addAction(actionCircles);
        menuShow->addAction(actionCenters);
        menuShow->addAction(actionTriangles);
        menuShow->addAction(actionVoronoi_transparency);
        menuCompile->addAction(actionCheck_Delaunay);
        menuCompile->addAction(actionProcess_flip);
        menuCompile->addAction(actionProcess_polygons);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionLoad->setText(QCoreApplication::translate("MainWindow", "Load", nullptr));
        actionQuit->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
#if QT_CONFIG(shortcut)
        actionQuit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCircles->setText(QCoreApplication::translate("MainWindow", "Circles", nullptr));
#if QT_CONFIG(shortcut)
        actionCircles->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCenters->setText(QCoreApplication::translate("MainWindow", "Centers", nullptr));
        actionTriangles->setText(QCoreApplication::translate("MainWindow", "Triangles", nullptr));
#if QT_CONFIG(shortcut)
        actionTriangles->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+T", nullptr));
#endif // QT_CONFIG(shortcut)
        actionVoronoi_transparency->setText(QCoreApplication::translate("MainWindow", "Voronoi transparency", nullptr));
#if QT_CONFIG(shortcut)
        actionVoronoi_transparency->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+V", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCheck_Delaunay->setText(QCoreApplication::translate("MainWindow", "Check Delaunay", nullptr));
#if QT_CONFIG(shortcut)
        actionCheck_Delaunay->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+D", nullptr));
#endif // QT_CONFIG(shortcut)
        actionProcess_flip->setText(QCoreApplication::translate("MainWindow", "Process flip", nullptr));
#if QT_CONFIG(shortcut)
        actionProcess_flip->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+F", nullptr));
#endif // QT_CONFIG(shortcut)
        actionProcess_polygons->setText(QCoreApplication::translate("MainWindow", "Process polygons", nullptr));
#if QT_CONFIG(shortcut)
        actionProcess_polygons->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+P", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Drone list:", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuShow->setTitle(QCoreApplication::translate("MainWindow", "Show", nullptr));
        menuCompile->setTitle(QCoreApplication::translate("MainWindow", "Compile", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
