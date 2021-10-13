#include "renderer.h"
#include <QDebug>
#include <QtConcurrent>
#include <QVideoSurfaceFormat>

#include "worker.h"

Renderer::Renderer(QObject* parent)
        : QThread(parent)
        , m_threads(QThread::idealThreadCount())
        , m_precision(0)
{
    connect(
                this, &Renderer::rendered,
                this, &Renderer::updateImage
    );
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

int Renderer::precision() const
{
    return m_precision;
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
    // shorthands
    const auto frame_format = QVideoFrame::Format_RGB32;
    const auto image_format = QImage::Format_RGB32;

    // buffer for multi-thread calculations
    QVector<MandelData> buffer;

    forever {
        // wait for instruction changes
        const Instructions todo = instructions.getChanges();
        if (todo.shouldStop())
            return;

        // more shorthands
        const auto& oWidth  = todo.outSize().width();
        const auto& oHeight = todo.outSize().height();
        const auto& cWidth  = todo.calcSize().width();
        const auto& cHeight = todo.calcSize().height();
        const auto& cCenter  = todo.calcCenter();

        // resize buffer to width*height
        buffer.resize(oWidth * oHeight);

        // visual structs
        p_surface->start({ todo.outSize(), frame_format });
        QImage image(      todo.outSize(), image_format  );

        // prepare all of its values
        for (size_t i = 0; i < buffer.size(); i++) {
            auto& data = buffer[i];

            // get local c = (x, y)
            int row = i / oWidth;
            int col = i % oWidth;
            qreal x =  (static_cast<qreal>(col) / oWidth  - 0.5) * cWidth  + cCenter.x();
            qreal y = -(static_cast<qreal>(row) / oHeight - 0.5) * oHeight + cCenter.y();

            // initialize data
            data.c = {x, y};
            data.z = {0, 0};
            data.i = 40;
        }

        // update image
        QRgb* image_buffer = reinterpret_cast<QRgb*>(image.bits());
        for (size_t i = 0; i < oWidth * oHeight; i++)
            image_buffer[i] = qRgb(buffer[i].i, 0, 0);
        emit rendered(image, 50);
    }
}

void Renderer::updateImage(const QImage &image, int precision)
{
    QVideoFrame frame(image);
    p_surface->present(frame);
    if (m_precision == precision)
        return;
    m_precision = precision;
    emit precisionChanged();
}
