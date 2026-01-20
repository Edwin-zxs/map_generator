#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <vector>
#include "Obstacle.h"

class MapWidget : public QWidget {
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = nullptr);
    void addCircle(double x, double y, double radius);
    void addRectangle(double x, double y, double width, double height);
    void saveMap(const QString &fileName);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::vector<CircleObstacle> circles;
    std::vector<RectObstacle> rectangles;
    const double mapSize = 200.0;
};

#endif // MAPWIDGET_H
