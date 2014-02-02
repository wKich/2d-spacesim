#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWindow>
#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QVector2D>

#include "spaceship.h"

class MainWindow : public QWindow
{
    Q_OBJECT
public:
    explicit MainWindow();
    ~MainWindow();

    void render();

protected:
    bool event(QEvent *event);
    void exposeEvent(QExposeEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

signals:

public slots:
    void renderLater();
    void renderNow();

private:
    //Rendering
    bool m_update_pending;

    QOpenGLContext* m_context;
    QOpenGLShaderProgram* m_program;
    QOpenGLShaderProgram* m_bgProgram;
    QOpenGLBuffer* m_glBuffer;
    QOpenGLBuffer* m_bgGLBuffer;

    int m_vertexAttr;
    int m_colorUni;
    int m_matrixUni;
    int m_resolutionUni;
    int m_backgroundOffsetUni;

    void initialize();

    //Other
    QVector2D offset;
    SpaceShip ship;
};

#endif // MAINWINDOW_H
