#include "manager.h"

Manager::Manager(QObject *parent)
        : QObject(parent)
        , m_scale(1)
{
}

qreal Manager::scale() const
{
        return m_scale;
}

void Manager::setScale(qreal scale)
{
        if (scale < 0.5)
                scale = 0.5;
        if (m_scale == scale)
                return;

        m_scale = scale;
        emit scaleChanged();
}

