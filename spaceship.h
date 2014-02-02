#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <QObject>
#include <QVector2D>

class SpaceShip : public QObject
{
    Q_OBJECT
public:
    explicit SpaceShip(QObject *parent = 0);
    void enableThrottleUp(const bool &f = true);
    void enableThrottleLeft(const bool &f = true);
    void enableThrottleDown(const bool &f = true);
    void enableThrottleRight(const bool &f = true);
    void move();
    const QVector2D & getPosition() const;

signals:

public slots:

private:
    bool throttleUp;
    bool throttleLeft;
    bool throttleDown;
    bool throttleRight;
    QVector2D throttle;
    QVector2D velocity;
    QVector2D position;
};

#endif // SPACESHIP_H
