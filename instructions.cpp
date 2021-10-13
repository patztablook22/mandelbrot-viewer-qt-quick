#include "instructions.h"

#include <QMutexLocker>

Instructions::Instructions()
        : m_scale(10)
        , m_calcCenter{0, 0}
        , m_stop(false)
        , m_exponent(2)
{
    p_palette = new Palette;
    p_palette->setSource(":/palettes/BWIron1.plt");
    mutex = new QMutex;
    condition = new QWaitCondition;
}


qreal Instructions::scale() const
{
        return m_scale;
}

void Instructions::setScale(qreal scale)
{
        if (scale < 0.5)
            scale = 0.5;
        QMutexLocker l(mutex);
        m_scale = scale;
        change();
}

QSize Instructions::outSize() const
{
        return m_outSize;
}

void Instructions::setOutSize(QSize size)
{
        QMutexLocker l(mutex);
        m_outSize = size;
        change();
}

QSizeF Instructions::calcSize() const
{
    // power to make it more practical for higher scales
    return QSizeF(m_outSize) / pow(m_scale, 2);
}

QPointF Instructions::calcCenter() const
{
        return m_calcCenter;
}

void Instructions::setCalcCenter(QPointF center)
{
        QMutexLocker l(mutex);
        m_calcCenter = center;
        change();
}

qreal Instructions::exponent() const
{
    return m_exponent;
}

void Instructions::setExponent(qreal exponent)
{
    QMutexLocker l(mutex);
    m_exponent = exponent;
    change();
}

Palette* Instructions::palette() const
{
    return p_palette;
}

void Instructions::setPalette(Palette *palette)
{
    QMutexLocker l(mutex);
    p_palette = palette;
    change();
}

const Instructions Instructions::getChanges()
{
        if (!m_changed)
                condition->wait(mutex);
        m_changed = false;
        mutex->unlock();

        // pass by value!
        return *this;
}

void Instructions::change()
{
        m_changed = true;
        condition->wakeAll();
}

bool Instructions::changed() const
{
    return m_changed;
}

void Instructions::stop()
{
        QMutexLocker l(mutex);
        m_stop = true;
        change();
}

bool Instructions::shouldStop() const
{
        return m_stop;
}
