#ifndef OBSTACLE_H
#define OBSTACLE_H

struct CircleObstacle {
    double x;
    double y;
    double radius;
};

struct RectObstacle {
    double x;
    double y;
    double width;
    double height;
};

#endif // OBSTACLE_H
