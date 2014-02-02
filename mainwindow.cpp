#include "mainwindow.h"

MainWindow::MainWindow() :
    m_update_pending(false),
    m_context(nullptr),
    m_program(nullptr),
    m_bgProgram(nullptr),
    m_glBuffer(nullptr),
    m_bgGLBuffer(nullptr)
{
    setSurfaceType(QWindow::OpenGLSurface);
}

MainWindow::~MainWindow()
{
}

void MainWindow::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, width(), height());

    QMatrix4x4 matrix;
    matrix.ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);

    m_bgProgram->bind();
    m_bgProgram->setUniformValue(m_matrixUni, matrix);

    m_bgGLBuffer->bind();
    m_bgProgram->setAttributeBuffer(m_vertexAttr, GL_FLOAT, 0, 2);
    m_bgProgram->enableAttributeArray(m_vertexAttr);
    m_bgProgram->setUniformValue(m_resolutionUni, QVector2D(width(), height()));
    m_bgProgram->setUniformValue(m_backgroundOffsetUni, ship.getPosition());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

    m_bgGLBuffer->release();
    m_bgProgram->release();

    m_program->bind();
    m_program->setUniformValue(m_matrixUni, matrix);

    m_glBuffer->bind();
    m_program->setAttributeBuffer(m_vertexAttr, GL_FLOAT, 0, 2);
    m_program->enableAttributeArray(m_vertexAttr);
    m_program->setUniformValue(m_colorUni, QColor(Qt::white));
    glDrawArrays(GL_POINTS, 0, 1);

    m_glBuffer->release();
    m_program->release();
}

bool MainWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void MainWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
        ship.enableThrottleUp(true);
        break;
    case Qt::Key_A:
        ship.enableThrottleLeft(true);
        break;
    case Qt::Key_S:
        ship.enableThrottleDown(true);
        break;
    case Qt::Key_D:
        ship.enableThrottleRight(true);
        break;
    case Qt::Key_Escape:
        this->close();
        break;
    default:
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
        ship.enableThrottleUp(false);
        break;
    case Qt::Key_A:
        ship.enableThrottleLeft(false);
        break;
    case Qt::Key_S:
        ship.enableThrottleDown(false);
        break;
    case Qt::Key_D:
        ship.enableThrottleRight(false);
        break;
    default:
        break;
    }
}

void MainWindow::renderLater()
{
    if (!m_update_pending) {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

void MainWindow::renderNow()
{
    if (!isExposed())
        return;

    m_update_pending = false;

    if (!m_context) {
        initialize();
    } else {
        m_context->makeCurrent(this);
    }

    ship.move();

    render();
    m_context->swapBuffers(this);
    renderLater();
}

void MainWindow::initialize()
{
    m_context = new QOpenGLContext(this);
    m_context->setFormat(requestedFormat());
    m_context->create();
    m_context->makeCurrent(this);

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/GLSL/main.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/GLSL/main.frag");
    m_program->link();
    m_vertexAttr = m_program->attributeLocation("qt_Vertex");
    m_colorUni = m_program->uniformLocation("qt_Color");
    m_matrixUni = m_program->uniformLocation("qt_Matrix");

    m_bgProgram = new QOpenGLShaderProgram(this);
    m_bgProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/GLSL/main.vert");
    m_bgProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/GLSL/bg.frag");
    m_bgProgram->link();
    m_vertexAttr = m_bgProgram->attributeLocation("qt_Vertex");
    m_resolutionUni = m_bgProgram->uniformLocation("qt_Resolution");
    m_backgroundOffsetUni = m_bgProgram->uniformLocation("qt_BackgroundOffset");
    m_matrixUni = m_bgProgram->uniformLocation("qt_Matrix");

    m_glBuffer = new QOpenGLBuffer;
    m_glBuffer->create();
    m_glBuffer->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_glBuffer->bind();
    m_glBuffer->allocate(sizeof(QVector2D));
    QVector2D point(0.0, 0.0);
    m_glBuffer->write(0, &point, sizeof(QVector2D));
    m_glBuffer->release();

    m_bgGLBuffer = new QOpenGLBuffer;
    m_bgGLBuffer->create();
    m_bgGLBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_bgGLBuffer->bind();
    QVector<QVector2D> bgPlane;
    bgPlane.append(QVector2D(-1.0, -1.0));
    bgPlane.append(QVector2D(1.0, -1.0));
    bgPlane.append(QVector2D(-1.0, 1.0));
    bgPlane.append(QVector2D(1.0, -1.0));
    bgPlane.append(QVector2D(1.0, 1.0));
    bgPlane.append(QVector2D(-1.0, 1.0));
    m_bgGLBuffer->allocate(bgPlane.size() * sizeof(QVector2D));
    m_bgGLBuffer->write(0, bgPlane.constData(), bgPlane.size() * sizeof(QVector2D));
    m_bgGLBuffer->release();
}
