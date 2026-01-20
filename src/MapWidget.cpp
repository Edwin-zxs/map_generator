#include "MapWidget.h"
#include <QPainter>
#include <QtMath>
#include <QImage>

MapWidget::MapWidget(QWidget *parent) : QWidget(parent) {
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setMinimumSize(400, 400); // Reasonable default size
}

void MapWidget::addCircle(double x, double y, double radius) {
    circles.push_back({x, y, radius});
    obstacleHistory.push_back(ObstacleType::Circle);
    update(); // Trigger repaint
}

void MapWidget::addRectangle(double x, double y, double width, double height) {
    rectangles.push_back({x, y, width, height});
    obstacleHistory.push_back(ObstacleType::Rectangle);
    update(); // Trigger repaint
}

void MapWidget::deleteLastObstacle() {
    if (obstacleHistory.empty()) {
        return;
    }

    ObstacleType lastType = obstacleHistory.back();
    obstacleHistory.pop_back();

    if (lastType == ObstacleType::Circle) {
        if (!circles.empty()) {
            circles.pop_back();
        }
    } else if (lastType == ObstacleType::Rectangle) {
        if (!rectangles.empty()) {
            rectangles.pop_back();
        }
    }
    update();
}

void MapWidget::saveMap(const QString &fileName) {
    // Create an image with the map size (assuming 1 unit = 1 pixel for simplicity, or 200x200 output)
    // The requirement says "save the current map into a PGM". 
    // Usually mapSize=200 means 200 units. Let's create a 200x200 pixel image.
    int w = static_cast<int>(mapSize);
    int h = static_cast<int>(mapSize);
    
    QImage image(w, h, QImage::Format_Grayscale8);
    image.fill(Qt::white); // Free space

    QPainter painter(&image);
    // Coordinate system: default QImage has (0,0) at top-left.
    // Our map data (x, y) also assumes top-left origin based on previous paintEvent analysis.
    // If we wanted to strictly follow a mathematical 2D plane where y is up, we'd need to flip.
    // But given paintEvent didn't flip, we won't flip here either to be consistent.
    
    // Draw primitives
    
    // Draw Circles (Black obstacles)
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    for (const auto& c : circles) {
        painter.drawEllipse(QPointF(c.x, c.y), c.radius, c.radius);
    }

    // Draw Rectangles (Black obstacles)
    painter.setBrush(Qt::black);
    for (const auto& r : rectangles) {
        double topLeftX = r.x - r.width / 2.0;
        double topLeftY = r.y - r.height / 2.0;
        painter.drawRect(QRectF(topLeftX, topLeftY, r.width, r.height));
    }
    
    painter.end();

    // Save as PGM
    image.save(fileName, "PGM");
}

void MapWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Calculate scale to fit 200x200 map into the widget
    // We want to keep aspect ratio 1:1, so take the min scale factor
    double scaleX = width() / mapSize;
    double scaleY = height() / mapSize;
    double scale = std::min(scaleX, scaleY);

    // Center the map in the widget
    double offsetX = (width() - mapSize * scale) / 2.0;
    double offsetY = (height() - mapSize * scale) / 2.0;

    // Save initial state
    painter.save();

    // Translate and Scale coordinate system
    // 0,0 is top-left in Qt, but map coordinates usually are (0,0) top-left too for image based maps
    // If (0,0) should be bottom-left, we'd need to flip Y.
    // Assuming standard image coordinate system (0,0 top left) based on "map generator".
    painter.translate(offsetX, offsetY);
    painter.scale(scale, scale);

    // Draw Map Boundary
    painter.setPen(QPen(Qt::black, 2.0 / scale)); // Constant width pen regardless of scale
    painter.setBrush(Qt::white);
    painter.drawRect(QRectF(0, 0, mapSize, mapSize));

    // Draw Circles
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::blue);
    for (const auto& c : circles) {
        // drawEllipse takes bounding rectangle or center+radii
        // QPointF center(c.x, c.y);
        // painter.drawEllipse(center, c.radius, c.radius);
        // BUT drawEllipse(center, rx, ry)
        painter.drawEllipse(QPointF(c.x, c.y), c.radius, c.radius);
    }

    // Draw Rectangles
    painter.setBrush(Qt::red);
    for (const auto& r : rectangles) {
        // Draw rectangle centered at r.x, r.y
        double topLeftX = r.x - r.width / 2.0;
        double topLeftY = r.y - r.height / 2.0;
        painter.drawRect(QRectF(topLeftX, topLeftY, r.width, r.height));
    }

    painter.restore();
}
