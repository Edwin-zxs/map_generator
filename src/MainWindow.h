#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include "MapWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onAddCircle();
    void onAddRectangle();
    void onSaveMap();
    void onDeleteObstacle();

private:
    MapWidget *mapWidget;

    // Circle Inputs
    QLineEdit *circleXEdit;
    QLineEdit *circleYEdit;
    QLineEdit *circleRadiusEdit;

    // Rectangle Inputs
    QLineEdit *rectXEdit;
    QLineEdit *rectYEdit;
    QLineEdit *rectWidthEdit;
    QLineEdit *rectHeightEdit;

    void setupUi();
};

#endif // MAINWINDOW_H
