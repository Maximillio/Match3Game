#ifndef BALL_H
#define BALL_H

enum {Empty = 0, Obstacle = 1};
enum {Red = 2, Blue, Green, Yellow, Purple, White};

struct Ball {
    int m_type;
    bool m_checked = false;
};

#endif // BALL_H
