#include "instructions.h"

#include <QMutexLocker>

Instructions::Instructions()
        : m_scale(10)
        , m_calcCenter{0, 0}
        , m_outSize{0, 0}
        , m_exponent(2)
        , m_stop(false)
        , m_imageOnly(true)
        , m_changed(false)
{
        p_palette = new Palette;
        p_palette->setSource(":/palettes/Sepia.plt");
        mutex = new QMutex;
        condition = new QWaitCondition;
}

Instructions::~Instructions() {
}

/*
 * Getters just get, they don't need to be thread-safe, because the whole class
 * is being copied into the separate thread, just that copy needs to be thread-safe
 * see `Instructions::getChanges()`
 *
 * Setters, on the other hand, need to be thread safe in respect to the copying
 * => each setters locks the mutex and calls change() to make the class know that new
 * instructions are available
 * see `Instructions::change()`
 *
 */

qreal Instructions::scale() const {
        return m_scale;
}

void Instructions::setScale(qreal scale) {
        QMutexLocker l(mutex);
        m_scale = scale;
        change();
}

QSize Instructions::outSize() const {
        return m_outSize;
}

void Instructions::setOutSize(QSize size) {
        QMutexLocker l(mutex);
        m_outSize = size;
        change();
}

QSizeF Instructions::calcSize() const {
        // power to make it more practical for higher scales
        return QSizeF(m_outSize) / pow(m_scale, 2);
}

QPointF Instructions::calcCenter() const {
        return m_calcCenter;
}

void Instructions::setCalcCenter(QPointF center) {
        QMutexLocker l(mutex);
        m_calcCenter = center;
        change();
}

qreal Instructions::exponent() const {
        return m_exponent;
}

void Instructions::setExponent(qreal exponent) {
        QMutexLocker l(mutex);
        m_exponent = exponent;
        change();
}

Palette* Instructions::palette() const {
        return p_palette;
}

void Instructions::setPalette(Palette *palette) {
        QMutexLocker l(mutex);
        p_palette   = palette;
        change(true);
}

bool Instructions::imageOnly() const {
        return m_imageOnly;
}

Instructions Instructions::getChanges() const {
        if (!m_changed)
                condition->wait(mutex);
        m_changed = false;
        mutex->unlock();

        // pass by value!
        auto tmp = *this;
        m_imageOnly = true;
        return tmp;
}

void Instructions::change(bool imageOnly) {
        // pass imageOnly = true
        // if the mandelbrot set doesn't need to be recalculated
        // and just the image needs to be updated using already calculated data

        m_changed   = true;
        m_imageOnly = imageOnly && m_imageOnly;
        condition->wakeAll();
}

bool Instructions::changed() const {
        return m_changed;
}

void Instructions::stop() {
        QMutexLocker l(mutex);
        m_stop = true;
        change();
}

bool Instructions::shouldStop() const {
        return m_stop;
}
