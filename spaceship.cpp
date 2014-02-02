#include "spaceship.h"

SpaceShip::SpaceShip(QObject *parent) :
    QObject(parent)
{
}

void SpaceShip::enableThrottleUp(const bool &f)
{
    throttleUp = f;
    if (throttleUp && throttleDown ||
       !(throttleUp || throttleDown))
    {
        throttle.setY(0.0);
    } else if (throttleUp) {
        throttle.setY(0.01);
    } else if (throttleDown) {
        throttle.setY(-0.01);
    }
}

void SpaceShip::enableThrottleLeft(const bool &f)
{
    throttleLeft = f;
    if (throttleLeft && throttleRight ||
       !(throttleLeft || throttleRight))
    {
        throttle.setX(0.0);
    } else if (throttleLeft) {
        throttle.setX(-0.01);
    } else if (throttleRight) {
        throttle.setX(0.01);
    }
}

void SpaceShip::enableThrottleDown(const bool &f)
{
    throttleDown = f;
    if (throttleDown && throttleUp ||
       !(throttleDown || throttleUp))
    {
        throttle.setY(0.0);
    } else if (throttleDown) {
        throttle.setY(-0.01);
    } else if (throttleUp) {
        throttle.setY(0.01);
    }
}

void SpaceShip::enableThrottleRight(const bool &f)
{
    throttleRight = f;
    if (throttleRight && throttleLeft ||
       !(throttleRight || throttleLeft))
    {
        throttle.setX(0.0);
    } else if (throttleRight) {
        throttle.setX(0.01);
    } else if (throttleLeft) {
        throttle.setX(-0.01);
    }
}

void SpaceShip::move()
{
    velocity += throttle;
    position += velocity;
}

const QVector2D &SpaceShip::getPosition() const
{
    return position;
}
