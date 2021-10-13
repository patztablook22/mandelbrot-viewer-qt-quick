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
    p_palette = new Palette;
    p_palette->setSource(":/palettes/BWIron1.plt");
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
    return instructions.calcSize();
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

Palette* Renderer::palette() const
{
    return p_palette;
}

void Renderer::setPalette(Palette *palette)
{
    if (p_palette == palette || palette == nullptr || !palette->valid())
        return;
    p_palette = palette;
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

    // worker callable struct
    Worker worker;

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

            // set thread count
            QThreadPool::globalInstance()->setMaxThreadCount(m_threads);
            qDebug() << QThreadPool::globalInstance()->maxThreadCount();

            // prepare all of its values
            for (size_t i = 0; i < buffer.size(); i++) {
                    auto& data = buffer[i];

                    // get local c = (x, y)
                    int row = i / oWidth;
                    int col = i % oWidth;
                    qreal x =  (static_cast<qreal>(col) / oWidth  - 0.5) * cWidth  + cCenter.x();
                    qreal y = -(static_cast<qreal>(row) / oHeight - 0.5) * cHeight + cCenter.y();

                    // initialize data
                    data.c = {x, y};
                    data.z = {0, 0};
                    data.i = -1;
            }

        /* the fractal will be rendered gradually with higher and higher precision
         * rendering will stp at `max_iterations`
         * image will be updated at `iteration_target`
         *
         * `max_iterations` shouldn't be above 1024 and below 32 for both performance and aesthetic reasons
         */

            int max_iterations = todo.scale() / 2;
            if (max_iterations < 128)
                    max_iterations = 128;
            else if (max_iterations > 1024)
                    max_iterations = 1024;

            int iteration_target = max_iterations / 2;
            if (iteration_target > 128)
                    iteration_target = 128;

            int iterations_done = 0;
            forever {
                    // perform the multi-thread calculation
                    worker.iterationsBegin = iterations_done;
                    worker.iterationsEnd   = iteration_target;
                    QtConcurrent::blockingMap(buffer, worker);

                    // update image
                    QRgb* image_buffer = reinterpret_cast<QRgb*>(image.bits());
                    for (size_t i = 0; i < oWidth * oHeight; i++) {
                            int m = buffer[i].i;
                            if (m < 0)
                                    m = 0;
                            m = m * 256 / iteration_target;
                            image_buffer[i] = p_palette->getColor(m);
                    }
                    emit rendered(image, iteration_target);

                    iterations_done = iteration_target;
                    if (iterations_done == max_iterations)
                        break;
                    iteration_target += 32;
                    if (iteration_target > max_iterations)
                        iteration_target = max_iterations;

                    if (instructions.changed())
                        break;
            }

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
