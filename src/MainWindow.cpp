#include "MainWindow.h"
#include <QDebug>
#include <QFileDialog>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QDoubleValidator>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi();
    setWindowTitle("Robot Map Generator");
    resize(800, 600);
}

void MainWindow::setupUi() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // --- Upper Half ---
    QHBoxLayout *upperLayout = new QHBoxLayout();
    
    // Left side of upper half (Controls)
    QVBoxLayout *controlsLayout = new QVBoxLayout();

    // 1. Circular Obstacles Group
    QGroupBox *circleGroup = new QGroupBox("Circular Obstacles");
    QFormLayout *circleForm = new QFormLayout();
    
    circleXEdit = new QLineEdit();
    circleYEdit = new QLineEdit();
    circleRadiusEdit = new QLineEdit();
    
    // Set numeric validators
    circleXEdit->setValidator(new QDoubleValidator(this));
    circleYEdit->setValidator(new QDoubleValidator(this));
    circleRadiusEdit->setValidator(new QDoubleValidator(0.0, 1000.0, 2, this));

    QPushButton *addCircleBtn = new QPushButton("Add circle");
    connect(addCircleBtn, &QPushButton::clicked, this, &MainWindow::onAddCircle);

    circleForm->addRow("Center X:", circleXEdit);
    circleForm->addRow("Center Y:", circleYEdit);
    circleForm->addRow("Radius:", circleRadiusEdit);
    circleForm->addRow(addCircleBtn);
    circleGroup->setLayout(circleForm);

    // 2. Rectangular Obstacles Group
    QGroupBox *rectGroup = new QGroupBox("Rectangular Obstacles");
    QFormLayout *rectForm = new QFormLayout();

    rectXEdit = new QLineEdit();
    rectYEdit = new QLineEdit();
    rectWidthEdit = new QLineEdit();
    rectHeightEdit = new QLineEdit();

    // Set numeric validators
    rectXEdit->setValidator(new QDoubleValidator(this));
    rectYEdit->setValidator(new QDoubleValidator(this));
    rectWidthEdit->setValidator(new QDoubleValidator(0.0, 1000.0, 2, this));
    rectHeightEdit->setValidator(new QDoubleValidator(0.0, 1000.0, 2, this));

    QPushButton *addRectBtn = new QPushButton("Add rectangle");
    connect(addRectBtn, &QPushButton::clicked, this, &MainWindow::onAddRectangle);

    rectForm->addRow("Center X:", rectXEdit);
    rectForm->addRow("Center Y:", rectYEdit);
    rectForm->addRow("Length (Width):", rectWidthEdit); // Assuming 'Length' matches Width or Height, used Width here as label
    rectForm->addRow("Width (Height):", rectHeightEdit);
    rectForm->addRow(addRectBtn);
    rectGroup->setLayout(rectForm);

    controlsLayout->addWidget(circleGroup);
    controlsLayout->addWidget(rectGroup);

    // Righ side of upper half (Controls)
    QVBoxLayout *gButtonsLayout = new QVBoxLayout();
    // 3. Save and exit Button 
    QPushButton *saveMapBtn = new QPushButton("Save Map");
    connect(saveMapBtn, &QPushButton::clicked, this, &MainWindow::onSaveMap);
    gButtonsLayout->addWidget(saveMapBtn);
    
    QPushButton *exitBtn = new QPushButton("Exit");
    connect(exitBtn, &QPushButton::clicked, this, &MainWindow::close);
    gButtonsLayout->addWidget(exitBtn);

    upperLayout->addLayout(controlsLayout, 1); // Stretch factor 1
    upperLayout->addLayout(gButtonsLayout, 1); // Stretch factor 1
    // --- Lower Half ---
    mapWidget = new MapWidget();
    mapWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Assemble Main Layout
    // Upper part takes less space than the map potentially, or equal. Let's let them share.
    mainLayout->addLayout(upperLayout, 1);
    mainLayout->addWidget(mapWidget, 2); // Give map more vertical space
}

void MainWindow::onAddCircle() {
    bool okX, okY, okR;
    double x = circleXEdit->text().toDouble(&okX);
    double y = circleYEdit->text().toDouble(&okY);
    double r = circleRadiusEdit->text().toDouble(&okR);

    if (okX && okY && okR) {
        mapWidget->addCircle(x, y, r);
        // Optional: clear inputs? User might want to add similar obstacles. Keeping them for now.
    }
}

void MainWindow::onAddRectangle() {
    bool okX, okY, okW, okH;
    double x = rectXEdit->text().toDouble(&okX);
    double y = rectYEdit->text().toDouble(&okY);
    double w = rectWidthEdit->text().toDouble(&okW);
    double h = rectHeightEdit->text().toDouble(&okH);

    if (okX && okY && okW && okH) {
        mapWidget->addRectangle(x, y, w, h);
    }
}

void MainWindow::onSaveMap() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Map"), "", tr("PGM Files (*.pgm);;All Files (*)"));
    if (fileName.isEmpty()) {
        return;
    }
    mapWidget->saveMap(fileName);
}

