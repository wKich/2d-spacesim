#include "mainwindow.h"

MainWindow::MainWindow() :
    m_update_pending(false),
    m_context(nullptr),
    m_program(nullptr),
    m_glBuffer(nullptr)
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
    matrix.ortho(0.0f, width(), 0.0f, height(), 0.0f, 1.0f);

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
    case Qt::Key_Escape:
        this->close();
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

    m_glBuffer = new QOpenGLBuffer;
    m_glBuffer->create();
    m_glBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_glBuffer->bind();
    m_glBuffer->allocate(sizeof(QVector2D));
    QVector2D point(width() / 2, height() / 2);
    m_glBuffer->write(0, &point, sizeof(QVector2D));
    m_glBuffer->release();
}
