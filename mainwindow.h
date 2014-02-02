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

signals:

public slots:
    void renderLater();
    void renderNow();

private:
    //Rendering
    bool m_update_pending;

    QOpenGLContext* m_context;
    QOpenGLShaderProgram* m_program;
    QOpenGLBuffer* m_glBuffer;

    int m_vertexAttr;
    int m_colorUni;
    int m_matrixUni;

    void initialize();
};

#endif // MAINWINDOW_H
