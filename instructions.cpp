#include "instructions.h"


qreal Instructions::scale() const
{
        return m_scale;
}

void Instructions::setScale(qreal scale)
{
        if (scale < 0.5)
                scale = 0.5;
        if (m_scale == scale)
                return;

        m_scale = scale;
}

QSize Instructions::outSize() const
{
    return m_outSize;
}

void Instructions::setOutSize(QSize size)
{
    m_outSize = size;
}

QPointF Instructions::calcCenter() const
{
    return m_calcCenter;
}

void Instructions::setCalcCenter(QPointF center)
{
    m_calcCenter = center;
}

