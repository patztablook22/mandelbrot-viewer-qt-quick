#include "renderer.h"
#include <QDebug>
#include <QtConcurrent>

Renderer::Renderer(QObject* parent)
        : QThread(parent)
        , m_threads(QThread::idealThreadCount())
{
    start();
}

Renderer::~Renderer()
{
    instructions.stop();
    wait();
}

QAbstractVideoSurface* Renderer::videoSurface() const
{
    return p_surface;
}

void Renderer::setVideoSurface(QAbstractVideoSurface *surface)
{
    if (surface == p_surface || surface == nullptr)
        return;
    p_surface = surface;
}

QSize Renderer::outSize() const
{
    return instructions.outSize();
}

qreal Renderer::scale() const
{
    return instructions.scale();
}

QSizeF Renderer::calcSize() const
{
    return outSize();
}

QPointF Renderer::calcCenter() const
{
    return instructions.calcCenter();
}

void Renderer::setCalcCenter(QPointF center)
{
    if (instructions.calcCenter() == center)
        return;
    instructions.setCalcCenter(center);
    emit calcCenterChanged();
}

void Renderer::setOutSize(QSize size)
{
    if (size.width() < 1 || size.height() < 1)
        return;
    if (size == instructions.outSize())
        return;
    instructions.setOutSize(size);
    emit outSizeChanged();
    emit calcSizeChanged();
}

void Renderer::setScale(qreal scale)
{
    instructions.setScale(scale);
    emit scaleChanged();
    emit calcSizeChanged();
}

int Renderer::threads() const
{
    return m_threads;
}

void Renderer::setThreads(int threads)
{
    if (threads < 1)
        threads = 1;
    else if (threads > 100)
        threads = 100;

    if (m_threads == threads)
        return;
    m_threads = threads;
    emit threadsChanged();
}

void Renderer::run()
{
    forever {
        // wait for instruction changes
        const Instructions todo = instructions.getChanges();
        qDebug() << "changed";
        if (todo.shouldStop())
            return;

    }
}
