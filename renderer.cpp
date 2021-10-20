#include "renderer.h"
#include <QDebug>
#include <QtConcurrent>
#include <QVideoSurfaceFormat>

Renderer::Renderer(QObject* parent)
        : QThread(parent)
        , m_precision(0)
        , p_surface(nullptr)
{
        connect(
                this, &Renderer::rendered,
                this, &Renderer::updateImage
        );

        m_threads = QThread::idealThreadCount();
}

////////////////////////////////////////////////////////////////////////////////////////////
// getters and setters follow:

Renderer::~Renderer() {
        instructions.stop();
        wait();
}

QAbstractVideoSurface* Renderer::videoSurface() const {
        return p_surface;
}

void Renderer::setVideoSurface(QAbstractVideoSurface *surface) {
        if (surface == p_surface || surface == nullptr)
                return;
        bool idle = (p_surface == nullptr);
        p_surface = surface;
        if (idle)
                start();
}

int Renderer::precision() const {
        return m_precision;
}

QSize Renderer::outSize() const {
        return instructions.outSize();
}

qreal Renderer::scale() const {
        return instructions.scale();
}

QSizeF Renderer::calcSize() const {
        return instructions.calcSize();
}

QPointF Renderer::calcCenter() const {
        return instructions.calcCenter();
}

void Renderer::setCalcCenter(QPointF center) {
        if (instructions.calcCenter() == center)
                return;
        instructions.setCalcCenter(center);
        emit calcCenterChanged();
}

void Renderer::setOutSize(QSize size) {
        if (size.width() < 1 || size.height() < 1)
                return;
        if (size == instructions.outSize())
                return;
        instructions.setOutSize(size);
        emit outSizeChanged();
        emit calcSizeChanged();
}

void Renderer::setScale(qreal scale) {
        if (scale < 0.5)
                scale = 0.5;
        if (scale == instructions.scale())
                return;
        instructions.setScale(scale);
        emit scaleChanged();
        emit calcSizeChanged();
}

Palette* Renderer::palette() const {
        return instructions.palette();
}

void Renderer::setPalette(Palette *palette) {
        Palette* p_palette = instructions.palette();
        if (p_palette == palette || palette == nullptr || !palette->valid())
                return;
        instructions.setPalette(palette);
}

int Renderer::threads() const {
        return m_threads;
}

void Renderer::setThreads(int threads) {
        if (threads < 1)
                threads = 1;
        else if (threads > 100)
                threads = 100;

        if (m_threads == threads)
                return;
        m_threads = threads;
        emit threadsChanged();
}

qreal Renderer::exponent() const {
        return instructions.exponent();
}

void Renderer::setExponent(qreal exponent) {
        if (exponent < 1)
                exponent = 1;
        else if (exponent > 13)
                exponent = 13;
        if (instructions.exponent() == exponent)
                return;

        instructions.setExponent(exponent);
        emit exponentChanged();
}

void Renderer::setPrecision(int precision) {
        if (m_precision == precision)
                return;
        m_precision = precision;
        emit precisionChanged();
}

////////////////////////////////////////////////////////////////////////////////////////////
// actual rendering follows:

void Renderer::run() {

        // worker callable struct
        Worker worker;

        forever {
                // wait for instruction changes
                const Instructions todo = instructions.getChanges();
                if (todo.shouldStop())
                        return;

                // get shorthands for long instruction variables etc.
                const auto& oWidth  = todo.outSize().width();
                const auto& oHeight = todo.outSize().height();
                const auto& palette = todo.palette();
                const auto  threads = m_threads;
                worker.exponent     = todo.exponent();

                // if imageOnly flag,
                // it's enough to just update the image using already calculated data
                if (todo.imageOnly()) {
                        drawImage(todo.palette(), m_precision);
                        QImage img(reinterpret_cast<uchar*>(image_buffer.data()), activeImage.size().width(), activeImage.size().height(), QImage::Format_RGB32);
                        emit rendered(img, m_precision);
                        continue;
                }
                // else
                // the whole process needs to be done

                setPrecision(0);
                updateSurfaceFormat(todo.outSize());

                // prepare data and image buffers
                try {
                        prepareData(todo);
                        image_buffer.resize(oWidth * oHeight);
                } catch (std::bad_alloc) {
                        qDebug() << "ran out of memory";
                        qApp->quit();
                }

                // set thread count
                QThreadPool::globalInstance()->setMaxThreadCount(threads);

                /* the fractal will be rendered gradually with higher and higher precision
                 * rendering will stp at `max_iterations`
                 * image will be updated at each `iteration_target`
                 *
                 * `max_iterations` shouldn't be above 1024 and below 32 for both performance and aesthetic reasons
                 */

                int max_iterations   = getMaxIterations(todo.scale());
                int iteration_target = getFirstIterationTarget(max_iterations);
                int iterations_done  = 0;

                forever {
                        // perform multi-thread calculation on the buffer
                        worker.iterationsBegin = iterations_done;
                        worker.iterationsEnd   = iteration_target;
                        QtConcurrent::blockingMap(data_buffer, worker);

                        // translate that buffer into image data
                        drawImage(palette, iteration_target);
                        QImage img(reinterpret_cast<uchar*>(image_buffer.data()), oWidth, oHeight, QImage::Format_RGB32);
                        emit rendered(img, iteration_target);

                        // set up new precision target
                        iterations_done = iteration_target;
                        if (iterations_done == max_iterations)
                                break;
                        iteration_target = getNextIterationTarget(iteration_target, max_iterations);

                        if (instructions.changed())
                                break;
                }

        }
}

void Renderer::prepareData(const Instructions& todo) {
        // `buffer` holds data for mandelbrot calculation
        // see worker.h / worker.cpp

        // get shorthands first
        const auto& oWidth  = todo.outSize().width();
        const auto& oHeight = todo.outSize().height();
        const auto& cWidth  = todo.calcSize().width();
        const auto& cHeight = todo.calcSize().height();
        const auto& cCenter = todo.calcCenter();

        // resize buffer
        data_buffer.resize(oWidth * oHeight);

        // now initialize its values
        for (size_t i = 0; i < data_buffer.size(); i++) {
                auto& data = data_buffer[i];

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
}

int Renderer::getMaxIterations(qreal scale) {
        int max_iterations = scale / 2;
        if (max_iterations < 128)
                max_iterations = 128;
        else if (max_iterations > 1024)
                max_iterations = 1024;
        return max_iterations;
}

int Renderer::getFirstIterationTarget(int max) {

        int iteration_target = max / 2;
        if (iteration_target > 128)
                iteration_target = 128;
        return iteration_target;
}

int Renderer::getNextIterationTarget(int current, int max) {
        int next = current + 16;
        if (next > max)
                next = max;
        return next;
}

void Renderer::drawImage(Palette* palette, size_t precision) {
        for (size_t i = 0; i < data_buffer.size(); i++) {
                int m = data_buffer[i].i;
                if (m < 0)
                        m = 0;
                m = m * 256 / precision;
                image_buffer[i] = palette->getColor(m % 256);
        }
}

void Renderer::updateSurfaceFormat(const QSize& size) {
        QVideoSurfaceFormat format(size, QVideoFrame::Format_RGB32);
        p_surface->start(format);
}

void Renderer::updateImage(const QImage& image, int precision) {
        activeImage = image;

        // present the image as a frame
        QVideoFrame frame(image);
        p_surface->present(frame);

        setPrecision(precision);
}

void Renderer::exportTo(const QString& path) const {
        qDebug() << "exporting to" << path;
        activeImage.save(path);
}
