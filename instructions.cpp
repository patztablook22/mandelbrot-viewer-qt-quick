#include "instructions.h"

#include <QMutexLocker>

Instructions::Instructions()
{
    mutex = new QMutex;
    condition = new QWaitCondition;
}


qreal Instructions::scale() const
{
        return m_scale;
}

void Instructions::setScale(qreal scale)
{
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
    return outSize();
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
